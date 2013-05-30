#include <stdio.h>
#include <stdlib.h>
#include "mtwist.c"
#include "utils.c"
#define MAX_PACKET 500
#define MAX_TO_SEND 3

int main (int argc, char *argv[]) {

	int i,j;
	uint32_t timeSeed;
	uint32_t seed;
	mt_state state;
	int blocks; // Nombre de bloc total du fichier.
	int packets[MAX_PACKET];
	double rand;
	int degree;
	char packet[256];
	
	/**** Initialisation ****/
	state = mt_default_state;
	timeSeed = mt_goodseed();
	blocks=4; //to remove
	// blocks = getBlock(argv[0]);
	
	double distribute[blocks+1];//store the ideal soliton distribution
	int xoredBlocks[blocks];
	mt_seed32new(timeSeed); // On initialise avec baseSeed
	seed = mt_lrand();
	seed = seed %16;
	seed = seed <<4;
	sortTable((double)blocks, distribute);	
	printf("New seed : %i\n",seed);
	


	/***** Encode ******/
	for (i = 0; i< MAX_TO_SEND;i++) {
		printf("Packet %d :\n",i);
		/*** Load new seed ***/
		mt_seed32new(seed);

		/*** Random numbers generation ***/
		rand = mt_drand();
		//printf("rand : %f\n",rand);
		degree = getDeg(rand,distribute,blocks);
		printf("Degree : %d\n",degree);
		for(j=0;j<degree;j++){
			xoredBlocks[j]=mt_lrand();
			xoredBlocks[j+1]=-1;
		}
		getXored(xoredBlocks,degree+1,blocks);	
		printf("Xored Blocks : ");
		printf("b%d",xoredBlocks[0]);
		for (j=1;j<degree+1;j++) {
			printf(", b%d",xoredBlocks[j]);
		}
		printf("\n");
		
		/*** XOR ***/

		/*** UDP packet building ***/

		/*** Seed update ***/
		seed ++;
	}
}
