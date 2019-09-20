#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Definição da estrutura de uma matriz com alocação dinâmica.
typedef struct Matriz
{
	double *M;
	int col;
	int lin;
	int tam;
}matriz;

//Assinaturas das funções.
matriz submatriz(matriz A,int l, int c);
void exibir_matriz(matriz A);
double determinante(matriz A);
double sub_det(matriz A);
void tamanho(matriz A);
matriz produto(matriz A,matriz B);
matriz transposta(matriz A);
matriz transpor_produto(matriz a,matriz b);
void transpor(matriz *A);
void alocar(matriz *A);
matriz identidade(int ordem);
int igual(matriz a, matriz b);
matriz trianguloPascal(int ordem);
void copiar(matriz original, matriz *copia);
matriz aleatoria(int lin,int col,int min,int max);
void limpar(matriz *a);

//Função básica para receber uma matriz a partir da entrada do teclado.
void inserir_matriz(matriz *A)
{
	printf("Inserir a quantidade de linhas: ");
	scanf("%d",&A->lin);
	printf("Inserir a quantidade de colunas: ");
	scanf("%d",&A->col);
	A->tam = A->lin*A->col;
	alocar(A);//Como alocação dinâmica será muito recorrente, fiz uma subrotina específica.
	for(int i=0;i<A->lin;i++)
	{
		for(int j=0;j<A->col;j++)
		{
			printf("Elemento:[%d][%d]: ",i+1,j+1);
			scanf("%lf",&A->M[i*A->col+j]);
		}
	}
}

//Exibição padrão para uma matriz retangular
void exibir_matriz(matriz A)
{
	printf("\n|");
	for(int i=0;i<A.tam;i++)
	{
		if(i%A.col) printf("\t");
		else if(i>0)	printf("|\n|");//Espaço superior
		printf("%g",A.M[i]);
	}
	printf("|\n");//espaço inferior
}

//Determinante por submatriz, por laplace
double determinante(matriz A)
{
	double det=0;
	if(A.lin==A.col)
	{
		if (A.lin==2)
		{
			det =  A.M[0]*A.M[3]-A.M[1]*A.M[2];//Determinante 2x2
		}
		else//3x3 ou maior
		{
			//Essa função é recursiva, cada
			//matriz alocada é liberada após seu uso total.
			for(int j=0;j<A.col;j++)
				det += A.M[j]*sub_det(submatriz(A,0,j));
		}
	}
	else	printf("Não é matriz quadrada.");	
	return det;
}

/* Se l ou c estiverem fora do escopo do tamanho da matriz,
 * como negativos ou superior ao tamanho, nada será excluido da matriz.
 * */

matriz submatriz(matriz A,int l, int c)
{
	matriz r;
	if(l>=0 && l<A.lin)	r.lin = A.lin-1;
	else			r.lin = A.lin;
	if(c>=0 && c<A.col)	r.col = A.col-1;
	else 			r.col = A.col;
	r.M = (double *)malloc(r.col*r.lin*sizeof(double));
	int ri=0,rj=0,i,j;
	i = l>=A.lin-1?0:l+1;//Se for a última linha ou maior, começa do 0
	j = c>=A.col-1?0:c+1;//o mesmo para as colunas
	do
	{

		r.M[ri*r.col+rj]=A.M[i*A.col+j];
		rj++;
		j++;
		if(rj==r.col && ri<r.lin) 
		{
			rj=0;
			ri++;
		}
		if(j==A.col)	j=0;
		if(j==c) 
		{
			j++;
			i++;
		}
	}while(rj<r.col);
	return r;
}

double sub_det(matriz A)
{
	double r = determinante(A);
	free(A.M);
	return r;
}

void tamanho(matriz A){	printf("\n%d por %d\n",A.lin,A.col);}

