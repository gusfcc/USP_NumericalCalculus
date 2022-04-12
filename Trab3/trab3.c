#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Funções para a aplicação do método MMQ
double* minQuad(double **matrix, double *vetX, double *vetY, int n);
double somat1a(int n);
double somatx(double *vetX , int n);
double somatx2(double *vetX , int n);
double somatxfx(double *vetX, double *vetY , int n);

//Função para o calculo da soma dos resíduos ao quadrado
double sResiduosQ(double *vetY, double *vetYg, int n);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Função para transformar xNew em 1/sqrt(x)
double* alterarX(double *vetX, int n);

//Função f(x) = (Sigma)0 + kd^-1/2, para um único x e para um vetor de x
double functionGx(double *vetConstantes, double x);
double* vetFunctionGx(double *vetConstantes, double *vetX, int n);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Função para transformar yNew em ln(Y)
double* alterarYEuler(double *vetY, int n);

//Função f(x) =  a * e^bx, para um único x e para um vetor de x
double exponetialFunctionGx(double* constantes, double x);
double* vetExponetialFunction(double *constantes, double *vetX, int n);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Funções para o calculo do sistema
void printMatrix(double **matrix, int max);
void escalonamento(double **matrix, int linhas, int colunas);
double somatoria(double **matrix, int l, int max);
double* answer(double **matrix, int max);
void printResposta(double *resposta, int max);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

int main()
{
	//Variáveis para ambos os items
	double **matrix; //Matriz para resolver o sistema criado no método MMQ
	double vetX[8] = {0.006, 0.011, 0.017, 0.025, 0.039, 0.060, 0.081, 0.105};
	double vetY[8] = {334, 276, 249, 235, 216, 197, 194, 182};
	double *vetYg; //Valores de Y da função g(x), sendo g(x) a função f(x) linearizada
	double somaQResid = 0; //Soma dos resíduos ao quadrado
	
	matrix = (double **)malloc(2 * sizeof(double*));
	for(int i = 0; i < 2; i++) matrix[i] = (double *)malloc((3) * sizeof(double));
	//A matriz extendida sempre será 2 por 3
	
	vetYg = (double *)malloc(8 * sizeof(double));
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	//Variáveis para o item a)
	double *constantesA; //Vetor que contem as constantes da equação f(x) = (Sigma)0 + kd^-1/2. 
	//(Sigma)0 == constantesA[0] e k == constantesA[1]
	double *vetXaNew; //vetXaNew = 1/sqrt(vetX)
	
	vetXaNew = (double *)malloc(8 * sizeof(double));
	
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	//Variáveis para o item b)
	double *constantesB; //Vetor que contem as constantes da equação f(x) = a * e^bx.
	// a == constantesB[0] e b == constantesB[1]
	double *vetYbNew; //vetYaNew = ln(vetY)
	
	vetYbNew = (double *)malloc(8 * sizeof(double));
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	//Item a)
	
	printf("\nItem a) \n");
	vetXaNew = alterarX(vetX, 8); //Ajustar os valores de x para poder usar o MMQ
		
	constantesA = minQuad(matrix, vetXaNew, vetY, 8); //Aplicar o método do MMQ para descobrir as constantes
	
	printf("\nConstantes da enquacao f(x) = (Sigma)0 + kd^-1/2: \n");
	printResposta(constantesA, 2);
	
	printf("\n\nA = (Sigma)0, B = k");
	
	printf("\n");
	
	vetYg = vetFunctionGx(constantesA, vetX, 8); //Aplicar os pontos dados para receber os y da g(x)
	somaQResid = sResiduosQ(vetY, vetYg, 8);
	
	printf("\nSoma dos residuos ao quadrado: ");
	printf("\n%lf\n\n", somaQResid);
		
	printf("Pontos Arbitrarios: ");
	
	printf("\n(0.15, %lf)\n", functionGx(constantesA, 0.15));
	printf("\n(0.006, %lf)\n", functionGx(constantesA, 0.006));
	printf("\n(0.05, %lf)\n", functionGx(constantesA, 0.05));
	
	printf("\n");
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
	
	//Item b)
	
	printf("\nItem b) \n");
	vetYbNew = alterarYEuler(vetY, 8); //Ajustar os valores de y para poder aplicar o MMQ
	
	constantesB = minQuad(matrix, vetX, vetYbNew, 8); //Aplicar o método do MMQ para descobrir as constantes
	
	
	constantesB[0] = exp(constantesB[0]); //Modificar o valor da constante a para o y não ajustado
	
	printf("\nConstantes da enquacao f(x) = a * e^bx: \n");
	printResposta(constantesB, 2);
	
	printf("\n\nA = a, B = b");
	
	printf("\n");
	
	vetYg = vetExponetialFunction(constantesB, vetX, 8); //Aplicar os pontos dados para receber os y da g(x)
	somaQResid = sResiduosQ(vetY, vetYg, 8);
	
	printf("\nSoma dos residuos ao quadrado: ");
	printf("\n%lf\n\n", somaQResid);
	
	printf("Pontos Arbitrarios: ");
	
	printf("\n(0.15, %lf)\n", exponetialFunctionGx(constantesB, 0.15));
	printf("\n(0.006, %lf)\n", exponetialFunctionGx(constantesB, 0.006));
	printf("\n(0.05, %lf)\n", exponetialFunctionGx(constantesB, 0.05));
	
}


