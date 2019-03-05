//compile with shake :  gcc -Wall -Wextra -O3 -fomit-frame-pointer -march=native -o sample master_falcon.c

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"randombytes.c"
#include"fips202.c"
#include "cpucycles.c"

#define OPT_TEST //To test the most optimized sampler

//#define MID_TEST // To test the medium espresso-only sampler

//#define NO_TEST // To test the most basic constant-time sampler

#ifdef OPT_TEST
	#include"c_sample.c"
#endif
#ifdef MID_TEST
	#include"c_sample1.c"
#endif
#ifdef NO_TEST
	#include"c_sample2.c"
#endif


unsigned long long int hist[HIST_LIMIT];

//-------shake------
void initRandom(uint8_t *x, uint32_t len){

	randombytes(x,len);
}
//-------shake ends-

int main(){

	int64_t i,j,k;
	uint64_t bit[IP_BIT];// to hold the input random bits
        static int sample[64];// this is fixed for 64 bit word
	//uint64_t out[NUM_OP_BITS], out_t[NUM_OP_BITS];
	
	volatile unsigned long long int clock1, clock2,clock3,clock4,clock5;
	clock1=0;clock2=0;clock3=0;clock4=0;clock5=0;
	
	//-------------------shake--------------------

	uint8_t seed[32];
	uint32_t SEEDBYTES=32;
	uint64_t s[25];
	uint8_t temp_byte[SHAKE128_RATE];
	uint32_t nrounds=KECCAK_ROUND;
 	unsigned char* byte_bank = (unsigned char*) ((void*) bit);

	for (i = 0; i < 25; ++i)
	    s[i] = 0;
	initRandom(seed,SEEDBYTES);
	keccak_absorb(s, SHAKE128_RATE, seed, SEEDBYTES, 0x1F);
	

	//-------------------shake ends---------------
	//unsigned long long int disable_update,control;
	
	for(i=0;i<HIST_LIMIT;i++)
		hist[i]=0;

	/*for(i=0;i<10;i++)
		segment[i]=0;
	*/

	clock3=0;
	clock5=0;
	int repeat=1000000;
	for(i=0;i<repeat;i++){	

		/*for(k=0;k<NUM_OP_BITS;k++){
			out[k]=out[k]^out[k];
			out_t[k]=out_t[k]^out_t[k];
		}*/

		clock1=0;
		clock2=0;
		//clock1 = cpucycles();
//------------------sha-------------------------
		/*for (m=0; m<data_length; m++)
		{
		    	if (current_seed1[m]!=255)
		    	{
		    		for (l=0; l<m; l++) current_seed1[l] = 0;
		    		current_seed1[m]++;
		    		break;
		    	}
	    	}*/
//----------------sha ends----------------------

	clock4 = cpucycles();

//-------------------shake----------------------
	keccak_squeezeblocks(byte_bank, nrounds, s, SHAKE128_RATE);
		
	keccak_squeezeblocks(temp_byte, 1, s, SHAKE128_RATE);

	for(j=0;j<(IP_BIT*8-SHAKE128_RATE*nrounds);j++){
		byte_bank[nrounds*SHAKE128_RATE+j]=temp_byte[j];			
	}

//-------------------shake ends-----------------


		clock1 = cpucycles();



//-----------------function starts----------------
	const_sample(bit, sample);
//-----------------function ends-----------------

//-----------------------------------------------------------------------------------------------------------

		clock2 = cpucycles();

		clock3=clock3+(clock2-clock1);
		clock5=clock5+(clock2-clock4);

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

		

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

		//printf("%lld%lld%lld%lld%lld%lld%lld%lld \n",bit[7]&1,bit[6]&1,bit[5]&1,bit[4]&1,out[3]&1,out[2]&1,out[1]&1,out[0]&1);
		//printf("%ld %lld : %lld %lld %lld %lld %lld %lld\n",i,clock3,out[4]&1,out[3]&1,out[2]&1,out[1]&1,out[0]&1);
		for(k=0;k<64;k++){
			hist[sample[k]]++;
			printf("  %d  ,",sample[k]);
		}
		printf("\n %ld %lld\n",i,clock3);

		//if(disable_update!=0xffffffffffffffff)//even if the sample is not found
			//break;
	}

	printf("Time (excluding random number geenration): %lld\n",clock3/repeat);
	printf("Time (including random number geenration): %lld\n",clock5/repeat);
	//printf("\nbit : %d\n",lsb);
//---------------------------print the histogram------------------------------
	for(i=0;i<HIST_LIMIT;i++)
		printf(" %ld : %llu",i,hist[i]);
//--------------------------print the histogram ends--------------------------

	printf("\n");
	return 1;
}








