#include "matriz.h"

int main()
{
	matriz a;
	inserir_matriz(&a);
	exibir_matriz(a);
	printf("%d",igual(a,identidade(a.lin)));
	return 0;
}