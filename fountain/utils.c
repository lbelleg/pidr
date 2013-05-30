
void sortTable(double k, double tab[]) {
	int i;
	tab[0] = 0;
	tab[1] = 1/ k;
	printf("k : %f, %f\n",k,tab[1]);
	for (i=2;i<(int)k;i++) {
		tab[i]=1/(double)(i*(i-1));
		tab[i]=tab[i]+tab[i-1];
		printf("tab[%d] : %f\n",i,tab[i]);
	}
	tab[(int)k]=1.;
	printf("tab[%d] : %f\n",(int)k,tab[(int)k]);
}

/** Algo "naïf" **/
int getDeg(double rand, double tab[], int size) {
	int i;
	for (i=0;i<=size+1;i++) {
		if(rand < tab[i]) {
			//printf("tab[%d] : %f\n",i,tab[i]);
			return i;
			}
	}
	printf("Woops! Error in the degree calculation!\n");
	exit(0);
}

int search (int tab[], int l, int val) {
	int i;
	for (i=0;i<=l;i++) {
		if (tab[i]==val)
			return i;
	}
	return -1;
}

int removeVal (int tab[],int l, int index) {
	int i;
	for (i=index;i<l;i++) {
		tab[i]=tab[i+1];
	}
	tab[l]=-1;
}

void getXored(int tab[],int l,int nbBloc) {
	int i, temp;
	int available[nbBloc];
	/*** Init available***/
	for (i=0;i<nbBloc;i++) {
		available[i]=i+1;
	}

	for (i=0;i<l-1;i++) {
		temp=tab[i]%(nbBloc-i);
		temp=(temp+(nbBloc-i))%(nbBloc-i);
		tab[i]=available[temp];
		if (removeVal(available,l-i,temp)==-1)//remove available[temp] from temp
			printf("Error in utils.c -> remove\n");
	}
}

int xor (char *a, char* b) {

}
