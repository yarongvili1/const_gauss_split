#if !defined(HGSAMPLER_CLASS_SUFFIX) || !defined(HGSAMPLER_INCLUDE) || !defined(HGSAMPLER_SIGMA) || !defined(HGSAMPLER_N_OUT)
	#error "HGSAMPLER_CLASS_SUFFIX, HGSAMPLER_INCLUDE and HGSAMPLER_SIGMA and HGSAMPLER_N_OUT macros must be defined"
#endif

#define BITS_PER_SAMPLE HGSAMPLER_N_OUT
#define HGSAMPLER_CONCAT(x,y) x##y
#define HGSAMPLER_CONCAT2(x,y) HGSAMPLER_CONCAT(x,y)
#define HGSAMPLER_CLASS_NAME HGSAMPLER_CONCAT2(HalfGaussianSampler,HGSAMPLER_CLASS_SUFFIX())

class HGSAMPLER_CLASS_NAME
{
public:
	static constexpr double sigma = HGSAMPLER_SIGMA;
	static constexpr int BitsPerSample = BITS_PER_SAMPLE;
private:
	Bvec bit[128];// to hold the bits
	Bvec out[BITS_PER_SAMPLE], out_t[BITS_PER_SAMPLE];
	int sample[NSAMPLES];
	int cursor;
private:
	uint8_t	 sample_o[NSAMPLES] ALIGN;
 	Bvec *sample_n = (Bvec*) ((void*) sample_o);
	Bvec sample_t;
	Bvec mask = BMASK;
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

#include HGSAMPLER_INCLUDE

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
	HGSAMPLER_CLASS_NAME()
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

#undef BITS_PER_SAMPLE
#undef HGSAMPLER_CONCAT
#undef HGSAMPLER_CONCAT2
#undef HGSAMPLER_CLASS_NAME

#undef HGSAMPLER_CLASS_SUFFIX
#undef HGSAMPLER_INCLUDE
#undef HGSAMPLER_SIGMA
#undef HGSAMPLER_N_OUT
