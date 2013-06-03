

struct datagram {
	int size;//sur 64 bits
	unsigned char xored[512];
	int seed;//64  bits
	int crc; //128 bits
};

typedef struct link link;

struct link {
	int bloc;//change head type
	struct link *tail;
};

typedef link *set;

int decode (int size) {
	int decoded[size];//if bloc[i] is decoded, decoded[i]=0 else 1
	int rcv=0; // number of bloc successfully decoded
	set blocks=NULL;
	int i;

	/*** Init ***/
	for (i=0;i<size;i++) {
		decoded[i]=0;
	}
	
	
}

/*** fill the datagram struct with the msg received***/
int fillwith(struct datagram, char *rcv) {
	char size[64];
	char seed[64];
	char xored[512];
	char crc[128];
	int i;

	for (i=0; i<64;i++) {
		size[i]=*rcv+i;
	}

	for (i=0; i<64; i++}
		seed[i]=*rcv+64+i;
	}		
}