double* minQuad(double **matrix, double *vetX, double *vetY, int n)
{
	
	matrix[0][0] = somat1a(n);
	matrix[0][1] = somatx(vetX, n);
	matrix[0][2] = somatx(vetY, n);
	
	matrix[1][0] = matrix[0][1];
	matrix[1][1] = somatx2(vetX, n);
	matrix[1][2] = somatxfx(vetX, vetY, n);
	
	escalonamento(matrix, 2, 3);
	return answer(matrix, 2);
}

double somat1a(int n)
{
	double sum = 0;
	for(int i = 0; i < n; i++)
		sum ++;
	
	return sum;
}

double somatx(double *vetX , int n)
{
	double sum = 0;
	for(int i = 0; i < n; i++)
		sum += vetX[i];
	
	return sum;
}

double somatx2(double *vetX , int n)
{
	double sum = 0;
	for(int i = 0; i < n; i++)
		sum += vetX[i] * vetX[i];
	
	return sum;
}

double somatxfx(double *vetX, double *vetY , int n)
{
	double sum = 0;
	for(int i = 0; i < n; i++)
		sum += vetX[i] * vetY[i];
	
	return sum;
}

double sResiduosQ(double *vetY, double *vetYg, int n)
{
	double sum = 0;
		
	for(int i = 0; i < n; i++)
		sum += (vetY[i] - vetYg[i]) * (vetY[i] - vetYg[i]);
	
	
	return sum;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

double* alterarX(double *vetX, int n)
{
	double *vetXNew;
	vetXNew = (double *)malloc(n * sizeof(double));
	
	for(int i = 0; i < n; i++)
	{
		vetXNew[i] = (1/sqrt(vetX[i]));
	}
	return vetXNew;
}

double functionGx(double *vetConstantes, double x)
{	
	return vetConstantes[0] + vetConstantes[1] * (1/sqrt(x));
}

double* vetFunctionGx(double *vetConstantes, double *vetX, int n)
{
	double *vetYg;
	
	vetYg = (double *)malloc(n * sizeof(double));
	
	
	for(int i = 0; i < n; i++)
	{
		vetYg[i] = vetConstantes[0] + vetConstantes[1] * (1/sqrt(vetX[i]));
	}
	
	
	return vetYg;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

double* alterarYEuler(double *vetY, int n)
{
	double *vetYNew;
	vetYNew = (double *)malloc(n * sizeof(double));
	
	for(int i = 0; i < n; i++)
	{
		vetYNew[i] = (log(vetY[i]));
	}
	return vetYNew;
}

double exponetialFunctionGx(double* constantes, double x)
{	
	return constantes[0] * exp(constantes[1] * x);
}

double* vetExponetialFunction(double *constantes, double *vetX, int n)
{
	double *vetYg;
	
	vetYg = (double *)malloc(n * sizeof(double));
	
	
	for(int i = 0; i < n; i++)
	{
		vetYg[i] = constantes[0] * exp(constantes[1] * vetX[i]);
	}
	
	return vetYg;
	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void escalonamento(double **matrix, int linhas, int colunas)
{
	int passo, i, j, flag = 0;
	double m, pivo = 0, aux;
	
	for(passo = 0; passo < linhas; passo++){ //Cada passo para o processo de escalonamento, ele termina no passo linhas - 1 pois a ultima linha ja estera escalonada;
		pivo = matrix[passo][passo];		 //O elemento pivo eh previamento selecionado por ser o primeiro elemento da diagonal principal;	
		
		for(i = passo; i < linhas; i++){ 	 
			if(fabs(matrix[i][passo]) > fabs(pivo)){ //O elemento pivo eh comparado com os outros elementos para ser entao o elemento de maior valor absoluto o pivo;
				pivo = matrix[i][passo];
				flag = i; //Indicar qual linha precisa ser permutada/substituida;
			}
		}
		
		if(pivo != 0){ //O elemento do pivo nao pode ser nulo;
			if(flag != 0){
				for(i = 0; i < colunas; i++){ //Faz a troca das linhas caso o pivo nao seja ja o elemento da diagonal principal;
					aux = matrix[passo][i];
					matrix[passo][i] = matrix[flag][i];
					matrix[flag][i] = aux;
				}
			}
			
			for(i = passo + 1; i < linhas; i++){ //Formulas de escalonamento de matriz;
				m = matrix[i][passo] / pivo;
				matrix[i][passo] = 0;
				
				for(j = passo + 1; j < colunas; j++){
					matrix[i][j] = matrix[i][j] - (m * matrix[passo][j]);		
				} 
			}
		}
		flag = 0; //Zera o indicador para que todo passo possa ser indicado a posicao do pivo na matriz;
	}
}


double somatoria(double **matrix, int l, int max)
{
	double soma = 0;	
	for(int c = l + 1; c < max; c++) //a somatoria e de todos os valores entre i e a ultima coluna, sem contar ambos
	{
		soma = soma + matrix[l][c];
	}
	
	return soma;
	
}

void printMatrix(double **matrix, int max)
{
	
	for(int i = 0; i < max; i++)
	{
		for(int j = 0; j < (max+1); j++)
		{
			printf("%7.4lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

double* answer(double **original, int max)
{
	double *respostas; //vetor dos valores de x
	double **matrix; //criado para não alterar a matriz original
	
	respostas = (double *)malloc(max * sizeof(double));
	
	matrix = (double **)malloc(max * sizeof(double*));
	for(int i = 0; i < max; i++) matrix[i] = (double *)malloc((max+1) * sizeof(double));
	
	for(int i = 0; i < max; i++) //copia da matriz original
	{
		for(int j = 0; j < max + 1; j++)
		{
			matrix[i][j] = original[i][j];
		}
	}
	
	for(int i = max - 1; i >= 0; i--) //indo na diagonal principal da matriz do sistema de baixo para cima
	{
		 respostas[i] = (matrix[i][max] - somatoria(matrix, i, max)) / matrix[i][i];
		 //valores de x = resposta da linha - somatoria das: constantes da linha multiplicados pelos x já encontrados e substituidos, tudo dividido pelo valor atual da diagonal
		 
		 for(int j = i - 1; j >= 0; j--) //o valor do x achado acima é multiplicado na coluna inteira de baixo para cima
		 {
			 matrix[j][i] = matrix[j][i] * respostas[i];
		 }
	}
	
	return respostas;
}

void printResposta(double *resposta, int max)
{
	for(int i = 0; i < max; i++)
		if(i != max - 1){
			printf("A = %5.4lf, ", resposta[i]);
		}else{
			printf("B = %5.4lf", resposta[i]);
		}
}


