#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Matriz
{
	double **M;
	int col;
	int lin;
}matriz;

matriz submatriz(matriz A,int l, int c);
void exibir_matriz(matriz A);
double determinante(matriz A);
void tamanho(matriz A);
matriz produto(matriz A,matriz B);
matriz transposta(matriz A);
void alocar(matriz *A);
matriz identidade(int ordem);
int igual(matriz a, matriz b);


void inserir_matriz(matriz *A)
{
	printf("Inserir a quantidade de linhas: ");
	scanf("%d",&A->lin);
	printf("Inserir a quantidade de colunas: ");
	scanf("%d",&A->col);
	alocar(A);
	for(int i=0;i<A->lin;i++)
	{
		for(int j=0;j<A->col;j++)
		{
			printf("Elemento:[%d][%d]: ",i+1,j+1);
			scanf("%lf",&A->M[i][j]);
		}
	}
}

void exibir_matriz(matriz A)
{
	for(int i=0;i<A.lin;i++)
	{
		printf("\n|");
		for(int j=0;j<A.col;j++)
		{			
			printf("%g\t",A.M[i][j]);
		}
		printf("|");
	}
}

double determinante(matriz A)
{
	double det=0;
	if(A.lin==A.col)
	{
		if (A.lin==2)
		{
			det =  A.M[0][0]*A.M[1][1]-A.M[0][1]*A.M[1][0];
		}
		else
		{
			for(int j=0;j<A.col;j++)
			{
				det+=A.M[0][j]*determinante(submatriz(A,0,j));
			}
		}
	}
	else 
	{
		printf("Não é matriz quadrada.");
	}	
	return det;
}

matriz submatriz(matriz A,int l, int c)
{
	matriz r;
	r.lin = A.lin-1;
	r.col = A.col-1;
	r.M = malloc(r.lin*sizeof(double));
	int ri=0,rj;
	for(int i=0;i<A.lin;i++)
	{
		if(i!=l)
		{
			r.M[ri] = malloc(r.col*sizeof(double));
			rj=0;
			for(int j=0;j<A.col;j++) 
			{
				if(j!=c)
				{
					r.M[ri][rj]=A.M[i][j];
					rj++;
				}					
			}
			ri++;
		}
	}
	return r;
}

void tamanho(matriz A)
{
	printf("\n%d por %d\n",A.lin,A.col);
}

matriz produto(matriz A, matriz B)
{
	matriz r;
	if (A.col==B.lin)
	{
		r.col = B.col;
		r.lin = A.lin;
		r.M = malloc(r.lin*sizeof(double));
		for (int c=0;c<A.lin;c++)
		{
			r.M[c]=malloc(r.col*sizeof(double));
			for (int l=0;l<B.col;l++)
			{
				r.M[c][l]=0;
				for (int k=0;k<A.col;k++)
				{
					r.M[c][l]+=A.M[c][k]*B.M[k][l];
				}
			}
		}
	}
	else
	{
		r.col=1;
		r.lin=1;
		alocar(&r);
		r.M[0]=0;
	}
	
	return r;
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
			r.M[i][j]=A.M[j][i];
		}
	}
	return r;
}

void alocar(matriz *A)
{
	A->M = malloc(A->lin*sizeof(double));
	for (int i=0;i<A->lin;i++)
		A->M[i] = malloc(A->col*(sizeof(double)));
}

matriz identidade(int ordem)
{
	matriz r;
	r.lin=ordem;
	r.col=ordem;
	alocar(&r);
	for (int i=0;i<ordem;i++)
	{
		for (int j=0;j<ordem;j++)
		{
			if(i==j)
			{
				r.M[i][j]=1;
			}
			else
			{
				r.M[i][j]=0;
			}
		}
	}
	return r;
}

int igual(matriz a, matriz b)
{
	int r=1;
	if(a.lin==b.lin && a.col==b.col)
	{
		for (int i=0;i<a.lin;i++)
		{
			if (!r) break;
			for(int j=0;j<a.col;j++)
			{
				if(a.M[i][j]!=b.M[i][j])
				{
					r=0;
					break;
				}
			}
		}
	}
	else
	{
		r = 0;
	}	
	return r;
}