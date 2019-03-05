//compile with shake :  gcc -Wall -Wextra -O3 -fomit-frame-pointer -march=native -o sample sample_opt.c

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"randombytes.c"
#include"fips202.c"
#include "cpucycles.c"


unsigned long long int hist[84];

#define ZERO 0x0000000000000000 
#define BITS_PER_SAMPLE 7

//-------shake------
void initRandom(uint8_t *x, uint32_t len){

	randombytes(x,len);
}
//-------shake ends-

int main(){

	long int i,j,k;
	unsigned long long int bit[128];// to hold the bits
        //unsigned long long int sample[64];
	int sample[64];
	uint64_t out[BITS_PER_SAMPLE], out_t[BITS_PER_SAMPLE];

	volatile unsigned long long int clock1, clock2,clock3,clock4,clock5;
	clock1=0;clock2=0;clock3=0;clock4=0;clock5=0;

	//----------------extract-------------

	uint8_t	 sample_o[64];
 	uint64_t *sample_n = (uint64_t*) ((void*) sample_o);
	uint64_t sample_t;
	uint64_t mask=(1UL | (1UL<<8) | (1UL<<16) | (1UL<<24) | (1UL<<32) | (1UL<<40)| (1UL<<48)| (1UL<<56));
	//----------------extract ends---------	

	//-------------------shake--------------------

	uint8_t seed[32];
	uint32_t SEEDBYTES=32;
	uint64_t s[25];
	uint8_t temp_byte[SHAKE128_RATE];
	uint32_t nrounds=6;
 	unsigned char* byte_bank = (unsigned char*) ((void*) bit);

	for (i = 0; i < 25; ++i)
	    s[i] = 0;
	initRandom(seed,SEEDBYTES);
	keccak_absorb(s, SHAKE128_RATE, seed, SEEDBYTES, 0x1F);
	

	//-------------------shake ends---------------
	unsigned long long int disable_update,control;
	
	for(i=0;i<84;i++)
		hist[i]=0;

	/*for(i=0;i<10;i++)
		segment[i]=0;
	*/

	clock3=0;clock5=0;
	int repeat=1000000;
	for(i=0;i<repeat;i++){	

		memset(out,0,BITS_PER_SAMPLE*sizeof(uint64_t));
		memset(out_t,0,BITS_PER_SAMPLE*sizeof(uint64_t));
		

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

	for(j=0;j<(128*8-SHAKE128_RATE*nrounds);j++){
		byte_bank[nrounds*SHAKE128_RATE+j]=temp_byte[j];			
	}

//-------------------shake ends-----------------

		clock1 = cpucycles();

//-----------------------------------------------------------------------------------------------------------

		disable_update=0; //0-> to update every time. 1-> don't update anymore. Once switched to 1 stays at 1
		control=0xffffffffffffffff; //used to control the disable_update properly
/*--------------------------------*/


		//----------------0--------------
		out_t[2] = (~bit[2]&bit[1]) | (bit[2]&~bit[1]);

		out_t[1] = (bit[2]);

		out_t[0] = (~bit[3]) | (~bit[2]&~bit[1]);

		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[0];
		disable_update=disable_update|(~control);
		//----------------1--------------

		out_t[3] = (bit[3]&~bit[2]) | (~bit[4]&~bit[3]&bit[2]);

		out_t[2] = (bit[4]&~bit[3]&bit[2]);

		out_t[1] = (~bit[4]&bit[3]) | (bit[3]&bit[2]);

		out_t[0] = (~bit[4]&bit[3]&bit[2]) | (bit[4]&bit[3]&~bit[2]);

		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[1];
		disable_update=disable_update|(~control);
		//----------------2--------------


		out_t[3] = (~bit[5]&~bit[3]) | (~bit[4]&~bit[3]) | (bit[5]&bit[4]&bit[3]);

		out_t[2] = (bit[5]&bit[4]) | (~bit[5]&~bit[4]);

		out_t[1] = (bit[5]&~bit[4]) | (bit[5]&~bit[3]) | (~bit[5]&bit[4]&bit[3]) | (
		    ~bit[6]&bit[4]&bit[3]);

		out_t[0] = (bit[5]&~bit[4]) | (~bit[4]&bit[3]) | (bit[6]&bit[5]&bit[3]);

		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[2];
		disable_update=disable_update|(~control);
		//----------------3--------------
		out_t[4] = (~bit[7]&~bit[6]&bit[5]&bit[4]);

		out_t[3] = (~bit[6]&~bit[4]) | (~bit[5]&~bit[4]) | (bit[6]&bit[5]&bit[4]) | (
		    bit[7]&bit[5]&bit[4]);

		out_t[2] = (~bit[7]&bit[6]&bit[5]) | (bit[7]&~bit[6]&bit[4]) | (~bit[6]
		    &~bit[5]&bit[4]) | (bit[6]&bit[5]&~bit[4]);

		out_t[1] = (bit[6]&~bit[5]&bit[4]) | (~bit[6]&~bit[5]&~bit[4]) | (bit[7]
		    &~bit[6]&bit[5]&bit[4]);

		out_t[0] = (bit[6]&bit[5]) | (~bit[5]&~bit[4]) | (bit[7]&bit[5]&bit[4]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[3];
		disable_update=disable_update|(~control);
		//----------------4--------------
		out_t[4] = (~bit[8]&~bit[7]&~bit[6]&bit[5]);

		out_t[3] = (~bit[7]&bit[6]&bit[5]) | (bit[7]&~bit[6]&bit[5]) | (bit[8]
		    &~bit[6]&bit[5]);

		out_t[2] = (bit[8]&~bit[6]) | (bit[7]&bit[5]) | (~bit[6]&~bit[5]);

		out_t[1] = (bit[8]&~bit[7]) | (~bit[8]&bit[6]) | (~bit[7]&~bit[5]) | (
		    bit[6]&~bit[5]);

		out_t[0] = (~bit[8]&~bit[7]) | (~bit[8]&~bit[6]) | (~bit[7]&~bit[6]) | (
		    ~bit[7]&~bit[5]) | (~bit[6]&~bit[5]) | (bit[8]&bit[7]&bit[6]&bit[5]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[4];
		disable_update=disable_update|(~control);
		//----------------5--------------
		out_t[4] = (bit[8]&bit[7]&~bit[6]);

		out_t[3] = (~bit[7]&bit[6]) | (~bit[9]&~bit[8]&bit[6]);

		out_t[2] = (~bit[8]&~bit[7]) | (bit[8]&bit[7]&bit[6]) | (bit[9]&bit[7]
		    &bit[6]);

		out_t[1] = (~bit[9]&~bit[7]&bit[6]) | (bit[8]&bit[7]&~bit[6]) | (bit[9]
		    &~bit[8]&bit[7]&bit[6]);

		out_t[0] = (~bit[9]&bit[8]&bit[7]) | (bit[9]&bit[8]&~bit[7]) | (bit[9]
		    &~bit[8]&bit[7]&bit[6]) | (bit[8]&~bit[7]&~bit[6]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[5];
		disable_update=disable_update|(~control);
		//----------------6--------------
		out_t[4] = (bit[8]&~bit[7]);

		out_t[3] = (~bit[8]&bit[7]) | (~bit[10]&~bit[9]&bit[7]);

		out_t[2] = (~bit[10]&bit[9]&bit[7]) | (bit[10]&~bit[9]&bit[7]) | (
		    ~bit[10]&~bit[9]&bit[8]&~bit[7]) | (~bit[9]&~bit[8]&bit[7]);

		out_t[1] = (bit[10]&~bit[9]) | (~bit[9]&~bit[8]) | (~bit[8]&~bit[7]) | (
		    bit[9]&bit[8]&bit[7]) | (bit[10]&bit[7]);

		out_t[0] = (bit[10]&bit[9]&bit[7]) | (~bit[9]&~bit[8]&~bit[7]) | (
		    ~bit[10]&bit[9]&bit[8]&~bit[7]) | (~bit[10]&~bit[9]&bit[7]) | (
		    ~bit[9]&bit[8]&bit[7]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[6];
		disable_update=disable_update|(~control);
		//----------------7--------------
		out_t[4] = (~bit[11]&bit[9]&~bit[8]) | (~bit[10]&bit[9]&~bit[8]);

		out_t[3] = (~bit[9]&bit[8]) | (~bit[11]&~bit[10]&bit[8]) | (bit[11]
		    &bit[10]&bit[9]&~bit[8]);

		out_t[2] = (~bit[11]&bit[10]&bit[8]) | (bit[11]&~bit[10]&bit[8]) | (
		    bit[11]&bit[10]&bit[9]&~bit[8]) | (~bit[11]&~bit[10]&bit[9]&~bit[8]) | (
		    ~bit[10]&~bit[9]&bit[8]);

		out_t[1] = (bit[11]&bit[10]&bit[8]) | (~bit[11]&~bit[10]&bit[8]) | (
		    bit[11]&bit[9]&~bit[8]) | (~bit[10]&~bit[9]&~bit[8]);

		out_t[0] = (bit[11]&bit[8]) | (bit[10]&~bit[8]) | (~bit[11]&bit[9]&~bit[8]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[7];
		disable_update=disable_update|(~control);
		//----------------8--------------

		out_t[4] = (~bit[12]&~bit[10]&bit[9]) | (~bit[11]&~bit[10]&bit[9]) | (
		    bit[11]&bit[10]&~bit[9]);

		out_t[3] = (~bit[11]&bit[10]&bit[9]) | (bit[12]&bit[11]&~bit[10]&bit[9]);

		out_t[2] = (bit[11]&bit[10]) | (bit[12]&bit[11]&bit[9]) | (~bit[12]
		    &~bit[11]&~bit[10]&bit[9]);

		out_t[1] = (~bit[11]&bit[10]&bit[9]) | (bit[12]&~bit[10]&bit[9]) | (
		    ~bit[11]&~bit[10]&~bit[9]) | (~bit[12]&bit[11]&bit[10]&~bit[9]);

		out_t[0] = (~bit[12]&bit[10]&bit[9]) | (bit[12]&bit[11]&~bit[9]) | (
		    bit[12]&~bit[11]&~bit[10]&bit[9]) | (~bit[12]&bit[11]&~bit[10]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[8];
		disable_update=disable_update|(~control);
		//----------------9--------------

		out_t[4] = (bit[11]&~bit[10]);

		out_t[3] = (~bit[11]&bit[10]) | (~bit[13]&~bit[12]&bit[11]);

		out_t[2] = (bit[13]&~bit[12]&bit[11]) | (~bit[13]&~bit[11]&bit[10]) | (
		    ~bit[13]&bit[12]&bit[11]&~bit[10]) | (~bit[12]&~bit[11]&bit[10]);

		out_t[1] = (~bit[12]&~bit[11]) | (~bit[13]&bit[12]&bit[11]) | (bit[13]
		    &~bit[11]&bit[10]) | (bit[13]&bit[11]&~bit[10]);

		out_t[0] = (~bit[13]&~bit[12]&bit[10]) | (bit[12]&bit[11]&bit[10]) | (
		    bit[12]&~bit[11]&~bit[10]) | (bit[13]&~bit[12]&bit[11]&~bit[10]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[9];
		disable_update=disable_update|(~control);
		//----------------10--------------
		out_t[4] = (bit[13]&~bit[11]) | (bit[12]&~bit[11]);

		out_t[3] = (~bit[12]&bit[11]) | (~bit[14]&bit[13]&~bit[12]);

		out_t[2] = (~bit[13]&bit[12]) | (~bit[13]&bit[11]) | (~bit[14]&bit[12]
		    &bit[11]) | (bit[14]&bit[13]&~bit[12]&~bit[11]);

		out_t[1] = (~bit[14]&~bit[12]&bit[11]) | (~bit[14]&bit[12]&~bit[11]) | (
		    bit[14]&bit[13]&bit[12]&bit[11]) | (bit[14]&bit[13]&~bit[12]&~bit[11]) | (
		    ~bit[14]&~bit[13]&bit[11]);

		out_t[0] = (bit[14]&bit[12]&bit[11]) | (bit[13]&~bit[12]&~bit[11]) | (
		    bit[14]&bit[13]&~bit[11]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[10];
		disable_update=disable_update|(~control);
		//----------------11--------------
		out_t[4] = (~bit[13]&bit[12]) | (bit[14]&bit[13]&~bit[12]);

		out_t[3] = (~bit[15]&bit[14]&bit[13]) | (~bit[14]&bit[13]&bit[12]);

		out_t[2] = (~bit[14]&bit[12]) | (bit[15]&bit[14]&bit[13]);

		out_t[1] = (bit[15]&bit[14]&bit[13]) | (bit[14]&bit[13]&~bit[12]) | (
		    ~bit[14]&~bit[13]&~bit[12]) | (~bit[15]&~bit[14]&bit[13]&bit[12]);

		out_t[0] = (bit[15]&bit[13]&bit[12]) | (~bit[15]&~bit[13]&bit[12]) | (
		    bit[14]&bit[13]&bit[12]) | (bit[15]&bit[14]&~bit[12]) | (bit[14]
		    &~bit[13]&~bit[12]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[11];
		disable_update=disable_update|(~control);
		//----------------12--------------
		out_t[4] = (~bit[16]&~bit[14]&bit[13]) | (~bit[15]&~bit[14]&bit[13]);

		out_t[3] = (~bit[16]&~bit[15]&bit[13]) | (~bit[15]&bit[14]&bit[13]) | (
		    bit[16]&bit[15]&~bit[14]&bit[13]);

		out_t[2] = (bit[15]&bit[14]&bit[13]) | (~bit[15]&~bit[14]&~bit[13]) | (
		    bit[16]&~bit[14]&bit[13]);

		out_t[1] = (bit[15]&~bit[14]&~bit[13]) | (~bit[16]&~bit[15]&bit[13]) | (
		    bit[15]&bit[14]&bit[13]) | (bit[16]&~bit[14]&bit[13]);

		out_t[0] = (~bit[15]&~bit[14]) | (~bit[15]&~bit[13]) | (~bit[14]&~bit[13]) | (
		    ~bit[16]&bit[14]&bit[13]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[12];
		disable_update=disable_update|(~control);
		//----------------13--------------
		out_t[4] = (~bit[15]&bit[14]) | (bit[16]&bit[15]&~bit[14]);

		out_t[3] = (~bit[17]&~bit[16]&bit[15]&bit[14]) | (~bit[17]&bit[16]
		    &bit[15]&~bit[14]);

		out_t[2] = (~bit[16]&~bit[15]) | (~bit[16]&bit[14]) | (bit[16]&bit[15]
		    &~bit[14]) | (~bit[17]&bit[15]&bit[14]);

		out_t[1] = (~bit[17]&~bit[15]&bit[14]) | (bit[17]&bit[16]&~bit[14]) | (
		    bit[17]&~bit[16]&bit[15]&bit[14]) | (bit[16]&~bit[15]&~bit[14]);

		out_t[0] = (~bit[16]&bit[15]) | (bit[16]&~bit[15]) | (bit[17]&~bit[14]) | (
		    ~bit[15]&~bit[14]) | (~bit[17]&bit[15]&bit[14]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[13];
		disable_update=disable_update|(~control);
		//----------------14--------------
		out_t[4] = (~bit[15]) | (~bit[18]&~bit[17]&~bit[16]);

		out_t[3] = (bit[17]&~bit[16]) | (~bit[16]&~bit[15]) | (~bit[17]&bit[16]
		    &bit[15]) | (bit[18]&~bit[17]&bit[15]);

		out_t[2] = (~bit[18]&~bit[17]&~bit[15]) | (~bit[17]&bit[16]&~bit[15]) | (
		    ~bit[18]&bit[17]&~bit[16]&bit[15]) | (bit[18]&~bit[17]&~bit[16]
		    &bit[15]);

		out_t[1] = (~bit[18]&bit[16]) | (bit[18]&~bit[17]&~bit[16]) | (bit[18]
		    &bit[17]&bit[15]);

		out_t[0] = (~bit[18]&~bit[15]) | (bit[16]&~bit[15]) | (~bit[18]&bit[17]
		    &bit[16]) | (bit[18]&~bit[16]&bit[15]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[14];
		disable_update=disable_update|(~control);
		//----------------15--------------

		out_t[4] = (bit[18]&bit[17]) | (bit[18]&~bit[16]) | (bit[17]&~bit[16]) | (
		    ~bit[18]&~bit[17]&bit[16]) | (~bit[19]&~bit[17]&bit[16]);

		out_t[3] = (bit[19]&bit[18]&~bit[17]) | (bit[18]&~bit[17]&~bit[16]) | (
		    ~bit[19]&bit[18]&bit[17]&bit[16]) | (~bit[19]&~bit[18]&bit[17]
		    &~bit[16]) | (~bit[20]&bit[18]&bit[17]&bit[16]);

		out_t[2] = (~bit[20]&~bit[19]&bit[18]&bit[17]) | (bit[20]&bit[19]&bit[18]
		    &bit[16]) | (bit[19]&bit[18]&~bit[17]&bit[16]) | (~bit[19]&bit[18]
		    &bit[17]&~bit[16]) | (bit[19]&~bit[18]&bit[17]&~bit[16]);

		out_t[1] = (bit[20]&bit[19]&bit[18]&bit[17]) | (~bit[19]&~bit[18]&~bit[17]
		    &bit[16]) | (bit[19]&bit[18]&bit[17]&~bit[16]) | (~bit[19]&bit[18]
		    &~bit[17]&~bit[16]) | (~bit[20]&~bit[19]&bit[18]&bit[17]&bit[16]);

		out_t[0] = (bit[19]&bit[17]&~bit[16]) | (bit[18]&~bit[17]&~bit[16]) | (
		    bit[20]&~bit[19]&bit[17]&bit[16]) | (~bit[19]&~bit[18]&bit[17]
		    &bit[16]) | (bit[19]&~bit[18]&~bit[17]&bit[16]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[15];
		disable_update=disable_update|(~control);
		//----------------16--------------
		out_t[4] = (~bit[18]&~bit[17]);

		out_t[3] = (bit[18]&~bit[17]) | (~bit[20]&~bit[18]&bit[17]) | (~bit[19]
		    &~bit[18]&bit[17]);

		out_t[2] = (bit[18]&~bit[17]) | (~bit[20]&~bit[19]&bit[18]) | (~bit[20]
		    &~bit[19]&~bit[17]) | (bit[20]&bit[19]&~bit[18]&bit[17]);

		out_t[1] = (bit[20]&~bit[19]&bit[18]) | (bit[20]&~bit[19]&~bit[17]) | (
		    ~bit[19]&bit[18]&~bit[17]) | (bit[20]&bit[19]&~bit[18]&bit[17]) | (
		    ~bit[20]&~bit[19]&~bit[18]&bit[17]) | (~bit[20]&bit[19]&~bit[18]
		    &~bit[17]);

		out_t[0] = (~bit[20]&bit[18]) | (~bit[19]&~bit[18]);

		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[16];
		disable_update=disable_update|(~control);
		//----------------17--------------

		out_t[5] = (~bit[22]&bit[21]&bit[20]&bit[19]&bit[18]);

		out_t[4] = (~bit[18]) | (bit[22]&bit[21]&bit[20]&bit[19]);

		out_t[3] = (~bit[21]&~bit[19]) | (~bit[20]&~bit[19]) | (~bit[19]&~bit[18]) | (
		    bit[22]&bit[21]&bit[20]&bit[19]&bit[18]);

		out_t[2] = (~bit[21]&~bit[20]) | (~bit[20]&~bit[18]) | (bit[21]&~bit[19]
		    &bit[18]) | (~bit[21]&~bit[19]&~bit[18]) | (bit[22]&bit[21]&bit[20]
		    &bit[18]);

		out_t[1] = (~bit[20]&~bit[18]) | (bit[21]&~bit[20]&bit[19]) | (bit[20]
		    &~bit[19]&bit[18]) | (~bit[21]&bit[20]&bit[19]) | (~bit[21]&~bit[19]
		    &bit[18]) | (bit[22]&bit[21]&bit[19]&bit[18]);

		out_t[0] = (bit[20]&~bit[19]&bit[18]) | (~bit[21]&~bit[20]&~bit[18]) | (
		    bit[21]&~bit[20]&bit[19]&bit[18]) | (~bit[21]&~bit[19]&bit[18]) | (
		    bit[22]&bit[21]&bit[19]&bit[18]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[17];
		disable_update=disable_update|(~control);
		//----------------18--------------

		out_t[5] = ZERO;

		out_t[4] = (~bit[21]&~bit[19]) | (~bit[20]&~bit[19]);

		out_t[3] = (~bit[21]&~bit[20]) | (bit[21]&bit[20]&~bit[19]) | (~bit[22]
		    &~bit[20]&~bit[19]);

		out_t[2] = (bit[21]&~bit[20]&bit[19]) | (bit[21]&bit[20]&~bit[19]) | (
		    ~bit[22]&~bit[21]&bit[20]&bit[19]) | (~bit[22]&~bit[21]&~bit[20]
		    &~bit[19]) | (bit[22]&bit[21]&~bit[19]);

		out_t[1] = (bit[21]&bit[20]&~bit[19]) | (~bit[21]&~bit[20]&~bit[19]) | (
		    bit[22]&~bit[21]&bit[20]&bit[19]) | (~bit[22]&bit[21]&bit[19]) | (
		    ~bit[22]&~bit[20]&bit[19]) | (~bit[22]&bit[20]&~bit[19]);

		out_t[0] = (bit[21]&~bit[20]&bit[19]) | (~bit[22]&bit[20]&~bit[19]) | (
		    bit[22]&~bit[21]&bit[20]) | (bit[22]&~bit[21]&bit[19]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[18];
		disable_update=disable_update|(~control);
		//----------------19--------------

		out_t[5] = (~bit[23]&bit[22]&bit[21]&bit[20]) | (~bit[23]&~bit[22]
		    &~bit[21]&~bit[20]);

		out_t[4] = (bit[23]&bit[22]&bit[21]&bit[20]) | (~bit[23]&bit[22]&~bit[20]) | (
		    ~bit[22]&bit[21]&~bit[20]) | (bit[23]&~bit[21]&~bit[20]);

		out_t[3] = (bit[23]&bit[22]) | (bit[23]&~bit[21]) | (bit[22]&~bit[21]) | (
		    ~bit[21]&bit[20]) | (~bit[23]&~bit[22]&bit[21]&~bit[20]);

		out_t[2] = (bit[23]&bit[22]&bit[21]) | (bit[23]&~bit[22]&~bit[20]) | (
		    ~bit[23]&~bit[22]&bit[21]&bit[20]) | (~bit[23]&bit[22]&~bit[21]
		    &~bit[20]);

		out_t[1] = (~bit[22]&~bit[21]&bit[20]) | (bit[23]&~bit[22]&~bit[20]) | (
		    ~bit[24]&bit[22]&bit[21]&bit[20]) | (~bit[23]&bit[22]&bit[21]&~bit[20]);

		out_t[0] = (bit[23]&bit[22]&bit[21]) | (~bit[23]&~bit[22]&~bit[21]) | (
		    ~bit[23]&~bit[21]&bit[20]) | (bit[23]&bit[22]&~bit[20]) | (bit[23]
		    &bit[21]&~bit[20]) | (bit[22]&bit[21]&~bit[20]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[19];
		disable_update=disable_update|(~control);
		//----------------20--------------

		out_t[5] = (~bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]);

		out_t[4] = (~bit[22]&~bit[21]) | (bit[23]&bit[22]&bit[21]) | (~bit[24]
		    &~bit[23]&~bit[21]) | (bit[25]&bit[24]&bit[22]&bit[21]);

		out_t[3] = (bit[23]&bit[22]) | (bit[25]&bit[24]&bit[22]) | (~bit[24]
		    &~bit[23]&~bit[22]) | (bit[24]&~bit[23]&~bit[21]);

		out_t[2] = (~bit[24]&~bit[22]&~bit[21]) | (bit[23]&~bit[22]&~bit[21]) | (
		    bit[25]&bit[24]&~bit[23]&bit[21]) | (~bit[24]&bit[23]&bit[22]&bit[21]) | (
		    bit[24]&~bit[23]&~bit[22]&bit[21]);

		out_t[1] = (bit[23]&~bit[22]&bit[21]) | (bit[24]&~bit[23]&~bit[21]) | (
		    bit[25]&bit[24]&~bit[23]&bit[22]) | (~bit[25]&bit[24]&bit[23]&bit[21]) | (
		    ~bit[24]&bit[23]&~bit[21]) | (~bit[24]&bit[22]&~bit[21]);

		out_t[0] = (~bit[24]&bit[23]&~bit[22]&bit[21]) | (bit[24]&~bit[23]
		    &~bit[22]&bit[21]) | (bit[24]&bit[23]&bit[22]) | (bit[25]&~bit[23]
		    &bit[22]) | (~bit[23]&bit[22]&~bit[21]) | (~bit[25]&~bit[24]&bit[22]
		    &bit[21]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[20];
		disable_update=disable_update|(~control);
		//----------------21--------------

		out_t[5] = (~bit[26]&~bit[24]&bit[23]&bit[22]) | (~bit[25]&~bit[24]
		    &bit[23]&bit[22]);

		out_t[4] = (~bit[23]&~bit[22]) | (bit[24]&bit[23]&bit[22]) | (~bit[25]
		    &~bit[24]&~bit[22]) | (bit[26]&bit[25]&bit[23]&bit[22]);

		out_t[3] = (~bit[25]&bit[24]&bit[23]) | (bit[25]&bit[23]&~bit[22]) | (
		    bit[26]&bit[25]&~bit[24]&bit[23]);

		out_t[2] = (~bit[25]&~bit[23]) | (~bit[24]&~bit[23]) | (bit[25]&bit[24]
		    &bit[23]) | (bit[26]&bit[25]&bit[23]) | (bit[25]&bit[23]&~bit[22]) | (
		    bit[24]&bit[23]&~bit[22]);

		out_t[1] = (bit[25]&bit[24]&~bit[23]) | (~bit[25]&~bit[24]&~bit[23]) | (
		    ~bit[26]&bit[25]&bit[24]&bit[22]) | (~bit[26]&~bit[25]&~bit[24]
		    &bit[22]) | (bit[25]&~bit[24]&bit[23]&~bit[22]) | (~bit[24]&~bit[23]
		    &bit[22]);

		out_t[0] = (bit[25]&bit[24]&bit[22]) | (~bit[24]&~bit[23]&~bit[22]) | (
		    ~bit[25]&bit[24]&bit[23]&~bit[22]) | (~bit[25]&~bit[24]&bit[22]) | (
		    ~bit[26]&~bit[25]&bit[23]&bit[22]) | (bit[26]&~bit[24]&bit[23]
		    &bit[22]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[21];
		disable_update=disable_update|(~control);
		//----------------22--------------

		out_t[5] = (~bit[25]&bit[24]&bit[23]);

		out_t[4] = (bit[25]&bit[24]&bit[23]) | (~bit[26]&~bit[24]&~bit[23]) | (
		    ~bit[25]&~bit[24]&~bit[23]);

		out_t[3] = (bit[25]&bit[24]&bit[23]) | (~bit[25]&bit[24]&~bit[23]) | (
		    bit[26]&bit[25]&~bit[24]&~bit[23]);

		out_t[2] = (bit[24]&~bit[23]) | (~bit[27]&~bit[26]&bit[24]) | (~bit[26]
		    &bit[25]&bit[24]) | (bit[26]&bit[25]&~bit[23]);

		out_t[1] = (~bit[26]&~bit[25]&~bit[24]) | (~bit[25]&~bit[24]&bit[23]) | (
		    bit[27]&~bit[26]&~bit[25]&bit[23]) | (~bit[27]&bit[25]&bit[24]
		    &bit[23]) | (~bit[26]&bit[25]&bit[24]&~bit[23]) | (bit[26]&bit[25]
		    &~bit[24]&~bit[23]);

		out_t[0] = (bit[27]&~bit[26]&~bit[25]) | (~bit[26]&~bit[24]&bit[23]) | (
		    ~bit[26]&~bit[25]&~bit[23]) | (bit[26]&~bit[24]&~bit[23]) | (~bit[27]
		    &bit[26]&bit[24]&bit[23]) | (bit[26]&bit[25]&bit[24]&bit[23]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[22];
		disable_update=disable_update|(~control);
		//----------------23--------------

		out_t[5] = (~bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]);

		out_t[4] = (~bit[25]&~bit[24]) | (bit[26]&bit[25]&bit[24]) | (bit[28]
		    &bit[27]&bit[25]&bit[24]);

		out_t[3] = (bit[26]&bit[25]) | (bit[25]&~bit[24]) | (~bit[26]&~bit[25]
		    &bit[24]) | (bit[28]&bit[27]&~bit[26]&bit[24]);

		out_t[2] = (~bit[27]&bit[25]) | (~bit[26]&~bit[24]) | (bit[28]&~bit[26]
		    &bit[25]) | (bit[26]&~bit[25]&bit[24]);

		out_t[1] = (~bit[27]&bit[26]&~bit[25]) | (~bit[27]&~bit[26]&~bit[24]) | (
		    ~bit[28]&bit[27]&bit[26]&bit[25]) | (bit[27]&bit[26]&bit[25]&~bit[24]) | (
		    bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]);

		out_t[0] = (bit[26]&~bit[25]) | (~bit[28]&bit[26]&bit[24]) | (bit[27]
		    &bit[26]&bit[24]) | (~bit[27]&~bit[25]&bit[24]) | (~bit[26]&bit[25]
		    &~bit[24]) | (~bit[28]&bit[27]&bit[25]&bit[24]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[23];
		disable_update=disable_update|(~control);
		//----------------24--------------

		out_t[5] = (~bit[27]&bit[26]&bit[25]);

		out_t[4] = (~bit[27]&~bit[25]) | (~bit[26]&~bit[25]) | (bit[27]&bit[26]
		    &bit[25]);

		out_t[3] = (bit[27]&bit[26]) | (~bit[28]&~bit[27]&~bit[26]&~bit[25]);

		out_t[2] = (~bit[28]&bit[27]) | (~bit[28]&bit[25]) | (~bit[29]&bit[27]
		    &bit[26]&bit[25]) | (bit[28]&~bit[27]&~bit[26]) | (bit[28]&~bit[26]
		    &~bit[25]);

		out_t[1] = (bit[28]&~bit[27]&~bit[26]) | (bit[28]&~bit[26]&bit[25]) | (
		    ~bit[27]&~bit[26]&bit[25]) | (bit[29]&bit[28]&bit[27]&bit[25]) | (
		    ~bit[29]&bit[28]&~bit[27]&bit[25]) | (~bit[28]&~bit[27]&bit[26]
		    &~bit[25]) | (~bit[28]&bit[27]&~bit[26]&~bit[25]) | (~bit[29]&~bit[28]
		    &bit[27]&bit[26]&bit[25]);

		out_t[0] = (~bit[28]&~bit[26]&bit[25]) | (bit[29]&~bit[28]&bit[27]
		    &bit[25]) | (bit[28]&~bit[27]&bit[26]&bit[25]) | (~bit[28]&~bit[27]
		    &bit[26]&~bit[25]) | (bit[28]&~bit[27]&~bit[26]&~bit[25]) | (~bit[29]
		    &~bit[27]&bit[26]&bit[25]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[24];
		disable_update=disable_update|(~control);
		//----------------25--------------

		out_t[5] = (~bit[29]&bit[28]&bit[27]&bit[26]) | (bit[29]&~bit[28]&bit[27]
		    &bit[26]);

		out_t[4] = (~bit[27]&~bit[26]) | (~bit[29]&~bit[28]&~bit[26]) | (bit[29]
		    &bit[28]&bit[27]&bit[26]);

		out_t[3] = (bit[29]&bit[28]&bit[27]) | (~bit[28]&~bit[27]&bit[26]) | (
		    bit[29]&bit[27]&~bit[26]) | (bit[28]&bit[27]&~bit[26]);

		out_t[2] = (bit[29]&~bit[28]&bit[27]) | (~bit[29]&bit[28]&~bit[27]) | (
		    ~bit[29]&bit[28]&~bit[26]) | (~bit[30]&bit[29]&bit[27]&bit[26]) | (
		    ~bit[28]&~bit[27]&~bit[26]);

		out_t[1] = (~bit[30]&bit[29]&bit[27]) | (bit[29]&bit[28]&bit[27]) | (
		    ~bit[29]&~bit[28]&~bit[27]) | (~bit[29]&~bit[27]&bit[26]) | (bit[29]
		    &bit[28]&~bit[26]) | (bit[29]&bit[27]&~bit[26]) | (~bit[30]&bit[28]
		    &bit[27]&bit[26]);

		out_t[0] = (bit[28]&~bit[27]&bit[26]) | (~bit[29]&bit[27]&~bit[26]) | (
		    bit[30]&bit[29]&bit[27]&bit[26]) | (bit[29]&~bit[28]&~bit[27]&~bit[26]) | (
		    bit[30]&bit[28]&bit[27]) | (bit[29]&bit[28]&bit[27]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[25];
		disable_update=disable_update|(~control);
		//----------------26--------------

		out_t[5] = (~bit[29]&bit[28]&bit[27]) | (bit[30]&bit[29]&~bit[28]&bit[27]);

		out_t[4] = (bit[29]&bit[28]&bit[27]) | (~bit[29]&~bit[28]&~bit[27]);

		out_t[3] = (bit[29]&bit[28]&bit[27]) | (~bit[30]&~bit[29]&~bit[27]) | (
		    bit[29]&~bit[28]&~bit[27]);

		out_t[2] = (bit[30]&~bit[27]) | (bit[29]&~bit[27]) | (~bit[30]&bit[29]
		    &bit[28]) | (bit[30]&bit[29]&~bit[28]);

		out_t[1] = (~bit[30]&~bit[29]&bit[27]) | (bit[29]&~bit[28]&~bit[27]) | (
		    bit[30]&~bit[29]&bit[28]&~bit[27]) | (~bit[30]&~bit[28]&~bit[27]) | (
		    ~bit[31]&bit[30]&bit[29]&bit[27]) | (~bit[31]&bit[29]&bit[28]&bit[27]);

		out_t[0] = (bit[30]&~bit[29]&~bit[28]) | (~bit[31]&bit[30]&bit[27]) | (
		    bit[30]&~bit[28]&bit[27]) | (~bit[30]&bit[29]&~bit[27]) | (~bit[30]
		    &bit[28]&~bit[27]) | (bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[31]
		    &~bit[29]&bit[28]&bit[27]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[26];
		disable_update=disable_update|(~control);
		//----------------27--------------

		out_t[5] = (~bit[31]&bit[30]&bit[29]&bit[28]) | (bit[31]&~bit[30]&bit[29]
		    &bit[28]) | (~bit[32]&bit[30]&bit[29]&bit[28]);

		out_t[4] = (~bit[30]&~bit[28]) | (~bit[29]&~bit[28]) | (bit[32]&bit[31]
		    &bit[30]&bit[29]&bit[28]);

		out_t[3] = (bit[30]&bit[29]&~bit[28]) | (bit[32]&bit[31]&bit[30]&bit[29]) | (
		    ~bit[31]&~bit[30]&~bit[29]&bit[28]);

		out_t[2] = (~bit[29]&~bit[28]) | (~bit[32]&~bit[31]&bit[30]) | (~bit[31]
		    &bit[30]&~bit[29]) | (bit[31]&~bit[30]&bit[28]) | (~bit[31]&bit[30]
		    &~bit[28]) | (bit[32]&bit[31]&bit[29]&bit[28]);

		out_t[1] = (~bit[30]&~bit[29]) | (bit[29]&~bit[28]) | (~bit[32]&bit[31]
		    &~bit[30]) | (bit[32]&bit[30]&bit[29]);

		out_t[0] = (bit[31]&~bit[29]&bit[28]) | (~bit[31]&~bit[30]&~bit[28]) | (
		    bit[32]&bit[31]&~bit[30]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (
		    ~bit[31]&bit[30]&~bit[29]) | (~bit[32]&bit[31]&bit[30]&bit[28]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[27];
		disable_update=disable_update|(~control);
		//----------------28--------------

		out_t[5] = (bit[31]&bit[30]&bit[29]);

		out_t[4] = (~bit[31]&~bit[29]) | (~bit[30]&~bit[29]);

		out_t[3] = (~bit[32]&~bit[31]&~bit[30]) | (~bit[31]&~bit[30]&bit[29]) | (
		    bit[31]&bit[30]&~bit[29]) | (~bit[33]&~bit[32]&bit[31]&bit[30]);

		out_t[2] = (bit[32]&bit[31]&bit[29]) | (bit[31]&~bit[30]&bit[29]) | (
		    ~bit[32]&bit[31]&bit[30]&~bit[29]) | (bit[32]&~bit[31]&~bit[30]
		    &~bit[29]) | (bit[33]&~bit[32]&bit[31]&bit[30]);

		out_t[1] = (~bit[32]&~bit[30]) | (bit[31]&~bit[30]) | (bit[33]&~bit[32]
		    &bit[29]) | (~bit[31]&bit[30]&bit[29]) | (bit[32]&bit[31]&~bit[29]);

		out_t[0] = (~bit[32]&~bit[31]&bit[30]) | (~bit[32]&bit[31]&~bit[30]) | (
		    bit[32]&~bit[31]&~bit[30]) | (~bit[33]&bit[32]&bit[31]&bit[30]) | (
		    bit[32]&bit[31]&bit[30]&~bit[29]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[28];
		disable_update=disable_update|(~control);
		//----------------29--------------

		out_t[5] = (~bit[32]&bit[31]&bit[30]) | (~bit[32]&~bit[31]&~bit[30]) | (
		    ~bit[34]&~bit[33]&bit[31]&bit[30]);

		out_t[4] = (~bit[32]&bit[31]&~bit[30]) | (bit[32]&~bit[31]&~bit[30]) | (
		    bit[33]&bit[32]&bit[31]&bit[30]) | (~bit[33]&bit[31]&~bit[30]) | (
		    bit[34]&bit[32]&bit[31]&bit[30]);

		out_t[3] = (~bit[33]&~bit[32]&bit[30]) | (bit[33]&bit[32]&~bit[30]) | (
		    bit[32]&~bit[31]&~bit[30]);

		out_t[2] = (~bit[34]&bit[33]&bit[31]) | (~bit[32]&~bit[31]&bit[30]) | (
		    bit[33]&bit[31]&~bit[30]) | (~bit[32]&bit[31]&~bit[30]) | (~bit[33]
		    &bit[32]&~bit[31]&~bit[30]) | (bit[34]&~bit[33]&bit[32]&bit[31]
		    &bit[30]) | (bit[33]&~bit[32]&bit[30]);

		out_t[1] = (bit[34]&bit[32]&bit[31]) | (~bit[33]&bit[32]&bit[31]) | (
		    bit[33]&bit[32]&~bit[30]) | (~bit[33]&~bit[32]&~bit[30]) | (~bit[34]
		    &bit[33]&~bit[32]&bit[30]) | (bit[33]&~bit[32]&~bit[31]&bit[30]);

		out_t[0] = (~bit[33]&bit[32]&~bit[31]) | (bit[33]&bit[32]&~bit[30]) | (
		    ~bit[34]&~bit[33]&~bit[32]&bit[31]) | (bit[33]&~bit[32]&~bit[31]
		    &bit[30]) | (~bit[33]&~bit[32]&bit[31]&~bit[30]) | (bit[34]&bit[33]
		    &bit[31]&bit[30]) | (bit[34]&bit[32]&bit[31]&bit[30]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[29];
		disable_update=disable_update|(~control);
		//----------------30--------------

		out_t[5] = (~bit[33]&bit[32]&bit[31]) | (bit[34]&bit[33]&~bit[32]&bit[31]);

		out_t[4] = (bit[33]&bit[32]&bit[31]) | (~bit[33]&~bit[32]&~bit[31]);

		out_t[3] = (bit[33]&bit[32]) | (bit[33]&~bit[31]) | (bit[32]&~bit[31]) | (
		    ~bit[34]&~bit[33]&~bit[32]&bit[31]);

		out_t[2] = (bit[34]&bit[33]&~bit[32]) | (bit[33]&~bit[32]&~bit[31]) | (
		    ~bit[34]&bit[33]&bit[32]&bit[31]) | (~bit[34]&~bit[33]&bit[32]
		    &~bit[31]) | (~bit[35]&~bit[34]&bit[32]&bit[31]) | (~bit[35]&bit[33]
		    &bit[32]&bit[31]);

		out_t[1] = (bit[34]&~bit[32]&bit[31]) | (~bit[34]&bit[33]&~bit[31]) | (
		    ~bit[34]&~bit[32]&~bit[31]) | (~bit[35]&~bit[34]&bit[33]&bit[32]) | (
		    bit[35]&bit[34]&bit[33]&bit[31]) | (bit[34]&~bit[33]&bit[32]&~bit[31]) | (
		    bit[35]&~bit[34]&~bit[33]&bit[32]&bit[31]) | (bit[33]&~bit[32]
		    &~bit[31]) | (~bit[35]&bit[34]&~bit[33]&bit[31]);

		out_t[0] = (~bit[34]&bit[33]&~bit[32]) | (bit[34]&~bit[33]&~bit[32]) | (
		    bit[34]&bit[32]&~bit[31]) | (~bit[34]&~bit[33]&bit[32]) | (~bit[34]
		    &bit[32]&bit[31]) | (~bit[35]&bit[33]&~bit[32]&bit[31]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[30];
		disable_update=disable_update|(~control);
		//----------------31--------------

		out_t[5] = (~bit[35]&bit[34]&bit[33]&bit[32]) | (bit[35]&~bit[34]&bit[33]
		    &bit[32]) | (~bit[36]&bit[34]&bit[33]&bit[32]);

		out_t[4] = (~bit[33]&~bit[32]) | (~bit[35]&~bit[34]&~bit[32]) | (bit[36]
		    &bit[35]&bit[34]&bit[33]&bit[32]);

		out_t[3] = (~bit[35]&~bit[34]&~bit[33]) | (~bit[34]&~bit[33]&bit[32]) | (
		    bit[35]&bit[33]&~bit[32]) | (bit[34]&bit[33]&~bit[32]) | (bit[36]
		    &bit[35]&bit[34]&bit[33]) | (~bit[36]&bit[35]&~bit[34]&bit[32]);

		out_t[2] = (bit[34]&~bit[33]) | (~bit[35]&bit[34]&~bit[32]) | (bit[35]
		    &~bit[34]&~bit[32]) | (bit[36]&bit[35]&bit[33]&bit[32]);

		out_t[1] = (bit[35]&~bit[34]&~bit[32]) | (bit[35]&bit[33]&~bit[32]) | (
		    ~bit[34]&bit[33]&~bit[32]) | (~bit[36]&bit[35]&~bit[34]&bit[33]) | (
		    ~bit[35]&bit[34]&bit[33]&bit[32]) | (~bit[35]&~bit[34]&~bit[33]
		    &bit[32]) | (bit[36]&bit[34]&bit[33]&bit[32]);

		out_t[0] = (~bit[36]&~bit[35]&bit[34]) | (~bit[35]&bit[34]&~bit[33]) | (
		    bit[35]&~bit[34]&~bit[33]) | (bit[35]&bit[33]&~bit[32]) | (bit[34]
		    &bit[33]&~bit[32]) | (~bit[34]&~bit[33]&~bit[32]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[31];
		disable_update=disable_update|(~control);
		//----------------32--------------

		out_t[5] = (bit[36]&bit[35]&bit[34]&bit[33]);

		out_t[4] = (~bit[36]&~bit[33]) | (~bit[35]&~bit[33]) | (~bit[34]&~bit[33]);

		out_t[3] = (~bit[35]&~bit[34]) | (~bit[34]&~bit[33]) | (bit[36]&bit[35]
		    &~bit[33]);

		out_t[2] = (~bit[36]&~bit[35]) | (bit[35]&~bit[34]&bit[33]) | (bit[36]
		    &bit[34]&~bit[33]) | (~bit[37]&bit[36]&bit[35]&bit[33]);

		out_t[1] = (~bit[36]&bit[34]&~bit[33]) | (bit[35]&bit[34]&~bit[33]) | (
		    bit[36]&~bit[35]&bit[34]&bit[33]) | (~bit[36]&bit[35]&~bit[34]
		    &bit[33]) | (bit[36]&~bit[35]&~bit[34]&~bit[33]);

		out_t[0] = (~bit[36]&bit[35]) | (bit[35]&bit[34]&~bit[33]) | (bit[36]
		    &~bit[34]&bit[33]) | (~bit[35]&~bit[34]&~bit[33]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[32];
		disable_update=disable_update|(~control);
		//----------------33--------------

		out_t[5] = (~bit[36]&bit[35]&bit[34]) | (~bit[38]&~bit[37]&bit[35]
		    &bit[34]);

		out_t[4] = (~bit[37]&~bit[34]) | (~bit[36]&~bit[34]) | (~bit[35]&~bit[34]) | (
		    bit[37]&bit[36]&bit[35]&bit[34]) | (bit[38]&bit[36]&bit[35]&bit[34]);

		out_t[3] = (~bit[37]&~bit[36]) | (~bit[35]&~bit[34]) | (bit[37]&bit[36]
		    &bit[35]) | (bit[38]&bit[36]&bit[35]&bit[34]);

		out_t[2] = (~bit[37]&bit[36]&~bit[35]) | (bit[37]&~bit[36]&~bit[35]) | (
		    bit[38]&~bit[37]&bit[36]&bit[34]) | (bit[37]&bit[36]&bit[35]&~bit[34]) | (
		    ~bit[36]&~bit[35]&~bit[34]) | (~bit[38]&bit[37]&bit[35]&bit[34]);

		out_t[1] = (~bit[36]&bit[35]) | (bit[37]&bit[36]&~bit[35]) | (bit[38]
		    &bit[35]&bit[34]) | (~bit[37]&~bit[36]&~bit[34]) | (bit[37]&~bit[35]
		    &bit[34]);

		out_t[0] = (~bit[38]&~bit[37]&bit[34]) | (~bit[37]&~bit[35]&bit[34]) | (
		    bit[37]&~bit[35]&~bit[34]) | (bit[38]&bit[36]&bit[34]) | (bit[36]
		    &~bit[35]&bit[34]) | (bit[36]&bit[35]&~bit[34]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[33];
		disable_update=disable_update|(~control);
		//----------------34--------------

		out_t[5] = (~bit[38]&bit[36]&bit[35]) | (~bit[37]&bit[36]&bit[35]) | (
		    bit[38]&bit[37]&~bit[36]&bit[35]);

		out_t[4] = (~bit[38]&~bit[36]&~bit[35]) | (~bit[37]&~bit[36]&~bit[35]) | (
		    bit[38]&bit[37]&bit[36]&bit[35]);

		out_t[3] = (~bit[38]&~bit[37]&bit[36]) | (bit[38]&bit[37]&~bit[36]) | (
		    ~bit[38]&~bit[37]&~bit[35]) | (~bit[39]&bit[38]&bit[37]&bit[35]);

		out_t[2] = (bit[38]&~bit[37]&bit[36]) | (bit[38]&~bit[37]&~bit[35]) | (
		    bit[37]&bit[36]&~bit[35]) | (~bit[39]&~bit[38]&bit[37]&bit[36]) | (
		    ~bit[38]&~bit[37]&~bit[36]&bit[35]) | (~bit[39]&bit[38]&bit[37]
		    &~bit[36]&bit[35]);

		out_t[1] = (bit[38]&~bit[37]) | (~bit[39]&~bit[37]&bit[36]) | (bit[38]
		    &bit[36]&bit[35]) | (~bit[38]&bit[36]&~bit[35]) | (bit[38]&~bit[36]
		    &~bit[35]) | (bit[39]&bit[38]&bit[35]);

		out_t[0] = (bit[38]&~bit[35]) | (~bit[39]&bit[38]&~bit[37]&bit[36]) | (
		    ~bit[38]&bit[37]&~bit[36]&bit[35]) | (bit[39]&~bit[38]&~bit[37]
		    &bit[36]&bit[35]) | (bit[39]&bit[37]&~bit[36]&bit[35]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[34];
		disable_update=disable_update|(~control);
		//----------------35--------------

		out_t[5] = (~bit[40]&~bit[38]&~bit[37]&bit[36]) | (~bit[39]&~bit[38]&~bit[37]&bit[36]);

		out_t[4] = (~bit[38]&bit[37]&bit[36]) | (bit[38]&~bit[37]&bit[36]) | (~bit[39]&~bit[38]&~bit[37]&~bit[36]) | (bit[40]&bit[39]&~bit[37]&bit[36]);

		out_t[3] = (bit[38]&bit[37]&bit[36]) | (bit[39]&~bit[37]&~bit[36]) | (bit[40]&bit[39]&~bit[38]&~bit[37]) | (~bit[40]&~bit[39]&~bit[37]&bit[36]) | (~bit[40]&bit[38]&~bit[37]) | (~bit[39]&bit[38]&~bit[37]);

		out_t[2] = (~bit[39]&bit[38]&bit[36]) | (bit[40]&~bit[37]&bit[36]) | (~bit[40]&bit[38]&bit[37]&bit[36]) | (~bit[39]&~bit[38]&bit[37]&~bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]) | (~bit[40]&~bit[39]&bit[37]&bit[36]);

		out_t[1] = (bit[39]&~bit[38]&~bit[36]) | (~bit[40]&bit[39]&~bit[38]&bit[37]) | (bit[40]&bit[39]&~bit[38]&~bit[37]) | (~bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[40]&bit[39]&bit[38]&bit[37]&bit[36]) | (~bit[40]&~bit[39]&bit[38]&bit[37]&bit[36]) | (bit[40]&~bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[40]&bit[39]&bit[38]&~bit[37]&bit[36]) | (~bit[40]&~bit[39]&~bit[38]&~bit[37]&bit[36]);

		out_t[0] = (~bit[39]&bit[38]&bit[37]) | (bit[40]&bit[37]&bit[36]) | (~bit[40]&~bit[38]&~bit[37]&bit[36]) | (bit[39]&~bit[38]&bit[37]&~bit[36]) | (bit[39]&bit[38]&~bit[37]&~bit[36]) | (~bit[39]&~bit[38]&~bit[37]&~bit[36]) | (bit[40]&bit[39]&bit[38]&bit[36]) | (~bit[40]&~bit[39]&~bit[37]&bit[36]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[35];
		disable_update=disable_update|(~control);
		//----------------36--------------

		out_t[5] = (~bit[41]&~bit[38]&bit[37]) | (~bit[40]&~bit[38]&bit[37]) | (
		    ~bit[39]&~bit[38]&bit[37]) | (bit[40]&bit[39]&bit[38]&~bit[37]);

		out_t[4] = (~bit[40]&bit[38]&bit[37]) | (~bit[39]&bit[38]&bit[37]) | (
		    bit[41]&bit[40]&bit[39]&~bit[38]&bit[37]);

		out_t[3] = (bit[40]&bit[39]&bit[38]) | (~bit[39]&~bit[38]&~bit[37]) | (
		    bit[41]&bit[40]&bit[39]&bit[37]) | (~bit[40]&~bit[39]&bit[38]&bit[37]);

		out_t[2] = (bit[40]&~bit[39]&bit[37]) | (~bit[39]&~bit[38]&bit[37]) | (
		    bit[39]&~bit[38]&~bit[37]) | (~bit[41]&bit[39]&bit[38]&bit[37]) | (
		    ~bit[40]&~bit[39]&bit[38]&~bit[37]) | (bit[41]&bit[40]&~bit[38]
		    &bit[37]) | (~bit[41]&bit[40]&bit[39]&~bit[37]);

		out_t[1] = (~bit[40]&~bit[38]) | (bit[39]&~bit[38]&~bit[37]) | (bit[40]
		    &~bit[39]&bit[38]&~bit[37]) | (bit[41]&bit[40]&bit[39]&bit[38]
		    &bit[37]) | (~bit[41]&~bit[39]&bit[38]&bit[37]);

		out_t[0] = (~bit[40]&bit[39]&~bit[37]) | (bit[41]&bit[40]&bit[38]&bit[37]) | (
		    bit[41]&~bit[39]&bit[38]&bit[37]) | (~bit[41]&~bit[39]&~bit[38]
		    &bit[37]) | (~bit[41]&bit[39]&bit[38]&~bit[37]) | (~bit[41]&~bit[40]
		    &~bit[38]&bit[37]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[36];
		disable_update=disable_update|(~control);
		//----------------37--------------

		out_t[5] = (bit[40]&bit[39]&~bit[38]) | (~bit[41]&~bit[40]&~bit[39]
		    &bit[38]);

		out_t[4] = (bit[41]&~bit[39]&bit[38]) | (bit[40]&~bit[39]&bit[38]) | (
		    ~bit[41]&~bit[40]&bit[39]&bit[38]);

		out_t[3] = (~bit[41]&bit[40]&bit[39]) | (~bit[41]&bit[40]&bit[38]) | (
		    bit[41]&~bit[40]&bit[38]) | (~bit[41]&~bit[40]&~bit[39]&~bit[38]) | (
		    ~bit[42]&bit[40]&bit[39]&bit[38]);

		out_t[2] = (bit[41]&~bit[39]) | (bit[40]&~bit[39]) | (~bit[42]&~bit[40]
		    &bit[38]) | (~bit[42]&bit[40]&~bit[38]) | (bit[42]&bit[41]&bit[40]) | (
		    bit[42]&bit[41]&bit[38]);

		out_t[1] = (bit[41]&~bit[40]&~bit[39]) | (bit[41]&~bit[39]&bit[38]) | (
		    ~bit[42]&bit[41]&~bit[40]&bit[38]) | (bit[42]&~bit[40]&~bit[39]
		    &bit[38]) | (bit[41]&bit[40]&bit[39]&~bit[38]) | (~bit[41]&~bit[40]
		    &bit[39]&~bit[38]) | (~bit[41]&bit[40]&~bit[39]&~bit[38]) | (~bit[42]
		    &~bit[41]&bit[40]&bit[39]&bit[38]) | (bit[42]&bit[41]&bit[40]&bit[38]);

		out_t[0] = (~bit[41]&~bit[40]&~bit[39]) | (~bit[42]&~bit[39]&bit[38]) | (
		    ~bit[41]&~bit[40]&~bit[38]) | (bit[41]&~bit[39]&~bit[38]) | (~bit[42]
		    &bit[41]&bit[40]&~bit[38]) | (bit[42]&~bit[41]&bit[40]&bit[39]
		    &bit[38]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[37];
		disable_update=disable_update|(~control);
		//----------------38--------------

		out_t[5] = (bit[40]&~bit[39]) | (~bit[42]&~bit[41]&~bit[40]&bit[39]);

		out_t[4] = (~bit[41]&bit[40]&bit[39]) | (bit[41]&~bit[40]&bit[39]) | (
		    bit[42]&~bit[40]&bit[39]);

		out_t[3] = (~bit[42]&bit[41]&bit[39]) | (bit[41]&bit[40]&bit[39]) | (
		    ~bit[41]&bit[40]&~bit[39]) | (bit[42]&~bit[41]&~bit[40]&bit[39]) | (
		    ~bit[43]&bit[42]&~bit[40]&bit[39]) | (~bit[43]&~bit[42]&bit[40]
		    &~bit[39]);

		out_t[2] = (~bit[42]&~bit[41]&bit[40]) | (bit[43]&bit[42]&~bit[40]
		    &bit[39]) | (bit[42]&~bit[41]&~bit[40]&bit[39]) | (bit[43]&~bit[42]
		    &bit[40]) | (~bit[43]&bit[41]&bit[40]&bit[39]) | (bit[42]&bit[41]
		    &bit[40]&~bit[39]);

		out_t[1] = (~bit[43]&~bit[41]&~bit[39]) | (bit[42]&~bit[41]&~bit[39]) | (
		    ~bit[41]&~bit[40]&~bit[39]) | (bit[43]&~bit[42]&bit[41]&bit[40]) | (
		    ~bit[43]&bit[42]&~bit[41]&bit[40]) | (bit[43]&bit[42]&bit[41]&bit[39]) | (
		    ~bit[43]&~bit[42]&bit[41]&bit[39]) | (~bit[43]&~bit[42]&~bit[40]
		    &bit[39]);

		out_t[0] = (bit[43]&~bit[42]&~bit[39]) | (~bit[42]&~bit[40]&~bit[39]) | (
		    bit[43]&~bit[42]&bit[41]&~bit[40]) | (~bit[43]&bit[42]&~bit[41]
		    &bit[39]) | (bit[43]&bit[42]&bit[41]&bit[40]&bit[39]) | (~bit[43]
		    &~bit[42]&bit[40]&bit[39]) | (~bit[43]&bit[42]&bit[40]&~bit[39]) | (
		    ~bit[43]&bit[41]&bit[40]&~bit[39]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[38];
		disable_update=disable_update|(~control);
		//----------------39--------------

		out_t[5] = (~bit[43]&~bit[41]&bit[40]) | (~bit[42]&~bit[41]&bit[40]) | (
		    bit[42]&bit[41]&~bit[40]);

		out_t[4] = (~bit[43]&bit[41]&bit[40]) | (~bit[42]&bit[41]&bit[40]) | (
		    bit[43]&bit[42]&~bit[41]&bit[40]);

		out_t[3] = (bit[43]&bit[42]&bit[40]) | (bit[42]&bit[41]&~bit[40]) | (
		    ~bit[42]&~bit[41]&~bit[40]) | (~bit[43]&~bit[42]&bit[41]&bit[40]) | (
		    ~bit[44]&~bit[43]&~bit[42]&bit[40]);

		out_t[2] = (~bit[44]&bit[43]&bit[40]) | (bit[43]&bit[42]&bit[40]) | (
		    ~bit[43]&bit[42]&~bit[40]) | (~bit[44]&~bit[42]&bit[41]&bit[40]) | (
		    bit[44]&~bit[43]&~bit[42]&~bit[41]&bit[40]) | (bit[42]&~bit[41]
		    &~bit[40]);

		out_t[1] = (~bit[44]&bit[42]&bit[41]) | (bit[44]&bit[43]&~bit[42]&bit[40]) | (
		    bit[44]&~bit[42]&~bit[41]&bit[40]) | (~bit[43]&~bit[42]&bit[41]
		    &~bit[40]) | (~bit[43]&bit[42]&~bit[41]&~bit[40]) | (bit[43]&bit[42]
		    &~bit[41]&bit[40]);

		out_t[0] = (bit[43]&bit[42]&~bit[40]) | (bit[44]&~bit[42]&bit[41]&bit[40]) | (
		    ~bit[44]&bit[42]&~bit[41]&bit[40]) | (bit[44]&bit[42]&bit[41]&~bit[40]) | (
		    ~bit[43]&~bit[42]&~bit[41]&~bit[40]) | (bit[43]&bit[42]&bit[41]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[39];
		disable_update=disable_update|(~control);
		//----------------40--------------

		out_t[5] = (~bit[44]&bit[43]&~bit[42]&bit[41]) | (bit[44]&~bit[43]
		    &~bit[42]&bit[41]) | (~bit[45]&bit[43]&~bit[42]&bit[41]);

		out_t[4] = (~bit[43]&bit[42]&bit[41]) | (~bit[45]&~bit[44]&bit[42]
		    &bit[41]) | (bit[45]&bit[44]&bit[43]&~bit[42]&bit[41]);

		out_t[3] = (~bit[44]&~bit[42]&~bit[41]) | (bit[44]&~bit[43]&~bit[42]) | (
		    bit[45]&bit[44]&bit[43]&bit[41]) | (~bit[45]&bit[44]&bit[42]&bit[41]) | (
		    bit[45]&~bit[44]&bit[42]&bit[41]) | (~bit[44]&~bit[43]&bit[42]
		    &bit[41]);

		out_t[2] = (~bit[44]&~bit[43]&~bit[41]) | (~bit[43]&bit[42]&~bit[41]) | (
		    bit[44]&~bit[43]&~bit[42]&bit[41]) | (bit[44]&bit[43]&~bit[42]
		    &~bit[41]) | (~bit[45]&bit[44]&bit[43]&bit[42]&bit[41]) | (bit[45]
		    &~bit[44]&bit[43]&bit[42]&bit[41]) | (~bit[45]&~bit[44]&bit[43]
		    &~bit[42]&bit[41]) | (bit[45]&bit[44]&~bit[42]&bit[41]);

		out_t[1] = (bit[43]&~bit[41]) | (bit[45]&bit[43]&bit[42]) | (~bit[44]
		    &bit[43]&~bit[42]) | (bit[44]&~bit[42]&~bit[41]) | (~bit[45]&bit[44]
		    &~bit[43]&~bit[42]) | (~bit[44]&~bit[43]&bit[42]&bit[41]);

		out_t[0] = (bit[44]&bit[43]&bit[41]) | (~bit[44]&~bit[42]&bit[41]) | (
		    ~bit[44]&bit[42]&~bit[41]) | (bit[44]&~bit[42]&~bit[41]) | (bit[45]
		    &bit[44]&bit[42]&bit[41]) | (~bit[45]&~bit[42]&bit[41]) | (~bit[45]
		    &~bit[44]&~bit[43]&bit[41]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[40];
		disable_update=disable_update|(~control);
		//----------------41--------------

		out_t[5] = (~bit[45]&~bit[43]&bit[42]) | (~bit[44]&~bit[43]&bit[42]) | (
		    bit[45]&bit[43]&~bit[42]) | (bit[44]&bit[43]&~bit[42]);

		out_t[4] = (bit[43]&bit[42]) | (bit[45]&bit[44]&bit[42]) | (~bit[46]
		    &bit[45]&~bit[44]&bit[43]);

		out_t[3] = (bit[44]&bit[43]&~bit[42]) | (~bit[46]&~bit[44]&bit[43]
		    &bit[42]) | (~bit[45]&~bit[44]&bit[43]&bit[42]) | (bit[45]&bit[44]
		    &~bit[43]&bit[42]) | (bit[46]&bit[45]&bit[43]&~bit[42]) | (~bit[45]
		    &~bit[44]&~bit[43]&~bit[42]) | (~bit[46]&~bit[45]&~bit[44]&bit[42]);

		out_t[2] = (~bit[45]&bit[44]&bit[43]) | (bit[46]&bit[45]&~bit[44]&bit[43]) | (
		    bit[45]&bit[44]&~bit[43]&bit[42]) | (bit[46]&~bit[45]&~bit[44]
		    &~bit[43]&bit[42]) | (~bit[46]&bit[45]&~bit[44]&~bit[43]) | (bit[45]
		    &~bit[44]&~bit[43]&~bit[42]);

		out_t[1] = (bit[46]&bit[45]&~bit[44]) | (bit[46]&~bit[44]&~bit[43]
		    &bit[42]) | (~bit[45]&bit[44]&~bit[43]&~bit[42]) | (bit[45]&~bit[44]
		    &~bit[43]&~bit[42]) | (~bit[46]&~bit[45]&~bit[44]&bit[43]&bit[42]) | (
		    ~bit[46]&bit[45]&bit[44]&bit[42]) | (~bit[46]&bit[44]&~bit[43]
		    &bit[42]) | (bit[45]&bit[44]&~bit[43]&bit[42]) | (~bit[46]&bit[44]
		    &bit[43]&~bit[42]) | (bit[45]&bit[44]&bit[43]&~bit[42]);

		out_t[0] = (bit[44]&~bit[43]&~bit[42]) | (bit[46]&bit[45]&~bit[44]
		    &bit[43]) | (bit[46]&bit[45]&bit[43]&bit[42]) | (~bit[46]&~bit[45]
		    &bit[43]&bit[42]) | (~bit[46]&bit[45]&bit[44]&~bit[42]) | (bit[46]
		    &~bit[45]&bit[44]&~bit[42]) | (~bit[45]&~bit[44]&bit[43]&bit[42]) | (
		    ~bit[46]&bit[45]&~bit[43]&bit[42]) | (~bit[46]&~bit[44]&~bit[43]
		    &bit[42]) | (bit[45]&~bit[44]&~bit[43]&bit[42]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[41];
		disable_update=disable_update|(~control);
		//----------------42--------------

		out_t[5] = (~bit[45]&bit[44]&bit[43]) | (bit[45]&~bit[44]&bit[43]);

		out_t[4] = (~bit[47]&bit[45]&bit[44]&bit[43]) | (~bit[46]&bit[45]&bit[44]
		    &bit[43]) | (~bit[46]&~bit[45]&~bit[44]&~bit[43]);

		out_t[3] = (bit[45]&~bit[44]) | (bit[46]&~bit[44]&~bit[43]) | (bit[47]
		    &bit[46]&bit[45]&bit[43]) | (~bit[46]&~bit[45]&bit[44]&~bit[43]) | (
		    ~bit[47]&~bit[46]&bit[44]&bit[43]);

		out_t[2] = (bit[46]&~bit[45]&bit[44]) | (~bit[46]&bit[45]&~bit[44]) | (
		    bit[47]&bit[44]&bit[43]) | (~bit[46]&bit[45]&~bit[43]) | (bit[46]
		    &~bit[45]&~bit[43]);

		out_t[1] = (bit[46]&~bit[43]) | (~bit[47]&bit[46]&bit[44]) | (bit[47]
		    &~bit[46]&bit[43]) | (~bit[46]&~bit[44]&bit[43]) | (bit[47]&bit[45]
		    &bit[44]&bit[43]) | (~bit[47]&bit[45]&~bit[44]&bit[43]);

		out_t[0] = (bit[47]&~bit[46]&bit[44]&bit[43]) | (~bit[47]&bit[45]&~bit[44]
		    &bit[43]) | (bit[46]&bit[45]&~bit[44]&bit[43]) | (bit[46]&bit[45]
		    &bit[44]&~bit[43]) | (bit[46]&~bit[45]&~bit[44]&~bit[43]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[42];
		disable_update=disable_update|(~control);
		//----------------43--------------

		out_t[5] = (~bit[45]&bit[44]) | (~bit[47]&~bit[46]&bit[44]);

		out_t[4] = (bit[47]&bit[45]&bit[44]) | (bit[46]&bit[45]&bit[44]) | (
		    ~bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]);

		out_t[3] = (~bit[47]&bit[46]&~bit[45]) | (bit[47]&~bit[46]&bit[44]) | (
		    bit[48]&~bit[46]&~bit[45]) | (~bit[46]&~bit[45]&~bit[44]);

		out_t[2] = (~bit[47]&~bit[46]&~bit[44]) | (~bit[46]&bit[45]&~bit[44]) | (
		    bit[47]&bit[46]&~bit[45]&~bit[44]) | (~bit[48]&~bit[47]&bit[46]
		    &bit[45]&bit[44]) | (~bit[48]&bit[47]&~bit[45]&bit[44]) | (bit[48]
		    &~bit[46]&~bit[45]&bit[44]);

		out_t[1] = (bit[47]&bit[46]&~bit[45]) | (bit[46]&bit[45]&~bit[44]) | (
		    bit[47]&~bit[45]&~bit[44]) | (bit[48]&~bit[47]&bit[46]&bit[45]) | (
		    ~bit[48]&bit[47]&bit[45]&bit[44]) | (~bit[48]&bit[46]&~bit[45]
		    &bit[44]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]);

		out_t[0] = (~bit[47]&~bit[44]) | (~bit[45]&~bit[44]) | (~bit[47]&bit[46]
		    &bit[45]) | (~bit[48]&bit[47]&~bit[45]) | (~bit[47]&~bit[46]&~bit[45]) | (
		    bit[47]&~bit[46]&bit[45]&bit[44]) | (~bit[48]&~bit[46]&bit[44]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[43];
		disable_update=disable_update|(~control);
		//----------------44--------------

		out_t[5] = (~bit[48]&~bit[46]&bit[45]) | (~bit[47]&~bit[46]&bit[45]) | (
		    bit[48]&bit[47]&bit[46]&~bit[45]);

		out_t[4] = (~bit[48]&bit[46]&bit[45]) | (~bit[47]&bit[46]&bit[45]) | (
		    bit[48]&bit[47]&~bit[46]&bit[45]) | (~bit[49]&bit[48]&bit[47]&bit[46]
		    &~bit[45]);

		out_t[3] = (~bit[47]&~bit[46]) | (~bit[46]&~bit[45]) | (bit[49]&bit[48]
		    &bit[47]) | (~bit[49]&~bit[48]&~bit[47]) | (bit[48]&bit[47]&bit[45]) | (
		    ~bit[48]&~bit[47]&~bit[45]);

		out_t[2] = (~bit[49]&~bit[48]&~bit[46]) | (~bit[48]&~bit[46]&~bit[45]) | (
		    bit[49]&~bit[48]&~bit[47]&bit[46]&bit[45]) | (bit[48]&~bit[47]
		    &~bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]) | (~bit[49]&bit[48]
		    &bit[47]&bit[45]) | (~bit[49]&bit[48]&bit[46]&bit[45]);

		out_t[1] = (bit[48]&bit[47]&bit[46]&bit[45]) | (bit[49]&~bit[48]&~bit[46]
		    &bit[45]) | (~bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[48]&~bit[47]
		    &bit[46]&~bit[45]) | (bit[48]&bit[47]&~bit[46]&~bit[45]) | (~bit[48]
		    &~bit[47]&~bit[46]&~bit[45]) | (bit[49]&bit[48]&bit[46]) | (bit[49]
		    &~bit[47]&bit[46]&bit[45]);

		out_t[0] = (~bit[49]&bit[48]&~bit[46]) | (bit[48]&bit[47]&~bit[46]) | (
		    ~bit[49]&bit[47]&bit[45]) | (~bit[48]&bit[46]&~bit[45]) | (bit[48]
		    &~bit[46]&~bit[45]) | (bit[49]&~bit[48]&~bit[47]&bit[45]) | (bit[49]
		    &~bit[47]&bit[46]&bit[45]) | (bit[49]&bit[47]&bit[46]&~bit[45]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[44];
		disable_update=disable_update|(~control);
		//----------------45--------------

		out_t[5] = (~bit[48]&~bit[47]&bit[46]) | (bit[49]&bit[47]&~bit[46]) | (
		    bit[48]&bit[47]&~bit[46]);

		out_t[4] = (bit[49]&~bit[48]&bit[47]) | (~bit[48]&bit[47]&bit[46]) | (
		    bit[48]&~bit[47]&bit[46]);

		out_t[3] = (bit[48]&bit[47]) | (bit[48]&bit[46]) | (~bit[48]&~bit[47]
		    &~bit[46]) | (~bit[49]&~bit[47]&~bit[46]);

		out_t[2] = (~bit[50]&bit[48]&bit[47]) | (~bit[49]&bit[48]&bit[47]) | (
		    ~bit[49]&bit[47]&bit[46]) | (bit[49]&bit[48]&~bit[47]&~bit[46]) | (
		    ~bit[49]&~bit[48]&~bit[47]&~bit[46]) | (~bit[50]&~bit[49]&~bit[48]
		    &bit[46]);

		out_t[1] = (~bit[49]&bit[48]&bit[47]) | (~bit[49]&~bit[47]&bit[46]) | (
		    bit[49]&~bit[47]&~bit[46]) | (~bit[50]&bit[49]&~bit[48]&bit[47]) | (
		    bit[50]&bit[49]&bit[47]&bit[46]) | (~bit[50]&~bit[48]&bit[47]&bit[46]);

		out_t[0] = (~bit[50]&~bit[49]&bit[48]) | (~bit[50]&~bit[48]&bit[46]) | (
		    bit[48]&~bit[47]&~bit[46]) | (bit[50]&bit[49]&bit[48]&bit[47]) | (
		    ~bit[49]&~bit[48]&~bit[47]&bit[46]) | (bit[50]&bit[49]&bit[47]
		    &~bit[46]) | (~bit[50]&~bit[47]&bit[46]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[45];
		disable_update=disable_update|(~control);
		//----------------46--------------

		out_t[5] = (bit[50]&~bit[48]&bit[47]) | (bit[49]&~bit[48]&bit[47]) | (
		    ~bit[51]&~bit[50]&~bit[49]&bit[48]&bit[47]);

		out_t[4] = (bit[51]&bit[48]&bit[47]) | (bit[50]&bit[48]&bit[47]) | (
		    bit[49]&bit[48]&bit[47]);

		out_t[3] = (bit[51]&bit[48]&bit[47]) | (~bit[49]&~bit[48]&~bit[47]) | (
		    ~bit[51]&~bit[50]&bit[49]&bit[47]) | (bit[50]&~bit[49]&bit[47]) | (
		    bit[50]&bit[48]&bit[47]);

		out_t[2] = (bit[50]&~bit[49]&bit[47]) | (bit[49]&~bit[48]&~bit[47]) | (
		    bit[51]&~bit[50]&bit[49]&~bit[48]) | (~bit[50]&~bit[49]&bit[48]
		    &~bit[47]) | (~bit[51]&~bit[50]&bit[49]&bit[48]&bit[47]) | (bit[51]
		    &~bit[49]&bit[48]&bit[47]) | (~bit[51]&bit[50]&~bit[48]&bit[47]);

		out_t[1] = (~bit[50]&bit[49]&~bit[47]) | (~bit[50]&~bit[48]&~bit[47]) | (
		    bit[51]&~bit[50]&bit[48]&bit[47]) | (bit[50]&~bit[49]&bit[48]&~bit[47]) | (
		    ~bit[51]&bit[50]&~bit[49]&bit[47]) | (bit[51]&bit[49]&~bit[48]
		    &bit[47]) | (bit[50]&~bit[49]&~bit[48]&bit[47]);

		out_t[0] = (bit[50]&~bit[47]) | (~bit[51]&bit[50]&bit[49]) | (~bit[51]
		    &bit[50]&~bit[48]) | (bit[51]&~bit[50]&bit[48]&bit[47]) | (bit[51]
		    &~bit[49]&bit[48]&bit[47]) | (bit[51]&bit[49]&~bit[48]&bit[47]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[46];
		disable_update=disable_update|(~control);
		//----------------47--------------

		out_t[5] = (bit[49]&bit[48]);

		out_t[4] = (~bit[49]&~bit[48]) | (~bit[51]&~bit[50]&bit[49]);

		out_t[3] = (~bit[51]&bit[50]&bit[49]) | (bit[51]&~bit[50]&bit[49]) | (
		    ~bit[51]&~bit[50]&~bit[49]&bit[48]) | (bit[50]&bit[49]&~bit[48]);

		out_t[2] = (bit[51]&~bit[50]) | (bit[51]&bit[49]&bit[48]) | (~bit[50]
		    &~bit[49]&~bit[48]) | (~bit[51]&bit[50]&bit[49]&~bit[48]) | (~bit[52]
		    &bit[50]&bit[49]&bit[48]);

		out_t[1] = (~bit[52]&bit[51]&bit[49]) | (~bit[51]&bit[50]&~bit[49]) | (
		    bit[51]&~bit[50]&bit[48]) | (~bit[50]&bit[49]&bit[48]) | (~bit[51]
		    &~bit[49]&~bit[48]) | (bit[51]&bit[50]&~bit[48]) | (bit[51]&bit[49]
		    &~bit[48]) | (bit[52]&~bit[51]&bit[49]&bit[48]);

		out_t[0] = (~bit[52]&~bit[50]&bit[49]) | (~bit[51]&bit[50]&~bit[48]) | (
		    bit[51]&~bit[50]&~bit[48]) | (bit[52]&bit[50]&bit[49]&bit[48]) | (
		    ~bit[51]&~bit[50]&~bit[49]&bit[48]) | (~bit[50]&bit[49]&~bit[48]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[47];
		disable_update=disable_update|(~control);
		//----------------48--------------

		out_t[5] = (~bit[52]&bit[50]&bit[49]) | (~bit[51]&bit[50]&bit[49]) | (
		    bit[52]&bit[51]&~bit[50]&bit[49]) | (~bit[52]&~bit[51]&~bit[50]
		    &~bit[49]);

		out_t[4] = (bit[52]&~bit[50]&~bit[49]) | (bit[51]&~bit[50]&~bit[49]) | (
		    bit[52]&bit[51]&bit[50]&bit[49]) | (~bit[53]&bit[52]&bit[51]&bit[49]);

		out_t[3] = (bit[52]&~bit[51]&~bit[49]) | (~bit[52]&bit[50]&~bit[49]) | (
		    ~bit[53]&~bit[52]&~bit[51]&bit[50]) | (~bit[53]&bit[52]&bit[51]
		    &bit[50]&bit[49]) | (bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]);

		out_t[2] = (bit[53]&bit[52]&bit[51]&bit[49]) | (bit[52]&~bit[51]&bit[50]
		    &bit[49]) | (~bit[52]&~bit[51]&~bit[50]&bit[49]) | (bit[52]&bit[51]
		    &bit[50]&~bit[49]) | (~bit[52]&~bit[51]&bit[50]&~bit[49]) | (~bit[52]
		    &bit[51]&~bit[50]&~bit[49]) | (bit[52]&~bit[51]&~bit[50]&~bit[49]) | (
		    bit[53]&~bit[51]&bit[50]&bit[49]);

		out_t[1] = (~bit[52]&bit[51]&bit[49]) | (~bit[52]&~bit[50]&~bit[49]) | (
		    ~bit[53]&bit[52]&~bit[51]&bit[49]) | (bit[53]&~bit[52]&bit[50]
		    &bit[49]) | (bit[52]&bit[51]&bit[50]&~bit[49]) | (bit[53]&bit[51]
		    &bit[49]) | (bit[53]&bit[52]&~bit[50]&bit[49]);

		out_t[0] = (bit[52]&~bit[49]) | (~bit[50]&~bit[49]) | (bit[52]&~bit[51]
		    &~bit[50]) | (~bit[53]&bit[51]&bit[50]&bit[49]) | (bit[53]&~bit[51]
		    &bit[50]&bit[49]) | (bit[52]&bit[51]&bit[50]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[48];
		disable_update=disable_update|(~control);
		//----------------49--------------

		out_t[5] = (~bit[54]&~bit[51]&bit[50]) | (~bit[53]&~bit[51]&bit[50]) | (
		    ~bit[52]&~bit[51]&bit[50]) | (bit[53]&bit[52]&bit[51]&~bit[50]);

		out_t[4] = (~bit[52]&bit[51]&bit[50]) | (~bit[52]&~bit[51]&~bit[50]) | (
		    ~bit[54]&~bit[53]&bit[51]&bit[50]) | (bit[53]&bit[52]&bit[51]&~bit[50]) | (
		    bit[54]&bit[53]&bit[52]&~bit[51]&bit[50]) | (~bit[54]&~bit[52]
		    &bit[50]) | (~bit[53]&~bit[52]&bit[50]);

		out_t[3] = (bit[52]&~bit[51]&~bit[50]) | (~bit[54]&~bit[53]&bit[52]
		    &~bit[51]) | (bit[54]&bit[53]&~bit[51]&bit[50]) | (~bit[54]&bit[53]
		    &bit[51]&bit[50]) | (bit[54]&bit[52]&bit[51]&bit[50]) | (~bit[53]
		    &~bit[52]&bit[51]&bit[50]);

		out_t[2] = (~bit[53]&~bit[52]&~bit[50]) | (~bit[54]&~bit[53]&~bit[52]
		    &bit[51]) | (bit[54]&bit[53]&~bit[52]&bit[50]) | (bit[54]&bit[53]
		    &~bit[51]&bit[50]) | (bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (
		    ~bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]);

		out_t[1] = (~bit[54]&bit[52]&bit[50]) | (bit[54]&~bit[53]&bit[51]&bit[50]) | (
		    bit[54]&bit[53]&~bit[51]&bit[50]) | (bit[54]&bit[53]&bit[51]&~bit[50]) | (
		    bit[53]&~bit[52]&bit[51]&~bit[50]) | (~bit[53]&~bit[52]&~bit[51]
		    &~bit[50]) | (bit[52]&~bit[51]&bit[50]) | (~bit[54]&~bit[53]&~bit[51]
		    &bit[50]);

		out_t[0] = (bit[54]&~bit[52]&bit[50]) | (bit[54]&bit[53]&bit[52]&bit[51]) | (
		    ~bit[53]&bit[52]&bit[51]&bit[50]) | (~bit[53]&bit[52]&~bit[51]
		    &~bit[50]) | (bit[54]&~bit[53]&bit[52]&~bit[51]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[49];
		disable_update=disable_update|(~control);
		//----------------50--------------

		out_t[5] = (bit[52]&~bit[51]);

		out_t[4] = (~bit[55]&~bit[52]&bit[51]) | (~bit[54]&~bit[52]&bit[51]) | (
		    ~bit[53]&~bit[52]&bit[51]) | (~bit[55]&~bit[53]&bit[52]&~bit[51]) | (
		    ~bit[54]&~bit[53]&bit[52]&~bit[51]);

		out_t[3] = (~bit[54]&~bit[53]&~bit[52]) | (~bit[55]&~bit[53]&bit[51]) | (
		    bit[55]&bit[54]&~bit[53]&bit[52]) | (~bit[54]&bit[53]&bit[52]&~bit[51]) | (
		    bit[55]&bit[54]&bit[53]&~bit[52]&bit[51]) | (~bit[53]&bit[52]&bit[51]);

		out_t[2] = (~bit[55]&~bit[54]&bit[51]) | (bit[53]&bit[52]&bit[51]) | (
		    ~bit[55]&bit[54]&bit[53]&bit[52]) | (bit[55]&bit[54]&~bit[53]&~bit[51]) | (
		    ~bit[54]&bit[53]&~bit[52]&~bit[51]) | (bit[54]&~bit[53]&~bit[52]
		    &~bit[51]) | (bit[55]&bit[54]&~bit[52]&bit[51]) | (bit[55]&~bit[53]
		    &~bit[52]&bit[51]);

		out_t[1] = (~bit[54]&bit[52]) | (~bit[54]&bit[53]&bit[51]) | (bit[54]
		    &bit[53]&~bit[51]) | (bit[55]&bit[52]&~bit[51]) | (~bit[55]&~bit[53]
		    &bit[52]&bit[51]) | (bit[55]&bit[54]&~bit[52]&bit[51]);

		out_t[0] = (~bit[55]&bit[53]&bit[52]) | (bit[54]&~bit[53]&~bit[52]) | (
		    bit[54]&~bit[52]&bit[51]) | (~bit[55]&~bit[53]&~bit[52]&bit[51]) | (
		    ~bit[55]&~bit[54]&bit[52]&~bit[51]) | (bit[55]&~bit[54]&~bit[53]
		    &bit[52]&bit[51]) | (bit[55]&bit[54]&bit[52]&~bit[51]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[50];
		disable_update=disable_update|(~control);
		//----------------51--------------

		out_t[5] = (bit[53]&~bit[52]) | (bit[55]&bit[54]&~bit[52]) | (~bit[56]
		    &~bit[54]&~bit[53]&bit[52]) | (~bit[55]&~bit[54]&~bit[53]&bit[52]);

		out_t[4] = (~bit[55]&~bit[54]&bit[53]) | (bit[55]&bit[54]&~bit[53]) | (
		    bit[54]&~bit[53]&bit[52]) | (~bit[56]&~bit[54]&bit[53]&bit[52]) | (
		    bit[56]&bit[55]&~bit[53]&bit[52]);

		out_t[3] = (~bit[55]&bit[54]&bit[53]) | (~bit[55]&bit[54]&bit[52]) | (
		    bit[54]&bit[53]&bit[52]) | (bit[56]&bit[55]&~bit[54]&bit[52]) | (
		    bit[55]&~bit[54]&bit[53]&~bit[52]);

		out_t[2] = (bit[55]&bit[54]&~bit[53]) | (bit[55]&bit[53]&~bit[52]) | (
		    ~bit[56]&~bit[55]&bit[54]&bit[53]&bit[52]) | (~bit[56]&~bit[55]
		    &~bit[54]&~bit[53]&bit[52]) | (bit[56]&bit[55]&~bit[54]&bit[52]);

		out_t[1] = (~bit[55]&~bit[54]&bit[53]) | (bit[56]&~bit[54]&bit[52]) | (
		    ~bit[56]&bit[53]&~bit[52]) | (bit[56]&~bit[55]&bit[53]&bit[52]) | (
		    ~bit[56]&bit[54]&~bit[53]&bit[52]) | (bit[55]&bit[54]&bit[53]&~bit[52]) | (
		    ~bit[54]&~bit[53]&~bit[52]) | (~bit[56]&bit[55]&bit[54]&bit[52]);

		out_t[0] = (~bit[56]&~bit[55]&~bit[54]) | (~bit[55]&~bit[54]&~bit[53]) | (
		    bit[56]&~bit[55]&bit[54]&bit[53]) | (bit[55]&~bit[54]&bit[53]&~bit[52]) | (
		    bit[56]&bit[55]&bit[54]&bit[52]) | (bit[56]&bit[55]&~bit[53]&bit[52]) | (
		    ~bit[56]&bit[54]&~bit[53]&bit[52]) | (~bit[56]&bit[55]&bit[54]
		    &~bit[52]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[51];
		disable_update=disable_update|(~control);
		//----------------52--------------

		out_t[5] = (~bit[55]&~bit[54]&bit[53]) | (bit[56]&bit[54]&~bit[53]) | (
		    bit[55]&bit[54]&~bit[53]) | (~bit[57]&~bit[56]&~bit[54]&bit[53]);

		out_t[4] = (bit[56]&~bit[55]&bit[54]) | (~bit[55]&bit[54]&bit[53]) | (
		    bit[57]&bit[55]&~bit[54]&bit[53]) | (bit[56]&bit[55]&~bit[54]&bit[53]) | (
		    ~bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]);

		out_t[3] = (~bit[56]&~bit[55]&~bit[54]&~bit[53]) | (~bit[57]&bit[56]
		    &bit[55]&bit[54]) | (~bit[57]&~bit[56]&~bit[55]&bit[53]) | (~bit[56]
		    &bit[55]&bit[54]&bit[53]) | (bit[57]&bit[55]&~bit[54]&bit[53]) | (
		    bit[56]&bit[55]&~bit[54]&bit[53]) | (bit[57]&bit[55]&bit[54]&~bit[53]);

		out_t[2] = (bit[55]&bit[54]&bit[53]) | (~bit[57]&bit[56]&~bit[55]&~bit[54]) | (
		    bit[57]&~bit[56]&~bit[54]&bit[53]) | (~bit[56]&bit[55]&~bit[54]
		    &~bit[53]) | (bit[56]&~bit[55]&~bit[54]&~bit[53]) | (bit[57]&bit[55]
		    &bit[54]) | (~bit[57]&bit[56]&bit[54]&~bit[53]);

		out_t[1] = (bit[56]&~bit[54]&~bit[53]) | (~bit[57]&bit[56]&bit[55]
		    &~bit[54]) | (bit[57]&~bit[56]&~bit[55]&bit[53]) | (bit[57]&~bit[56]
		    &~bit[54]&bit[53]) | (bit[57]&bit[56]&~bit[55]&~bit[53]) | (bit[57]
		    &bit[56]&bit[55]&bit[54]&bit[53]) | (~bit[57]&~bit[56]&bit[55]
		    &bit[54]&bit[53]) | (~bit[57]&bit[56]&~bit[55]&bit[54]&bit[53]) | (
		    bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (bit[57]&~bit[55]
		    &~bit[54]&bit[53]);

		out_t[0] = (~bit[57]&~bit[56]&bit[55]&bit[53]) | (~bit[56]&~bit[55]
		    &bit[54]&~bit[53]) | (bit[57]&bit[56]&bit[55]&~bit[54]&bit[53]) | (
		    ~bit[57]&bit[56]&~bit[55]&~bit[54]&bit[53]) | (~bit[57]&bit[56]
		    &bit[55]&bit[54]&~bit[53]) | (bit[57]&~bit[56]&bit[54]&bit[53]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[52];
		disable_update=disable_update|(~control);
		//----------------53--------------

		out_t[5] = (bit[55]&~bit[54]) | (bit[57]&bit[56]&~bit[54]) | (~bit[58]
		    &~bit[57]&~bit[56]&~bit[55]&bit[54]);

		out_t[4] = (~bit[58]&~bit[56]&bit[55]) | (~bit[57]&~bit[56]&bit[55]) | (
		    bit[57]&bit[56]&~bit[55]) | (bit[56]&~bit[55]&bit[54]) | (bit[58]
		    &~bit[56]&bit[54]) | (bit[57]&~bit[56]&bit[54]);

		out_t[3] = (bit[57]&~bit[56]&~bit[55]&bit[54]) | (~bit[57]&bit[56]
		    &bit[55]&~bit[54]) | (bit[58]&bit[57]&~bit[56]&bit[55]&~bit[54]) | (
		    ~bit[58]&~bit[57]&bit[56]&bit[54]) | (bit[58]&~bit[57]&~bit[55]
		    &bit[54]);

		out_t[2] = (~bit[58]&bit[57]&bit[56]) | (bit[57]&bit[56]&~bit[55]) | (
		    bit[56]&bit[55]&bit[54]) | (bit[58]&~bit[57]&~bit[56]&~bit[55]) | (
		    ~bit[58]&bit[57]&~bit[55]&bit[54]) | (~bit[58]&~bit[57]&~bit[56]
		    &~bit[54]) | (bit[58]&bit[57]&~bit[56]&bit[55]&~bit[54]);

		out_t[1] = (~bit[58]&bit[56]&~bit[55]) | (bit[58]&~bit[57]&~bit[56]
		    &bit[55]) | (~bit[58]&bit[57]&bit[56]&bit[54]) | (bit[58]&~bit[57]
		    &bit[55]&bit[54]) | (~bit[57]&~bit[56]&bit[55]&bit[54]) | (~bit[57]
		    &bit[56]&~bit[55]&~bit[54]) | (bit[58]&bit[57]&bit[56]&bit[55]
		    &~bit[54]) | (bit[58]&~bit[56]&~bit[55]&bit[54]) | (bit[57]&~bit[56]
		    &~bit[55]&~bit[54]);

		out_t[0] = (bit[58]&bit[57]&~bit[55]) | (bit[57]&~bit[55]&bit[54]) | (
		    ~bit[56]&~bit[55]&~bit[54]) | (~bit[58]&~bit[57]&bit[56]&bit[55]) | (
		    bit[58]&~bit[57]&bit[56]&bit[54]) | (~bit[58]&~bit[56]&bit[55]
		    &bit[54]) | (~bit[58]&bit[57]&bit[55]&~bit[54]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[53];
		disable_update=disable_update|(~control);
		//----------------54--------------
		out_t[5] = (bit[57]&~bit[55]) | (bit[56]&~bit[55]) | (bit[59]&bit[58]
		    &bit[57]&bit[56]);

		out_t[4] = (bit[57]&~bit[56]) | (~bit[56]&bit[55]) | (bit[59]&bit[58]
		    &bit[57]&bit[55]);

		out_t[3] = (~bit[59]&~bit[57]&bit[56]) | (~bit[58]&~bit[57]&bit[56]) | (
		    ~bit[59]&~bit[57]&bit[55]) | (~bit[58]&~bit[57]&bit[55]) | (~bit[57]
		    &bit[56]&bit[55]);

		out_t[2] = (~bit[59]&~bit[58]&bit[56]) | (~bit[58]&~bit[57]&bit[55]) | (
		    ~bit[57]&~bit[56]&bit[55]) | (~bit[58]&bit[57]&~bit[55]) | (bit[59]
		    &bit[58]&bit[57]&bit[56]&bit[55]) | (bit[59]&bit[58]&~bit[57]&bit[56]
		    &~bit[55]) | (~bit[58]&~bit[56]&bit[55]);

		out_t[1] = (~bit[59]&bit[58]&bit[57]) | (~bit[59]&bit[57]&bit[56]) | (
		    ~bit[58]&~bit[57]&~bit[56]) | (bit[57]&bit[56]&bit[55]) | (bit[59]
		    &~bit[57]&~bit[56]&bit[55]) | (bit[59]&~bit[57]&bit[56]&~bit[55]) | (
		    ~bit[59]&~bit[58]&bit[56]&bit[55]);

		out_t[0] = (~bit[58]&bit[56]&bit[55]) | (bit[58]&~bit[57]&bit[56]&~bit[55]) | (
		    ~bit[58]&~bit[57]&~bit[56]&~bit[55]) | (~bit[60]&bit[59]&bit[57]
		    &bit[56]&bit[55]) | (~bit[59]&~bit[57]&bit[56]) | (~bit[59]&bit[57]
		    &~bit[56]) | (~bit[59]&~bit[56]&bit[55]) | (~bit[59]&bit[58]&bit[57]
		    &~bit[55]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[54];
		disable_update=disable_update|(~control);
		//----------------55--------------

		out_t[5] = (~bit[59]&~bit[56]) | (~bit[58]&~bit[56]) | (~bit[57]&~bit[56]);

		out_t[4] = (~bit[58]&~bit[57]) | (~bit[57]&bit[56]) | (~bit[60]&~bit[59]
		    &~bit[57]) | (bit[59]&bit[58]&bit[57]&~bit[56]);

		out_t[3] = (~bit[58]&bit[56]) | (bit[59]&bit[58]&~bit[56]) | (~bit[60]
		    &~bit[59]&~bit[58]&bit[57]) | (bit[60]&bit[58]&~bit[57]&~bit[56]);

		out_t[2] = (~bit[59]&bit[56]) | (bit[60]&~bit[59]&~bit[57]) | (~bit[59]
		    &~bit[58]&~bit[57]) | (bit[59]&bit[57]&~bit[56]) | (~bit[60]&bit[59]
		    &bit[58]&~bit[57]) | (bit[60]&~bit[58]&bit[57]&~bit[56]);

		out_t[1] = (~bit[60]&bit[59]&~bit[58]) | (~bit[60]&bit[58]&bit[57]) | (
		    bit[60]&bit[58]&~bit[57]) | (bit[59]&~bit[58]&bit[56]) | (bit[60]
		    &~bit[59]&~bit[58]&bit[57]&~bit[56]) | (~bit[59]&bit[58]&~bit[57]
		    &bit[56]) | (bit[59]&bit[58]&bit[57]&~bit[56]);

		out_t[0] = (~bit[60]&bit[56]) | (bit[58]&bit[57]&bit[56]) | (~bit[60]
		    &bit[59]&bit[58]&bit[57]) | (~bit[60]&~bit[59]&~bit[58]&~bit[57]) | (
		    bit[60]&~bit[59]&bit[57]&~bit[56]) | (bit[60]&bit[59]&~bit[57]
		    &~bit[56]) | (bit[60]&bit[58]&~bit[57]&~bit[56]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[55];
		disable_update=disable_update|(~control);
		//----------------56--------------

		out_t[5] = (~bit[60]&~bit[57]) | (~bit[59]&~bit[57]) | (~bit[58]&~bit[57]);

		out_t[4] = (~bit[59]&~bit[58]) | (~bit[60]&~bit[58]&bit[57]) | (bit[60]
		    &bit[59]&bit[58]&~bit[57]);

		out_t[3] = (~bit[60]&~bit[59]&bit[58]) | (bit[60]&bit[59]&~bit[58]) | (
		    ~bit[59]&bit[58]&bit[57]) | (bit[60]&bit[59]&~bit[57]) | (bit[59]
		    &~bit[58]&~bit[57]);

		out_t[2] = (~bit[60]&~bit[59]&~bit[58]) | (bit[60]&bit[58]&~bit[57]) | (
		    ~bit[60]&~bit[58]&~bit[57]) | (~bit[60]&bit[59]&bit[58]&bit[57]) | (
		    bit[60]&bit[59]&~bit[58]&bit[57]) | (~bit[61]&bit[59]&~bit[57]);

		out_t[1] = (~bit[60]&~bit[59]&bit[57]) | (~bit[59]&~bit[58]&bit[57]) | (
		    ~bit[61]&~bit[60]&~bit[58]&~bit[57]) | (bit[61]&~bit[60]&bit[59]
		    &bit[58]&~bit[57]) | (bit[61]&bit[60]&bit[59]&~bit[58]&~bit[57]) | (
		    ~bit[61]&bit[60]&bit[59]&bit[57]) | (~bit[61]&bit[59]&bit[58]&bit[57]) | (
		    ~bit[61]&bit[60]&~bit[59]&~bit[57]) | (~bit[61]&bit[60]&bit[58]
		    &~bit[57]);

		out_t[0] = (bit[61]&bit[60]&bit[59]) | (~bit[61]&~bit[59]&bit[58]) | (
		    ~bit[61]&~bit[60]&bit[57]) | (bit[61]&~bit[58]&~bit[57]) | (~bit[61]
		    &bit[60]&~bit[59]) | (bit[60]&~bit[59]&~bit[57]) | (~bit[60]&bit[59]
		    &bit[58]&bit[57]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[56];
		disable_update=disable_update|(~control);
		//----------------57--------------

		out_t[5] = (~bit[61]&~bit[58]) | (~bit[60]&~bit[58]) | (~bit[59]&~bit[58]);

		out_t[4] = (~bit[60]&~bit[59]) | (~bit[62]&~bit[61]&~bit[59]) | (~bit[61]
		    &~bit[59]&~bit[58]) | (bit[61]&bit[60]&bit[59]&~bit[58]);

		out_t[3] = (~bit[62]&~bit[61]&~bit[60]) | (~bit[61]&~bit[60]&bit[59]) | (
		    ~bit[62]&~bit[60]&bit[58]) | (bit[61]&bit[60]&~bit[58]) | (bit[61]
		    &bit[60]&~bit[59]) | (bit[62]&~bit[61]&~bit[59]&bit[58]);

		out_t[2] = (bit[61]&~bit[58]) | (bit[62]&bit[61]&~bit[60]) | (bit[61]
		    &bit[60]&~bit[59]) | (~bit[61]&bit[60]&bit[59]&bit[58]) | (bit[62]
		    &~bit[60]&~bit[59]&~bit[58]) | (~bit[62]&~bit[61]&~bit[60]&~bit[59]
		    &bit[58]) | (bit[62]&bit[60]&~bit[59]&bit[58]);

		out_t[1] = (bit[62]&bit[61]&bit[59]) | (~bit[62]&bit[61]&~bit[58]) | (
		    ~bit[62]&bit[60]&~bit[58]) | (bit[61]&bit[60]&~bit[58]) | (bit[62]
		    &~bit[61]&~bit[60]&~bit[59]) | (~bit[62]&~bit[61]&bit[59]&bit[58]) | (
		    bit[62]&~bit[61]&~bit[59]&bit[58]) | (bit[61]&bit[60]&bit[59]);

		out_t[0] = (~bit[62]&bit[61]&bit[60]) | (bit[62]&~bit[61]&bit[60]&~bit[58]) | (
		    bit[62]&bit[61]&~bit[60]&bit[58]) | (~bit[61]&~bit[60]&bit[59]
		    &bit[58]) | (~bit[62]&~bit[60]&bit[59]&~bit[58]) | (bit[62]&~bit[60]
		    &~bit[59]&~bit[58]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[57];
		disable_update=disable_update|(~control);
		//----------------58--------------
		out_t[5] = (bit[62]&~bit[59]) | (bit[61]&~bit[59]) | (bit[60]&~bit[59]);

		out_t[4] = (~bit[60]&bit[59]) | (~bit[62]&bit[61]&~bit[60]) | (bit[62]
		    &~bit[61]&~bit[60]) | (~bit[63]&bit[61]&~bit[60]);

		out_t[3] = (~bit[63]&~bit[61]&bit[60]) | (~bit[62]&~bit[61]&bit[60]) | (
		    ~bit[63]&~bit[61]&bit[59]) | (~bit[62]&~bit[61]&bit[59]) | (~bit[61]
		    &bit[60]&bit[59]) | (~bit[63]&~bit[62]&bit[60]&bit[59]) | (bit[63]
		    &bit[62]&bit[61]&~bit[60]&~bit[59]);

		out_t[2] = (bit[63]&bit[62]&~bit[61]) | (bit[63]&~bit[62]&bit[59]) | (
		    ~bit[63]&~bit[62]&bit[60]&~bit[59]) | (bit[63]&bit[62]&~bit[60]
		    &~bit[59]) | (bit[62]&~bit[61]&~bit[60]&~bit[59]) | (~bit[62]&~bit[61]
		    &bit[59]) | (~bit[63]&bit[62]&bit[60]&bit[59]) | (~bit[63]&bit[61]
		    &~bit[60]&bit[59]);

		out_t[1] = (bit[63]&~bit[62]&bit[60]) | (~bit[63]&~bit[62]&bit[61]
		    &~bit[60]) | (bit[63]&bit[61]&bit[60]&bit[59]) | (bit[62]&~bit[61]
		    &~bit[60]&bit[59]) | (~bit[63]&bit[62]&~bit[61]&~bit[59]) | (bit[63]
		    &bit[62]&bit[61]&~bit[60]&~bit[59]) | (~bit[63]&~bit[62]&~bit[61]
		    &bit[59]);

		out_t[0] = (~bit[63]&~bit[62]&bit[60]&bit[59]) | (~bit[63]&~bit[61]
		    &bit[60]&bit[59]) | (bit[63]&~bit[61]&~bit[60]&bit[59]) | (bit[63]
		    &~bit[61]&bit[60]&~bit[59]) | (~bit[62]&bit[61]&~bit[60]&~bit[59]) | (
		    bit[63]&bit[62]&bit[61]&bit[60]&bit[59]) | (~bit[63]&bit[62]&bit[61]
		    &bit[60]&~bit[59]) | (~bit[63]&bit[62]&~bit[61]&~bit[60]&~bit[59]) | (
		    bit[63]&~bit[62]&~bit[60]&bit[59]) | (bit[63]&~bit[62]&bit[60]
		    &~bit[59]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[58];
		disable_update=disable_update|(~control);
		//----------------59--------------

		out_t[5] = (bit[62]&~bit[60]) | (bit[61]&~bit[60]) | (~bit[62]&~bit[61]
		    &bit[60]) | (bit[63]&~bit[62]&~bit[61]);

		out_t[4] = (bit[62]&~bit[61]) | (~bit[62]&bit[61]&bit[60]) | (bit[63]
		    &~bit[61]&~bit[60]) | (~bit[64]&~bit[63]&~bit[62]&bit[61]);

		out_t[3] = (bit[62]&bit[60]) | (bit[63]&~bit[62]&~bit[60]) | (~bit[64]
		    &bit[62]&bit[61]) | (bit[64]&~bit[63]&bit[61]&~bit[60]);

		out_t[2] = (~bit[64]&~bit[63]&bit[60]) | (~bit[63]&~bit[62]&bit[60]) | (
		    bit[64]&bit[63]&bit[61]&~bit[60]) | (bit[63]&~bit[62]&bit[61]&~bit[60]) | (
		    ~bit[63]&bit[62]&~bit[61]&~bit[60]) | (bit[64]&~bit[62]&bit[61]
		    &~bit[60]);

		out_t[1] = (~bit[63]&bit[62]&bit[61]) | (~bit[64]&~bit[62]&bit[60]) | (
		    ~bit[64]&bit[63]&~bit[62]&bit[61]) | (bit[64]&~bit[63]&bit[62]
		    &bit[60]) | (bit[64]&~bit[63]&bit[61]&~bit[60]) | (bit[64]&bit[62]
		    &bit[61]&~bit[60]) | (~bit[64]&bit[63]&bit[62]&~bit[61]&~bit[60]);

		out_t[0] = (~bit[64]&~bit[63]&bit[62]) | (~bit[64]&bit[62]&bit[60]) | (
		    bit[64]&bit[63]&bit[62]&~bit[60]) | (~bit[64]&~bit[62]&~bit[61]
		    &~bit[60]) | (~bit[63]&~bit[62]&bit[61]&bit[60]) | (bit[64]&~bit[63]
		    &~bit[62]&~bit[60]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[59];
		disable_update=disable_update|(~control);
		//----------------60--------------

		out_t[5] = (~bit[63]&~bit[62]&bit[61]) | (bit[64]&bit[62]&~bit[61]) | (
		    bit[63]&bit[62]&~bit[61]);

		out_t[4] = (bit[64]&~bit[63]&bit[62]) | (bit[63]&~bit[62]&bit[61]) | (
		    ~bit[65]&~bit[64]&bit[63]&bit[62]) | (~bit[64]&bit[62]&bit[61]);

		out_t[3] = (bit[64]&bit[63]&bit[62]) | (bit[63]&~bit[62]&bit[61]) | (
		    ~bit[64]&~bit[63]&bit[62]&bit[61]) | (bit[65]&bit[63]&bit[62]&~bit[61]) | (
		    ~bit[65]&~bit[63]&bit[62]&bit[61]) | (~bit[65]&~bit[64]&~bit[62]
		    &bit[61]);

		out_t[2] = (~bit[62]&~bit[61]) | (~bit[65]&bit[64]&~bit[62]) | (bit[65]
		    &~bit[64]&~bit[62]) | (~bit[65]&bit[63]&bit[61]) | (~bit[65]&bit[64]
		    &~bit[61]) | (bit[65]&bit[64]&bit[62]&bit[61]) | (bit[65]&~bit[64]
		    &bit[63]&~bit[61]);

		out_t[1] = (~bit[63]&~bit[61]) | (bit[65]&bit[64]&~bit[63]) | (bit[65]
		    &~bit[62]&bit[61]) | (bit[65]&bit[62]&~bit[61]) | (~bit[65]&~bit[64]
		    &~bit[63]&bit[62]) | (~bit[64]&bit[63]&~bit[62]&bit[61]) | (~bit[65]
		    &bit[64]&bit[63]&bit[62]&bit[61]);

		out_t[0] = (~bit[65]&~bit[64]&~bit[62]) | (~bit[64]&~bit[63]&~bit[61]) | (
		    ~bit[64]&~bit[62]&~bit[61]) | (bit[65]&~bit[64]&bit[62]&bit[61]) | (
		    bit[65]&bit[64]&~bit[62]&bit[61]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[60];
		disable_update=disable_update|(~control);
		//----------------61--------------

		out_t[5] = (bit[63]&~bit[62]) | (~bit[64]&~bit[63]&bit[62]);

		out_t[4] = (~bit[65]&~bit[64]&bit[63]) | (bit[64]&~bit[63]&bit[62]) | (
		    ~bit[66]&~bit[64]&bit[63]&~bit[62]);

		out_t[3] = (bit[64]&bit[63]) | (bit[66]&bit[65]&bit[63]) | (~bit[65]
		    &bit[64]&bit[62]) | (bit[65]&bit[63]&bit[62]) | (~bit[66]&~bit[65]
		    &bit[63]&~bit[62]);

		out_t[2] = (~bit[63]&bit[62]) | (~bit[65]&~bit[64]&~bit[63]) | (bit[65]
		    &~bit[64]&bit[62]) | (~bit[66]&~bit[65]&bit[64]&bit[63]) | (~bit[65]
		    &bit[64]&bit[63]&~bit[62]) | (bit[66]&~bit[64]&bit[63]&~bit[62]);

		out_t[1] = (~bit[65]&~bit[64]) | (~bit[64]&~bit[63]&~bit[62]) | (bit[66]
		    &~bit[65]&bit[63]&bit[62]) | (bit[66]&~bit[64]&bit[63]) | (~bit[66]
		    &bit[65]&bit[63]&bit[62]) | (~bit[66]&bit[64]&~bit[63]&bit[62]) | (
		    ~bit[66]&bit[64]&bit[63]&~bit[62]);

		out_t[0] = (~bit[65]&bit[64]&~bit[63]) | (bit[64]&~bit[63]&bit[62]) | (
		    bit[65]&~bit[64]&~bit[62]) | (~bit[65]&bit[64]&bit[62]) | (~bit[66]
		    &~bit[64]&bit[62]) | (bit[66]&bit[64]&bit[63]&~bit[62]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[61];
		disable_update=disable_update|(~control);
		//----------------62--------------

		out_t[5] = (bit[64]&~bit[63]) | (bit[66]&bit[65]&~bit[63]) | (~bit[67]
		    &~bit[66]&~bit[65]&~bit[64]&bit[63]);

		out_t[4] = (~bit[66]&~bit[65]&bit[64]) | (bit[66]&bit[65]&~bit[64]) | (
		    bit[66]&~bit[64]&bit[63]) | (bit[65]&~bit[64]&bit[63]) | (~bit[65]
		    &bit[64]&~bit[63]) | (bit[67]&~bit[66]&~bit[65]&bit[63]);

		out_t[3] = (~bit[66]&bit[65]&bit[64]) | (bit[66]&~bit[65]&bit[63]) | (
		    bit[67]&~bit[65]&~bit[64]&bit[63]) | (~bit[67]&bit[66]&bit[65]
		    &~bit[64]&~bit[63]);

		out_t[2] = (bit[66]&bit[64]&bit[63]) | (bit[65]&bit[64]&~bit[63]) | (
		    bit[67]&bit[66]&bit[65]&~bit[63]) | (~bit[66]&~bit[65]&~bit[64]
		    &~bit[63]) | (bit[67]&~bit[66]&~bit[64]&bit[63]) | (~bit[67]&bit[65]
		    &~bit[64]&bit[63]);

		out_t[1] = (~bit[66]&~bit[65]&~bit[63]) | (~bit[66]&bit[64]&~bit[63]) | (
		    ~bit[67]&bit[66]&bit[64]&bit[63]) | (bit[67]&bit[65]&~bit[64]&bit[63]) | (
		    ~bit[67]&~bit[66]&~bit[64]&bit[63]) | (~bit[67]&bit[65]&bit[64]
		    &~bit[63]);

		out_t[0] = (~bit[67]&~bit[66]&bit[64]) | (~bit[67]&bit[66]&~bit[64]) | (
		    ~bit[67]&bit[65]&bit[63]) | (bit[66]&bit[65]&~bit[64]&bit[63]) | (
		    bit[66]&~bit[65]&~bit[64]&~bit[63]) | (~bit[67]&~bit[65]&bit[64]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[62];
		disable_update=disable_update|(~control);
		//----------------63--------------

		out_t[5] = (bit[66]&~bit[64]) | (bit[65]&~bit[64]) | (~bit[68]&~bit[66]
		    &~bit[65]&bit[64]) | (~bit[67]&~bit[66]&~bit[65]&bit[64]);

		out_t[4] = (bit[66]&~bit[65]) | (~bit[67]&~bit[66]&bit[65]) | (~bit[68]
		    &~bit[66]&bit[65]&bit[64]) | (bit[68]&bit[67]&~bit[65]&bit[64]);

		out_t[3] = (~bit[68]&~bit[67]&bit[66]) | (~bit[67]&bit[66]&bit[65]) | (
		    bit[68]&bit[67]&~bit[66]&bit[64]) | (bit[66]&bit[65]&bit[64]) | (
		    ~bit[68]&bit[67]&bit[65]&~bit[64]) | (bit[67]&~bit[66]&bit[65]
		    &~bit[64]);

		out_t[2] = (bit[68]&bit[66]&~bit[65]) | (bit[67]&bit[66]&~bit[65]) | (
		    ~bit[67]&bit[66]&bit[64]) | (~bit[68]&~bit[67]&bit[66]&bit[65]) | (
		    ~bit[68]&~bit[67]&~bit[65]&bit[64]) | (bit[67]&~bit[66]&bit[65]
		    &~bit[64]) | (bit[68]&bit[67]&~bit[66]&bit[64]) | (bit[68]&bit[67]
		    &bit[65]&~bit[64]);

		out_t[1] = (bit[68]&~bit[67]&~bit[65]) | (bit[68]&~bit[66]&bit[64]) | (
		    ~bit[68]&~bit[67]&bit[65]&bit[64]) | (bit[68]&bit[66]&bit[65]&~bit[64]) | (
		    ~bit[68]&~bit[66]&bit[65]&~bit[64]) | (~bit[67]&~bit[66]&~bit[65]
		    &~bit[64]);

		out_t[0] = (~bit[68]&bit[67]&bit[66]) | (bit[68]&bit[67]&~bit[66]) | (
		    ~bit[68]&~bit[67]&~bit[66]&bit[65]&bit[64]) | (bit[67]&~bit[66]
		    &~bit[65]) | (bit[68]&~bit[67]&~bit[65]&bit[64]) | (~bit[67]&bit[66]
		    &~bit[65]&bit[64]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[63];
		disable_update=disable_update|(~control);
		//----------------64--------------

		out_t[5] = (bit[66]&~bit[65]) | (~bit[67]&~bit[66]&bit[65]) | (~bit[69]
		    &~bit[68]&~bit[66]&bit[65]);

		out_t[4] = (~bit[67]&bit[66]) | (~bit[69]&~bit[68]&bit[66]) | (~bit[68]
		    &bit[66]&~bit[65]) | (bit[69]&bit[67]&~bit[66]&bit[65]) | (bit[68]
		    &bit[67]&~bit[66]&bit[65]);

		out_t[3] = (~bit[69]&bit[68]&bit[67]&bit[66]) | (~bit[69]&~bit[68]
		    &~bit[67]&bit[66]) | (bit[69]&~bit[68]&bit[67]&bit[65]) | (bit[68]
		    &bit[67]&~bit[66]&bit[65]);

		out_t[2] = (bit[69]&~bit[68]&bit[65]) | (bit[68]&bit[66]&~bit[65]) | (
		    ~bit[69]&bit[68]&~bit[67]) | (~bit[68]&~bit[67]&~bit[66]) | (bit[69]
		    &bit[66]&bit[65]) | (bit[69]&~bit[67]&~bit[65]);

		out_t[1] = (~bit[68]&~bit[67]&~bit[65]) | (bit[69]&bit[68]&bit[67]
		    &bit[66]) | (~bit[69]&~bit[68]&bit[67]&bit[66]) | (bit[69]&~bit[68]
		    &~bit[67]&bit[66]) | (~bit[69]&~bit[68]&~bit[67]&~bit[66]) | (
		    ~bit[69]&~bit[67]&bit[66]&~bit[65]) | (~bit[69]&bit[68]&bit[67]
		    &~bit[66]&bit[65]) | (bit[69]&bit[68]&~bit[67]&~bit[66]&bit[65]);

		out_t[0] = (~bit[69]&bit[68]&bit[66]&bit[65]) | (bit[68]&bit[67]&~bit[66]
		    &bit[65]) | (bit[68]&bit[67]&bit[66]&~bit[65]) | (~bit[68]&~bit[67]
		    &bit[66]&~bit[65]) | (~bit[68]&bit[67]&~bit[66]&~bit[65]) | (bit[69]
		    &~bit[68]&bit[67]&bit[66]&bit[65]) | (bit[69]&~bit[68]&~bit[67]
		    &~bit[66]&bit[65]) | (~bit[69]&bit[67]&bit[66]&~bit[65]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[64];
		disable_update=disable_update|(~control);
		//----------------65--------------

		out_t[5] = (bit[67]&~bit[66]) | (bit[69]&bit[68]&~bit[66]) | (~bit[70]
		    &~bit[68]&~bit[67]&bit[66]) | (~bit[69]&~bit[68]&~bit[67]&bit[66]);

		out_t[4] = (~bit[69]&~bit[68]&bit[67]) | (bit[69]&bit[68]&~bit[67]) | (
		    bit[68]&~bit[67]&bit[66]) | (~bit[70]&~bit[68]&bit[67]&bit[66]) | (
		    bit[70]&bit[69]&~bit[67]&bit[66]);

		out_t[3] = (~bit[69]&bit[68]&bit[67]) | (~bit[69]&bit[68]&bit[66]) | (
		    ~bit[70]&bit[67]&~bit[66]) | (bit[70]&bit[69]&~bit[68]&bit[66]) | (
		    bit[69]&bit[68]&~bit[67]&~bit[66]) | (~bit[70]&bit[68]&bit[66]) | (
		    bit[69]&~bit[68]&bit[67]&~bit[66]);

		out_t[2] = (bit[70]&bit[69]&bit[67]) | (bit[70]&bit[69]&bit[66]) | (
		    ~bit[70]&~bit[69]&~bit[67]&bit[66]) | (bit[69]&~bit[68]&bit[67]
		    &~bit[66]) | (~bit[70]&~bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[69]
		    &~bit[68]&~bit[67]&bit[66]);

		out_t[1] = (bit[70]&~bit[69]&bit[68]&bit[66]) | (~bit[70]&~bit[69]
		    &bit[67]&bit[66]) | (bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[70]
		    &bit[69]&bit[67]&~bit[66]) | (bit[69]&~bit[68]&bit[67]&~bit[66]) | (
		    ~bit[69]&~bit[68]&~bit[67]&~bit[66]) | (~bit[70]&bit[69]&bit[68]
		    &~bit[67]&~bit[66]) | (bit[70]&bit[69]&~bit[68]&bit[66]);

		out_t[0] = (bit[70]&bit[69]&bit[68]) | (bit[69]&~bit[67]&~bit[66]) | (
		    ~bit[70]&bit[68]&bit[67]&bit[66]) | (~bit[70]&~bit[68]&~bit[67]
		    &bit[66]) | (~bit[70]&bit[69]&~bit[68]&~bit[66]) | (bit[70]&~bit[68]
		    &bit[67]&bit[66]) | (bit[70]&~bit[69]&bit[67]&~bit[66]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[65];
		disable_update=disable_update|(~control);
		//----------------66--------------

		out_t[5] = (~bit[70]&bit[69]&~bit[67]) | (~bit[71]&bit[68]&~bit[67]) | (
		    ~bit[69]&bit[68]&~bit[67]) | (bit[70]&~bit[68]&~bit[67]);

		out_t[4] = (~bit[69]&bit[67]) | (~bit[68]&bit[67]) | (~bit[70]&bit[69]
		    &~bit[68]) | (bit[70]&~bit[69]&~bit[68]) | (bit[71]&bit[70]&bit[69]
		    &bit[68]&~bit[67]);

		out_t[3] = (~bit[70]&bit[69]&bit[67]) | (bit[70]&~bit[68]&~bit[67]) | (
		    ~bit[71]&bit[69]&bit[68]&bit[67]) | (bit[71]&bit[70]&bit[69]&~bit[67]) | (
		    ~bit[70]&~bit[69]&bit[68]&~bit[67]) | (~bit[69]&~bit[68]&bit[67]) | (
		    ~bit[71]&~bit[69]&bit[68]&~bit[67]);

		out_t[2] = (~bit[71]&~bit[70]&bit[68]) | (bit[70]&bit[69]&~bit[68]) | (
		    ~bit[70]&~bit[69]&bit[67]) | (bit[71]&bit[70]&bit[68]&~bit[67]) | (
		    ~bit[71]&bit[70]&~bit[68]&~bit[67]) | (bit[71]&bit[69]&bit[68]
		    &bit[67]);

		out_t[1] = (bit[70]&bit[69]&~bit[68]) | (bit[70]&~bit[68]&bit[67]) | (
		    ~bit[71]&bit[69]&bit[68]&bit[67]) | (bit[71]&~bit[69]&bit[68]&~bit[67]) | (
		    bit[71]&bit[70]&bit[69]);

		out_t[0] = (~bit[71]&bit[69]&~bit[68]) | (bit[71]&~bit[70]&bit[69]
		    &bit[68]) | (bit[71]&bit[69]&bit[68]&bit[67]) | (bit[70]&~bit[69]
		    &bit[68]&~bit[67]) | (~bit[71]&~bit[70]&bit[67]) | (~bit[71]&~bit[69]
		    &bit[67]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[66];
		disable_update=disable_update|(~control);
		//----------------67--------------

		out_t[5] = (~bit[68]) | (~bit[71]&~bit[70]&~bit[69]);

		out_t[4] = (~bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (bit[71]
		    &~bit[70]&~bit[69]) | (bit[70]&~bit[69]&bit[68]) | (~bit[72]&~bit[69]
		    &~bit[68]) | (~bit[71]&~bit[69]&~bit[68]);

		out_t[3] = (bit[72]&~bit[70]&bit[69]) | (bit[71]&~bit[70]&bit[68]) | (
		    ~bit[71]&~bit[70]&~bit[68]) | (~bit[72]&~bit[71]&bit[69]&~bit[68]) | (
		    ~bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[72]&bit[71]
		    &bit[70]&~bit[69]&~bit[68]) | (~bit[72]&~bit[70]&~bit[68]);

		out_t[2] = (bit[72]&bit[71]&~bit[69]) | (bit[71]&~bit[69]&bit[68]) | (
		    ~bit[72]&bit[71]&~bit[70]&bit[68]) | (bit[72]&~bit[71]&bit[69]
		    &bit[68]) | (~bit[71]&bit[70]&bit[69]&bit[68]) | (bit[71]&bit[70]
		    &bit[69]&~bit[68]) | (~bit[72]&~bit[71]&~bit[70]&bit[69]&~bit[68]) | (
		    ~bit[72]&~bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[72]&bit[70]
		    &~bit[69]&bit[68]) | (bit[72]&bit[70]&bit[69]&~bit[68]);

		out_t[1] = (~bit[72]&bit[70]&bit[68]) | (~bit[72]&~bit[69]&bit[68]) | (
		    bit[72]&~bit[71]&~bit[70]&bit[69]) | (~bit[72]&~bit[71]&~bit[70]
		    &~bit[69]) | (bit[71]&bit[70]&bit[69]&bit[68]) | (~bit[72]&bit[71]
		    &bit[69]&~bit[68]) | (bit[72]&bit[71]&~bit[70]&~bit[69]&~bit[68]) | (
		    bit[72]&~bit[71]&bit[70]&~bit[69]) | (bit[72]&~bit[71]&bit[70]
		    &~bit[68]);

		out_t[0] = (~bit[72]&bit[71]&~bit[70]&bit[68]) | (~bit[72]&bit[71]
		    &bit[69]&bit[68]) | (~bit[72]&~bit[70]&bit[69]&bit[68]) | (~bit[71]
		    &bit[70]&~bit[69]&bit[68]) | (bit[72]&~bit[71]&bit[69]&~bit[68]) | (
		    bit[72]&~bit[70]&bit[69]&~bit[68]) | (~bit[71]&~bit[70]&~bit[69]
		    &~bit[68]) | (bit[72]&bit[71]&bit[70]&~bit[69]&~bit[68]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[67];
		disable_update=disable_update|(~control);
		//----------------68--------------

		out_t[5] = (bit[71]&~bit[69]) | (bit[70]&~bit[69]) | (~bit[71]&~bit[70]
		    &bit[69]) | (~bit[73]&~bit[72]&~bit[70]&bit[69]);

		out_t[4] = (~bit[73]&~bit[71]&bit[70]) | (~bit[72]&~bit[71]&bit[70]) | (
		    bit[73]&bit[71]&~bit[70]) | (bit[72]&bit[71]&~bit[70]) | (~bit[71]
		    &bit[70]&~bit[69]) | (~bit[72]&bit[71]&~bit[69]);

		out_t[3] = (bit[72]&bit[71]&~bit[69]) | (bit[71]&~bit[70]&~bit[69]) | (
		    ~bit[72]&bit[71]&bit[70]&bit[69]) | (bit[73]&bit[72]&~bit[71]&bit[70]
		    &bit[69]) | (~bit[73]&bit[72]&~bit[70]&bit[69]) | (bit[73]&~bit[72]
		    &~bit[70]&bit[69]) | (~bit[72]&~bit[71]&~bit[70]&bit[69]);

		out_t[2] = (~bit[72]&bit[71]&~bit[70]) | (~bit[73]&bit[71]&bit[69]) | (
		    bit[73]&bit[72]&~bit[71]&bit[69]) | (~bit[73]&~bit[72]&~bit[70]
		    &bit[69]) | (bit[72]&bit[71]&bit[70]&~bit[69]) | (~bit[72]&~bit[71]
		    &bit[70]&~bit[69]) | (bit[73]&bit[72]&~bit[70]&bit[69]) | (~bit[73]
		    &~bit[71]&bit[70]&~bit[69]);

		out_t[1] = (bit[73]&bit[72]&bit[70]) | (bit[72]&bit[71]&bit[70]) | (
		    bit[73]&~bit[70]&bit[69]) | (~bit[73]&~bit[72]&~bit[71]&bit[70]) | (
		    ~bit[73]&bit[72]&bit[71]&~bit[69]) | (~bit[72]&~bit[71]&bit[70]
		    &bit[69]);

		out_t[0] = (~bit[73]&bit[71]&~bit[69]) | (~bit[72]&~bit[71]&~bit[69]) | (
		    bit[73]&~bit[72]&~bit[71]&~bit[70]) | (bit[73]&bit[72]&bit[71]
		    &bit[69]) | (~bit[73]&bit[72]&~bit[71]&bit[69]) | (~bit[73]&~bit[72]
		    &bit[70]) | (~bit[72]&bit[71]&bit[70]&bit[69]) | (bit[72]&~bit[71]
		    &bit[70]&bit[69]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[68];
		disable_update=disable_update|(~control);
		//----------------69--------------

		out_t[5] = (bit[72]&~bit[70]) | (bit[71]&~bit[70]) | (bit[73]&bit[72]
		    &bit[71]) | (~bit[73]&~bit[72]&~bit[71]&bit[70]);

		out_t[4] = (~bit[73]&bit[72]&~bit[71]) | (bit[72]&~bit[71]&~bit[70]) | (
		    bit[73]&bit[72]&bit[71]&bit[70]) | (bit[73]&~bit[72]&~bit[71]&bit[70]) | (
		    ~bit[74]&~bit[72]&bit[71]&~bit[70]) | (~bit[73]&~bit[72]&bit[71]
		    &~bit[70]) | (~bit[74]&bit[73]&~bit[71]&bit[70]);

		out_t[3] = (~bit[73]&bit[72]&~bit[70]) | (~bit[74]&~bit[73]&bit[72]
		    &~bit[71]) | (bit[74]&bit[73]&~bit[71]&bit[70]) | (bit[73]&~bit[72]
		    &~bit[71]&bit[70]) | (~bit[74]&bit[72]&~bit[71]&~bit[70]) | (~bit[74]
		    &bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[74]&~bit[73]&~bit[72]
		    &bit[71]&bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (
		    ~bit[75]&bit[73]&bit[72]&bit[71]&bit[70]);

		out_t[2] = (bit[73]&~bit[72]&bit[70]) | (bit[74]&~bit[72]&bit[71]&bit[70]) | (
		    bit[74]&bit[73]&~bit[71]&bit[70]) | (bit[73]&bit[72]&bit[71]&~bit[70]) | (
		    ~bit[74]&~bit[73]&bit[72]&~bit[71]&~bit[70]) | (bit[75]&bit[74]
		    &bit[73]&bit[71]) | (bit[74]&bit[73]&bit[71]&~bit[70]);

		out_t[1] = (bit[74]&bit[72]&~bit[71]) | (~bit[74]&~bit[72]&bit[70]) | (
		    ~bit[73]&~bit[72]&bit[70]) | (~bit[74]&bit[72]&bit[71]&~bit[70]) | (
		    bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (~bit[74]&~bit[73]
		    &bit[71]&~bit[70]) | (~bit[75]&~bit[74]&bit[73]&bit[71]&bit[70]);

		out_t[0] = (bit[74]&~bit[72]&bit[71]) | (~bit[74]&bit[72]&bit[71]&bit[70]) | (
		    ~bit[74]&~bit[72]&~bit[71]&bit[70]) | (bit[74]&~bit[73]&bit[71]
		    &~bit[70]) | (bit[73]&bit[72]&~bit[71]&~bit[70]) | (~bit[73]&~bit[72]
		    &~bit[71]&~bit[70]) | (bit[75]&bit[73]&bit[72]&bit[71]&bit[70]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[69];
		disable_update=disable_update|(~control);
		//----------------70--------------

		out_t[5] = (~bit[73]&~bit[71]) | (~bit[72]&~bit[71]) | (bit[73]&bit[72]
		    &bit[71]) | (~bit[74]&bit[73]&bit[72]);

		out_t[4] = (bit[74]&bit[73]&bit[72]) | (~bit[73]&~bit[72]&bit[71]) | (
		    ~bit[74]&bit[73]&bit[71]);

		out_t[3] = (~bit[74]&~bit[72]&~bit[71]) | (~bit[73]&~bit[72]&~bit[71]) | (
		    ~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (~bit[74]&bit[73]&bit[72]
		    &bit[71]) | (bit[74]&bit[73]&~bit[72]&bit[71]) | (bit[74]&bit[73]
		    &bit[72]&~bit[71]) | (~bit[75]&bit[74]&bit[73]);

		out_t[2] = (bit[75]&bit[74]&bit[73]) | (bit[74]&~bit[72]&bit[71]) | (
		    ~bit[75]&~bit[74]&bit[72]&bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]) | (
		    bit[75]&~bit[74]&~bit[73]) | (~bit[74]&~bit[73]&bit[72]) | (~bit[73]
		    &~bit[72]&~bit[71]) | (~bit[76]&bit[75]&bit[73]&bit[72]&bit[71]);

		out_t[1] = (bit[74]&~bit[73]&bit[72]) | (bit[76]&bit[75]&bit[72]&bit[71]) | (
		    ~bit[75]&bit[74]&bit[72]&~bit[71]) | (bit[75]&bit[74]&bit[73]&~bit[72]
		    &~bit[71]) | (~bit[75]&~bit[73]&bit[72]) | (~bit[75]&~bit[74]&bit[73]
		    &~bit[72]) | (~bit[76]&~bit[75]&bit[73]&bit[71]) | (bit[75]&~bit[74]
		    &~bit[73]&bit[71]) | (~bit[76]&bit[74]&bit[72]&bit[71]) | (~bit[75]
		    &bit[74]&~bit[72]&bit[71]) | (~bit[74]&~bit[73]&~bit[72]&~bit[71]);

		out_t[0] = (~bit[74]&~bit[72]&bit[71]) | (bit[76]&~bit[74]&bit[73]
		    &bit[71]) | (~bit[76]&bit[75]&bit[74]&bit[73]&bit[72]) | (bit[75]
		    &~bit[74]&~bit[73]&bit[72]&~bit[71]) | (~bit[75]&~bit[74]&~bit[72]) | (
		    bit[76]&~bit[75]&bit[72]&bit[71]) | (~bit[75]&~bit[73]&bit[72]
		    &bit[71]) | (~bit[75]&bit[74]&~bit[73]&~bit[71]) | (~bit[75]&bit[73]
		    &bit[72]&~bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]) | (bit[75]
		    &bit[73]&~bit[72]&~bit[71]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[70];
		disable_update=disable_update|(~control);
		//----------------71--------------

		out_t[5] = (~bit[74]&~bit[72]) | (~bit[73]&~bit[72]) | (~bit[76]&~bit[75]
		    &~bit[72]) | (bit[76]&bit[74]&bit[73]&bit[72]) | (bit[75]&bit[74]
		    &bit[73]&bit[72]);

		out_t[4] = (bit[76]&bit[74]&bit[73]) | (bit[75]&bit[74]&bit[73]) | (
		    ~bit[76]&~bit[74]&~bit[73]) | (~bit[75]&~bit[74]&~bit[73]) | (
		    ~bit[74]&~bit[73]&bit[72]) | (~bit[76]&~bit[75]&~bit[73]&bit[72]);

		out_t[3] = (~bit[76]&bit[75]&bit[74]) | (bit[76]&~bit[75]&bit[74]) | (
		    ~bit[76]&~bit[75]&~bit[74]&bit[72]) | (bit[76]&bit[75]&~bit[74]
		    &~bit[73]&~bit[72]) | (bit[75]&bit[74]&~bit[73]&bit[72]) | (bit[75]
		    &bit[74]&bit[73]&~bit[72]) | (~bit[75]&bit[74]&~bit[73]&~bit[72]);

		out_t[2] = (bit[76]&~bit[75]&bit[73]) | (bit[76]&~bit[75]&bit[72]) | (
		    ~bit[76]&bit[75]&~bit[74]&bit[72]) | (bit[76]&bit[74]&bit[73]&bit[72]) | (
		    ~bit[76]&~bit[75]&~bit[74]&~bit[72]) | (bit[76]&bit[75]&~bit[73]
		    &~bit[72]) | (~bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]);

		out_t[1] = (bit[76]&bit[75]&~bit[73]) | (bit[76]&bit[75]&~bit[72]) | (
		    ~bit[77]&bit[75]&bit[74]&bit[72]) | (bit[75]&bit[74]&~bit[73]&bit[72]) | (
		    ~bit[76]&~bit[74]&bit[73]&~bit[72]) | (bit[76]&~bit[75]&~bit[74]
		    &bit[73]&bit[72]) | (~bit[76]&~bit[75]&bit[74]&~bit[73]&~bit[72]) | (
		    ~bit[77]&bit[76]&bit[74]&bit[73]&bit[72]);

		out_t[0] = (bit[76]&~bit[75]&~bit[72]) | (~bit[76]&bit[75]&~bit[74]
		    &bit[73]) | (~bit[76]&bit[74]&~bit[73]&bit[72]) | (bit[76]&bit[74]
		    &~bit[73]&~bit[72]) | (~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (
		    bit[77]&bit[76]&bit[75]&bit[73]&bit[72]) | (bit[76]&~bit[75]&~bit[73]) | (
		    bit[76]&~bit[74]&bit[73]&bit[72]) | (~bit[77]&~bit[76]&bit[75]
		    &bit[73]&bit[72]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[71];
		disable_update=disable_update|(~control);
		//----------------72--------------

		out_t[5] = (~bit[74]&~bit[73]) | (~bit[76]&~bit[75]&~bit[73]) | (bit[77]
		    &bit[75]&bit[74]&bit[73]) | (bit[76]&bit[75]&bit[74]&bit[73]);

		out_t[4] = (bit[77]&bit[75]&bit[74]) | (bit[76]&bit[75]&bit[74]) | (
		    ~bit[76]&~bit[75]&~bit[74]) | (~bit[75]&~bit[74]&bit[73]) | (bit[76]
		    &bit[74]&~bit[73]) | (bit[75]&bit[74]&~bit[73]) | (~bit[77]&bit[76]
		    &~bit[75]&~bit[73]);

		out_t[3] = (~bit[77]&bit[75]&~bit[74]) | (~bit[76]&bit[75]&~bit[73]) | (
		    bit[77]&bit[76]&~bit[75]&~bit[73]) | (bit[76]&~bit[75]&bit[74]
		    &~bit[73]) | (bit[77]&~bit[76]&bit[75]) | (bit[76]&bit[75]&bit[74]
		    &bit[73]);

		out_t[2] = (~bit[76]&~bit[75]&bit[73]) | (bit[77]&bit[76]&~bit[73]) | (
		    bit[76]&bit[74]&~bit[73]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (
		    bit[77]&~bit[76]&bit[74]&bit[73]) | (~bit[78]&~bit[77]&bit[76]
		    &bit[75]&bit[74]) | (~bit[77]&~bit[76]&bit[75]&~bit[74]) | (~bit[77]
		    &~bit[76]&bit[75]&~bit[73]);

		out_t[1] = (bit[77]&~bit[76]&bit[75]) | (bit[76]&~bit[75]&bit[74]&bit[73]) | (
		    bit[77]&bit[76]&~bit[74]&~bit[73]) | (~bit[78]&bit[77]&bit[75]) | (
		    bit[75]&~bit[74]&bit[73]) | (bit[78]&~bit[77]&bit[76]&bit[73]) | (
		    ~bit[77]&bit[76]&~bit[74]&bit[73]) | (bit[76]&bit[75]&bit[74]&~bit[73]) | (
		    ~bit[77]&~bit[75]&bit[74]&~bit[73]) | (~bit[77]&~bit[76]&~bit[74]
		    &~bit[73]);

		out_t[0] = (~bit[77]&bit[76]&bit[74]) | (~bit[78]&bit[77]&~bit[76]
		    &bit[75]) | (bit[77]&~bit[76]&bit[75]&~bit[73]) | (bit[78]&bit[76]
		    &bit[75]&bit[74]&bit[73]) | (~bit[77]&~bit[76]&~bit[75]&bit[73]) | (
		    ~bit[76]&bit[75]&~bit[74]&bit[73]) | (bit[77]&bit[76]&~bit[75]
		    &~bit[73]) | (~bit[76]&~bit[75]&~bit[74]&~bit[73]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[72];
		disable_update=disable_update|(~control);
		//----------------73--------------

		out_t[5] = (~bit[77]&~bit[74]) | (~bit[76]&~bit[74]) | (~bit[75]&~bit[74]);

		out_t[4] = (~bit[76]&~bit[75]) | (~bit[75]&bit[74]) | (~bit[78]&~bit[77]
		    &~bit[75]) | (~bit[78]&~bit[77]&~bit[76]&bit[74]) | (bit[77]&bit[76]
		    &bit[75]&~bit[74]);

		out_t[3] = (bit[77]&bit[76]&~bit[74]) | (~bit[77]&bit[76]&bit[75]&bit[74]) | (
		    ~bit[78]&~bit[77]&~bit[76]&~bit[74]) | (bit[78]&bit[76]&~bit[75]
		    &~bit[74]) | (~bit[78]&bit[77]&~bit[76]&bit[74]) | (bit[78]&~bit[76]
		    &bit[75]&bit[74]) | (~bit[77]&~bit[76]&~bit[75]&bit[74]);

		out_t[2] = (~bit[78]&bit[77]&~bit[74]) | (bit[78]&bit[77]&~bit[76]
		    &bit[74]) | (~bit[77]&bit[76]&~bit[75]&bit[74]) | (bit[77]&bit[75]
		    &bit[74]) | (bit[77]&bit[76]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]
		    &bit[75]) | (bit[78]&~bit[77]&~bit[75]&~bit[74]);

		out_t[1] = (~bit[78]&bit[77]&bit[76]) | (~bit[78]&bit[76]&bit[74]) | (
		    bit[78]&~bit[76]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]&bit[75]) | (
		    bit[78]&bit[77]&~bit[76]&~bit[75]) | (~bit[78]&bit[77]&bit[75]
		    &bit[74]) | (~bit[78]&~bit[77]&~bit[75]&bit[74]) | (~bit[77]&~bit[76]
		    &bit[75]&~bit[74]) | (bit[78]&~bit[77]&~bit[75]&~bit[74]);

		out_t[0] = (bit[77]&bit[75]&~bit[74]) | (~bit[78]&~bit[77]&bit[76]
		    &bit[75]) | (bit[78]&~bit[77]&~bit[76]&bit[74]) | (bit[78]&bit[77]
		    &~bit[75]&bit[74]) | (~bit[77]&~bit[76]&~bit[75]&bit[74]) | (~bit[78]
		    &bit[77]&~bit[76]&~bit[74]) | (bit[78]&~bit[77]&~bit[75]&~bit[74]) | (
		    ~bit[77]&bit[76]&~bit[75]&~bit[74]);

		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[73];
		disable_update=disable_update|(~control);
		//----------------74--------------

		out_t[5] = (~bit[77]&bit[76]&~bit[75]) | (bit[77]&~bit[76]&~bit[75]) | (
		    ~bit[79]&~bit[78]&bit[76]&~bit[75]);

		out_t[4] = (bit[77]&~bit[76]) | (~bit[76]&bit[75]) | (bit[78]&bit[77]
		    &~bit[75]) | (~bit[79]&~bit[78]&~bit[77]&bit[76]) | (bit[79]&~bit[78]
		    &bit[76]&~bit[75]);

		out_t[3] = (bit[79]&bit[77]&~bit[75]) | (bit[77]&~bit[76]&~bit[75]) | (
		    bit[78]&bit[76]&~bit[75]) | (~bit[79]&bit[78]&~bit[77]&bit[75]) | (
		    bit[79]&~bit[77]&bit[76]&bit[75]) | (~bit[78]&~bit[77]&~bit[76]
		    &bit[75]);

		out_t[2] = (~bit[79]&~bit[78]&bit[77]) | (~bit[78]&bit[77]&bit[75]) | (
		    ~bit[79]&bit[76]&~bit[75]) | (bit[77]&bit[76]&~bit[75]) | (bit[79]
		    &~bit[78]&bit[76]&bit[75]) | (bit[79]&bit[78]&~bit[77]&~bit[76]
		    &bit[75]);

		out_t[1] = (bit[79]&~bit[78]&bit[76]) | (bit[79]&~bit[78]&~bit[75]) | (
		    ~bit[77]&bit[76]&~bit[75]) | (~bit[79]&~bit[78]&~bit[77]&~bit[76]) | (
		    bit[79]&bit[78]&~bit[76]&bit[75]) | (~bit[79]&bit[78]&bit[77]&bit[76]) | (
		    ~bit[79]&bit[78]&bit[77]&bit[75]) | (~bit[79]&bit[77]&bit[76]&bit[75]);

		out_t[0] = (~bit[79]&bit[77]&bit[75]) | (~bit[79]&bit[78]&bit[76]&bit[75]) | (
		    ~bit[78]&~bit[77]&~bit[76]&bit[75]) | (bit[79]&bit[77]&bit[76]
		    &~bit[75]) | (bit[78]&bit[77]&bit[76]&bit[75]) | (bit[79]&bit[78]
		    &~bit[77]&~bit[75]) | (~bit[79]&bit[78]&~bit[76]&~bit[75]);


		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[74];
		disable_update=disable_update|(~control);
		//----------------75--------------
		out_t[6] = (~bit[80]&~bit[79]&~bit[78]&~bit[77]&~bit[76]);

		out_t[5] = (bit[77]&~bit[76]) | (bit[79]&~bit[76]) | (~bit[79]&bit[78]
		    &~bit[77]) | (bit[80]&~bit[78]&~bit[77]) | (~bit[78]&~bit[77]&bit[76]);

		out_t[4] = (~bit[78]&bit[77]) | (~bit[80]&~bit[79]&bit[77]) | (bit[79]
		    &bit[78]&~bit[77]) | (bit[80]&~bit[77]&~bit[76]) | (bit[79]&~bit[77]
		    &~bit[76]) | (bit[78]&~bit[77]&~bit[76]);

		out_t[3] = (~bit[80]&bit[79]&bit[78]) | (bit[79]&bit[78]&~bit[77]) | (
		    ~bit[79]&~bit[78]&bit[76]) | (bit[79]&~bit[77]&~bit[76]) | (bit[78]
		    &~bit[77]&~bit[76]) | (bit[80]&bit[78]&~bit[76]) | (bit[80]&~bit[79]
		    &bit[78]&bit[77]) | (bit[80]&~bit[79]&~bit[78]&~bit[77]);

		out_t[2] = (bit[80]&bit[78]&bit[77]) | (~bit[80]&bit[78]&~bit[77]&bit[76]) | (
		    bit[79]&~bit[78]&~bit[77]) | (bit[79]&~bit[78]&bit[76]) | (~bit[80]
		    &bit[79]&bit[77]&~bit[76]) | (~bit[79]&~bit[78]&bit[77]&~bit[76]) | (
		    bit[80]&~bit[78]&~bit[77]&~bit[76]);

		out_t[1] = (bit[80]&~bit[79]&bit[78]) | (~bit[80]&bit[79]&bit[76]) | (
		    ~bit[80]&~bit[78]&bit[76]) | (bit[79]&~bit[77]&bit[76]) | (bit[80]
		    &~bit[79]&~bit[76]) | (~bit[80]&bit[79]&~bit[78]&~bit[77]) | (
		    bit[80]&~bit[78]&bit[77]&~bit[76]) | (~bit[79]&bit[78]&~bit[77]
		    &~bit[76]) | (~bit[79]&~bit[78]&bit[77]&~bit[76]);

		out_t[0] = (~bit[80]&bit[78]&bit[77]) | (~bit[79]&bit[78]&bit[77]) | (
		    ~bit[80]&bit[78]&~bit[76]) | (~bit[80]&bit[77]&~bit[76]) | (bit[80]
		    &~bit[79]&~bit[78]&~bit[77]) | (bit[80]&~bit[78]&bit[77]&bit[76]) | (
		    bit[80]&bit[78]&~bit[77]&bit[76]) | (~bit[80]&bit[79]&~bit[78]
		    &~bit[77]&bit[76]) | (~bit[79]&bit[78]&bit[76]);

		out[6]=(~disable_update&(out_t[6]))|(disable_update&out[6]);
		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[75];
		disable_update=disable_update|(~control);
		//----------------76--------------
		out_t[6] = ZERO;

		out_t[5] = (bit[78]&~bit[77]) | (~bit[79]&~bit[78]&bit[77]) | (bit[80]
		    &bit[79]&~bit[77]) | (~bit[81]&~bit[80]&~bit[78]&bit[77]);

		out_t[4] = (~bit[81]&~bit[79]&bit[78]) | (~bit[80]&~bit[79]&bit[78]) | (
		    bit[80]&bit[79]&~bit[78]) | (~bit[79]&bit[78]&bit[77]) | (~bit[81]
		    &~bit[80]&bit[78]&bit[77]) | (bit[81]&bit[79]&~bit[78]&bit[77]);

		out_t[3] = (~bit[81]&bit[80]&bit[79]) | (bit[80]&bit[79]&~bit[77]) | (
		    bit[81]&~bit[80]&bit[79]&bit[77]) | (bit[81]&bit[80]&bit[78]&~bit[77]) | (
		    ~bit[81]&~bit[80]&bit[78]&~bit[77]) | (~bit[81]&~bit[80]&~bit[79]
		    &~bit[78]&bit[77]) | (bit[81]&bit[79]&bit[78]);

		out_t[2] = (bit[81]&~bit[80]&bit[77]) | (~bit[81]&bit[80]&~bit[79]
		    &bit[77]) | (bit[81]&bit[79]&~bit[78]&bit[77]) | (~bit[80]&bit[79]
		    &bit[78]&~bit[77]) | (bit[81]&~bit[79]&bit[78]&~bit[77]) | (~bit[81]
		    &bit[80]&bit[79]&~bit[78]&~bit[77]) | (~bit[80]&~bit[79]&bit[78]
		    &bit[77]);

		out_t[1] = (~bit[81]&bit[80]&bit[79]) | (bit[81]&bit[79]&bit[78]&bit[77]) | (
		    ~bit[81]&bit[79]&bit[78]&~bit[77]) | (bit[80]&~bit[79]&bit[78]
		    &~bit[77]) | (~bit[80]&~bit[79]&~bit[78]&~bit[77]) | (~bit[81]
		    &~bit[80]&~bit[79]&bit[78]&bit[77]) | (bit[81]&bit[80]&~bit[78]
		    &bit[77]) | (bit[81]&~bit[79]&~bit[78]&bit[77]);

		out_t[0] = (bit[80]&~bit[77]) | (~bit[81]&bit[80]&bit[79]&bit[78]) | (
		    bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[81]&~bit[79]&bit[78]&bit[77]);

		out[6]=(~disable_update&(out_t[6]))|(disable_update&out[6]);
		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[76];
		disable_update=disable_update|(~control);
		//----------------77--------------
		out_t[6] = (~bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]);

		out_t[5] = (bit[79]&~bit[78]) | (bit[83]&bit[81]&bit[80]&bit[79]) | (
		    bit[82]&bit[81]&bit[80]&bit[79]) | (~bit[82]&~bit[81]&~bit[80]
		    &~bit[79]&bit[78]);

		out_t[4] = (bit[82]&~bit[79]&bit[78]) | (bit[81]&~bit[79]&bit[78]) | (
		    bit[80]&~bit[79]&bit[78]) | (bit[83]&bit[81]&bit[80]&bit[78]) | (
		    bit[82]&bit[81]&bit[80]&bit[78]) | (~bit[82]&~bit[80]&bit[79]&~bit[78]) | (
		    ~bit[81]&~bit[80]&bit[79]&~bit[78]);

		out_t[3] = (bit[82]&~bit[80]&bit[78]) | (bit[81]&~bit[80]&bit[78]) | (
		    bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[83]&bit[81]&bit[79]&bit[78]) | (
		    bit[82]&bit[81]&bit[79]&bit[78]) | (~bit[81]&bit[80]&bit[79]&~bit[78]) | (
		    ~bit[82]&~bit[81]&~bit[80]&bit[79]);

		out_t[2] = (~bit[82]&~bit[81]&bit[80]) | (bit[82]&~bit[81]&~bit[80]) | (
		    ~bit[81]&~bit[79]&~bit[78]) | (bit[82]&bit[81]&bit[80]&bit[79]) | (
		    ~bit[82]&bit[81]&~bit[80]&~bit[79]) | (~bit[81]&~bit[80]&bit[79]) | (
		    bit[82]&~bit[80]&~bit[78]) | (bit[83]&bit[81]&bit[80]&bit[79]) | (
		    bit[81]&bit[80]&bit[79]&~bit[78]);

		out_t[1] = (~bit[82]&~bit[81]&bit[80]&bit[78]) | (~bit[81]&bit[80]
		    &~bit[79]&bit[78]) | (bit[82]&~bit[81]&bit[79]&~bit[78]) | (bit[82]
		    &~bit[80]&bit[79]&~bit[78]) | (~bit[81]&~bit[80]&~bit[79]&~bit[78]) | (
		    ~bit[82]&bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[83]&~bit[82]
		    &bit[80]&bit[79]&bit[78]);

		out_t[0] = (bit[82]&~bit[80]&~bit[79]) | (bit[82]&~bit[80]&~bit[78]) | (
		    ~bit[80]&~bit[79]&~bit[78]) | (bit[82]&bit[81]&bit[79]&~bit[78]) | (
		    ~bit[82]&bit[81]&bit[80]&~bit[79]&bit[78]) | (~bit[82]&bit[81]
		    &~bit[80]&bit[79]) | (bit[82]&~bit[81]&bit[80]&bit[78]) | (~bit[82]
		    &~bit[81]&bit[79]&bit[78]) | (~bit[83]&bit[80]&bit[79]&bit[78]);

		out[6]=(~disable_update&(out_t[6]))|(disable_update&out[6]);
		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[77];
		disable_update=disable_update|(~control);
		//----------------78--------------
		out_t[6] = (~bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]);

		out_t[5] = (~bit[81]&~bit[79]) | (~bit[80]&~bit[79]) | (~bit[83]&~bit[82]
		    &~bit[79]) | (bit[84]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[83]
		    &bit[82]&bit[81]&bit[80]&bit[79]);

		out_t[4] = (~bit[81]&~bit[80]) | (~bit[83]&~bit[82]&~bit[80]) | (bit[84]
		    &bit[82]&bit[81]&bit[80]) | (bit[83]&bit[82]&bit[81]&bit[80]) | (
		    ~bit[83]&bit[82]&bit[81]&~bit[79]) | (bit[83]&~bit[82]&bit[81]
		    &~bit[79]);

		out_t[3] = (~bit[82]&~bit[81]) | (bit[83]&bit[82]&bit[81]) | (~bit[83]
		    &~bit[81]&~bit[79]) | (bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[84]
		    &bit[82]&bit[81]&bit[80]) | (bit[83]&bit[81]&~bit[80]&bit[79]) | (
		    bit[83]&bit[81]&bit[80]&~bit[79]) | (bit[82]&bit[81]&bit[80]&~bit[79]);

		out_t[2] = (bit[83]&bit[81]&~bit[80]) | (bit[83]&bit[82]&~bit[79]) | (
		    bit[83]&bit[81]&~bit[79]) | (bit[82]&~bit[81]&bit[80]&bit[79]) | (
		    bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[82]&bit[81]&bit[80]&~bit[79]) | (
		    ~bit[82]&bit[81]&~bit[80]&~bit[79]) | (~bit[83]&~bit[82]&~bit[81]
		    &bit[80]&~bit[79]) | (bit[83]&bit[82]&bit[80]) | (bit[84]&bit[82]
		    &bit[80]&bit[79]);

		out_t[1] = (~bit[82]&bit[80]) | (~bit[83]&~bit[82]&~bit[81]) | (~bit[83]
		    &~bit[81]&bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[79]) | (bit[84]
		    &~bit[83]&bit[80]&bit[79]) | (bit[83]&bit[82]&~bit[80]&~bit[79]) | (
		    ~bit[84]&bit[83]&bit[81]&bit[80]&bit[79]) | (bit[83]&~bit[81]&bit[80]
		    &~bit[79]);

		out_t[0] = (~bit[83]&~bit[82]&bit[80]&bit[79]) | (~bit[83]&bit[82]
		    &bit[80]&~bit[79]) | (bit[83]&bit[82]&~bit[80]&~bit[79]) | (bit[83]
		    &~bit[81]&~bit[80]&~bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[80]
		    &~bit[79]) | (~bit[83]&~bit[82]&bit[81]&~bit[80]&~bit[79]) | (
		    ~bit[83]&bit[82]&bit[81]&bit[79]) | (bit[82]&~bit[81]&~bit[80]
		    &bit[79]);

		out[6]=(~disable_update&(out_t[6]))|(disable_update&out[6]);
		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[78];
		disable_update=disable_update|(~control);
		//----------------79--------------
		out_t[6] = (bit[84]&bit[83]&~bit[82]&bit[81]&bit[80]);

		out_t[5] = (~bit[82]&~bit[80]) | (~bit[81]&~bit[80]) | (bit[82]&bit[81]
		    &bit[80]) | (~bit[84]&~bit[83]&bit[82]&bit[81]);

		out_t[4] = (~bit[82]&~bit[81]) | (bit[83]&bit[82]&bit[81]) | (bit[84]
		    &bit[82]&bit[81]) | (~bit[83]&~bit[81]&~bit[80]) | (~bit[84]&~bit[83]
		    &bit[82]&bit[80]);

		out_t[3] = (bit[83]&bit[82]&~bit[80]) | (bit[84]&~bit[83]&bit[82]&bit[81]) | (
		    ~bit[84]&~bit[83]&~bit[82]&~bit[81]) | (~bit[84]&bit[82]&bit[81]
		    &bit[80]) | (~bit[84]&bit[83]&~bit[81]&bit[80]) | (bit[84]&~bit[83]
		    &~bit[81]&bit[80]) | (~bit[83]&~bit[82]&bit[81]&~bit[80]);

		out_t[2] = (bit[84]&bit[83]&bit[82]) | (bit[83]&bit[82]&~bit[80]) | (
		    ~bit[84]&~bit[83]&bit[81]&bit[80]) | (~bit[83]&~bit[82]&bit[81]
		    &bit[80]) | (bit[84]&bit[83]&~bit[81]&bit[80]) | (bit[84]&bit[82]
		    &~bit[81]&bit[80]) | (~bit[84]&bit[83]&bit[81]&~bit[80]) | (bit[84]
		    &bit[82]&bit[81]&~bit[80]) | (bit[84]&~bit[83]&~bit[82]&~bit[81]
		    &~bit[80]) | (~bit[85]&~bit[83]&bit[81]&bit[80]);

		out_t[1] = (~bit[84]&~bit[83]&~bit[82]) | (bit[84]&~bit[81]&bit[80]) | (
		    ~bit[84]&bit[83]&bit[82]&~bit[80]) | (bit[84]&bit[83]&~bit[82]
		    &~bit[80]) | (bit[84]&~bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[84]
		    &~bit[83]&bit[80]) | (~bit[85]&bit[84]&bit[83]&bit[80]) | (bit[85]
		    &~bit[83]&bit[82]&bit[80]) | (bit[83]&~bit[82]&~bit[81]&~bit[80]) | (
		    ~bit[85]&bit[83]&bit[82]&bit[81]&bit[80]);

		out_t[0] = (bit[84]&~bit[83]&bit[81]) | (bit[83]&bit[82]&~bit[81]) | (
		    ~bit[85]&~bit[83]&bit[82]&bit[81]) | (~bit[84]&bit[83]&~bit[82]
		    &bit[81]) | (~bit[84]&~bit[83]&~bit[82]&~bit[81]) | (~bit[84]&~bit[81]
		    &~bit[80]) | (bit[85]&bit[83]&bit[82]&bit[80]) | (~bit[84]&~bit[83]
		    &bit[82]&~bit[80]) | (~bit[83]&~bit[82]&~bit[81]&~bit[80]);

		out[6]=(~disable_update&(out_t[6]))|(disable_update&out[6]);
		out[5]=(~disable_update&(out_t[5]))|(disable_update&out[5]);
		out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);
		out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);
		out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);
		out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);
		out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);
		//control and disable_update update--
		control=control&bit[79];
		disable_update=disable_update|(~control);
		//----------------80--------------
		out[6]=( control & ( (~bit[80]&(~bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81])) | ( bit[80]&((~bit[81]&(ZERO)) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[80]&((~bit[83]&~bit[81]) | (~bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]))) | ( bit[80]&((~bit[81]&((~bit[86]&~bit[82]) | (~bit[85]&~bit[82]) | (~bit[84]&~bit[82]) | (~bit[83]&~bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[80]&((~bit[82]&bit[81]) | (~bit[85]&~bit[83]&~bit[82]) | (~bit[84]&~bit[83]&~bit[82]) | (bit[83]&bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[81]))) | ( bit[80]&((~bit[81]&((~bit[86]&~bit[83]) | (~bit[85]&~bit[83]) | (~bit[84]&~bit[83]) | (~bit[83]&bit[82]) | (bit[86]&bit[85]&bit[84]&bit[83]&~bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[80]&((bit[83]&~bit[81]) | (~bit[85]&~bit[84]&~bit[83]&bit[81]) | (~bit[84]&~bit[83]&bit[82]&bit[81]) | (bit[85]&bit[84]&~bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[82]))) | ( bit[80]&((~bit[81]&((~bit[86]&~bit[84]) | (~bit[85]&~bit[84]) | (~bit[84]&bit[83]) | (bit[86]&bit[85]&~bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[80]&((bit[84]&~bit[83]&bit[81]) | (~bit[85]&~bit[84]&~bit[81]) | (~bit[84]&bit[82]&~bit[81]) | (bit[85]&bit[84]&~bit[83]&~bit[82]) | (bit[85]&~bit[83]&~bit[82]&bit[81]) | (bit[86]&bit[85]&bit[84]&bit[82]&bit[81]))) | ( bit[80]&((~bit[81]&((~bit[86]&~bit[85]) | (~bit[85]&bit[84]) | (~bit[85]&bit[83]) | (bit[84]&bit[83]) | (~bit[86]&bit[83]&bit[82]) | (bit[86]&bit[84]&~bit[82]) | (bit[86]&bit[85]&~bit[84]&~bit[83]&bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[80]&((~bit[85]&~bit[84]&bit[82]) | (bit[85]&~bit[84]&~bit[82]) | (~bit[85]&bit[83]&~bit[81]) | (~bit[85]&bit[84]&~bit[83]&bit[81]) | (bit[85]&~bit[83]&~bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (~bit[84]&bit[83]&~bit[82]))) | ( bit[80]&((~bit[81]&((~bit[85]&bit[83]&bit[82]) | (~bit[86]&~bit[85]&~bit[84]&bit[82]) | (bit[86]&bit[85]&bit[84]&~bit[82]) | (~bit[86]&~bit[85]&bit[84]&~bit[82]) | (~bit[86]&bit[85]&~bit[84]&~bit[82]) | (~bit[86]&bit[85]&bit[84]&~bit[83]&bit[82]) | (bit[86]&bit[85]&~bit[84]&~bit[83]&bit[82]) | (bit[86]&~bit[85]&~bit[84]&~bit[83]&~bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[80]&((~bit[84]&bit[83]&bit[82]) | (bit[85]&~bit[83]&bit[81]) | (bit[85]&bit[83]&~bit[81]) | (~bit[85]&bit[83]&~bit[82]&bit[81]) | (~bit[85]&bit[84]&~bit[83]&bit[82]&~bit[81]) | (bit[85]&~bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[82]&bit[81]))) | ( bit[80]&((~bit[81]&((~bit[86]&bit[84]&bit[82]) | (bit[86]&bit[84]&~bit[82]) | (~bit[86]&~bit[85]&~bit[84]&bit[83]) | (bit[86]&~bit[85]&~bit[84]&~bit[83]) | (bit[86]&bit[85]&~bit[84]&bit[83]) | (bit[85]&~bit[84]&bit[83]&bit[82]) | (bit[85]&bit[84]&~bit[83]&~bit[82]))) /*| ( bit[81]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[80]&bit[81];
		//disable_update=disable_update|(~control);

		//---------------82-83--------------------
		out[6]=( control & ( (~bit[82]&((~bit[88]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&bit[86]&bit[85]&bit[84]&bit[83]))) | ( bit[82]&((~bit[83]&(bit[88]&~bit[87]&bit[86]&bit[85]&bit[84])) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[82]&((~bit[87]&~bit[83]) | (~bit[86]&~bit[83]) | (~bit[85]&~bit[83]) | (~bit[84]&~bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]))) | ( bit[82]&((~bit[83]&((~bit[86]&~bit[84]) | (~bit[85]&~bit[84]) | (bit[87]&bit[86]&bit[85]&bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[82]&((~bit[85]&~bit[84]) | (~bit[84]&~bit[83]) | (~bit[87]&~bit[86]&~bit[84]) | (bit[87]&bit[86]&bit[85]&~bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]))) | ( bit[82]&((~bit[83]&((bit[87]&bit[86]&bit[85]) | (~bit[88]&~bit[86]&~bit[85]) | (~bit[87]&~bit[86]&~bit[85]) | (~bit[86]&~bit[85]&bit[84]) | (bit[86]&bit[85]&~bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[82]&((~bit[86]&~bit[85]) | (~bit[87]&~bit[85]&~bit[83]) | (bit[86]&bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]) | (bit[87]&bit[85]&~bit[84]&bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[83]))) | ( bit[82]&((~bit[83]&((~bit[88]&bit[87]&bit[86]) | (bit[86]&~bit[85]&bit[84]) | (bit[86]&bit[85]&~bit[84]) | (bit[88]&bit[87]&~bit[86]&~bit[85]&~bit[84]) | (~bit[87]&bit[86]&~bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[82]&((~bit[87]&~bit[86]&~bit[83]) | (bit[88]&bit[87]&bit[86]&bit[84]) | (~bit[87]&~bit[86]&bit[85]&bit[84]) | (bit[87]&~bit[86]&bit[85]&~bit[84]) | (~bit[87]&bit[86]&~bit[84]&bit[83]) | (bit[87]&bit[86]&~bit[85]&~bit[83]) | (bit[87]&bit[86]&bit[84]&~bit[83]) | (bit[86]&~bit[85]&bit[84]&bit[83]) | (~bit[87]&~bit[85]&~bit[84]&bit[83]))) | ( bit[82]&((~bit[83]&((~bit[87]&~bit[85]&bit[84]) | (~bit[87]&bit[85]&~bit[84]) | (bit[88]&bit[87]&~bit[85]&~bit[84]) | (~bit[89]&bit[87]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]) | (~bit[87]&~bit[86]&bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[82]&((~bit[87]&~bit[85]&bit[84]) | (bit[87]&~bit[84]&bit[83]) | (bit[87]&bit[84]&~bit[83]) | (~bit[87]&bit[85]&~bit[84]&~bit[83]) | (~bit[88]&~bit[87]&bit[86]&bit[84]&bit[83]) | (bit[87]&~bit[85]&~bit[83]) | (~bit[86]&bit[85]&~bit[84]&bit[83]) | (~bit[87]&~bit[86]&~bit[84]&~bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[83]))) | ( bit[82]&((~bit[83]&((~bit[88]&~bit[87]&~bit[85]) | (~bit[88]&~bit[85]&bit[84]) | (~bit[89]&~bit[88]&bit[87]&bit[84]) | (bit[87]&~bit[86]&bit[85]&bit[84]) | (~bit[88]&~bit[87]&bit[86]&~bit[84]) | (bit[88]&~bit[87]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&bit[86]&~bit[85]&~bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[82]&((bit[87]&bit[86]&~bit[85]) | (bit[87]&bit[85]&~bit[84]) | (~bit[87]&~bit[85]&~bit[84]&bit[83]) | (bit[87]&~bit[85]&~bit[83]) | (bit[88]&bit[86]&bit[85]&bit[84]) | (~bit[87]&bit[86]&bit[85]&bit[83]) | (~bit[87]&~bit[86]&bit[84]&~bit[83]) | (bit[86]&bit[85]&bit[84]&~bit[83]))) | ( bit[82]&((~bit[83]&((~bit[88]&~bit[86]&bit[85]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]) | (~bit[88]&bit[86]&~bit[85]&bit[84]) | (~bit[88]&bit[87]&bit[85]&~bit[84]) | (~bit[89]&bit[88]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[89]&~bit[88]&bit[87]&bit[84]) | (~bit[88]&bit[87]&~bit[85]&bit[84]) | (bit[88]&bit[86]&~bit[85]&~bit[84]))) /*| ( bit[83]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[82]&bit[83];
		//disable_update=disable_update|(~control);


		//---------------84-85--------------------
		out[6]=( control & ( (~bit[84]&(~bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85])) | ( bit[84]&((~bit[85]&(ZERO)) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[84]&((~bit[86]&~bit[85]) | (~bit[89]&~bit[88]&~bit[87]&~bit[85]) | (bit[90]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]))) | ( bit[84]&((~bit[85]&((~bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[87]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[84]&((~bit[88]&~bit[87]&~bit[86]) | (bit[88]&bit[86]&~bit[85]) | (bit[87]&bit[86]&~bit[85]) | (bit[90]&bit[88]&bit[87]&bit[86]) | (bit[89]&bit[88]&bit[87]&bit[86]) | (~bit[89]&~bit[87]&~bit[86]&bit[85]) | (bit[89]&~bit[88]&~bit[87]&~bit[85]))) | ( bit[84]&((~bit[85]&((bit[89]&~bit[87]) | (bit[88]&~bit[87]) | (bit[90]&~bit[87]) | (~bit[89]&~bit[88]&bit[87]&bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[86]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[84]&((bit[88]&~bit[87]&~bit[85]) | (bit[89]&bit[88]&~bit[87]&~bit[86]) | (~bit[89]&bit[87]&~bit[86]&bit[85]) | (~bit[88]&bit[87]&~bit[86]&bit[85]) | (~bit[88]&bit[87]&bit[86]&~bit[85]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[89]&~bit[87]&bit[86]&~bit[85]) | (bit[90]&bit[88]&bit[87]&bit[86]&bit[85]))) | ( bit[84]&((~bit[85]&((~bit[89]&bit[88]&bit[86]) | (bit[89]&~bit[88]&bit[87]) | (~bit[90]&bit[89]&~bit[87]&bit[86]) | (bit[90]&~bit[88]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&~bit[86]) | (~bit[90]&~bit[88]&~bit[87]&~bit[86]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[84]&((~bit[89]&bit[88]&~bit[85]) | (bit[87]&~bit[86]&~bit[85]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (~bit[89]&~bit[88]&bit[87]&~bit[86]) | (bit[89]&bit[88]&~bit[86]&bit[85]) | (bit[88]&bit[87]&bit[86]&bit[85]) | (~bit[89]&~bit[87]&bit[86]&bit[85]))) | ( bit[84]&((~bit[85]&((~bit[90]&~bit[89]&bit[88]) | (bit[89]&~bit[88]&bit[86]) | (~bit[90]&bit[87]&bit[86]) | (bit[90]&bit[89]&~bit[88]&~bit[87]) | (bit[90]&bit[89]&~bit[87]&bit[86]) | (bit[90]&~bit[88]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&bit[87]&~bit[86]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[84]&((~bit[88]&~bit[86]&~bit[85]) | (~bit[87]&~bit[86]&~bit[85]) | (bit[89]&bit[88]&~bit[87]&bit[86]) | (~bit[89]&~bit[88]&bit[86]&bit[85]) | (bit[89]&bit[87]&~bit[86]&bit[85]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[89]&~bit[88]&~bit[87]&bit[85]) | (bit[90]&~bit[89]&bit[87]&bit[86]&bit[85]))) | ( bit[84]&((~bit[85]&((bit[90]&bit[88]&bit[86]) | (bit[90]&~bit[89]&bit[88]&~bit[87]) | (~bit[90]&bit[89]&~bit[88]&bit[86]) | (bit[90]&~bit[89]&~bit[87]&bit[86]) | (~bit[90]&bit[89]&bit[88]&~bit[86]) | (bit[90]&bit[89]&~bit[88]&~bit[86]) | (~bit[90]&bit[88]&bit[87]&~bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[87]&~bit[86]) | (bit[89]&bit[87]&~bit[86]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[84]&((bit[89]&~bit[88]&bit[85]) | (bit[89]&~bit[87]&bit[85]) | (bit[89]&~bit[86]&bit[85]) | (~bit[89]&bit[87]&~bit[85]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (bit[89]&bit[88]&~bit[87]&~bit[86]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[89]&~bit[88]&~bit[86]&~bit[85]) | (~bit[90]&bit[89]&bit[88]&bit[87]&bit[86]))) | ( bit[84]&((~bit[85]&((~bit[90]&bit[89]&bit[87]) | (bit[90]&bit[88]&~bit[87]) | (bit[89]&bit[88]&bit[86]) | (bit[90]&~bit[88]&bit[86]) | (~bit[89]&~bit[87]&~bit[86]) | (bit[90]&~bit[89]&bit[88]&~bit[86]))) /*| ( bit[85]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[84]&bit[85];
		//disable_update=disable_update|(~control);

		//---------------86-87--------------------
		out[6]=( control & ( (~bit[86]&(~bit[91]&~bit[90]&~bit[89]&~bit[88]&~bit[87])) | ( bit[86]&((~bit[87]&(~bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88])) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[86]&((bit[91]&~bit[87]) | (bit[90]&~bit[87]) | (bit[89]&~bit[87]) | (bit[88]&~bit[87]))) | ( bit[86]&((~bit[87]&((~bit[92]&bit[90]&~bit[88]) | (~bit[91]&bit[90]&~bit[88]) | (~bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[89]&~bit[88]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[86]&((~bit[91]&bit[90]&~bit[88]) | (~bit[90]&bit[89]&~bit[88]) | (~bit[89]&~bit[88]&bit[87]) | (bit[91]&~bit[88]&~bit[87]))) | ( bit[86]&((~bit[87]&((~bit[89]&bit[88]) | (~bit[92]&bit[90]&~bit[89]) | (~bit[91]&bit[90]&~bit[89]) | (bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&~bit[91]&~bit[90]&bit[88]) | (bit[92]&bit[91]&bit[90]&bit[89]&~bit[88]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[86]&((bit[91]&bit[90]&~bit[88]&bit[87]) | (~bit[91]&bit[90]&~bit[89]) | (~bit[90]&~bit[89]&bit[87]) | (bit[91]&~bit[89]&~bit[87]) | (~bit[89]&bit[88]&~bit[87]))) | ( bit[86]&((~bit[87]&((bit[92]&bit[91]&bit[90]&~bit[88]) | (bit[92]&bit[91]&~bit[89]&~bit[88]) | (~bit[92]&~bit[91]&bit[90]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[91]&~bit[90]&bit[89]&bit[88]) | (~bit[92]&~bit[90]&~bit[89]&bit[88]) | (~bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&~bit[90]&bit[89]&~bit[88]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[86]&((~bit[90]&bit[89]&bit[88]) | (~bit[91]&~bit[90]&bit[87]) | (~bit[91]&bit[89]&bit[88]&bit[87]) | (bit[90]&~bit[89]&~bit[88]&~bit[87]) | (bit[91]&bit[90]&~bit[89]&bit[88]&bit[87]) | (bit[91]&bit[90]&bit[89]&~bit[88]&bit[87]) | (~bit[90]&bit[88]&~bit[87]) | (bit[91]&~bit[90]&~bit[89]&~bit[88]))) | ( bit[86]&((~bit[87]&((bit[92]&bit[91]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[89]) | (bit[92]&~bit[91]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&~bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[89]&~bit[88]) | (~bit[92]&bit[91]&bit[90]&bit[89]&bit[88]) | (~bit[91]&bit[90]&~bit[89]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[86]&((~bit[91]&~bit[90]) | (~bit[91]&~bit[89]) | (~bit[89]&bit[88]&bit[87]) | (~bit[90]&~bit[88]&~bit[87]) | (bit[91]&bit[90]&bit[89]&bit[87]) | (bit[91]&bit[89]&~bit[88]&bit[87]))) | ( bit[86]&((~bit[87]&((bit[92]&bit[90]&bit[89]) | (bit[92]&bit[89]&~bit[88]) | (~bit[92]&bit[90]&~bit[89]&bit[88]) | (bit[92]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&~bit[89]&~bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&bit[88]) | (bit[91]&bit[90]&~bit[89]&~bit[88]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[86]&((bit[91]&bit[88]&bit[87]) | (~bit[91]&bit[89]&~bit[87]) | (bit[91]&~bit[89]&~bit[87]) | (~bit[91]&~bit[90]&~bit[88]&bit[87]) | (bit[90]&bit[89]&~bit[88]&bit[87]) | (bit[90]&~bit[89]&bit[88]&~bit[87]))) | ( bit[86]&((~bit[87]&((bit[92]&bit[90]&~bit[89]) | (bit[91]&bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&bit[89]) | (~bit[92]&~bit[91]&~bit[90]&~bit[89]) | (~bit[91]&bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&bit[90]) | (bit[92]&bit[91]&bit[88]) | (bit[91]&bit[89]&bit[88]) | (bit[92]&~bit[89]&~bit[88]))) /*| ( bit[87]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[86]&bit[87];
		//disable_update=disable_update|(~control);

		//---------------88-89--------------------
		out[6]=( control & ( (~bit[88]&((~bit[93]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&~bit[89]))) | ( bit[88]&((~bit[89]&((~bit[94]&~bit[93]&~bit[92]&~bit[91]&~bit[90]) | (~bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]&bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[88]&((bit[90]&~bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[89]) | (~bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]))) | ( bit[88]&((~bit[89]&((~bit[92]&bit[91]&~bit[90]) | (bit[92]&~bit[91]&~bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[94]&~bit[91]&~bit[90]) | (bit[93]&~bit[91]&~bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[88]&((bit[92]&~bit[90]&bit[89]) | (bit[91]&~bit[90]&bit[89]) | (~bit[93]&~bit[92]&~bit[91]&bit[90]) | (bit[93]&bit[92]&bit[91]&~bit[90]) | (bit[93]&~bit[92]&~bit[91]&bit[89]))) | ( bit[88]&((~bit[89]&((bit[92]&~bit[90]) | (~bit[92]&~bit[91]&bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]) | (bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[94]&~bit[93]&bit[91]&~bit[90]) | (bit[94]&~bit[91]&~bit[90]) | (bit[93]&~bit[91]&~bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[88]&((bit[92]&~bit[91]&bit[90]) | (~bit[93]&bit[92]&~bit[91]&bit[89]) | (~bit[92]&bit[91]&bit[90]&~bit[89]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (bit[93]&~bit[91]&bit[90]&~bit[89]))) | ( bit[88]&((~bit[89]&((bit[92]&~bit[91]&bit[90]) | (bit[92]&bit[91]&~bit[90]) | (~bit[94]&~bit[93]&~bit[92]&bit[90]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]) | (bit[94]&~bit[93]&~bit[92]&~bit[91]) | (bit[95]&bit[93]&bit[92]&bit[90]) | (bit[94]&bit[93]&bit[92]&bit[90]) | (~bit[94]&~bit[93]&bit[92]&~bit[90]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[88]&((~bit[93]&~bit[92]&bit[91]&~bit[90]) | (~bit[93]&~bit[92]&bit[91]&bit[89]) | (bit[93]&~bit[91]&~bit[90]&bit[89]) | (~bit[93]&bit[92]&bit[90]&~bit[89]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&bit[89]))) | ( bit[88]&((~bit[89]&((~bit[94]&bit[93]&~bit[90]) | (bit[92]&bit[91]&~bit[90]) | (bit[94]&~bit[91]&~bit[90]) | (~bit[95]&bit[93]&bit[92]&bit[91]) | (~bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[94]&bit[93]&~bit[92]&~bit[91]) | (~bit[93]&bit[92]&~bit[91]&bit[90]) | (bit[94]&~bit[93]&~bit[92]&bit[91]&bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[88]&((bit[92]&~bit[91]&bit[89]) | (bit[92]&~bit[90]&bit[89]) | (~bit[92]&bit[91]&bit[90]&bit[89]) | (bit[93]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[90]&~bit[89]) | (bit[93]&~bit[91]&bit[90]&~bit[89]))) | ( bit[88]&((~bit[89]&((~bit[94]&bit[92]&~bit[91]) | (~bit[93]&bit[92]&~bit[90]) | (~bit[93]&~bit[91]&~bit[90]) | (bit[94]&~bit[93]&~bit[92]&bit[91]) | (bit[94]&bit[93]&bit[92]&bit[90]) | (bit[95]&bit[93]&bit[91]&bit[90]) | (bit[93]&~bit[92]&bit[91]&bit[90]) | (~bit[94]&bit[93]&~bit[92]&~bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[88]&((~bit[93]&~bit[92]&bit[91]) | (~bit[93]&bit[91]&bit[89]) | (~bit[93]&bit[90]&bit[89]) | (~bit[92]&~bit[90]&~bit[89]) | (bit[93]&~bit[92]&~bit[91]&~bit[89]) | (bit[93]&bit[91]&~bit[90]&~bit[89]) | (bit[93]&bit[92]&~bit[91]&~bit[90]&bit[89]))) | ( bit[88]&((~bit[89]&((~bit[94]&bit[93]&bit[91]) | (~bit[93]&bit[92]&~bit[91]) | (~bit[94]&bit[92]&bit[90]) | (~bit[94]&~bit[93]&~bit[91]&bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[94]&bit[93]&~bit[92]&~bit[91]&bit[90]) | (~bit[94]&bit[92]&bit[91]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]) | (bit[94]&~bit[92]&bit[91]&~bit[90]))) /*| ( bit[89]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[88]&bit[89];
		//disable_update=disable_update|(~control);
		//---------------90-91--------------------
		out[6]=( control & ( (~bit[90]&(bit[95]&~bit[94]&bit[93]&bit[92]&bit[91])) | ( bit[90]&((~bit[91]&((bit[95]&~bit[94]&bit[93]&bit[92]) | (~bit[97]&~bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[90]&((~bit[93]&~bit[91]) | (~bit[92]&~bit[91]) | (bit[94]&bit[93]&bit[92]&bit[91]))) | ( bit[90]&((~bit[91]&((~bit[94]&~bit[92]) | (~bit[93]&~bit[92]) | (bit[97]&bit[94]&bit[93]&bit[92]) | (bit[96]&bit[94]&bit[93]&bit[92]) | (bit[95]&bit[94]&bit[93]&bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[90]&((~bit[93]&~bit[92]) | (bit[94]&bit[93]&bit[92]) | (~bit[95]&~bit[94]&~bit[92]) | (bit[93]&bit[92]&~bit[91]))) | ( bit[90]&((~bit[91]&((bit[97]&bit[94]&bit[93]) | (bit[96]&bit[94]&bit[93]) | (bit[95]&bit[94]&bit[93]) | (~bit[96]&~bit[94]&~bit[93]) | (~bit[95]&~bit[94]&~bit[93]) | (~bit[94]&~bit[93]&~bit[92]) | (~bit[95]&bit[94]&~bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[90]&((bit[94]&bit[93]) | (~bit[94]&~bit[93]&~bit[92]) | (~bit[94]&~bit[93]&bit[91]) | (bit[93]&bit[92]&~bit[91]) | (bit[95]&bit[93]&~bit[92]))) | ( bit[90]&((~bit[91]&((bit[95]&bit[94]&~bit[92]) | (~bit[95]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&bit[93]) | (~bit[95]&bit[94]&~bit[93]&bit[92]) | (bit[96]&bit[95]&~bit[94]&~bit[93]&bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[92]) | (bit[96]&~bit[95]&bit[94]&bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[90]&((bit[94]&~bit[93]&~bit[92]) | (bit[94]&~bit[93]&bit[91]) | (~bit[94]&bit[92]&~bit[91]) | (~bit[96]&bit[95]&~bit[94]&bit[93]) | (bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[95]&bit[94]&bit[93]) | (~bit[95]&bit[93]&~bit[91]))) | ( bit[90]&((~bit[91]&((bit[96]&bit[95]&~bit[93]) | (~bit[94]&~bit[93]&~bit[92]) | (bit[96]&bit[94]&bit[93]&bit[92]) | (~bit[96]&~bit[95]&bit[94]&bit[93]&~bit[92]) | (bit[95]&bit[94]&~bit[93]) | (~bit[96]&bit[95]&~bit[94]&bit[93]) | (bit[97]&~bit[96]&~bit[95]&bit[92]) | (~bit[96]&~bit[95]&~bit[94]&bit[92]) | (~bit[97]&bit[95]&bit[93]&bit[92]) | (~bit[96]&~bit[95]&~bit[93]&bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[90]&((~bit[95]&bit[94]&~bit[93]) | (bit[95]&bit[93]&bit[92]) | (~bit[94]&bit[93]&bit[92]) | (~bit[95]&bit[92]&bit[91]) | (bit[95]&~bit[94]&bit[93]&~bit[91]) | (bit[95]&bit[94]&~bit[92]&bit[91]))) | ( bit[90]&((~bit[91]&((~bit[96]&~bit[95]&~bit[92]) | (~bit[96]&bit[94]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&~bit[93]) | (~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[95]&~bit[94]&~bit[93]&~bit[92]) | (bit[97]&~bit[96]&~bit[95]&bit[94]&bit[93]) | (~bit[97]&bit[96]&bit[94]&bit[93]&bit[92]) | (bit[96]&~bit[95]&~bit[94]&bit[93]&bit[92]) | (bit[96]&bit[95]&~bit[94]&~bit[93]&bit[92]) | (bit[96]&bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[97]&~bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[90]&((bit[96]&~bit[94]&bit[93]&bit[91]) | (~bit[95]&~bit[93]&~bit[92]&bit[91]) | (bit[95]&bit[94]&~bit[93]&~bit[91]) | (bit[94]&bit[93]&~bit[92]&~bit[91]) | (~bit[95]&~bit[94]&bit[93]&bit[92]) | (~bit[95]&~bit[94]&bit[93]&bit[91]) | (bit[95]&bit[93]&~bit[92]&bit[91]) | (~bit[95]&~bit[94]&~bit[93]&~bit[91]) | (~bit[96]&bit[94]&bit[93]&bit[92]&bit[91]))) | ( bit[90]&((~bit[91]&((bit[95]&bit[94]&~bit[93]) | (bit[96]&~bit[93]&bit[92]) | (bit[97]&bit[95]&bit[93]&bit[92]) | (bit[96]&bit[95]&bit[93]&~bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[93]) | (~bit[96]&~bit[95]&bit[93]&~bit[92]) | (~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[96]&~bit[94]&~bit[93]&~bit[92]))) /*| ( bit[91]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[90]&bit[91];

		//---------------92-93--------------------
		out[6]=( control & ( (~bit[92]&(bit[97]&bit[96]&~bit[95]&bit[94]&bit[93])) | ( bit[92]&((~bit[93]&((bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[92]&((~bit[95]&~bit[93]) | (~bit[94]&~bit[93]) | (bit[95]&bit[94]&bit[93]) | (~bit[97]&~bit[96]&bit[95]&bit[94]))) | ( bit[92]&((~bit[93]&((~bit[95]&~bit[94]) | (bit[99]&bit[96]&bit[95]&bit[94]) | (bit[98]&bit[96]&bit[95]&bit[94]) | (bit[97]&bit[96]&bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[92]&((bit[97]&bit[95]&bit[94]) | (bit[96]&bit[95]&bit[94]) | (~bit[96]&~bit[95]&~bit[94]) | (bit[95]&bit[94]&bit[93]) | (~bit[95]&~bit[94]&bit[93]))) | ( bit[92]&((~bit[93]&((~bit[96]&bit[95]&~bit[94]) | (~bit[98]&bit[95]&~bit[94]) | (bit[98]&~bit[97]&bit[96]&bit[95]) | (bit[99]&bit[96]&bit[95]&bit[94]) | (bit[97]&bit[96]&bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[92]&((~bit[97]&bit[95]&~bit[94]) | (~bit[96]&bit[95]&bit[93]) | (bit[96]&bit[95]&~bit[93]) | (bit[96]&~bit[94]&~bit[93]) | (bit[97]&bit[95]&~bit[93]))) | ( bit[92]&((~bit[93]&((~bit[96]&~bit[95]) | (~bit[96]&~bit[94]) | (~bit[98]&~bit[97]&~bit[95]&bit[94]) | (bit[98]&bit[97]&bit[95]&~bit[94]) | (bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[99]&~bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[97]&bit[96]&bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[92]&((~bit[97]&~bit[96]&~bit[94]) | (~bit[96]&~bit[95]&bit[93]) | (bit[97]&~bit[95]&bit[94]&bit[93]) | (bit[97]&~bit[96]&bit[94]&~bit[93]) | (bit[96]&~bit[95]&~bit[94]&~bit[93]) | (bit[97]&bit[96]&bit[95]&~bit[94]&bit[93]) | (~bit[97]&~bit[95]&~bit[93]) | (~bit[98]&~bit[96]&bit[94]&bit[93]) | (~bit[97]&bit[95]&bit[94]&bit[93]) | (~bit[97]&bit[96]&bit[94]&~bit[93]))) | ( bit[92]&((~bit[93]&((~bit[98]&~bit[97]&~bit[94]) | (~bit[97]&~bit[96]&~bit[94]) | (~bit[98]&~bit[95]&~bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]) | (~bit[99]&bit[98]&bit[96]&bit[94]) | (~bit[98]&~bit[96]&bit[95]&bit[94]) | (bit[98]&~bit[97]&~bit[95]) | (~bit[97]&~bit[96]&~bit[95]) | (bit[99]&~bit[98]&bit[95]&bit[94]) | (bit[97]&bit[96]&~bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[92]&((bit[97]&bit[95]&~bit[94]&bit[93]) | (bit[97]&~bit[96]&~bit[94]&~bit[93]) | (~bit[97]&~bit[96]&~bit[95]&bit[94]&~bit[93]) | (~bit[98]&bit[97]&bit[96]&~bit[95]&bit[94]&bit[93]) | (~bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[97]&~bit[96]&bit[95]&bit[93]) | (~bit[97]&~bit[95]&~bit[94]&bit[93]) | (bit[97]&bit[95]&bit[94]&~bit[93]) | (~bit[97]&bit[96]&~bit[94]&~bit[93]))) | ( bit[92]&((~bit[93]&((~bit[98]&~bit[97]&~bit[94]) | (~bit[97]&bit[95]&~bit[94]) | (bit[98]&~bit[97]&bit[96]&bit[94]) | (~bit[98]&bit[97]&~bit[95]&bit[94]) | (bit[98]&bit[96]&bit[95]&~bit[94]) | (~bit[98]&~bit[96]&bit[95]&~bit[94]) | (bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&~bit[96]&~bit[95]&~bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[97]&~bit[96]&bit[95]&bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[92]&((bit[97]&~bit[94]&bit[93]) | (~bit[96]&~bit[94]&bit[93]) | (bit[97]&bit[94]&~bit[93]) | (~bit[97]&~bit[95]&bit[94]&bit[93]) | (~bit[98]&bit[96]&bit[94]&bit[93]) | (~bit[98]&bit[95]&bit[94]&bit[93]) | (bit[97]&~bit[96]&bit[95]&~bit[93]) | (bit[97]&bit[96]&~bit[95]&~bit[93]) | (~bit[96]&~bit[95]&bit[94]&~bit[93]))) | ( bit[92]&((~bit[93]&((~bit[98]&~bit[97]&bit[95]&~bit[94]) | (bit[99]&~bit[98]&bit[97]&bit[95]&bit[94]) | (bit[98]&~bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[98]&~bit[97]&~bit[96]&~bit[95]&bit[94]) | (~bit[99]&bit[98]&bit[97]&bit[94]) | (~bit[98]&bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&~bit[95]&bit[94]) | (bit[98]&bit[96]&~bit[95]&bit[94]) | (~bit[97]&bit[96]&~bit[95]&~bit[94]) | (bit[98]&~bit[96]&~bit[95]&~bit[94]))) /*| ( bit[93]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[92]&bit[93];
		//disable_update=disable_update|(~control);

		//---------------94-95--------------------
		out[6]=( control & ( (~bit[94]&((~bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[100]&~bit[99]&~bit[97]&bit[96]&bit[95]))) | ( bit[94]&((~bit[95]&((~bit[99]&bit[98]&bit[97]&bit[96]) | (bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[94]&((~bit[97]&~bit[95]) | (~bit[96]&~bit[95]) | (bit[97]&bit[96]&bit[95]) | (bit[100]&bit[98]&bit[96]&bit[95]) | (bit[99]&bit[98]&bit[96]&bit[95]))) | ( bit[94]&((~bit[95]&((~bit[100]&~bit[97]&~bit[96]) | (~bit[99]&~bit[97]&~bit[96]) | (~bit[98]&~bit[97]&~bit[96]) | (bit[99]&bit[98]&bit[97]&bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[94]&((~bit[98]&bit[97]&bit[96]) | (~bit[98]&~bit[97]&~bit[96]) | (bit[97]&bit[96]&~bit[95]) | (~bit[100]&~bit[99]&bit[97]&bit[96]) | (~bit[99]&~bit[97]&~bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (bit[100]&bit[98]&~bit[97]&bit[96]&bit[95]))) | ( bit[94]&((~bit[95]&((bit[97]&~bit[96]) | (bit[99]&bit[98]&bit[97]) | (bit[100]&bit[99]&bit[98]&~bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[94]&((~bit[99]&bit[97]&~bit[95]) | (~bit[98]&bit[97]&~bit[95]) | (~bit[100]&~bit[99]&~bit[98]&bit[97]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]) | (bit[100]&bit[98]&bit[96]&bit[95]) | (bit[99]&bit[98]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&~bit[96]) | (bit[98]&~bit[97]&~bit[96]&~bit[95]))) | ( bit[94]&((~bit[95]&((~bit[98]&~bit[97]&bit[96]) | (~bit[99]&bit[97]&~bit[96]) | (~bit[98]&bit[97]&~bit[96]) | (bit[99]&bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[99]&~bit[97]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]) | (~bit[100]&~bit[99]&~bit[98]&~bit[96]) | (~bit[101]&bit[100]&bit[99]&bit[97]&bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[94]&((~bit[98]&bit[96]&~bit[95]) | (bit[99]&~bit[98]&~bit[96]&bit[95]) | (bit[99]&bit[98]&bit[97]&~bit[95]) | (~bit[99]&bit[98]&bit[97]&~bit[96]&bit[95]) | (~bit[99]&bit[98]&~bit[97]&~bit[96]&~bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[95]) | (~bit[99]&~bit[98]&~bit[97]&bit[95]) | (bit[100]&~bit[99]&bit[96]&bit[95]) | (~bit[100]&bit[99]&bit[98]&bit[96]&bit[95]))) | ( bit[94]&((~bit[95]&((~bit[101]&bit[99]&bit[98]&bit[96]) | (bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]) | (~bit[100]&bit[99]&bit[98]&bit[96]) | (~bit[100]&~bit[99]&~bit[98]&bit[96]) | (bit[100]&~bit[99]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[96]) | (~bit[100]&bit[99]&bit[97]&~bit[96]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]) | (bit[99]&~bit[98]&~bit[97]&~bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[94]&((~bit[99]&~bit[98]&~bit[95]) | (bit[99]&~bit[98]&bit[97]&bit[95]) | (bit[99]&~bit[98]&bit[96]&bit[95]) | (~bit[99]&~bit[97]&~bit[96]&bit[95]) | (bit[99]&bit[98]&bit[97]&~bit[95]) | (~bit[99]&~bit[97]&bit[96]&~bit[95]) | (~bit[100]&bit[99]&~bit[97]&bit[96]&bit[95]) | (bit[99]&~bit[97]&~bit[96]&~bit[95]) | (bit[100]&~bit[99]&bit[98]&bit[96]&bit[95]) | (bit[100]&~bit[99]&bit[97]&bit[96]&bit[95]) | (~bit[100]&~bit[98]&~bit[97]&bit[96]&bit[95]))) | ( bit[94]&((~bit[95]&((~bit[100]&bit[99]&~bit[98]&~bit[97]) | (~bit[100]&~bit[98]&~bit[97]&bit[96]) | (bit[99]&~bit[98]&~bit[97]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[96]) | (~bit[100]&~bit[99]&bit[98]&~bit[96]) | (bit[100]&bit[99]&bit[97]&~bit[96]) | (~bit[100]&~bit[99]&bit[97]&~bit[96]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (bit[100]&~bit[99]&bit[98]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[97]&~bit[96]) | (~bit[100]&bit[99]&bit[98]&bit[96]) | (~bit[100]&bit[98]&bit[97]&bit[96]) | (bit[101]&bit[99]&bit[98]&bit[97]&bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[94]&((bit[99]&bit[97]&~bit[95]) | (bit[99]&~bit[98]&~bit[96]&bit[95]) | (bit[99]&~bit[98]&bit[96]&~bit[95]) | (~bit[98]&bit[97]&bit[96]&~bit[95]) | (bit[99]&bit[98]&~bit[96]&~bit[95]) | (bit[100]&bit[99]&bit[98]&bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[99]&bit[98]&bit[97]&~bit[96]&bit[95]) | (~bit[99]&~bit[98]&~bit[97]&~bit[96]&~bit[95]) | (bit[100]&~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[100]&bit[99]&~bit[98]&bit[95]))) | ( bit[94]&((~bit[95]&( (bit[100]&bit[99]&bit[98]) | (bit[100]&~bit[99]&~bit[98]) | (~bit[100]&bit[99]&~bit[98]&bit[96]) | (~bit[101]&bit[98]&bit[97]&bit[96]) | (bit[99]&bit[98]&bit[97]&~bit[96]) | (bit[100]&bit[98]&~bit[97]) | (bit[100]&~bit[98]&bit[97]&~bit[96]))) /*| ( bit[95]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[94]&bit[95];
		//disable_update=disable_update|(~control);

		//---------------96-97--------------------
		out[6]=( control & ( (~bit[96]&(bit[100]&bit[99]&bit[98]&bit[97])) | ( bit[96]&((~bit[97]&(bit[101]&bit[100]&bit[99]&bit[98])) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[96]&((~bit[99]&~bit[97]) | (~bit[98]&~bit[97]))) | ( bit[96]&((~bit[97]&((~bit[102]&~bit[98]) | (~bit[101]&~bit[98]) | (~bit[100]&~bit[98]) | (~bit[99]&~bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[96]&((~bit[101]&~bit[99]&~bit[98]) | (~bit[100]&~bit[99]&~bit[98]) | (bit[99]&bit[98]&~bit[97]) | (~bit[99]&~bit[98]&~bit[97]))) | ( bit[96]&((~bit[97]&((~bit[102]&~bit[99]) | (~bit[101]&~bit[99]) | (~bit[100]&~bit[99]) | (bit[102]&bit[101]&bit[100]&bit[99]&~bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[96]&((bit[99]&~bit[98]) | (bit[99]&~bit[97]) | (~bit[100]&~bit[98]&~bit[97]) | (bit[101]&bit[100]&~bit[98]&bit[97]) | (~bit[101]&~bit[100]&~bit[99]&bit[98]&bit[97]))) | ( bit[96]&((~bit[97]&((~bit[101]&~bit[100]) | (~bit[102]&~bit[100]&bit[98]) | (bit[102]&bit[101]&bit[100]&~bit[99]) | (~bit[103]&~bit[102]&bit[101]&bit[99]&bit[98]) | (~bit[100]&bit[99]&~bit[98]) | (bit[102]&bit[101]&bit[99]&~bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[96]&((bit[101]&~bit[99]&bit[97]) | (~bit[100]&~bit[98]&bit[97]) | (~bit[100]&bit[98]&~bit[97]) | (bit[100]&~bit[99]&bit[98]&bit[97]) | (~bit[101]&bit[100]&~bit[98]&~bit[97]) | (~bit[102]&~bit[101]&bit[100]&bit[98]&bit[97]) | (~bit[100]&bit[99]&~bit[97]))) | ( bit[96]&((~bit[97]&((~bit[102]&~bit[101]&~bit[100]) | (~bit[102]&~bit[101]&bit[99]) | (bit[100]&bit[99]&~bit[98]) | (~bit[101]&~bit[100]&bit[99]) | (bit[102]&bit[101]&~bit[99]) | (bit[102]&bit[101]&bit[98]) | (bit[103]&bit[101]&bit[100]&bit[99]) | (bit[101]&~bit[100]&~bit[99]&~bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[96]&((~bit[101]&~bit[100]&bit[99]) | (bit[102]&~bit[101]&bit[99]&bit[97]) | (bit[101]&bit[100]&~bit[98]&bit[97]) | (~bit[101]&~bit[99]&bit[98]&~bit[97]) | (~bit[100]&bit[99]&~bit[98]&~bit[97]) | (~bit[102]&bit[101]&bit[100]&bit[99]&bit[97]) | (bit[101]&~bit[100]&~bit[99]&bit[98]&bit[97]) | (bit[100]&bit[99]&~bit[98]&bit[97]))) | ( bit[96]&((~bit[97]&((bit[102]&~bit[101]&bit[100]&bit[98]) | (bit[102]&bit[101]&~bit[100]&bit[98]) | (bit[102]&bit[101]&bit[100]&~bit[98]) | (~bit[102]&~bit[101]&bit[100]&~bit[98]) | (~bit[102]&bit[101]&~bit[100]&~bit[98]) | (~bit[102]&~bit[101]&~bit[100]&bit[99]&bit[98]) | (bit[102]&~bit[101]&~bit[100]&~bit[99]&~bit[98]) | (bit[103]&~bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (~bit[103]&bit[102]&bit[100]&bit[98]) | (bit[102]&bit[100]&~bit[99]&bit[98]) | (~bit[101]&bit[100]&~bit[99]&bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[96]&((~bit[101]&~bit[100]&~bit[99]) | (~bit[101]&bit[100]&~bit[98]) | (bit[100]&~bit[99]&~bit[98]) | (~bit[101]&~bit[99]&bit[97]) | (~bit[101]&~bit[98]&~bit[97]) | (bit[100]&~bit[98]&~bit[97]) | (~bit[101]&bit[100]&bit[99]&~bit[97]) | (bit[101]&~bit[100]&bit[99]&bit[98]&bit[97]) | (bit[102]&~bit[101]&bit[100]&bit[97]))) | ( bit[96]&((~bit[97]&((bit[102]&bit[100]&~bit[98]) | (bit[102]&~bit[101]&bit[100]&bit[99]) | (bit[103]&bit[101]&bit[100]&bit[98]) | (bit[102]&bit[101]&~bit[100]&bit[98]) | (bit[102]&~bit[100]&~bit[99]&bit[98]) | (~bit[102]&bit[101]&~bit[100]&~bit[98]) | (~bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[101]&bit[100]&~bit[99]&bit[98]) | (~bit[102]&~bit[101]&bit[99]&~bit[98]))) /*| ( bit[97]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[96]&bit[97];
		//---------------98-99--------------------
		out[6]=( control & ( (~bit[98]&((bit[102]&bit[101]&bit[100]&bit[99]) | (~bit[103]&~bit[101]&~bit[100]&~bit[99]) | (~bit[102]&~bit[101]&~bit[100]&~bit[99]))) | ( bit[98]&((~bit[99]&((~bit[103]&~bit[102]&~bit[101]&~bit[100]) | (~bit[105]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[98]&((bit[101]&~bit[99]) | (bit[100]&~bit[99]) | (bit[103]&bit[102]&~bit[99]) | (~bit[103]&~bit[102]&~bit[101]&~bit[100]&bit[99]))) | ( bit[98]&((~bit[99]&((bit[103]&~bit[100]) | (bit[102]&~bit[100]) | (bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]) | (~bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[98]&((bit[101]&~bit[100]) | (bit[103]&~bit[100]&bit[99]) | (bit[102]&~bit[100]&bit[99]) | (~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[102]&~bit[101]&~bit[99]) | (~bit[103]&~bit[102]&bit[100]&~bit[99]))) | ( bit[98]&((~bit[99]&((~bit[103]&~bit[102]&bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[101]) | (~bit[103]&bit[102]&~bit[101]) | (bit[103]&~bit[101]&~bit[100]) | (bit[104]&~bit[102]&~bit[101]&bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[98]&((bit[101]&~bit[100]&~bit[99]) | (~bit[103]&~bit[102]&~bit[101]&bit[100]) | (~bit[103]&bit[102]&~bit[101]&bit[99]) | (bit[103]&~bit[102]&~bit[101]&bit[99]) | (~bit[103]&bit[102]&bit[101]&~bit[99]) | (bit[103]&~bit[102]&bit[101]&~bit[99]) | (bit[103]&bit[102]&~bit[100]&~bit[99]) | (~bit[104]&~bit[103]&bit[102]&bit[100]&bit[99]))) | ( bit[98]&((~bit[99]&((~bit[103]&bit[102]&~bit[100]) | (bit[103]&~bit[102]&~bit[100]) | (~bit[105]&~bit[104]&bit[103]&bit[101]) | (bit[104]&bit[103]&~bit[101]&bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[100]) | (~bit[104]&bit[103]&~bit[102]) | (bit[104]&~bit[103]&~bit[102]&bit[100]) | (~bit[103]&~bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[102]&bit[101]&~bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[98]&((~bit[103]&~bit[102]&~bit[100]) | (~bit[102]&~bit[101]&bit[99]) | (~bit[101]&~bit[100]&bit[99]) | (bit[103]&bit[102]&bit[101]&bit[100]) | (bit[103]&~bit[102]&~bit[101]&bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[104]&bit[102]&bit[101]&bit[100]&bit[99]) | (~bit[103]&bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[102]&bit[100]&bit[99]))) | ( bit[98]&((~bit[99]&((bit[103]&~bit[102]&~bit[100]) | (bit[104]&bit[103]&~bit[102]&bit[101]) | (bit[104]&~bit[103]&~bit[101]&bit[100]) | (bit[104]&bit[102]&~bit[101]&bit[100]) | (~bit[103]&bit[102]&~bit[101]&bit[100]) | (bit[104]&bit[103]&bit[101]&~bit[100]) | (~bit[104]&bit[103]&~bit[101]&~bit[100]) | (bit[105]&bit[103]&bit[102]&bit[101]&bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[98]&((bit[103]&~bit[102]&~bit[100]) | (bit[103]&~bit[101]&~bit[100]) | (bit[103]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[103]&~bit[102]&bit[100]&bit[99]) | (~bit[103]&~bit[101]&bit[100]&bit[99]) | (~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[104]&~bit[103]&bit[102]&bit[101]&bit[99]))) | ( bit[98]&((~bit[99]&((~bit[104]&~bit[103]&bit[102]) | (bit[102]&~bit[101]&~bit[100]) | (bit[104]&bit[103]&bit[102]&bit[101]) | (~bit[104]&bit[103]&~bit[102]&~bit[101]) | (bit[104]&~bit[103]&~bit[102]&bit[100]) | (~bit[104]&~bit[103]&bit[101]) | (~bit[104]&~bit[103]&~bit[100]) | (bit[105]&bit[103]&bit[102]&bit[101]&bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[98]&((bit[103]&~bit[102]&bit[101]) | (bit[103]&bit[101]&~bit[100]) | (~bit[103]&bit[102]&bit[101]&bit[100]) | (bit[103]&bit[102]&~bit[101]&bit[100]) | (~bit[102]&~bit[101]&~bit[100]&bit[99]) | (bit[102]&~bit[101]&~bit[100]&~bit[99]) | (bit[103]&~bit[101]&bit[100]&~bit[99]) | (bit[104]&bit[102]&bit[101]&bit[100]&bit[99]))) | ( bit[98]&((~bit[99]&((bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[101]&bit[100]) | (~bit[104]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (~bit[104]&~bit[103]&~bit[102]&bit[101]&bit[100]) | (bit[104]&~bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[104]&bit[103]&~bit[102]&bit[101]&~bit[100]))) /*| ( bit[99]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[98]&bit[99];
		//disable_update=disable_update|(~control);

	//----------------100--------------

		out[6]=( control & ( (~bit[100]&(bit[104]&~bit[103]&bit[102]&bit[101])) | ( bit[100]&((~bit[101]&((~bit[105]&bit[104]&bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[6]);
		out[5]=( control & ( (~bit[100]&((~bit[103]&~bit[101]) | (~bit[102]&~bit[101]) | (bit[103]&bit[102]&bit[101]))) | ( bit[100]&((~bit[101]&((~bit[103]&~bit[102]) | (~bit[106]&~bit[104]&~bit[102]) | (~bit[105]&~bit[104]&~bit[102]) | (bit[105]&bit[104]&bit[103]&bit[102]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[5]);
		out[4]=( control & ( (~bit[100]&((bit[103]&bit[102]) | (~bit[103]&~bit[102]) | (~bit[105]&~bit[102]&~bit[101]) | (~bit[104]&~bit[102]&~bit[101]))) | ( bit[100]&((~bit[101]&((bit[105]&bit[104]&bit[103]) | (bit[104]&bit[103]&~bit[102]) | (~bit[106]&~bit[104]&~bit[103]&bit[102]) | (~bit[105]&~bit[104]&~bit[103]&bit[102]) | (bit[106]&bit[105]&bit[103]&~bit[102]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[4]);
		out[3]=( control & ( (~bit[100]&((~bit[104]&bit[103]&bit[101]) | (bit[103]&bit[102]&~bit[101]) | (~bit[103]&~bit[102]&~bit[101]) | (~bit[106]&~bit[105]&bit[104]&~bit[103]&bit[102]&bit[101]) | (~bit[105]&~bit[104]&bit[102]&~bit[101]) | (bit[105]&bit[104]&~bit[102]&~bit[101]))) | ( bit[100]&((~bit[101]&((~bit[105]&bit[104]&~bit[103]) | (~bit[105]&bit[104]&~bit[102]) | (~bit[104]&~bit[103]&~bit[102]) | (~bit[107]&bit[106]&bit[105]&~bit[104]) | (bit[106]&bit[105]&~bit[104]&~bit[103]) | (bit[106]&bit[105]&~bit[104]&~bit[102]) | (~bit[106]&bit[104]&bit[103]&~bit[102]) | (~bit[107]&~bit[106]&bit[105]&bit[104]&bit[103]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[3]);
		out[2]=( control & ( (~bit[100]&((~bit[104]&~bit[103]&~bit[102]) | (bit[105]&~bit[104]&~bit[101]) | (~bit[104]&bit[103]&~bit[101]) | (~bit[105]&bit[104]&bit[103]&bit[101]) | (bit[105]&bit[103]&~bit[102]&~bit[101]) | (~bit[105]&bit[104]&~bit[103]&bit[102]&~bit[101]) | (bit[106]&~bit[105]&bit[104]&bit[102]&bit[101]) | (~bit[106]&~bit[105]&bit[103]&bit[102]&bit[101]))) | ( bit[100]&((~bit[101]&((~bit[106]&~bit[105]&bit[103]) | (bit[106]&bit[105]&~bit[103]&bit[102]) | (bit[106]&bit[105]&bit[103]&~bit[102]) | (~bit[105]&~bit[104]&~bit[103]&~bit[102]) | (bit[105]&bit[104]&~bit[103]) | (~bit[107]&bit[105]&bit[104]&bit[102]) | (~bit[106]&bit[105]&bit[104]&bit[102]) | (~bit[105]&bit[104]&bit[103]&~bit[102]) | (~bit[106]&bit[105]&~bit[103]&~bit[102]) | (bit[107]&bit[106]&bit[105]&~bit[104]&bit[102]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[2]);
		out[1]=( control & ( (~bit[100]&((~bit[105]&bit[104]&~bit[102]) | (~bit[105]&bit[103]&~bit[101]) | (~bit[106]&bit[104]&bit[103]&bit[101]) | (~bit[105]&~bit[104]&bit[102]&bit[101]) | (bit[104]&bit[103]&~bit[102]&bit[101]) | (bit[105]&~bit[104]&~bit[103]&bit[102]&~bit[101]) | (~bit[105]&bit[103]&bit[102]))) | ( bit[100]&((~bit[101]&((bit[106]&~bit[104]&~bit[102]) | (bit[106]&bit[105]&~bit[104]&bit[103]) | (~bit[105]&bit[104]&~bit[103]&bit[102]) | (~bit[106]&bit[104]&bit[103]&~bit[102]) | (~bit[107]&bit[106]&~bit[105]&bit[104]&bit[102]) | (bit[105]&bit[104]&~bit[102]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (~bit[106]&bit[105]&bit[104]&~bit[103]) | (~bit[106]&~bit[105]&~bit[104]&~bit[103]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[1]);
		out[0]=( control & ( (~bit[100]&((~bit[105]&~bit[103]&~bit[101]) | (bit[106]&~bit[105]&bit[104]&~bit[103]) | (bit[105]&bit[104]&~bit[102]&bit[101]) | (~bit[105]&~bit[104]&~bit[102]&bit[101]) | (bit[105]&~bit[104]&bit[103]&~bit[101]) | (~bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (~bit[106]&bit[105]&bit[104]&~bit[103]&bit[101]) | (~bit[106]&~bit[105]&bit[104]&bit[103]&bit[102]&bit[101]) | (bit[104]&~bit[103]&~bit[102]&bit[101]))) | ( bit[100]&((~bit[101]&((~bit[106]&bit[105]&bit[104]) | (bit[106]&~bit[105]&bit[104]&bit[103]) | (bit[106]&bit[105]&~bit[104]&~bit[102]) | (~bit[106]&~bit[105]&~bit[103]) | (~bit[106]&~bit[103]&~bit[102]) | (bit[106]&~bit[105]&~bit[104]&bit[102]) | (bit[107]&bit[106]&bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[103]&bit[102]) | (~bit[107]&~bit[105]&bit[104]&bit[103]&bit[102]))) /*| ( bit[101]&(expr2))*/))  ) ) | (~control&out[0]);


		control=control&bit[100]&bit[101];
		//disable_update=disable_update|(~control);

		//---------------102-103--------------
		out[6]=( control & ( (~bit[102]&((~bit[107]&~bit[106]&bit[105]&bit[104]&bit[103]) | (bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]))) | ( bit[102]&((~bit[103]&((bit[108]&~bit[106]&bit[105]&bit[104]) | (bit[107]&~bit[106]&bit[105]&bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[6]);
		out[5]=( control & ( (~bit[102]&((~bit[105]&~bit[103]) | (~bit[104]&~bit[103]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (bit[106]&bit[105]&bit[104]&bit[103]))) | ( bit[102]&((~bit[103]&((~bit[105]&~bit[104]) | (bit[106]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[106]&~bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[5]);
		out[4]=( control & ( (~bit[102]&((bit[106]&bit[105]&bit[104]) | (~bit[106]&~bit[105]&~bit[104]) | (~bit[106]&~bit[104]&bit[103]) | (~bit[105]&~bit[104]&bit[103]) | (bit[105]&bit[104]&~bit[103]) | (bit[107]&~bit[106]&bit[105]&bit[103]))) | ( bit[102]&((~bit[103]&((~bit[107]&bit[106]&bit[105]) | (~bit[108]&~bit[107]&~bit[106]&~bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[105]&~bit[104]) | (bit[108]&~bit[106]&bit[105]&~bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[4]);
		out[3]=( control & ( (~bit[102]&((bit[105]&~bit[103]) | (~bit[107]&bit[106]&bit[105]) | (bit[106]&bit[105]&~bit[104]) | (bit[106]&~bit[104]&~bit[103]) | (bit[107]&~bit[106]&bit[105]&bit[104]) | (~bit[106]&~bit[105]&~bit[104]&bit[103]) | (~bit[107]&~bit[106]&bit[104]&~bit[103]) | (~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]))) | ( bit[102]&((~bit[103]&((bit[108]&bit[107]&bit[106]&bit[105]) | (bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[105]&bit[104]) | (~bit[108]&bit[107]&~bit[106]&~bit[104]) | (bit[108]&~bit[107]&~bit[106]&~bit[104]) | (bit[107]&~bit[106]&~bit[105]) | (~bit[106]&~bit[105]&bit[104]) | (~bit[109]&~bit[108]&bit[106]&bit[105]&bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[102]&((bit[106]&~bit[105]&~bit[104]) | (~bit[106]&bit[105]&~bit[103]) | (bit[105]&bit[104]&~bit[103]) | (~bit[108]&bit[106]&bit[105]&bit[104]) | (bit[107]&~bit[106]&bit[105]&bit[104]) | (bit[107]&~bit[106]&bit[104]&~bit[103]) | (bit[108]&bit[107]&bit[106]&~bit[105]&bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&bit[104]&~bit[103]))) | ( bit[102]&((~bit[103]&((bit[108]&~bit[107]&~bit[105]) | (bit[108]&~bit[107]&~bit[106]&bit[104]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]) | (~bit[108]&~bit[107]&bit[106]&~bit[104]) | (bit[108]&bit[107]&bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[106]&~bit[105]) | (~bit[109]&bit[107]&bit[106]&bit[104]) | (bit[107]&bit[106]&~bit[105]&bit[104]) | (bit[109]&~bit[108]&bit[106]&bit[105]&bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[102]&((~bit[107]&bit[106]&~bit[104]) | (~bit[106]&bit[105]&bit[104]&~bit[103]) | (~bit[107]&~bit[105]&~bit[104]&~bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[108]&~bit[106]&bit[105]&bit[104]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (bit[107]&bit[106]&~bit[105]&~bit[103]) | (bit[107]&~bit[105]&bit[104]&~bit[103]) | (~bit[108]&bit[107]&bit[106]&bit[104]&bit[103]) | (bit[108]&bit[106]&bit[105]&bit[104]&bit[103]))) | ( bit[102]&((~bit[103]&((~bit[108]&~bit[106]&bit[104]) | (~bit[108]&bit[105]&bit[104]) | (bit[108]&bit[105]&~bit[104]) | (bit[108]&~bit[107]&~bit[106]&~bit[104]) | (bit[108]&~bit[107]&bit[106]&~bit[105]&bit[104]) | (~bit[108]&~bit[107]&bit[106]&~bit[105]&~bit[104]) | (~bit[109]&~bit[107]&bit[105]&bit[104]) | (bit[109]&bit[107]&bit[106]&bit[105]&bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[102]&((~bit[107]&bit[105]&~bit[103]) | (bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[107]&bit[106]&~bit[105]&~bit[104]) | (~bit[107]&~bit[106]&~bit[104]&bit[103]) | (bit[106]&~bit[105]&~bit[104]&bit[103]) | (bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[108]&~bit[106]&bit[105]&bit[104]&bit[103]))) | ( bit[102]&((~bit[103]&((bit[108]&~bit[107]&~bit[105]) | (bit[108]&bit[107]&bit[106]&bit[105]) | (bit[108]&~bit[106]&~bit[105]&bit[104]) | (~bit[108]&bit[106]&bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]) | (~bit[109]&bit[108]&bit[105]&bit[104]) | (~bit[109]&bit[107]&bit[105]&bit[104]) | (~bit[108]&bit[107]&~bit[106]&~bit[104]) | (bit[108]&bit[106]&~bit[105]&~bit[104]) | (~bit[108]&~bit[106]&~bit[105]&~bit[104]) | (bit[109]&~bit[108]&~bit[107]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[106]&bit[105]&bit[104]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[102]&bit[103];
		//disable_update=disable_update|(~control);

		//---------------104-105--------------
		out[6]=( control & ( (~bit[104]&(~bit[108]&~bit[107]&bit[106]&bit[105])) | ( bit[104]&((~bit[105]&((bit[110]&~bit[108]&bit[107]&bit[106]) | (bit[109]&~bit[108]&bit[107]&bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[104]&((bit[108]&bit[106]&bit[105]) | (bit[107]&bit[106]&bit[105]) | (~bit[107]&~bit[106]&~bit[105]) | (~bit[109]&~bit[108]&~bit[106]&~bit[105]))) | ( bit[104]&((~bit[105]&((bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[108]&~bit[107]&~bit[106]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[104]&((~bit[108]&bit[107]&bit[106]) | (bit[108]&~bit[107]&bit[106]) | (bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[107]&bit[106]&~bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]))) | ( bit[104]&((~bit[105]&((~bit[109]&bit[108]&bit[107]) | (~bit[108]&bit[107]&~bit[106]) | (bit[108]&~bit[107]&~bit[106]) | (~bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]) | (bit[110]&bit[109]&~bit[107]&~bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[104]&((~bit[109]&~bit[108]&~bit[107]) | (~bit[107]&~bit[106]&bit[105]) | (bit[108]&bit[107]&~bit[105]) | (~bit[109]&~bit[108]&~bit[106]&bit[105]) | (~bit[108]&~bit[107]&bit[106]&~bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]) | (~bit[110]&bit[108]&bit[106]&bit[105]) | (~bit[109]&bit[108]&bit[106]&bit[105]))) | ( bit[104]&((~bit[105]&((bit[109]&bit[108]&~bit[106]) | (bit[108]&~bit[107]&~bit[106]) | (bit[110]&bit[109]&bit[108]&bit[107]) | (bit[110]&~bit[109]&~bit[108]&bit[106]) | (~bit[110]&~bit[108]&~bit[107]&bit[106]) | (bit[110]&bit[109]&~bit[107]&~bit[106]) | (~bit[111]&~bit[110]&bit[109]&~bit[108]&bit[106]) | (~bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[104]&((bit[109]&~bit[108]&~bit[106]) | (bit[108]&bit[106]&~bit[105]) | (bit[110]&bit[109]&bit[108]&bit[106]) | (~bit[109]&bit[108]&bit[107]&~bit[106]) | (~bit[110]&bit[109]&~bit[108]&bit[105]) | (~bit[108]&~bit[107]&~bit[106]&bit[105]) | (~bit[109]&~bit[107]&bit[106]&~bit[105]) | (~bit[109]&~bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[110]&~bit[109]&bit[108]&bit[106]) | (~bit[109]&bit[108]&~bit[107]&bit[106]) | (bit[109]&bit[107]&~bit[106]&~bit[105]))) | ( bit[104]&((~bit[105]&((bit[110]&bit[109]&~bit[108]) | (bit[111]&bit[109]&bit[107]) | (~bit[110]&~bit[109]&~bit[107]) | (~bit[109]&bit[108]&~bit[107]) | (bit[110]&~bit[108]&~bit[106]) | (bit[109]&bit[107]&~bit[106]) | (~bit[110]&bit[108]&~bit[107]&bit[106]) | (~bit[111]&bit[110]&bit[108]&bit[107]&bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[104]&((~bit[109]&bit[108]&~bit[106]) | (~bit[109]&bit[108]&~bit[105]) | (bit[107]&bit[106]&~bit[105]) | (bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]) | (bit[110]&bit[108]&bit[107]&bit[105]) | (~bit[110]&~bit[109]&bit[106]&bit[105]) | (bit[109]&bit[107]&~bit[106]&bit[105]) | (bit[109]&~bit[108]&~bit[106]&~bit[105]) | (bit[109]&~bit[107]&~bit[106]&~bit[105]))) | ( bit[104]&((~bit[105]&((bit[110]&~bit[109]&~bit[108]&bit[107]) | (bit[110]&bit[109]&~bit[107]&bit[106]) | (~bit[110]&bit[108]&bit[107]&~bit[106]) | (bit[109]&bit[108]&bit[107]&~bit[106]) | (bit[111]&~bit[110]&bit[109]&bit[107]&bit[106]) | (bit[111]&bit[110]&~bit[109]&bit[107]&bit[106]) | (~bit[110]&~bit[109]&bit[108]&~bit[107]&bit[106]) | (bit[110]&~bit[108]&~bit[107]&bit[106]) | (~bit[110]&bit[109]&~bit[107]&~bit[106]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]) | (~bit[111]&bit[110]&bit[109]&bit[108]&bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[104]&((bit[110]&~bit[109]&bit[107]&bit[106]) | (bit[109]&bit[108]&~bit[107]&bit[106]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[109]&bit[108]&~bit[107]&bit[105]) | (bit[109]&~bit[107]&bit[106]&bit[105]) | (~bit[109]&bit[107]&bit[106]&~bit[105]) | (bit[109]&~bit[108]&~bit[106]&~bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]) | (~bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]&~bit[105]) | (~bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[110]&~bit[108]&~bit[107]&bit[106]&bit[105]))) | ( bit[104]&((~bit[105]&((bit[111]&~bit[110]&bit[109]&bit[107]) | (bit[110]&bit[109]&bit[108]&~bit[107]) | (bit[110]&bit[108]&~bit[107]&bit[106]) | (~bit[110]&bit[109]&bit[107]&~bit[106]) | (~bit[110]&~bit[109]&~bit[107]&~bit[106]) | (bit[111]&bit[110]&~bit[109]&bit[108]&bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (~bit[111]&~bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[110]&bit[109]&~bit[108]) | (bit[109]&~bit[108]&~bit[107]&bit[106]) | (~bit[111]&bit[110]&~bit[108]&bit[107]&bit[106]))) /*| ( bit[105]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[105]&bit[104];
		//disable_update=disable_update|(~control);

		//---------------106-107--------------
		out[6]=( control & ( (~bit[106]&((~bit[110]&~bit[109]&bit[108]&bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[108]&bit[107]))) | ( bit[106]&((~bit[107]&(~bit[110]&bit[109]&bit[108])) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[106]&((bit[110]&bit[108]&bit[107]) | (bit[109]&bit[108]&bit[107]) | (~bit[111]&~bit[108]&~bit[107]) | (~bit[110]&~bit[108]&~bit[107]) | (~bit[109]&~bit[108]&~bit[107]))) | ( bit[106]&((~bit[107]&((bit[110]&bit[109]&bit[108]) | (~bit[110]&~bit[109]&~bit[108]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[106]&((~bit[110]&bit[109]&bit[108]) | (bit[110]&~bit[109]&bit[108]) | (~bit[111]&bit[108]&~bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[108]&~bit[107]) | (~bit[109]&bit[108]&~bit[107]) | (~bit[112]&~bit[111]&bit[109]&bit[108]))) | ( bit[106]&((~bit[107]&((bit[110]&bit[109]&bit[108]) | (~bit[110]&bit[109]&~bit[108]) | (bit[110]&~bit[109]&~bit[108]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[106]&((bit[111]&bit[110]&bit[109]) | (~bit[110]&~bit[109]&~bit[108]) | (~bit[111]&~bit[109]&~bit[107]) | (~bit[112]&~bit[111]&~bit[110]&bit[108]) | (bit[112]&bit[110]&bit[108]&bit[107]) | (~bit[111]&~bit[110]&bit[108]&~bit[107]) | (~bit[111]&~bit[109]&bit[108]) | (bit[110]&~bit[109]&bit[108]) | (~bit[109]&bit[108]&~bit[107]))) | ( bit[106]&((~bit[107]&((~bit[111]&bit[110]&bit[109]) | (~bit[112]&bit[110]&~bit[108]) | (~bit[111]&bit[110]&~bit[108]) | (bit[110]&bit[109]&~bit[108]) | (~bit[113]&~bit[112]&bit[110]&bit[109]) | (~bit[113]&~bit[111]&bit[109]&bit[108]) | (~bit[112]&~bit[111]&~bit[110]&~bit[109]) | (~bit[112]&~bit[111]&~bit[110]&bit[108]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[106]&((~bit[109]&~bit[108]&bit[107]) | (~bit[111]&bit[110]&~bit[109]&bit[107]) | (bit[112]&bit[109]&bit[108]&bit[107]) | (~bit[110]&~bit[109]&bit[108]&~bit[107]) | (~bit[111]&~bit[110]&bit[109]&~bit[108]&~bit[107]) | (bit[111]&bit[109]&bit[108]) | (~bit[112]&bit[111]&~bit[110]&bit[108]) | (~bit[112]&bit[111]&bit[110]&~bit[108]) | (bit[111]&bit[110]&~bit[108]&~bit[107]))) | ( bit[106]&((~bit[107]&((~bit[112]&~bit[111]&bit[110]) | (~bit[111]&bit[109]&~bit[108]) | (bit[113]&bit[112]&~bit[111]&~bit[110]) | (~bit[112]&bit[111]&~bit[110]&bit[108]) | (~bit[112]&bit[110]&bit[109]&~bit[108]) | (~bit[113]&bit[112]&bit[110]&bit[109]&bit[108]) | (bit[112]&bit[111]&bit[110]&~bit[109]&~bit[108]) | (bit[112]&~bit[111]&~bit[110]&~bit[109]) | (bit[112]&~bit[110]&~bit[109]&bit[108]) | (bit[113]&~bit[112]&bit[111]&bit[109]&bit[108]))) /*| ( bit[107]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[106]&((bit[111]&~bit[108]&~bit[107]) | (~bit[112]&~bit[109]&bit[108]&bit[107]) | (~bit[111]&~bit[110]&~bit[108]&bit[107]) | (~bit[111]&~bit[109]&~bit[108]&bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[107]) | (bit[112]&bit[111]&bit[110]&bit[109]&~bit[108]) | (~bit[111]&bit[110]&~bit[109]&bit[108]&~bit[107]) | (~bit[112]&bit[110]&bit[108]&bit[107]) | (~bit[110]&~bit[109]&~bit[108]&~bit[107]) | (bit[112]&~bit[111]&bit[109]&bit[108]&bit[107]))) | ( bit[106]&((~bit[107]&((~bit[112]&bit[111]&~bit[109]) | (bit[111]&~bit[109]&~bit[108]) | (bit[112]&~bit[111]&~bit[109]&bit[108]) | (~bit[112]&~bit[111]&bit[110]&bit[109]&~bit[108]) | (~bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]&bit[108]) | (bit[113]&bit[112]&~bit[111]&bit[108]) | (bit[112]&bit[110]&~bit[109]&~bit[108]) | (~bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (bit[113]&bit[111]&bit[110]&bit[109]&bit[108]) | (~bit[112]&~bit[111]&~bit[110]&bit[109]&bit[108]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[106]&((bit[111]&bit[110]&~bit[107]) | (~bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[110]&bit[109]&~bit[108]&bit[107]) | (~bit[111]&~bit[110]&~bit[109]&~bit[107]) | (bit[112]&bit[111]&bit[110]&bit[109]) | (bit[112]&~bit[109]&bit[108]&bit[107]) | (bit[111]&bit[109]&~bit[108]&~bit[107]) | (~bit[112]&bit[111]&~bit[110]&bit[108]&bit[107]))) | ( bit[106]&((~bit[107]&((bit[113]&bit[112]&bit[108]) | (bit[112]&~bit[109]&bit[108]) | (bit[112]&bit[110]&~bit[108]) | (~bit[112]&~bit[110]&~bit[108]) | (~bit[113]&~bit[112]&~bit[110]&bit[109]) | (bit[113]&bit[110]&bit[109]&bit[108]) | (~bit[113]&bit[111]&~bit[110]&bit[109]&bit[108]))) /*| ( bit[1X]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[106]&bit[107];
		//disable_update=disable_update|(~control);

		//---------------108-109--------------
		out[6]=( control & ( (~bit[108]&((bit[113]&~bit[111]&bit[110]&bit[109]) | (bit[112]&~bit[111]&bit[110]&bit[109]) | (~bit[114]&~bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((bit[113]&~bit[112]&bit[111]&bit[110]) | (~bit[115]&~bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[108]&((~bit[110]&~bit[109]) | (bit[114]&bit[111]&bit[110]&bit[109]) | (bit[113]&bit[111]&bit[110]&bit[109]) | (bit[112]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((~bit[112]&~bit[110]) | (~bit[111]&~bit[110]) | (bit[115]&bit[112]&bit[111]&bit[110]) | (bit[114]&bit[112]&bit[111]&bit[110]) | (bit[113]&bit[112]&bit[111]&bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[108]&((bit[110]&~bit[109]) | (bit[114]&bit[111]&bit[110]) | (bit[113]&bit[111]&bit[110]) | (bit[112]&bit[111]&bit[110]))) | ( bit[108]&((~bit[109]&((~bit[113]&~bit[112]&~bit[111]) | (bit[112]&bit[111]&~bit[110]) | (~bit[114]&~bit[112]&~bit[111]&~bit[110]) | (~bit[115]&bit[114]&bit[112]&bit[111]) | (~bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&~bit[113]&bit[112]&bit[111]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[108]&((~bit[111]&~bit[110]) | (~bit[111]&~bit[109]) | (~bit[113]&~bit[112]&~bit[110]) | (~bit[112]&~bit[110]&~bit[109]) | (bit[113]&~bit[112]&bit[110]&bit[109]) | (~bit[114]&~bit[113]&bit[112]&bit[110]&bit[109]) | (bit[114]&~bit[113]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((bit[115]&~bit[113]&bit[112]) | (bit[114]&~bit[113]&bit[112]) | (~bit[114]&bit[113]&~bit[112]&bit[110]) | (~bit[114]&~bit[113]&bit[111]&~bit[110]) | (bit[115]&bit[114]&bit[112]&bit[111]&bit[110]) | (~bit[114]&bit[112]&~bit[111]) | (bit[114]&bit[113]&~bit[112]&~bit[111]) | (bit[114]&bit[113]&~bit[111]&~bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[108]&((~bit[113]&bit[112]&~bit[110]) | (~bit[112]&~bit[111]&~bit[110]) | (bit[113]&~bit[112]&~bit[110]&bit[109]) | (~bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[114]&~bit[113]&~bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[113]&bit[112]&~bit[111]&bit[109]) | (~bit[114]&bit[113]&bit[112]&bit[110]&bit[109]) | (bit[113]&bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[112]&bit[110]&~bit[109]) | (~bit[114]&bit[113]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((bit[114]&bit[113]&~bit[112]&~bit[111]) | (~bit[114]&~bit[113]&bit[112]&bit[110]) | (bit[114]&bit[113]&~bit[111]&bit[110]) | (bit[113]&~bit[112]&~bit[111]&bit[110]) | (~bit[114]&bit[113]&bit[111]&~bit[110]) | (bit[113]&bit[112]&bit[111]&~bit[110]) | (~bit[114]&~bit[113]&~bit[111]&~bit[110]) | (~bit[113]&bit[112]&~bit[111]&~bit[110]) | (~bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[115]&~bit[114]&bit[113]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[110]) | (bit[114]&bit[113]&~bit[112]&bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[108]&((bit[113]&bit[112]&~bit[111]&~bit[110]) | (~bit[113]&~bit[112]&~bit[111]&~bit[110]) | (bit[113]&bit[111]&~bit[110]&bit[109]) | (~bit[113]&bit[112]&bit[111]&~bit[109]) | (~bit[113]&bit[111]&bit[110]&~bit[109]) | (~bit[114]&bit[113]&~bit[112]&~bit[111]&bit[110]) | (bit[114]&bit[112]&~bit[111]&bit[110]&bit[109]) | (bit[113]&~bit[112]&~bit[111]&bit[110]&~bit[109]) | (bit[113]&bit[112]&bit[111]&bit[109]) | (~bit[114]&bit[112]&bit[111]&bit[110]&bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((bit[114]&~bit[112]&~bit[110]) | (bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[114]&~bit[112]&~bit[111]&bit[110]) | (~bit[114]&bit[113]&bit[112]&~bit[110]) | (~bit[114]&bit[112]&bit[111]&~bit[110]) | (bit[114]&~bit[113]&bit[112]&~bit[111]&bit[110]) | (bit[115]&bit[113]&bit[111]&bit[110]) | (bit[115]&bit[112]&bit[111]&bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[108]&((bit[113]&~bit[112]&~bit[111]&~bit[110]) | (bit[112]&bit[111]&~bit[110]&bit[109]) | (~bit[113]&bit[112]&bit[110]&~bit[109]) | (bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[114]&~bit[113]&bit[112]&bit[110]) | (~bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[114]&bit[113]&bit[110]&bit[109]) | (bit[113]&~bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[112]&bit[111]&~bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]))) | ( bit[108]&((~bit[109]&((bit[114]&bit[113]&bit[112]) | (bit[114]&~bit[111]&bit[110]) | (~bit[114]&~bit[113]&bit[112]&~bit[110]) | (~bit[114]&bit[113]&~bit[112]&~bit[110]) | (bit[114]&~bit[113]&~bit[112]&~bit[110]) | (bit[113]&~bit[112]&~bit[111]) | (~bit[115]&bit[113]&~bit[112]&bit[110]) | (~bit[114]&~bit[113]&~bit[112]&bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]))) /*| ( bit[109]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[108]&bit[109];
		//disable_update=disable_update|(~control);

		//---------------110-111--------------
		out[6]=( control & ( (~bit[110]&((~bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[116]&~bit[115]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]))) | ( bit[110]&((~bit[111]&((~bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[117]&~bit[116]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[110]&((~bit[112]&~bit[111]) | (bit[116]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]))) | ( bit[110]&((~bit[111]&((~bit[114]&~bit[112]) | (~bit[113]&~bit[112]) | (~bit[116]&~bit[115]&~bit[112]) | (bit[117]&bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[115]&bit[114]&bit[113]&bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[110]&((bit[112]&~bit[111]) | (bit[116]&bit[114]&bit[113]&bit[112]) | (bit[115]&bit[114]&bit[113]&bit[112]))) | ( bit[110]&((~bit[111]&((~bit[114]&~bit[113]) | (~bit[116]&~bit[115]&~bit[113]) | (bit[117]&bit[115]&bit[114]&bit[113]) | (bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[116]&bit[115]&bit[114]&~bit[112]) | (bit[116]&~bit[115]&bit[114]&~bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[110]&((~bit[114]&~bit[112]&bit[111]) | (~bit[115]&~bit[113]&~bit[111]) | (~bit[114]&~bit[113]&~bit[111]) | (bit[115]&bit[114]&bit[112]&bit[111]) | (bit[116]&bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[113]&~bit[112]&bit[111]))) | ( bit[110]&((~bit[111]&((~bit[116]&~bit[115]&~bit[114]&bit[113]) | (bit[116]&bit[115]&bit[114]&~bit[113]) | (bit[116]&bit[115]&bit[113]&bit[112]) | (bit[116]&bit[114]&~bit[113]&bit[112]) | (bit[115]&bit[114]&~bit[113]&bit[112]) | (~bit[116]&~bit[115]&~bit[114]&~bit[112]) | (~bit[117]&~bit[116]&~bit[115]&bit[113]&bit[112]) | (~bit[116]&bit[115]&bit[114]&bit[113]&~bit[112]) | (bit[116]&~bit[115]&bit[114]&bit[113]&~bit[112]) | (bit[117]&~bit[116]&bit[115]&bit[114]&bit[113]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[110]&((~bit[115]&~bit[114]&~bit[111]) | (~bit[114]&bit[113]&~bit[111]) | (~bit[115]&bit[114]&~bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&~bit[111]) | (~bit[114]&~bit[113]&~bit[112]) | (~bit[113]&~bit[112]&bit[111]) | (~bit[116]&~bit[114]&bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]))) | ( bit[110]&((~bit[111]&((bit[116]&~bit[115]&~bit[113]&bit[112]) | (~bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[117]&~bit[116]&bit[114]&bit[113]&bit[112]) | (~bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[116]&bit[115]&~bit[114]&~bit[112]) | (bit[116]&~bit[115]&~bit[114]&~bit[112]) | (~bit[116]&bit[115]&bit[113]&~bit[112]) | (bit[116]&bit[114]&bit[113]&~bit[112]) | (bit[116]&bit[115]&~bit[113]&~bit[112]) | (~bit[117]&bit[116]&~bit[115]&bit[114]&bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[110]&((~bit[115]&~bit[114]&~bit[113]) | (~bit[116]&~bit[115]&~bit[114]&bit[111]) | (bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[114]&~bit[113]&~bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&~bit[111]) | (bit[116]&~bit[115]&bit[114]&bit[113]&bit[111]) | (~bit[115]&bit[113]&~bit[112]&bit[111]) | (~bit[115]&bit[114]&~bit[112]&~bit[111]))) | ( bit[110]&((~bit[111]&((bit[115]&~bit[113]&bit[112]) | (~bit[116]&bit[115]&~bit[114]&~bit[113]) | (~bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[115]&bit[113]&~bit[112]) | (~bit[116]&~bit[115]&bit[114]&~bit[113]&~bit[112]) | (bit[116]&~bit[115]&~bit[114]&~bit[113]&~bit[112]) | (~bit[117]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[114]&~bit[113]&bit[112]) | (bit[116]&bit[115]&bit[114]&~bit[112]) | (bit[116]&bit[114]&bit[113]&~bit[112]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[110]&((~bit[115]&~bit[112]&bit[111]) | (bit[113]&~bit[112]&bit[111]) | (~bit[115]&bit[113]&~bit[111]) | (bit[115]&~bit[113]&~bit[112]&~bit[111]) | (bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[116]&~bit[114]&bit[113]&bit[111]) | (~bit[114]&~bit[113]&bit[112]&~bit[111]) | (~bit[116]&bit[115]&bit[114]&bit[112]&bit[111]))) | ( bit[110]&((~bit[111]&((~bit[115]&~bit[114]&bit[113]) | (~bit[116]&~bit[113]&bit[112]) | (bit[116]&~bit[115]&~bit[112]) | (~bit[115]&bit[113]&~bit[112]) | (bit[116]&~bit[114]&bit[113]&~bit[112]) | (bit[116]&bit[114]&~bit[113]&~bit[112]) | (bit[117]&bit[116]&bit[113]&bit[112]) | (bit[117]&bit[114]&bit[113]&bit[112]) | (~bit[115]&bit[114]&~bit[113]&bit[112]))) /*| ( bit[111]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[110]&bit[111];
		//disable_update=disable_update|(~control);


		//---------------112-113--------------
		out[6]=( control & ( (~bit[112]&((~bit[118]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[115]&bit[114]&bit[113]) | (~bit[116]&~bit[115]&bit[114]&bit[113]))) | ( bit[112]&((~bit[113]&((~bit[118]&~bit[116]&bit[115]&bit[114]) | (~bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[117]&bit[116]&~bit[115]&bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[112]&((~bit[115]&~bit[113]) | (~bit[114]&~bit[113]) | (bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[118]&bit[117]&bit[116]&bit[114]&bit[113]))) | ( bit[112]&((~bit[113]&((bit[116]&bit[115]&bit[114]) | (~bit[116]&~bit[115]&~bit[114]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (~bit[118]&~bit[117]&~bit[115]&~bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[112]&((~bit[116]&~bit[115]&~bit[114]) | (~bit[115]&~bit[114]&~bit[113]) | (~bit[116]&bit[115]&bit[114]&bit[113]) | (bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&~bit[116]&~bit[114]&~bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (bit[117]&bit[115]&bit[114]&~bit[113]) | (~bit[118]&~bit[117]&bit[115]&bit[114]&bit[113]))) | ( bit[112]&((~bit[113]&((~bit[116]&bit[115]&~bit[114]) | (bit[118]&bit[117]&~bit[116]&bit[115]) | (~bit[118]&bit[116]&bit[115]&bit[114]) | (~bit[117]&bit[116]&bit[115]&bit[114]) | (bit[118]&bit[116]&~bit[115]&~bit[114]) | (bit[117]&bit[116]&~bit[115]&~bit[114]) | (~bit[118]&~bit[117]&bit[115]&~bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[112]&((bit[116]&~bit[115]&~bit[114]&bit[113]) | (~bit[117]&bit[116]&bit[115]&~bit[113]) | (~bit[117]&~bit[116]&~bit[115]&~bit[113]) | (~bit[116]&~bit[115]&bit[114]) | (~bit[118]&~bit[116]&bit[114]&bit[113]) | (~bit[116]&bit[115]&~bit[114]&bit[113]) | (bit[117]&~bit[116]&bit[114]&~bit[113]) | (bit[117]&bit[115]&~bit[114]&~bit[113]) | (bit[118]&bit[117]&bit[116]&bit[114]&bit[113]) | (bit[118]&~bit[117]&bit[115]&bit[114]&bit[113]) | (bit[117]&bit[116]&bit[115]&bit[114]&bit[113]))) | ( bit[112]&((~bit[113]&((bit[118]&bit[116]&~bit[114]) | (bit[117]&bit[116]&~bit[114]) | (~bit[118]&~bit[117]&~bit[116]&~bit[115]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (~bit[117]&~bit[116]&~bit[115]&bit[114]) | (~bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (bit[117]&bit[116]&~bit[115]) | (~bit[118]&bit[117]&~bit[115]&bit[114]) | (~bit[119]&~bit[118]&~bit[117]&bit[115]&bit[114]) | (~bit[119]&~bit[117]&bit[116]&bit[115]&bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[112]&((bit[118]&~bit[117]&bit[116]) | (~bit[117]&bit[116]&~bit[114]) | (bit[116]&~bit[115]&bit[113]) | (bit[117]&bit[115]&~bit[113]) | (~bit[118]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[115]&bit[114]&bit[113]) | (bit[117]&~bit[116]&~bit[114]&~bit[113]) | (~bit[118]&~bit[117]&~bit[116]&bit[114]&bit[113]) | (~bit[117]&bit[116]&~bit[113]))) | ( bit[112]&((~bit[113]&((bit[118]&~bit[117]&~bit[114]) | (bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[119]&~bit[118]&bit[116]&bit[114]) | (bit[118]&bit[117]&~bit[116]&bit[114]) | (bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]&bit[115]) | (~bit[118]&~bit[117]&bit[116]&~bit[115]&bit[114]) | (~bit[118]&bit[117]&~bit[116]&~bit[115]&~bit[114]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (~bit[119]&bit[118]&~bit[116]&bit[115]&bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[112]&((~bit[117]&bit[115]&~bit[114]) | (bit[118]&bit[115]&bit[113]) | (~bit[117]&~bit[116]&~bit[115]&bit[114]&~bit[113]) | (bit[117]&~bit[116]&~bit[114]) | (~bit[117]&~bit[114]&bit[113]) | (bit[117]&bit[116]&bit[115]&bit[113]) | (~bit[118]&~bit[117]&~bit[115]&bit[113]) | (bit[118]&bit[116]&bit[114]&bit[113]) | (bit[117]&bit[116]&~bit[115]&~bit[113]) | (bit[117]&bit[115]&bit[114]&~bit[113]))) | ( bit[112]&((~bit[113]&((bit[118]&~bit[115]&~bit[114]) | (bit[118]&~bit[117]&bit[116]&~bit[114]) | (~bit[118]&~bit[116]&bit[115]&~bit[114]) | (bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (~bit[118]&~bit[117]&~bit[116]&~bit[115]&bit[114]) | (bit[119]&bit[118]&~bit[117]&bit[116]) | (~bit[119]&bit[118]&bit[116]&~bit[115]) | (~bit[118]&bit[117]&bit[116]&~bit[114]) | (~bit[119]&bit[118]&bit[117]&bit[116]&bit[114]) | (bit[118]&bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[119]&~bit[117]&~bit[116]&bit[115]&bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[112]&((bit[117]&~bit[114]&~bit[113]) | (bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[117]&~bit[116]&~bit[114]&bit[113]) | (bit[117]&bit[116]&~bit[115]&~bit[113]) | (~bit[117]&bit[115]&bit[114]&~bit[113]) | (bit[118]&~bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[118]&~bit[116]&bit[115]&bit[114]) | (~bit[117]&bit[116]&~bit[115]&~bit[114]) | (~bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[118]&bit[117]&bit[115]&bit[114]&bit[113]) | (~bit[118]&bit[116]&~bit[115]&bit[114]&bit[113]))) | ( bit[112]&((~bit[113]&((bit[118]&bit[115]&~bit[114]) | (~bit[118]&~bit[115]&~bit[114]) | (~bit[119]&bit[117]&bit[115]&bit[114]) | (~bit[118]&bit[116]&bit[115]&bit[114]) | (bit[118]&~bit[116]&~bit[115]&bit[114]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]) | (~bit[117]&bit[116]&~bit[115]&~bit[114]) | (bit[117]&~bit[116]&~bit[115]&~bit[114]) | (bit[119]&~bit[118]&~bit[117]&bit[115]&bit[114]) | (bit[119]&bit[117]&bit[116]&~bit[115]&bit[114]))) /*| ( bit[113]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[112]&bit[113];
		disable_update=disable_update|(~control);
		//---------------114-115--------------
		out[6]=( control & ( (~bit[114]&((~bit[120]&~bit[117]&bit[116]&bit[115]) | (~bit[119]&~bit[117]&bit[116]&bit[115]) | (~bit[118]&~bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((~bit[118]&bit[117]&bit[116]) | (~bit[120]&~bit[119]&bit[117]&bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[114]&((bit[117]&bit[116]&bit[115]) | (~bit[118]&~bit[116]&~bit[115]) | (~bit[117]&~bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[118]&bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((~bit[118]&~bit[117]&~bit[116]) | (bit[120]&bit[118]&bit[117]&bit[116]) | (bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[120]&~bit[119]&~bit[117]&~bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[114]&((bit[116]&~bit[115]) | (~bit[120]&bit[117]&bit[116]) | (~bit[119]&bit[117]&bit[116]) | (~bit[118]&bit[117]&bit[116]) | (bit[118]&bit[117]&~bit[115]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]))) | ( bit[114]&((~bit[115]&((~bit[119]&bit[117]&~bit[116]) | (~bit[118]&bit[117]&~bit[116]) | (bit[119]&bit[118]&bit[117]&bit[116]) | (bit[119]&bit[118]&~bit[117]&~bit[116]) | (bit[120]&bit[118]&bit[117]&bit[116]) | (bit[120]&bit[118]&~bit[117]&~bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[114]&((~bit[119]&~bit[118]&~bit[117]) | (~bit[117]&~bit[116]&bit[115]) | (bit[120]&bit[119]&bit[118]&bit[115]) | (~bit[119]&~bit[118]&bit[116]&bit[115]) | (~bit[119]&~bit[117]&bit[116]&~bit[115]) | (~bit[118]&~bit[117]&bit[116]&~bit[115]) | (bit[118]&bit[117]&~bit[116]&~bit[115]) | (~bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((bit[119]&bit[118]&~bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]) | (bit[120]&~bit[119]&bit[117]&bit[116]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]) | (~bit[120]&~bit[119]&~bit[118]&bit[116]) | (bit[120]&~bit[119]&~bit[117]&~bit[116]) | (~bit[119]&~bit[118]&~bit[117]&~bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[114]&((~bit[118]&~bit[116]&bit[115]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]) | (bit[119]&~bit[118]&~bit[117]&bit[115]) | (~bit[119]&bit[118]&~bit[116]&~bit[115]) | (bit[118]&bit[117]&~bit[116]&~bit[115]) | (~bit[120]&~bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (bit[119]&bit[118]&~bit[117]&bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[116]&bit[115]) | (~bit[120]&~bit[119]&bit[117]&bit[116]&bit[115]) | (bit[120]&bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&bit[119]&bit[117]&~bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((~bit[120]&bit[119]&~bit[118]) | (bit[119]&bit[118]&~bit[116]) | (bit[119]&~bit[117]&~bit[116]) | (bit[121]&~bit[120]&bit[119]&bit[117]) | (~bit[121]&~bit[120]&~bit[118]&bit[117]) | (bit[120]&bit[119]&bit[118]&~bit[117]) | (bit[119]&~bit[118]&bit[117]&bit[116]) | (~bit[119]&~bit[118]&bit[117]&~bit[116]) | (bit[120]&bit[118]&~bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[120]&~bit[119]&~bit[118]&~bit[117]&bit[116]) | (~bit[121]&bit[120]&bit[118]&bit[117]&bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[114]&((~bit[118]&~bit[116]&bit[115]) | (~bit[117]&~bit[116]&bit[115]) | (~bit[120]&bit[119]&~bit[118]&bit[115]) | (bit[120]&bit[118]&~bit[117]&bit[115]) | (~bit[119]&bit[118]&bit[117]&~bit[115]) | (bit[119]&bit[118]&~bit[117]&~bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]) | (bit[120]&bit[119]&bit[117]&bit[115]) | (~bit[119]&~bit[118]&~bit[117]&~bit[115]) | (~bit[118]&bit[117]&bit[116]&~bit[115]) | (~bit[120]&bit[118]&bit[117]&bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((~bit[120]&~bit[119]&~bit[117]) | (~bit[120]&~bit[117]&~bit[116]) | (~bit[120]&bit[119]&bit[117]&bit[116]) | (bit[120]&bit[119]&~bit[117]&bit[116]) | (bit[120]&~bit[118]&bit[117]&~bit[116]) | (~bit[119]&bit[118]&~bit[117]&~bit[116]) | (~bit[121]&bit[120]&~bit[119]&~bit[118]&bit[117]) | (~bit[120]&~bit[119]&~bit[116]) | (~bit[120]&bit[118]&~bit[116]) | (~bit[121]&~bit[120]&bit[118]&bit[117]) | (bit[121]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&~bit[118]&bit[117]&bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[114]&((~bit[119]&~bit[118]&~bit[116]) | (bit[119]&~bit[117]&~bit[115]) | (bit[120]&bit[119]&~bit[117]&bit[116]) | (~bit[119]&~bit[117]&~bit[116]&bit[115]) | (bit[119]&~bit[118]&bit[116]&~bit[115]) | (bit[119]&bit[118]&~bit[116]&~bit[115]) | (~bit[120]&~bit[119]&~bit[118]&~bit[117]&bit[115]) | (bit[120]&~bit[119]&bit[117]&bit[116]&bit[115]) | (~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]))) | ( bit[114]&((~bit[115]&((~bit[120]&~bit[118]&~bit[116]) | (bit[120]&bit[119]&bit[118]&bit[117]) | (~bit[121]&bit[119]&~bit[118]&bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]) | (~bit[121]&bit[120]&bit[118]&bit[116]) | (bit[121]&~bit[120]&~bit[119]&bit[117]&bit[116]) | (bit[119]&~bit[118]&~bit[116]) | (bit[120]&~bit[118]&~bit[117]&bit[116]))) /*| ( bit[115]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[114]&bit[115];
		//disable_update=disable_update|(~control);

		//---------------116-117--------------

		out[6]=( control & ( (~bit[116]&((~bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&~bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&~bit[118]&bit[117]))) | ( bit[116]&((~bit[117]&(~bit[120]&bit[119]&bit[118])) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[116]&((~bit[118]&~bit[117]) | (bit[119]&bit[118]&bit[117]) | (~bit[120]&~bit[119]&~bit[117]) | (bit[122]&bit[120]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[118]&bit[117]))) | ( bit[116]&((~bit[117]&((bit[120]&bit[119]&bit[118]) | (~bit[121]&~bit[120]&~bit[119]&~bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[116]&((~bit[120]&bit[119]&bit[118]) | (~bit[122]&~bit[121]&bit[119]&bit[118]) | (bit[122]&bit[120]&~bit[119]&bit[118]) | (bit[121]&bit[120]&~bit[119]&bit[118]) | (~bit[121]&bit[120]&bit[118]&~bit[117]))) | ( bit[116]&((~bit[117]&((~bit[121]&bit[120]&bit[119]) | (~bit[122]&bit[120]&bit[119]&bit[118]) | (~bit[120]&bit[119]&~bit[118]) | (bit[121]&~bit[119]&~bit[118]) | (bit[120]&~bit[119]&~bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[116]&((~bit[119]&~bit[118]) | (bit[120]&~bit[119]&~bit[117]) | (bit[121]&bit[120]&~bit[118]&bit[117]) | (bit[121]&bit[120]&bit[118]&~bit[117]) | (~bit[121]&~bit[120]&~bit[118]&~bit[117]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&~bit[119]) | (~bit[122]&bit[121]&bit[120]&bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]))) | ( bit[116]&((~bit[117]&((bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[123]&~bit[121]&~bit[120]&bit[118]) | (~bit[122]&~bit[121]&bit[119]&bit[118]) | (~bit[121]&~bit[120]&~bit[119]&bit[118]) | (~bit[121]&bit[120]&~bit[119]&~bit[118]) | (bit[121]&~bit[120]&~bit[119]&~bit[118]) | (~bit[122]&~bit[120]&~bit[119]&bit[118]) | (~bit[122]&bit[121]&bit[120]&~bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[116]&((bit[121]&bit[120]&bit[118]) | (~bit[122]&bit[121]&bit[118]&bit[117]) | (bit[122]&bit[120]&bit[118]&bit[117]) | (~bit[121]&~bit[120]&~bit[118]&bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]) | (bit[121]&~bit[120]&~bit[118]&~bit[117]) | (~bit[122]&bit[121]&bit[120]&bit[119]&bit[117]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&~bit[117]) | (~bit[121]&bit[120]&bit[119]&~bit[118]&~bit[117]) | (~bit[121]&~bit[120]&~bit[119]&bit[117]) | (~bit[120]&~bit[119]&~bit[118]&~bit[117]))) | ( bit[116]&((~bit[117]&((~bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[122]&bit[121]&~bit[120]&~bit[119]) | (~bit[121]&~bit[120]&bit[119]&~bit[118]) | (~bit[123]&~bit[122]&bit[121]&bit[119]) | (~bit[123]&~bit[122]&~bit[120]&bit[119]) | (bit[123]&~bit[121]&~bit[120]&bit[119]) | (bit[122]&~bit[121]&bit[120]&bit[118]) | (bit[122]&bit[120]&bit[119]&bit[118]) | (bit[122]&bit[121]&bit[120]&~bit[118]) | (~bit[122]&bit[121]&~bit[120]&~bit[118]) | (bit[121]&bit[120]&bit[119]&~bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[116]&((~bit[120]&bit[119]&bit[118]) | (bit[121]&bit[119]&~bit[117]) | (bit[122]&bit[121]&bit[119]&~bit[118]) | (bit[122]&bit[121]&~bit[120]&bit[117]) | (bit[121]&~bit[120]&~bit[118]&bit[117]) | (~bit[121]&bit[120]&~bit[119]&~bit[117]) | (~bit[121]&~bit[119]&~bit[118]&~bit[117]) | (~bit[122]&~bit[121]&~bit[120]&bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]))) | ( bit[116]&((~bit[117]&((bit[122]&bit[121]&~bit[119]) | (~bit[122]&~bit[121]&~bit[120]&~bit[119]) | (~bit[123]&~bit[121]&bit[119]&bit[118]) | (bit[121]&bit[120]&~bit[119]&bit[118]) | (bit[122]&bit[121]&~bit[120]&~bit[118]) | (~bit[122]&~bit[121]&~bit[120]&~bit[118]) | (bit[122]&bit[120]&~bit[119]&~bit[118]) | (bit[123]&~bit[122]&bit[121]&bit[119]&bit[118]) | (bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&bit[122]&bit[119]&bit[118]) | (bit[121]&~bit[120]&~bit[119]&~bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[116]&((~bit[122]&~bit[121]&bit[119]&bit[118]) | (bit[121]&~bit[120]&bit[119]&~bit[118]) | (bit[122]&bit[121]&bit[119]&bit[117]) | (bit[120]&bit[119]&bit[118]&bit[117]) | (~bit[121]&~bit[119]&~bit[118]&bit[117]) | (~bit[121]&~bit[120]&bit[118]&~bit[117]) | (~bit[121]&bit[120]&bit[119]) | (bit[122]&~bit[121]&~bit[119]&bit[117]) | (bit[121]&~bit[119]&~bit[118]&~bit[117]) | (bit[120]&~bit[119]&~bit[118]&~bit[117]))) | ( bit[116]&((~bit[117]&((bit[122]&~bit[121]&~bit[120]&bit[119]) | (bit[122]&~bit[121]&~bit[120]&bit[118]) | (~bit[122]&bit[120]&~bit[119]&bit[118]) | (~bit[122]&bit[120]&bit[119]&~bit[118]) | (bit[122]&bit[120]&~bit[119]&~bit[118]) | (~bit[122]&~bit[120]&~bit[119]&~bit[118]) | (~bit[123]&bit[122]&bit[120]&bit[119]&bit[118]) | (~bit[122]&bit[121]&~bit[120]&bit[119]&bit[118]) | (bit[123]&~bit[122]&bit[120]&bit[118]) | (~bit[121]&bit[120]&~bit[119]&~bit[118]) | (bit[123]&~bit[121]&bit[120]&bit[119]&bit[118]) | (bit[123]&bit[121]&~bit[120]&bit[119]&bit[118]))) /*| ( bit[117]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[116]&bit[117];
		//disable_update=disable_update|(~control);

		//---------------118-119--------------
		out[6]=( control & ( (~bit[118]&((~bit[123]&~bit[121]&bit[120]&bit[119]) | (~bit[122]&~bit[121]&bit[120]&bit[119]) | (bit[122]&bit[121]&~bit[120]&bit[119]))) | ( bit[118]&((~bit[119]&((~bit[125]&~bit[122]&bit[121]&bit[120]) | (~bit[124]&~bit[122]&bit[121]&bit[120]) | (~bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[118]&((bit[121]&bit[120]&bit[119]) | (~bit[121]&~bit[120]&~bit[119]) | (bit[123]&bit[122]&bit[120]&bit[119]) | (~bit[123]&~bit[122]&~bit[120]&~bit[119]))) | ( bit[118]&((~bit[119]&((~bit[121]&~bit[120]) | (bit[122]&bit[121]&bit[120]) | (~bit[123]&~bit[122]&~bit[120]) | (bit[125]&bit[124]&bit[123]&bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[118]&((~bit[122]&bit[121]&bit[120]) | (~bit[121]&bit[120]&~bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[123]&bit[121]&bit[120]&bit[119]) | (bit[122]&bit[121]&~bit[120]&~bit[119]) | (bit[123]&bit[121]&~bit[120]&~bit[119]) | (~bit[124]&~bit[123]&bit[122]&bit[121]&bit[119]) | (~bit[124]&bit[123]&bit[122]&bit[120]&bit[119]))) | ( bit[118]&((~bit[119]&((bit[122]&bit[121]) | (bit[123]&bit[121]&~bit[120]) | (bit[125]&bit[124]&bit[123]&bit[121]) | (~bit[124]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&~bit[122]&~bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[118]&((~bit[123]&~bit[122]&~bit[121]) | (bit[122]&bit[121]&~bit[119]) | (bit[124]&bit[122]&bit[121]&~bit[120]) | (bit[123]&bit[122]&bit[121]&~bit[120]) | (~bit[123]&~bit[122]&bit[120]&bit[119]) | (~bit[122]&~bit[121]&~bit[120]&bit[119]) | (~bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[123]&bit[121]&~bit[120]&~bit[119]) | (bit[124]&bit[123]&bit[122]&bit[120]&bit[119]) | (~bit[124]&bit[123]&~bit[121]&bit[120]&bit[119]))) | ( bit[118]&((~bit[119]&((~bit[123]&bit[122]&bit[121]) | (bit[123]&~bit[122]&~bit[120]) | (~bit[122]&~bit[121]&~bit[120]) | (bit[125]&bit[124]&bit[123]&~bit[122]) | (~bit[125]&~bit[123]&bit[121]&bit[120]) | (~bit[124]&~bit[123]&bit[121]&bit[120]) | (~bit[124]&~bit[123]&bit[122]) | (bit[124]&bit[123]&~bit[121]&bit[120]) | (bit[124]&bit[122]&~bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[118]&((~bit[123]&bit[122]&~bit[119]) | (bit[122]&bit[121]&~bit[119]) | (bit[123]&~bit[120]&~bit[119]) | (bit[123]&bit[122]&bit[121]&~bit[120]) | (~bit[123]&bit[122]&~bit[121]&~bit[120]) | (bit[124]&bit[123]&bit[120]&bit[119]) | (bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (bit[124]&bit[122]&bit[121]&~bit[120]))) | ( bit[118]&((~bit[119]&((~bit[124]&bit[123]&bit[122]) | (bit[123]&bit[121]&~bit[120]) | (~bit[124]&~bit[123]&~bit[122]&~bit[121]) | (bit[124]&bit[123]&~bit[121]&bit[120]) | (bit[125]&bit[124]&~bit[122]&bit[121]&bit[120]) | (bit[124]&~bit[123]&bit[122]&~bit[121]&~bit[120]) | (~bit[125]&~bit[124]&bit[121]&bit[120]) | (~bit[124]&bit[122]&~bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[118]&((~bit[123]&~bit[122]&~bit[120]) | (~bit[123]&bit[121]&~bit[119]) | (~bit[122]&~bit[120]&~bit[119]) | (~bit[124]&~bit[123]&bit[122]&bit[120]) | (~bit[124]&~bit[123]&~bit[121]&bit[119]) | (bit[122]&~bit[121]&~bit[120]&bit[119]) | (bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[124]&bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[122]&~bit[121]&bit[119]) | (bit[124]&~bit[123]&~bit[120]&bit[119]))) | ( bit[118]&((~bit[119]&((bit[124]&~bit[123]&bit[122]&~bit[121]) | (bit[125]&~bit[122]&bit[121]&bit[120]) | (bit[124]&bit[122]&~bit[121]&~bit[120]) | (~bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[125]&~bit[124]&bit[123]&bit[121]) | (~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[125]&bit[124]&bit[122]&bit[120]) | (~bit[124]&bit[123]&~bit[122]&~bit[120]) | (~bit[124]&bit[123]&bit[121]&~bit[120]) | (~bit[123]&bit[122]&bit[121]&~bit[120]) | (~bit[123]&~bit[122]&~bit[121]&~bit[120]) | (bit[124]&bit[123]&~bit[122]&~bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[118]&((bit[123]&~bit[122]&bit[121]&~bit[120]) | (bit[124]&~bit[123]&bit[122]&bit[119]) | (bit[122]&~bit[121]&~bit[120]&bit[119]) | (bit[123]&~bit[122]&bit[121]&~bit[119]) | (bit[123]&bit[121]&~bit[120]&~bit[119]) | (bit[124]&bit[122]&bit[121]&bit[120]&bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (~bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&~bit[121]&~bit[120]&~bit[119]) | (~bit[124]&~bit[123]&bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[123]&bit[122]&~bit[120]&bit[119]))) | ( bit[118]&((~bit[119]&((bit[124]&bit[123]&~bit[122]) | (bit[124]&~bit[122]&~bit[121]) | (bit[123]&bit[122]&~bit[120]) | (~bit[124]&bit[121]&~bit[120]) | (bit[125]&bit[124]&bit[123]&~bit[121]) | (bit[125]&bit[124]&~bit[123]&bit[121]&bit[120]) | (bit[125]&~bit[123]&bit[122]&bit[121]&bit[120]))) /*| ( bit[119]&(expr2))*/))  ) ) | (~control&out[0]);

		control=control&bit[118]&bit[119];
		disable_update=disable_update|(~control);



		//----------------120--------------

		out[6]=( control & ( (~bit[120]&((~bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[124]&~bit[123]&~bit[122]&bit[121]) | (~bit[126]&~bit[125]&bit[123]&~bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((bit[124]&~bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[124]&bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&(ZERO)) /*| ( bit[Y]&(expr2))*/ ))))  ) ) | (~control&out[6]);

		out[5]=( control & ( (~bit[120]&((~bit[124]&bit[122]&bit[121]) | (~bit[123]&bit[122]&bit[121]) | (~bit[124]&~bit[122]&~bit[121]) | (~bit[123]&~bit[122]&~bit[121]) | (~bit[126]&~bit[125]&bit[122]&bit[121]) | (bit[126]&bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((bit[126]&bit[123]&bit[122]) | (bit[125]&bit[123]&bit[122]) | (bit[124]&bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&(~bit[124]&~bit[123])) /*| ( bit[Y]&(expr2))*/ ))))  ) ) | (~control&out[5]);

		out[4]=( control & ( (~bit[120]&((~bit[123]&bit[122]&~bit[121]) | (~bit[126]&~bit[124]&~bit[123]&bit[122]) | (~bit[125]&~bit[124]&~bit[123]&bit[122]) | (bit[126]&bit[124]&bit[123]&bit[121]) | (bit[125]&bit[124]&bit[123]&bit[121]) | (bit[124]&bit[123]&~bit[122]&~bit[121]))) | ( bit[120]&((~bit[121]&((~bit[123]&~bit[122]) | (~bit[125]&bit[124]&bit[123]&bit[122]) | (bit[125]&~bit[124]&bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[124]&~bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[122]) | (bit[126]&~bit[124]&bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&((bit[124]&~bit[123]) | (~bit[126]&~bit[125]&~bit[124]&bit[123]))) /*| ( bit[Y]&(expr2))*/ ))))  ) ) | (~control&out[4]);

		out[3]=( control & ( (~bit[120]&((bit[125]&bit[124]&bit[121]) | (bit[124]&~bit[123]&bit[121]) | (~bit[124]&bit[122]&~bit[121]) | (~bit[125]&~bit[124]&~bit[123]&~bit[121]) | (~bit[125]&bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[123]) | (bit[124]&bit[123]&~bit[122]&~bit[121]) | (bit[126]&bit[125]&~bit[123]&bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((~bit[124]&~bit[123]&~bit[122]) | (bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[125]&bit[124]&bit[123]&bit[122]) | (~bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[125]&bit[124]&bit[123]&~bit[122]) | (bit[125]&~bit[124]&~bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]) | (~bit[127]&~bit[126]&bit[124]&~bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&((~bit[126]&~bit[125]&~bit[123]) | (~bit[125]&bit[124]&~bit[123]) | (bit[126]&~bit[125]&~bit[124]&bit[123]) | (~bit[127]&~bit[126]&bit[125]&~bit[124]&bit[123]) | (~bit[127]&bit[126]&~bit[125]&~bit[124]))) /*| ( bit[Y]&(expr2))*/))))  ) ) | (~control&out[3]);

		out[2]=( control & ( (~bit[120]&((bit[125]&bit[124]&~bit[121]) | (~bit[123]&~bit[122]&~bit[121]) | (~bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[125]&bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[125]&~bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[121]) | (~bit[125]&bit[124]&~bit[123]&bit[121]) | (~bit[124]&bit[123]&bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[123]&bit[122]&bit[121]) | (~bit[126]&~bit[125]&~bit[123]&bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((bit[125]&bit[124]&bit[123]) | (bit[126]&bit[125]&bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[123]&bit[122]) | (~bit[126]&bit[125]&bit[123]&~bit[122]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]) | (bit[126]&~bit[125]&~bit[124]&bit[123]&~bit[122]) | (bit[127]&~bit[126]&bit[124]&bit[122]) | (~bit[126]&bit[124]&bit[123]&bit[122]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]) | (~bit[127]&bit[126]&~bit[124]&bit[123]&bit[122]) | (~bit[127]&bit[126]&bit[124]&~bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&((~bit[126]&bit[124]&~bit[123]) | (~bit[127]&bit[126]&~bit[124]&bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[124]&~bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]) | (bit[127]&bit[125]&~bit[124]&bit[123]) | (~bit[127]&bit[125]&~bit[124]&~bit[123]) )) /*| ( bit[Y]&(expr2))*/)) ))  ) ) | (~control&out[2]);

		out[1]=( control & ( (~bit[120]&((~bit[126]&~bit[125]&~bit[123]&bit[121]) | (bit[125]&~bit[124]&~bit[123]&~bit[121]) | (bit[125]&bit[124]&bit[122]&~bit[121]) | (~bit[125]&~bit[124]&bit[122]&~bit[121]) | (~bit[125]&bit[124]&~bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[123]&bit[121]) | (bit[126]&~bit[124]&~bit[123]&bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&bit[121]) | (bit[125]&~bit[124]&bit[123]&bit[122]&bit[121]) | (~bit[126]&bit[125]&bit[124]&~bit[122]&bit[121]) | (bit[126]&~bit[125]&~bit[124]&~bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((bit[126]&~bit[125]&~bit[124]) | (bit[126]&~bit[124]&bit[123]) | (bit[126]&bit[125]&bit[124]&~bit[123]) | (~bit[126]&~bit[125]&bit[124]&~bit[123]) | (bit[126]&~bit[125]&bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[122]) | (~bit[126]&bit[125]&bit[124]&bit[123]) | (~bit[127]&~bit[124]&bit[123]&bit[122]) | (bit[127]&bit[124]&~bit[123]&bit[122]) | (bit[126]&bit[124]&~bit[123]&~bit[122]))) | ( bit[121]&((~bit[122]&((~bit[127]&~bit[126]&~bit[123]) | (~bit[127]&bit[124]&~bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[124]) | (bit[126]&bit[125]&bit[124]&~bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]&bit[123]) | (~bit[126]&~bit[125]&~bit[124]&~bit[123]))) /*| ( bit[Y]&(expr2))*/))))  ) ) | (~control&out[1]);

		out[0]=( control & ( (~bit[120]&((~bit[125]&~bit[123]&~bit[122]&bit[121]) | (bit[125]&bit[124]&~bit[123]&~bit[121]) | (~bit[124]&bit[123]&bit[122]&~bit[121]) | (bit[125]&~bit[123]&~bit[122]&~bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&bit[121]) | (bit[126]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (bit[126]&~bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&bit[124]&bit[123]&~bit[122]&~bit[121]) | (~bit[125]&~bit[124]&~bit[123]&bit[122]) | (bit[126]&~bit[125]&bit[124]&bit[121]) | (~bit[126]&bit[123]&bit[122]&bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[122]&bit[121]) | (bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]))) | ( bit[120]&((~bit[121]&((~bit[126]&~bit[123]&~bit[122]) | (bit[127]&~bit[126]&bit[125]&~bit[124]) | (bit[127]&bit[125]&bit[124]&~bit[123]) | (bit[126]&~bit[125]&~bit[124]&bit[122]) | (bit[126]&bit[124]&bit[123]&~bit[122]) | (~bit[127]&~bit[126]&bit[124]&bit[122]) | (~bit[127]&bit[126]&bit[123]&bit[122]) | (bit[125]&~bit[124]&bit[123]&~bit[122]) | (bit[125]&bit[124]&~bit[123]&~bit[122]) | (~bit[127]&bit[125]&~bit[124]&~bit[123]&bit[122]))) | ( bit[121]&((~bit[122]&((~bit[127]&~bit[126]&~bit[125]&~bit[124]) | (~bit[127]&~bit[126]&~bit[125]&bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[123]) | (~bit[127]&bit[126]&bit[125]&~bit[124]&bit[123]) | (~bit[127]&bit[126]&bit[125]&bit[124]&~bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]&~bit[123]))) /*| ( bit[Y]&(expr2))*/)) ))  ) ) | (~control&out[0]);



		//----------------123--------------


		for(k=0;k<8;k++){//if sample_o is 8 bits it should rotate 8 times if sample_o is 16 bit then the loop should rotate 16 times 	
				  //At a time 8 samples will be filled up. So we need the loop to iterate 8 times to fill all the 64 samples.
			
				sample_t=out[4]&mask;
				sample_t=(sample_t<<1) | (out[3]&mask);
				sample_t=(sample_t<<1) | (out[2]&mask);
				sample_t=(sample_t<<1) | (out[1]&mask);
				sample_t=(sample_t<<1) | (out[0]&mask);
				sample_n[k]=sample_t;
				
								

				out[4]=out[4]>>1;
				out[3]=out[3]>>1;
				out[2]=out[2]>>1;
				out[1]=out[1]>>1;
				out[0]=out[0]>>1;

		}

		for(k=0;k<64;k++){
			sample[k]=(int)sample_o[k];
		}	


		clock2 = cpucycles();

		clock3=clock3+(clock2-clock1);
		
		clock5=clock5+(clock2-clock4);


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

		

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

		for(k=0;k<64;k++){
			hist[sample[k]]++;
			printf("  %d  ,",sample[k]);
		}
		printf("\n %ld %lld\n",i,clock3);

		if(disable_update!=0xffffffffffffffff)//even if the sample is not found within depth 40
			break;
	}

	printf("Time excluding random numbers generation: %lld\n",clock3/repeat);
	printf("Time including random numbers generation: %lld\n",clock5/repeat);

//---------------------------print the histogram------------------------------

	for(i=0;i<84;i++)
		printf(" %ld : %llu",i,hist[i]);
//--------------------------print the histogram ends--------------------------

	printf("\n");
	return 1;
}








