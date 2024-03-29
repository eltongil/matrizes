#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Definição da estrutura de uma matriz com alocação dinâmica.
typedef struct Matriz
{
	double **M;
	int col;
	int lin;
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
void trianguloPascal(matriz *tri,int ordem);
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
	alocar(A);//Como alocação dinâmica será muito recorrente, fiz uma subrotina específica.
	for(int i=1;i<A->lin;i++)
	{
		for(int j=1;j<A->col;j++)
		{
			printf("Elemento:[%d][%d]: ",i+2,j+1);
			scanf("%lf",&A->M[i][j]);
		}
	}
}

//Exibição padrão para uma matriz retangular
void exibir_matriz(matriz A)
{
	for(int i=0;i<A.lin;i++)
	{
		printf("\n|");//Espaço superior
		for(int j=0;j<A.col;j++)
		{			
			printf("%g",A.M[i][j]);
			if(j!=A.col-1) printf("\t");//Espaço lateral se não for o último elemento
		}
		printf("|");//barra fim de linha
	}
	printf("\n");//espaço inferior
}

//Determinante por submatriz, por laplace
double determinante(matriz A)
{
	double det=0;
	if(A.lin==A.col)
	{
		if (A.lin==2)
		{
			det =  A.M[0][0]*A.M[1][1]-A.M[0][1]*A.M[1][0];//Determinante 2x2
		}
		else//3x3 ou maior
		{
			//Essa função é recursiva, cada matriz alocada é liberada após seu uso total.
			for(int j=0;j<A.col;j++)
			{
				if(j%2)det-=A.M[0][j]*sub_det(submatriz(A,0,j));//Parte do determinante se ímpar
				else det += A.M[0][j]*sub_det(submatriz(A,0,j));// se par
			}
		}
	}
	else
		printf("Não é matriz quadrada.");	
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
	r.M = (double **)malloc(r.lin*sizeof(double));
	int ri=l+1,rj;
	for(int i=0;i<r.lin;i++)
	{
		r.M[i] =(double *)malloc(r.col*sizeof(double));
		if(ri>=l)ri=0;
		rj = c+1;
		for(int j=0;j<r.col;j++) 
		{
			if(rj>=A.col) rj=0;
			r.M[i][j]=A.M[ri][rj];
			rj++;	
		}
		ri++;
	}
	return r;
}

double sub_det(matriz A)
{
	double r = determinante(A);
	limpar(&A);
	return r;
}

void tamanho(matriz A){	printf("\n%d por %d\n",A.lin,A.col);}

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
		r.M[0][0]=0;
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

void transpor(matriz *A)
{
	double temp;
	for (int i=1;i<A->lin;i++)
	{
		for (int j=i;j<A->col;j++)
		{
			temp = A->M[i][j];
			A->M[i][j] = A->M[j][i];
			A->M[j][i] = temp;
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
		r.M = (double **)calloc(r.lin,sizeof(double));
		for (int c=0;c<A.lin;c++)
		{
			r.M[c]=(double *)calloc(r.col,sizeof(double));
			for (int l=0;l<t.col;l++)
			{
				for (int k=0;k<A.col;k++)
					r.M[c][l]+=A.M[c][k]*t.M[l][k];
			}
		}
		limpar(&t);
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
	A->M = (double **)calloc(A->lin,sizeof(double*));
	for (int i=0;i<A->lin;i++)
		A->M[i] = (double *)calloc(A->col,(sizeof(double)));
}

matriz identidade(int ordem)
{
	matriz r;
	r.lin=ordem;
	r.col=ordem;
	alocar(&r);
	for (int i=0;i<ordem;i++)
		r.M[i][i]=1;
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

void trianguloPascal(matriz *tri,int ordem)
{
	tri->lin = ordem+1;
	tri->col = tri->lin;
	tri->M = malloc(tri->lin*sizeof(double));
	for(int i=0;i<tri->lin;i++)
	{
		printf("\n");
		tri->M[i] = malloc((i)*sizeof(double));
		for(int j=0;j<=i;j++)
		{
			if (j==0 || j == i) tri->M[i][j]=1;
			else if (j==1 || j == i-1) tri->M[i][j]=i;
			else tri->M[i][j] = tri->M[i-1][j]+tri->M[i-1][j-1];
			printf("%g\t",tri->M[i][j]);
		}
	}
	printf("\n");
}

void copiar(matriz original,matriz *copia)
{
	copia->lin = original.lin;
	copia->col = original.col;
	alocar(copia);
	for (int i=0;i<original.lin;i++)
	{
		for(int j=0;j<original.col;j++)
			copia->M[i][j]=original.M[i][j];
	}	
}

matriz aleatoria(int lin, int col,int min,int max)
{
	matriz R;
	R.lin = lin;
	R.col = col;
	max-=min-1;
	R.M = (double **)malloc(lin*sizeof(double));
	for(int i=0;i<lin;i++)
	{
		R.M[i] = (double *)malloc(col*sizeof(double));
		for(int j=0;j<col;j++)
			R.M[i][j] = rand()%max+min;
	}
	return R;
}

void limpar(matriz *a)
{
	while (a->lin>0) free(a->M[--a->lin]);
	free(a->M);
}
