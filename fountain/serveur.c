#include <stdio.h>
#include <stdlib.h>
#include "mtwist.c"
#include "utilsServer.c"
#define MAX_PACKET 500
#define MAX_TO_SEND 3

int main(int argc, char *argv[]) {
	int size;
	struct datagram *packet;
	int initialised;
	set blocks=NULL;//contains all the blocks for each packet

	/*** Init ***/
	initilised=0;
	
	/*** UDP receiver***/
	for (;;) {
		/*** upon receive***/
		if (initilised) {
			
		} else { 
			initialised=1;
		}
	}
}
