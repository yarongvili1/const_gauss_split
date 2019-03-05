
//-----if you need to sample 128 bits for each sample. Not needed in KY sampler
#define IP_BIT 128
#define KECCAK_ROUND 6 //floor(IP_BIT*64/(KECCAK_OP_BIT*8))

//#define IP_BIT 117
//#define KECCAK_ROUND 5 

#define HIST_LIMIT 32
#define NUM_OP_BITS 5 // number of bits in output sample


#define ZERO 0x0000000000000000 
#define ONE 0xffffffffffffffff
