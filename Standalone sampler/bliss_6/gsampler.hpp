#ifndef __GAUSS_SPLIT_GSAMPLER_HPP_
#define __GAUSS_SPLIT_GSAMPLER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "randombytes.c"
#include "cpucycles.c"
#include "aes_rnd.c"

#ifdef __AVX__
	#include <immintrin.h>
	#define ALIGNMENT 32
	#define ALIGN __attribute__ ((aligned(ALIGNMENT)))
	typedef int8_t Bvec __attribute__ ((vector_size(ALIGNMENT))) ALIGN;
	#define BCONST(c) {c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c}
	#define BZERO BCONST(0)
	#define BONES BCONST(-1)
	#define BMASK BCONST(1)
	#define BTEST_NOT_ONES(x) (!_mm256_testz_si256(~((__m256i)x),~((__m256i)x)))
#else
	typedef uint64_t Bvec;
	#define BZERO 0
	#define BONES 0xffffffffffffffff
	#define BMASK 0x0101010101010101
	#define BTEST_NOT_ONES(x) (x!=BONES)
#endif

#define NSAMPLES ((int)(sizeof(Bvec)*8))

namespace GaussSplit
{

class AESSampler
{
protected:
	uint8_t bytes[16];
	int cursor;
protected:
	void fill()
	{
		aes_random_bytes(bytes);
		cursor = 0;
	}
public:
	AESSampler()
	{
		fill();
	}
};

class U64Sampler :
       public AESSampler
{
public:
	U64Sampler() : AESSampler()
	{
	}
	uint64_t operator()()
	{
		uint64_t r = ((uint64_t *)bytes)[cursor];
		if (++cursor == 2) fill();
		return r;
	}
};

class BitSampler :
       public AESSampler
{
public:
	BitSampler() : AESSampler()
	{
	}
	int operator()()
	{
		int r = (bytes[cursor >> 3] >> (cursor & ((1 << 3) - 1))) & 1;
		if (++cursor == 128) fill();
		return r;
	}
};


static const double log2 = 0.69314718055994530941723212146;
static const double p55 = 36028797018963968.0;

/*
 * Compute exp(x) for x such that |x| <= ln 2. We want a precision of 50
 * bits or so.
 */
static inline double exp_small(double x)
{
        /*
         * The algorithm used below is derived from the public domain
         * library fdlibm (http://www.netlib.org/fdlibm/e_exp.c).
         *
         * We compute exp(x/2) to make sure that the value is in the
         * proper range for the polynomial approximation, then square
         * it to get exp(x).
         *
         * This is constant-time IF the base floating-point operations
         * are constant-time (which is a big "if", especially for the
         * division operation).
         */
#define EXP_P1   ( 1.66666666666666019037e-01)
#define EXP_P2   (-2.77777777770155933842e-03)
#define EXP_P3   ( 6.61375632143793436117e-05)
#define EXP_P4   (-1.65339022054652515390e-06)
#define EXP_P5   ( 4.13813679705723846039e-08)

        double s, t, c;

        s = x;
        s *= 0.5;
        t = s * s;
        c = s - t * (EXP_P1 + t * (EXP_P2
                + t * (EXP_P3 + t * (EXP_P4 + t * EXP_P5))));
        s = 1.0 - ((s * c) / (c - 2.0) - s);
        return s * s;

#undef EXP_P1
#undef EXP_P2
#undef EXP_P3
#undef EXP_P4
#undef EXP_P5
}


/*
 * Sample a bit with probability exp(-x) for some x >= 0.
 */
static inline int BerExp(double x, uint64_t rnd1, uint64_t rnd2)
{
        int s;
        double r;
        uint64_t w, z;
        int b;
        uint32_t sw;

        /*
         * Reduce x modulo log(2): x = s*log(2) + r, with s an integer,
         * and 0 <= r < log(2).
         */
        s = (long)floor(x / log2);
        r = x - (((double)s) * log2);

        /*
         * It may happen (quite rarely) that s >= 64; if sigma = 1.2
         * (the minimum value for sigma), r = 0 and b = 1, then we get
         * s >= 64 if the half-Gaussian produced a z >= 13, which happens
         * with probability about 0.000000000230383991, which is
         * approximatively equal to 2^(-32). In any case, if s >= 64,
         * then BerExp will be non-zero with probability less than
         * 2^(-64), so we can simply saturate s at 63.
         */
        sw = s;
        sw ^= (sw ^ 63) & -((63 - sw) >> 31);
        s = (int)sw;

        /*
         * Sample a bit with probability 2^(-s):
         *  - generate a random 64-bit integer
         *  - keep only s bits
         *  - bit is 1 if the result is zero
         */
        w = rnd1;
        w ^= (w >> s) << s;
        b = 1 - (int)((w | -w) >> 63);

        /*
         * Sample a bit with probability exp(-r). Since |r| < log(2),
         * we can use exp_small(). The value is lower than 1; we
         * scale it to 2^55.
         * With combine (with AND) that bit with the previous bit, which
         * yields the expected result.
         */
        z = (uint64_t)llrint(exp_small(-r) * p55);
        w = rnd2;
        w &= ((uint64_t)1 << 55) - 1;
        b &= (int)((w - z) >> 63);

        return b;
}

/*
 * The sampler produces a random integer that follows a discrete Gaussian
 * distribution, centered on mu, and with standard deviation sigma.
 * The value of sigma MUST lie between 1 and 2 (in Falcon, it should
 * always be between 1.2 and 1.9).
 */
template<typename G, typename B, typename U64>
class Sampler
{
private:
	double mu, sigma, sigma0;
	G& gaussian0_sampler;
	B& bit_sampler;
	U64& u64_sampler;
	uint64_t rejections;
	int s;
	double r, dss, d2s0s0;
public:
	Sampler(double mu, double sigma, double sigma0, G& gaussian0_sampler, B& bit_sampler, U64& u64_sampler) :
		mu(mu), sigma(sigma), sigma0(sigma0), gaussian0_sampler(gaussian0_sampler), bit_sampler(bit_sampler), u64_sampler(u64_sampler), rejections(0)
	{
	        /*
	         * The bimodal Gaussian used for rejection sampling requires sigma <= sigma0.
	         */
	        /* assert(sigma < sigma0); */
	
	        /*
	         * Center is mu. We compute mu = s + r where s is an integer
	         * and 0 <= r < 1.
	         */
	        s = (long)floor(mu);
	        r = mu - (double)s;
	
	        dss = 1.0 / (2 * sigma * sigma);
	        d2s0s0 = 1.0 / (2 * sigma0 * sigma0);
	}
public:
	inline int operator()()
	{
	        /*
	         * We now need to sample on center r.
	         */
	        for (;;) {
	                int z, b;
	                double x;
	
	                /*
	                 * Sample z for a Gaussian distribution. Then get a
	                 * random bit b to turn the sampling into a bimodal
	                 * distribution: if b = 1, we use z+1, otherwise we
	                 * use -z. We thus have two situations:
	                 *
	                 *  - b = 1: z >= 1 and sampled against a Gaussian
	                 *    centered on 1.
	                 *  - b = 0: z <= 0 and sampled against a Gaussian
	                 *    centered on 0.
	                 */
	                z = gaussian0_sampler();
	                //printf(" %d ",z);
	                b = bit_sampler();
	                z = b + ((b << 1) - 1) * z;
	
	                /*
	                 * Rejection sampling. We want a Gaussian centered on r;
	                 * but we sampled against a Gaussian centered on b (0 or
	                 * 1). But we know that z is always in the range where
	                 * our sampling distribution is greater than the Gaussian
	                 * distribution, so rejection works.
	                 *
	                 * We got z with distribution:
	                 *    G(z) = exp(-((z-b)^2)/(2*sigma0^2))
	                 * We target distribution:
	                 *    S(z) = exp(-((z-r)^2)/(2*sigma^2))
	                 * Rejection sampling works by keeping the value z with
	                 * probability S(z)/G(z), and starting again otherwise.
	                 * This requires S(z) <= G(z), which is the case here.
	                 * Thus, we simply need to keep our z with probability:
	                 *    P = exp(-x)
	                 * where:
	                 *    x = ((z-r)^2)/(2*sigma^2) - ((z-b)^2)/(2*sigma0^2)
	                 *
	                 * Note that z and b are integer, and we set sigma0 = 2.
	                 */
	                x = (z - r) * (z - r) * dss;
	                x = x - ((z - b) * (z - b) * d2s0s0);
	                //printf("In the for loop\n");
	                if (BerExp(x, u64_sampler(), u64_sampler())) {
	                        //printf("In the if %d\n",if_count);
	                        //if_count++;
	                        /*
	                         * Rejection sampling was centered on r, but the
	                         * actual center is mu = s + r.
	                         */
	                        return s + z;
	                }
			++rejections;
	        }
	}
	inline uint64_t get_rejections() const {
		return rejections;
	}
};

#define HGSAMPLER_CLASS_SUFFIX() _S2_N5
#define HGSAMPLER_INCLUDE "gaussian_s2_n5.inl"
#define HGSAMPLER_SIGMA 2
#define HGSAMPLER_N_OUT 5
#include "hgsampler.inl"

template<typename HGSampler>
class GaussianSampler
{
private:
	HGSampler & half_sampler;
	BitSampler & bit_sampler;
public:
	GaussianSampler(HGSampler & half_sampler, BitSampler & bit_sampler) :
		half_sampler(half_sampler), bit_sampler(bit_sampler)
	{
	}
	inline int operator()()
	{
		int z, b;
                z = half_sampler();
		b = bit_sampler();
		z = ((b << 1) - 1) * z; // full Gaussian
		return z;
	}
};

template<typename HGSampler>
class PlainGaussianSampler
{
public:
	HGSampler half_gaussian_sampler;
	BitSampler bit_sampler;
	GaussianSampler<HGSampler> sampler;
public:
	PlainGaussianSampler() :
		sampler(half_gaussian_sampler, bit_sampler)
	{
	}
public:
	inline int operator()()
	{
		return sampler();
	}
	inline uint64_t get_rejections() const {
		return 0;
	}
};

template<typename HGSampler>
class AdjustedGaussianSampler
{
private:
	HGSampler half_gaussian0_sampler;
	BitSampler bit_sampler;
	U64Sampler u64_sampler;
	GaussianSampler<HGSampler> gaussian0_sampler;
	Sampler<GaussianSampler<HGSampler>, BitSampler, U64Sampler> sampler;
public:
	AdjustedGaussianSampler(double mu, double sigma) :
		gaussian0_sampler(half_gaussian0_sampler, bit_sampler),
		sampler(mu, sigma, HGSampler::sigma, gaussian0_sampler, bit_sampler, u64_sampler)
	{
	}
public:
	inline int operator()()
	{
		return sampler();
	}
	inline uint64_t get_rejections() const {
		return sampler.get_rejections();
	}
};


} // namespace GaussSplit


#endif // __GAUSS_SPLIT_GSAMPLER_HPP_
