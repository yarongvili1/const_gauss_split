//compile with shake :  gcc -Wall -Wextra -O3 -fomit-frame-pointer -march=native -o sample sample_opt.c

#ifdef __cplusplus
	#define restrict __restrict__
	extern "C" {
#endif

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"randombytes.c"
#include "cpucycles.c"

#include "internal.h"
#include "aes_rnd.c"

#ifdef __cplusplus
	}
#endif

/*
 * Sample a bit with probability exp(-x) for some x >= 0.
 */
static int
BerExp(fpr x, uint64_t rnd1, uint64_t rnd2)
{
        int s;
        fpr r;
        uint64_t w, z;
        int b;
        uint32_t sw;

        /*
         * Reduce x modulo log(2): x = s*log(2) + r, with s an integer,
         * and 0 <= r < log(2).
         */
        s = fpr_floor(fpr_div(x, fpr_log2));
        r = fpr_sub(x, fpr_mul(fpr_of(s), fpr_log2));

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
         * we can use fpr_exp_small(). The value is lower than 1; we
         * scale it to 2^55.
         * With combine (with AND) that bit with the previous bit, which
         * yields the expected result.
         */
        z = (uint64_t)fpr_rint(fpr_mul(fpr_exp_small(fpr_neg(r)), fpr_p55));
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
	fpr mu, sigma, sigma0;
	G& gaussian0_sampler;
	B& bit_sampler;
	U64& u64_sampler;
	uint64_t rejections;
	int s;
	fpr r, dss, d2s0s0;
public:
Sampler(fpr mu, fpr sigma, fpr sigma0, G& gaussian0_sampler, B& bit_sampler, U64& u64_sampler) :
	mu(mu), sigma(sigma), sigma0(sigma0), gaussian0_sampler(gaussian0_sampler), bit_sampler(bit_sampler), u64_sampler(u64_sampler), rejections(0)
{
        /*
         * The bimodal Gaussian used for rejection sampling requires sigma <= sigma0.
         */
        /* assert(fpr_lt(sigma, sigma0)); */

        /*
         * Center is mu. We compute mu = s + r where s is an integer
         * and 0 <= r < 1.
         */
        s = fpr_floor(mu);
        r = fpr_sub(mu, fpr_of(s));

        /*
         * dss = 1/(2*sigma^2).
         */
        dss = fpr_inv(fpr_mul(fpr_sqr(sigma), fpr_of(2)));
        /*
         * d2s0s0 = 1/(2*sigma0^2).
         */
        d2s0s0 = fpr_inv(fpr_mul(fpr_sqr(sigma0), fpr_of(2)));
}
public:
int operator()()
{
        /*
         * We now need to sample on center r.
         */
        for (;;) {
                int z, b;
                fpr x;

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
                x = fpr_mul(fpr_sqr(fpr_sub(fpr_of(z), r)), dss);
                x = fpr_sub(x, fpr_mul(fpr_of((z - b) * (z - b)), d2s0s0));
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
uint64_t get_rejections() const {
	return rejections;
}
};

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


#define ZERO 0x0000000000000000 
#define BITS_PER_SAMPLE 5

#define NHIST (1 << BITS_PER_SAMPLE)
unsigned long long int hist[NHIST];

#define NSAMPLES ((int)(sizeof(Bvec)*8))


class HalfGaussianSampler
{
private:
	Bvec bit[128];// to hold the bits
	Bvec out[BITS_PER_SAMPLE], out_t[BITS_PER_SAMPLE];
	int sample[NSAMPLES];
	int cursor;
private:
	uint8_t	 sample_o[NSAMPLES] ALIGN;
 	Bvec *sample_n = (Bvec*) ((void*) sample_o);
	Bvec sample_t;
	Bvec mask=BMASK;
	Bvec bzero = BZERO;
	Bvec bones = BONES;
	long int nrotate = (long int)(sizeof(sample_o)/sizeof(Bvec));
private:
void fill()
{
	long int j,k;
	Bvec disable_update,control;

//	memset(out,0,BITS_PER_SAMPLE*sizeof(*out));
//	memset(out_t,0,BITS_PER_SAMPLE*sizeof(*out_t));
	
	for(j=0;j<(int)sizeof(bit);j+=16){
		aes_random_bytes(((uint8_t *)bit) + j);
	}

	disable_update=bzero; //0-> to update every time. 1-> don't update anymore. Once switched to 1 stays at 1
	control=bones; //used to control the disable_update properly

#include "../../Gaussian_tool/Tool/combined.out"

		for(k=0;k<nrotate;k++){//if sample_o is 8 bits it should rotate 8 times if sample_o is 16 bit then the loop should rotate 16 times 	
				  //At a time 8 samples will be filled up. So we need the loop to iterate 8 times to fill all the 64 samples.
			
				sample_t=bzero;
				for(j=BITS_PER_SAMPLE-1;j>=0;j--) {
					sample_t=(sample_t<<1) | (out[j]&mask);
				}
				sample_n[k]=sample_t;
				
								

				for(j=BITS_PER_SAMPLE-1;j>=0;j--) {
					out[j]=out[j]>>1;
				}

		}

		for(k=0;k<NSAMPLES;k++){
			sample[k]=(int)sample_o[k];
		}

	cursor = 0;
}
public:
	HalfGaussianSampler()
	{
		fill();
	}
	int operator()()
	{
		int r = sample[cursor];
		if (++cursor == NSAMPLES) fill();
		return r;
	}
};

class GaussianSampler
{
private:
	HalfGaussianSampler & half_sampler;
	BitSampler & bit_sampler;
public:
	GaussianSampler(HalfGaussianSampler & half_sampler, BitSampler & bit_sampler) :
		half_sampler(half_sampler), bit_sampler(bit_sampler)
	{
	}
	int operator()()
	{
		int z, b;
                z = half_sampler();
		b = bit_sampler();
		z = ((b << 1) - 1) * z; // full Gaussian
		return z;
	}
};

void sampler1()
{
	long int i;
	int sample;
	HalfGaussianSampler half_gaussian0_sampler;
	BitSampler bit_sampler;
	U64Sampler u64_sampler;
	GaussianSampler gaussian0_sampler(half_gaussian0_sampler, bit_sampler);
	Sampler<GaussianSampler, BitSampler, U64Sampler> sampler(fpr_of(0), fpr_of(1.9), fpr_of(2), gaussian0_sampler, bit_sampler, u64_sampler);

	int repeat=100000;
	volatile unsigned long long int clock1, clock2, clock3;
	clock3 = 0;
	clock1 = cpucycles();
	for(i=0;i<repeat*NSAMPLES;i++){	
		sample = sampler();
		hist[sample + (NHIST / 2)]++;
	}
	clock2 = cpucycles();
	clock3 += clock2 - clock1;
	printf("Number of rejections: %lu\n",sampler.get_rejections());
	printf("Time for all samples: %lld\n",clock3);
	printf("Number of samples: %d\n",repeat*NSAMPLES);
	printf("Time per sample: %lld\n",clock3/repeat/NSAMPLES);
	for(i=0;i<NHIST;i++) {
		printf(" %ld : %llu",i - (NHIST / 2),hist[i]);
	}
	printf("\n");
}


int main(){
	sampler1();
	return 1;
}
