#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <mpfr.h>

//compile gcc sample.c -o sample -lmpfr -lgmp

//#define TAU 12
//#define LIM 215 //LIM = floor(sigma/(sqrt(1+k^2)))

#define TAU 9
#define LIM 3 //for Falcon only. TAU*LIM should be 27 as according to the reference implementation.


//#define SIGMA 215.73
//#define SIGMA 8
#define SIGMA 2
#define PRECISION 128


int size_ham[2][PRECISION];//0-> size, 1->hamming weight
//int table[TAU*SIGMA+1][PRECISION]; //the top rows are used to store the hamming distance and size
int table[TAU*LIM+1][PRECISION]; //tables to store the probability distribution
unsigned long long int histogram[TAU*LIM+1];
unsigned long long int col_histogram[PRECISION];
unsigned long long int col_sample_histogram[PRECISION][TAU*LIM+1];
unsigned long int NO_SAMPLE;


void create_binary(mpfr_t a, int prec,int row){

	unsigned int i = 0;
	mpfr_t v;
	mpfr_init2(v, 128);
	mpfr_set(v,a,MPFR_RNDD);
	int j = 0;//set j to trim some of the MSBs
	for (i = 0; i < prec+j; i++){
		mpfr_mul_ui(v,v,2,MPFR_RNDD);
		if (mpfr_cmp_ui(v, 1) >= 0){
			//printf("1");
			if (i>=j)
				table[row][i-j] = 1;
			mpfr_sub_ui(v,v,1,MPFR_RNDD);
		}
		else{
			//printf("0");
			if (i >= j)
				table[row][i-j] = 0;
		}
	}
	//printf("\n");
	mpfr_clear(v);
}


void to_binary(long int sample,int bit_num){

	long int i,k;
	i=1;
	for(k=bit_num-1;k>=0;k--){
		printf("%ld",(sample&(i<<k))>>k);
	}

}

