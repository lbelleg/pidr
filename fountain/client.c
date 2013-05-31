#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "mtwist.c"
#include "utils.c"
#include <string.h>
#define MAX_PACKET 500
#define MAX_TO_SEND 3
#define PACKET_SIZE 256

int main (int argc, char *argv[]) {

	int i,j;
	int fd;
	uint32_t timeSeed;
	uint32_t seed;
	mt_state state;
	int blocks; // Nombre de bloc total du fichier.
	int packets[MAX_PACKET];
	double rand;
	int degree;
	char * filename;
	char t [64]={0};
	char packet[PACKET_SIZE];
	const char *filemap;
	
	/**** Initialisation ****/
	state = mt_default_state;
	timeSeed = mt_goodseed();
	/*** block count ***/
	if (argc>1) {
	  filename = argv[1];
	} else {
	  printf("usage : the first arg has to be a filename\n");
	  exit(0);
	}
	 struct stat stt;
	 stat(filename,&stt);
	 printf("%d\n",(int)stt.st_size);
	 blocks = (int) stt.st_size/PACKET_SIZE;       
	/*** File mapping ***/
	 fd = open(filename, O_RDONLY);
	filemap = mmap(NULL, stt.st_size,PROT_READ,MAP_SHARED,fd,0);

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

		sprintf(t,"%064d",seed);
		strcpy(packet,t);
		sprintf(t,"%064d",blocks);
		strcat(packet,t);
		
		printf("packet : %s \n",packet);
		/*** filename ***/
		char name[500] ;
		sprintf(t,"%d",i);
		strcpy(name,"packet");
		strcat(name,t);
		strcat(name,".yay");
		//printf("%s\n",name);
		
		/*** filling ***/
		FILE *fp = fopen (name,"wb");
		fwrite(packet, 1, sizeof(packet), fp);
		fclose(fp);
		/*** Seed update ***/
		seed ++;
	}
}
