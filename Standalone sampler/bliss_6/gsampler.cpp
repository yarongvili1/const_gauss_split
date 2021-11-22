#include <stdio.h>
#include "gsampler.hpp"

using namespace GaussSplit;

typedef HalfGaussianSampler_S2_N5 HalfGaussianSampler;
static const int NHIST = (1 << (1 + HalfGaussianSampler::BitsPerSample));
unsigned long long int hist[NHIST];


template<typename Sampler>
void sample(Sampler &sampler)
{
	long int i,k;
	int sample;

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

void sample0()
{
	PlainGaussianSampler<HalfGaussianSampler> sampler;
	sample(sampler);
}

void sample1(double mu, double sigma)
{
	AdjustedGaussianSampler<HalfGaussianSampler> sampler(mu, sigma);
	sample(sampler);
}

int main(int argc, char **argv){
	if (argc < 3) {
		sample0();
		return 0;
	}
	double mu = atof(argv[1]);
	double sigma = atof(argv[2]);
	sample1(mu, sigma);
	return 1;
}