/*calculater the total samples, bits per sample and overflow beforehand*/
void run_sampling_d_lsb_sorted_espresso(){// for sorted samples
	

	long int i, j, k,l,m,limiter;
	unsigned long int d;
	
	long int hit, col;
	long int b, row, sample;
	long int repeat = 1;
	unsigned long int sample_counter=0;	

	int total_num_sample=1149; //sum of hamming weights of all columns
	int bits_each_sample=5;

	//-------------------------------
	//int sample_bit[total_num_sample][bits_each_sample];//this will change 1st one number of samples second one number of bits of each sample
	//int sample_array[total_num_sample][PRECISION];//this will change 1st one number of samples second one the array of random bits
	//int sorting_done[total_num_sample];//this will change too
	int *sample_bit[total_num_sample], *sample_array[total_num_sample];
	int *sorting_done;
	for(i=0;i<total_num_sample;i++){
		sample_bit[i]= (int *)malloc(bits_each_sample*sizeof(int));
		sample_array[i]= (int *)malloc(PRECISION*sizeof(int));		
	}
	sorting_done= (int *)malloc(total_num_sample*sizeof(int));
	//-------------------------------	

	for(i=0;i<total_num_sample;i++)
		sorting_done[i]=-1;

	time_t t;
	//int OVERFLOW=2*1176641;
	int OVERFLOW=2*28;


	int temp[32];
	//unsigned int arr[OVERFLOW][PRECISION],d_arr[OVERFLOW];
	//int valid_invalid[OVERFLOW];//highest possible overflow is less than 50 these should change too. number of samples
	//----------------
	unsigned int **arr,*d_arr;
	int *valid_invalid;

	arr= (unsigned int **)malloc(sizeof(unsigned int *) * OVERFLOW);
	arr[0]=(unsigned int *)malloc(OVERFLOW*PRECISION*sizeof(unsigned int));

	for(i=0;i<OVERFLOW;i++)
		arr[i]= (*arr+PRECISION*i);

	d_arr= (unsigned int *)malloc(OVERFLOW*sizeof(unsigned int));
	valid_invalid= (int *)malloc(OVERFLOW*sizeof(int));
	//----------------

	for(j=0;j<OVERFLOW;j++)
		d_arr[j]=0;		
		
	for(j=2;j<OVERFLOW;j++){//make all the entries invalid initially
		valid_invalid[j]=-1;	
	}
	valid_invalid[0]=1;//make the first two entries valid and appropriate bits
	valid_invalid[1]=1;
	arr[0][0]=0;arr[1][0]=1;
	d_arr[0]=0;d_arr[1]=1;

//-----------------for espresso----------------

	printf(".i %d\n",PRECISION);
	printf(".o %d\n",bits_each_sample);
	printf(".ilb ");
	for(i=PRECISION-1;i>=0;i--){
		printf(" bit[%ld] ",i);	
	}
	printf("\n.ob ");
	
	for(i=bits_each_sample-1;i>=0;i--){
		printf(" out_t[%ld] ",i);	
	}
	printf("\n");
//-----------------for espresso ends----------------

	for(i=1;i<PRECISION;i++){//scan column by column
		//printf("%d\n",i);
		//find valid bits
		for(j=0;j<OVERFLOW;j++){
				
			if(valid_invalid[j]==1){//found one valid entry append 0 and 1 then update d

				arr[j][i]=0; //append 0
				d_arr[j]=d_arr[j]*2;//multiply by 2
				//find another an empty space or invalid entry to append 1
				for(k=0;k<OVERFLOW;k++){
					if(valid_invalid[k]==-1){//found one
						valid_invalid[k]=2; //make that valid newly created
						d_arr[k]=d_arr[j]+1;//adjust d to reflect append 1
						for(l=0;l<i;l++){//to append 1
							arr[k][l]=arr[j][l];
						}//end for
						arr[k][i]=1;
						break;
					}
				}//end for
			}//end if			
		}//end inner for
		for(j=0;j<OVERFLOW;j++){//adjust the valid_invalid array
			if(valid_invalid[j]==2)
				valid_invalid[j]=1;
			/*if(valid_invalid[j]==1){
				for(l=0;l<=i;l++)
						printf(" %d ",arr[j][l]);
				printf(" d : %d\n",d_arr[j]);
			}*/
		}
		
		//logic to scan the columns for sample
		for(j=0;j<OVERFLOW;j++){
			if(valid_invalid[j]==1){
				/*for(l=0;l<=i;l++)
					printf(" %d ",arr[j][l]);
					printf(" d : %d\n",d_arr[j]);
				*/
				if(d_arr[j]>=size_ham[1][i]){ //like normal sampling
					//printf("\nInside if\n");
					d_arr[j]=d_arr[j]-size_ham[1][i];
				}
				else{
					//printf("\nInside else\n");
					for (row = size_ham[0][i]; row >= 0; row--){
						d_arr[j] = d_arr[j] - table[row][i];
						if (d_arr[j] == -1){
							//printf("\n-------------------\n");
							sample = row;
							//printf("Sample : %ld\n",sample);
							valid_invalid[j]=-1;
							//for(k=0;k<=i;k++)
							for(k=i+1;k<PRECISION;k++){
								//printf("-");
								sample_array[sample_counter][k]=-1;
							}
							for(k=i;k>=0;k--){
								//printf("%d",arr[j][k]);
								sample_array[sample_counter][k]=arr[j][k];
							}
							//printf(" | ");
							//printf(" ");
							//-------------print the binary----------------------
							//to_binary(sample,7);
							l=1;
							for(k=bits_each_sample-1;k>=0;k--){ //*****change for bit number change
								//printf("%ld",(sample&(l<<k))>>k);
								sample_bit[sample_counter][k]=(sample&(l<<k))>>k;
							}							  
							//-------------print the binary end------------------
							//printf(" %lu\n",sample_counter);
							sample_counter++;
							break;
						}//end if

					}//end for
				}//end else
			}//end if
			
		}//end for

	}//end outer for	

//----------------print and sort the samples------------------
/*
	for(k=0;k<sample_counter;k++){
		for(j=PRECISION-1;j>=0;j--){
			if(sample_array[k][j]==-1)
				printf("-");
			else
				printf("%d",sample_array[k][j]);
		}
		printf("  ");
		for(j=7-1;j>=0;j--)
			printf("%d",sample_bit[k][j]);		
		printf("\n");
	}
*/
	for(k=0;k<PRECISION;k++){ //iterate over possible list of 1 trail
		for(j=0;j<sample_counter;j++){//iterate over all the samples
			l=0;
			if(sorting_done[j]==-1){//start if
				for(m=PRECISION-1;m>=0;m--){//start for
					if(sample_array[j][m]==0)
						l=m-1;
				}//end for
				//printf(" %ld \n",l);
				if(l==k-1){
					sorting_done[j]=1;
						for(m=PRECISION-1;m>=0;m--){//start for
							if(sample_array[j][m]==-1)
								printf("-");
							else
								printf("%d",sample_array[j][m]);
						}//end for
						//printf(": out =  ");
						//printf("%d'b",bits_each_sample);
						printf(" ");
						for(m=bits_each_sample-1;m>=0;m--)
							printf("%d",sample_bit[j][m]);
						//printf("   | %ld ",l+1);
						printf("\n");

				}
				
			}//end if
		}
	}

//----------------print and sort the samples ends------------------

}



