#include "matriz.h"

int main(int argc,char *argv[])
{
	int tam;
	sscanf(argv[1],"%d",&tam);
	srand(time(NULL));
	matriz A = aleatoria(tam,tam,1,9);
	matriz B = aleatoria(tam,tam,1,9);
	matriz C = transpor_produto(A,B);
	free(A.M);
	free(B.M);
	free(C.M);
	return 0;
}
