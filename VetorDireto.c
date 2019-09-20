#include "matriz_vetor.h"

int main(int argc,char *argv[])
{
	srand(time(NULL));
	int tam;
	sscanf(argv[1],"%d",&tam);
	matriz A = aleatoria(tam,tam,1,9);
	matriz B = aleatoria(tam,tam,1,9);
	matriz C = produto(A,B);
	free(A.M);
	free(B.M);
	free(C.M);
}