void create_table()
{
	unsigned long int i,j;
	int k;
	mpfr_t s, t, u, e,pi,sigma_large,prob_sum;
	int precision = 250;
	
	int sigma = 1*SIGMA;

	mpfr_init2(s, precision);
	mpfr_init2(t, precision);
	mpfr_init2(u, precision);
	mpfr_init2(e, precision);
	mpfr_init2(pi, precision);


	mpfr_init2(sigma_large, precision);
	mpfr_init2(prob_sum, precision);

	mpfr_set_si(sigma_large,sigma,MPFR_RNDD); //set the large sigma
	
	mpfr_const_pi(pi, MPFR_RNDD);
	mpfr_mul_ui(pi, pi,2,MPFR_RNDD);
	mpfr_sqrt(pi, pi, MPFR_RNDD);
	
	mpfr_mul(pi,pi,sigma_large,MPFR_RNDD); //pi<- (sigma*sqrt(2*pi))	
	mpfr_set(s,sigma_large,MPFR_RNDD); //s<-sigma

	mpfr_sqr(s,s,MPFR_RNDD);		//s<-sigma^2
	mpfr_mul_ui(s, s, 2,MPFR_RNDD);	//s<- 2*sigma^2
	mpfr_neg(s, s, MPFR_RNDD);	//s<- -2*sigma^2
	mpfr_ui_div(s,1,s,MPFR_RNDD);	//s<- -1/2*sigma^2

	mpfr_set_d(prob_sum,0,MPFR_RNDD);

	for (i = 0; i <= TAU*LIM; i++){
		j = i*i;
		mpfr_mul_ui(e, s,j, MPFR_RNDD);//-i^2/(2*sigma^2)
		mpfr_exp(e, e, MPFR_RNDD);//e <- e^m
		//mpfr_add_ui(e, e, 1, MPFR_RNDD); //e<- e^m
		mpfr_div(e,e,pi,MPFR_RNDD);//e<-e^m/(sigma*sqrt(2*pi))
		if(i!=0){
			mpfr_mul_ui(e, e, 2,MPFR_RNDD);
		}
		mpfr_add(prob_sum,prob_sum,e,MPFR_RNDD);//add the probabilities	
		create_binary(e,128,i);
		//mpfr_printf(" i %d ||| %.150RNf \n",i,e);
		//mpfr_printf(" %d Total probability sum ||| %.250RNf \n",i,prob_sum);
	}

		//mpfr_printf(" Total probability sum ||| %.250RNf \n",prob_sum);
		//printf("precision of prob_sum %d\n",mpfr_get_prec(prob_sum));
		
	
	for (j = 0; j < PRECISION; j++){
		size_ham[0][j] = 0;
		size_ham[1][j] = 0;
	}
	
	for (i = 0; i<TAU*LIM+1 ; i++){
		for (j = 0; j < PRECISION; j++){
			if (table[i][j] == 1){
				size_ham[1][j]++;
				size_ham[0][j] = i;
			}
		}

	}

/*

	for (j = 0; j < PRECISION; j++){
		printf("%d, ", size_ham[0][j]);
	
	}
	printf("\n");
	for (j = 0; j < PRECISION; j++){
		printf("%d, ", size_ham[1][j]);

	}
	printf("\n");
	
	
	for (k = 0; k<=TAU*LIM; k++){
		for (j = 0; j < PRECISION; j++){
			printf("%d", table[k][j]);
		}
		printf("\n");
	}	

*/	

	/*
	for (j = 0; j < PRECISION; j++){
		printf("size_ham[0][%ld]=%d; ", j, size_ham[0][j]);
	
	}
	printf("\n");
	for (j = 0; j < PRECISION; j++){
		printf("size_ham[1][%ld]=%d; ", j, size_ham[1][j]);

	}
	printf("\n");
	
	
	for (k = 0; k<=TAU*LIM; k++){
		for (j = 0; j < PRECISION; j++){
			printf("ky_table[%d][%ld]=%d; ", k, j, table[k][j]);
		}
		printf("\n");
	}
	*/

	mpfr_clear(s);
	mpfr_clear(t);
	mpfr_clear(u);
	mpfr_clear(e);
	mpfr_clear(pi);
	mpfr_clear(sigma_large);
	mpfr_clear(prob_sum);

	return;
}