matriz produto(matriz A, matriz B)
{
	if (A.col==B.lin)
	{
		matriz r;
		r.col = B.col;
		r.lin = A.lin;
		r.tam = r.col*r.lin;
		r.M = (double*)calloc(r.tam,sizeof(double));
		for (int c=0;c<A.lin;c++)
		{
			for (int l=0;l<B.col;l++)
			{
				for (int k=0;k<A.col;k++)
				{
					r.M[c*r.col+l]+=A.M[c*A.col+k]*B.M[k*B.col+l];
				}
			}
		}
		return r;
	}
	else	printf("Dimensões incompatíveis para operar produto.");
}

matriz transposta(matriz A)
{
	matriz r;
	r.lin=A.col;
	r.col=A.lin;
	alocar(&r);
	for(int i=0;i<A.lin;i++)
	{
		for(int j=0;j<A.col;j++)
		{
			r.M[j*A.lin+i]=A.M[i*A.col+j];
		}
	}
	return r;
}

void transpor(matriz *A)
{
	int aux,i,j;
	for(i=0;i<A->lin;i++)
	{
		for(j=i+1;j<A->col;j++)
		{
			aux = A->M[i*A->col+j];
			A->M[i*A->col+j] = A->M[j*A->col+i];
			A->M[j*A->col+i] = aux;
		}
	}	
}

matriz transpor_produto(matriz A,matriz B)
{
	if (A.col==B.lin)
	{
		matriz t,r;
		t = transposta(B);
		r.col = B.col;
		r.lin = A.lin;
		r.tam=r.lin*r.col;
		r.M = (double *)calloc(r.tam,sizeof(double));
		for (int c=0;c<A.lin;c++)
		{
			for (int l=0;l<t.col;l++)
			{
				for (int k=0;k<A.col;k++)
					r.M[c*r.col+l]+=A.M[c*A.col+k]*t.M[l*t.col+k];
			}
		}
		free(t.M);
		return r;
	}
	else
	{
		printf("Dimensões incompatíveis.\n");
		return A;
	}
}

void alocar(matriz *A)
{
	A->tam = A->lin*A->col;
	A->M = (double *)calloc(A->tam,sizeof(double*));
}

matriz identidade(int ordem)
{
	matriz r;
	r.lin=ordem;
	r.col=ordem;
	alocar(&r);
	for (int i=0;i<r.tam;i++)
		r.M[i*(r.col+1)]=1;
	return r;
}

int igual(matriz a, matriz b)
{
	int r=1;
	if(a.lin==b.lin && a.col==b.col)
	{
		for (int i=0;i<a.tam;i++)
		{
			if (!r) break;
			if(a.M[i]!=b.M[i])
			{
				r=0;
				break;
			}
		}
	}
	else
	{
		r = 0;
	}	
	return r;
}

matriz trianguloPascal(int ordem)
{	
	matriz tri;
	tri.tam=0;
	for (int i=1;i<=ordem;i++)
		tri.tam+=i;
	tri.M = (double *)calloc(tri.tam,sizeof(double));
	int colunas;
	int pos=0;
	for(int i=0;i<tri.tam;i++)
	{
		colunas=i+1;
		printf("\n");
		for(int j=0;j<colunas;j++)
		{
			if (j==0 || j == i) tri.M[pos++]=1;
			else if (j==1 || j == i-1) tri.M[pos++]=i;
			else tri.M[pos++] = tri.M[pos-i]+tri.M[pos-i-1];
			printf("%g\t",tri.M[pos-1]);
		}
	}
	printf("\n");
}

void copiar(matriz original,matriz *copia)
{
	copia->lin = original.lin;
	copia->col = original.col;
	copia->tam = original.tam;
	copia->M = (double *)malloc(copia->tam*sizeof(double));
	for (int i=0;i<original.tam;i++) copia->M[i]=original.M[i];	
}

matriz aleatoria(int lin, int col,int min,int max)
{
	matriz R;
	R.lin = lin;
	R.col = col;
	R.tam = lin*col;
	max-=min-1;
	R.M = (double *)malloc(R.tam*sizeof(double));
	for(int i=0;i<R.tam;i++)R.M[i] = rand()%max+min;
	return R;
}