void main(){

	unsigned long int i,j;
	int k;
	unsigned long long int ham_sum,size_sum;
	int bit_num;
	int overflow;

	for(j=0;j<TAU*LIM+1;j++)
		histogram[j]=0;
	for(j=0;j<PRECISION;j++)
		col_histogram[j]=0;

	for(i=0;i<TAU*LIM+1;i++){	
		for(j=0;j<PRECISION;j++)
			col_sample_histogram[j][i]=0;
		}


	NO_SAMPLE=0;
	create_table();
	
	unsigned long long clock1, clock2;


//------------------------------------------------------------driver for different sampling routines--------------------------------------------
	//clock1 = cpucycles();
	bit_num=9;
	//for(i=0;i<(1<<bit_num);i++){
		//printf("||||||||||||||||||||||||||||||||||||||\n");
		//printf("i: %lu\n",i);
		//run_sampling_d(i,bit_num);
		//run_sampling_8bit_test(i,8);
		//printf("||||||||||||||||||||||||||||||||||||||\n");
	//}

	//run_sampling_d_lsb();
	//run_sampling_d_lsb_sorted();
	//run_sampling_d_lsb_truthtable();//for synopsys
	//run_sampling_d_lsb_sorted();
	//run_sampling_d_lsb_sorted_synopsys();//for synopsys sorted
	run_sampling_d_lsb_sorted_espresso();//for espresso sorted
	//run_sampling_d_lsb_bysample();
/*
	for(i=0;i<82;i++){	
		printf("lsb[%lu] = ",i);
		run_sampling_d_lsb_bysample_truthtable(i);	
		printf("\n");
	}
*/
	//run_sampling_8bit();
	//run_sampling(); //run sampling with no limit on bits i.e until produces a sample
	
	//clock2 = cpucycles();
	//printf("Time taken is : %lld\n", (clock2 - clock1)/10000);
//------------------------------------------------------------driver for different sampling routines end--------------------------------------------

	//printf("No sample is : %lu \n",NO_SAMPLE);


//------------------------------------------------------------loops for different calculations------------------------------------------------------
	/*
	ham_sum=0;
	size_sum=0;


	
	for(j=0;j<PRECISION;j++){
		printf("j : %d :: %llu, ",j,size_ham[1][j]);
	}
	
	printf("\n");

	ham_sum=1;
	for(j=0;j<PRECISION;j++){
		ham_sum=2*ham_sum-size_ham[1][j];
		printf("j : %d :: %llu, ",j,ham_sum);
	}
	
	printf("\n");
	*/
	/*
	
	for(j=0;j<TAU*LIM+1;j++)
		printf("j : %d :: %llu\n",j,histogram[j]);
	printf("------Column Histogram-----\n");
	for(j=0;j<PRECISION;j++)
		printf("j : %d :: %llu\n",j,col_histogram[j]);

	for(i=0;i<PRECISION;i++){	
		for(j=0;j<TAU*LIM+1;j++)
			if(col_sample_histogram[i][j]!=0)
				printf("Column : %d Sample : %d Number : %llu\n",i,j,col_sample_histogram[i][j]);
		}

		
	*/

//------------------------------------------------------------loops for different calculations end------------------------------------------------------

//------------------------------------------------------------printing routines-------------------------------------------------------------------------

	/*		
	for (j = 0; j < PRECISION; j++){
		printf("%d  ",size_ham[0][j]);
	
	}
	printf("\n");
	for (j = 0; j < PRECISION; j++){
		printf("%d  ",size_ham[1][j]);


	}
	*/
	/*
	printf("\n");
	
	
	for (k = 0; k<=TAU*LIM; k++){
		for (j = 0; j < PRECISION; j++){
			printf("%d", table[k][j]);
		}
		printf("\n");

	}
	
	
		
	ham_sum=0;
	for (j = 0; j < PRECISION; j++){

		ham_sum=ham_sum+size_ham[1][j];
		size_sum=size_sum+size_ham[0][j];
	}
		printf("i:%d\tSum Hamming weight : %llu\tSum size value :%llu \n ",j,ham_sum,size_sum);
	
	*/
	/*	
	ham_sum=0;
	size_sum=0;
	overflow=1;
	for (j = 0; j < PRECISION; j++){

		//ham_sum=2*ham_sum+size_ham[1][j];
		//size_sum=2*size_sum+1;
		overflow=2*overflow-size_ham[1][j];
		printf("i:%d\toverflow : %d\n ",j,overflow);

	}
	
	*/
	
//------------------------------------------------------------printing routines end -------------------------------------------------------------------------

}
