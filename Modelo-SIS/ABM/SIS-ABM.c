/*
Autor: Renato Maciel Félix

Programa que gera uma simulação de uma doença fictícia do modelo SIS (Suscetível-Infectado-Suscetível) utilizando Método Baseado em Agentes.

Última atualização: 06/02/2023
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define L 317		//Lado da malha	
#define N L*L		//Total de indivíduos
#define Tempo 365	//Dias simulados
#define MaxSimu 10	//Número de Simulações

#define Sucetivel 0	//Sucetível
#define Infectado 1	//Infectado
#define Morto 2

#define Beta 0.181269	//Taxa de infecciosidade

#define Gamma 1.0/15.0 	//Probabilidade de recuperação da infecção
#define m 1.0/(75.0*365.)	//Morte por qualquer outro motivo (mi)

#define PropI 0.01 	//Proporção inicial de infectados

int Simulacao;		//Variável para controlar em qual simulação estamos

//Variáveis para salvar a quantidade de cada grupo em determinado tempo
double Suscetivel_Total[MaxSimu+2][Tempo+2];
double Infectado_Total[MaxSimu+2][Tempo+2];

//Variáveis para o calculo do desvio padrão
double Suscetivel_Soma[Tempo+2];
double Infectado_Soma[Tempo+2];

//Variáveis para o calculo do desvio padrão
double Suscetivel_SomaQuad[Tempo+2];
double Infectado_SomaQuad[Tempo+2];

//Variáveis para o calculo do desvio padrão
double Suscetivel_Medio[Tempo+2];
double Infectado_Medio[Tempo+2];

//Variáveis para o calculo do desvio padrão
double Suscetivel_MedioQuad[Tempo+2];
double Infectado_MedioQuad[Tempo+2];

//Variáveis para o calculo do desvio padrão
double Dev_Suscetivel;
double Dev_Infectado;

//Variáveis para o processo de número aleatório
#define max 4294967295.0
unsigned s;	//Semente 
unsigned R;	//Raíz
double rn;

//Estrutura para quantidades de cada agente
struct grupos
{	int qs;
	int qi;
};
struct grupos quant;

//Definição da malha da população
int populacao[L+2][L+2];
int futuro[L+2][L+2];

//Contador para os dias
int cDias;

//Gerador de número aleatório
double aleatorio(unsigned *R)
{	*R=*R*s;
	rn=*R/max;

	return rn;
}

//Onde damos inicio a nossa população
void Inicial()
{	int i, j, k;
	int cTi;
	
	//Calculando o número de infectados iniciais
	cTi = N*PropI;
	
	//Iniciando os contadores de cada Agente
	quant.qs = 0;
	quant.qi = 0;
	
	//Inicializando a malha somente com indivíduos sucetíveis
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	populacao[i][j] = Sucetivel;
			quant.qs++;
		}
	}
		
	//Preencher a malha com os indivíduos infecciosos
	k=0;
	if(cTi>0)
	{	do
		{	//Encontrando um indivíduo aleatoriamente
			rn = aleatorio(&R);
			i = rn*L + 1;
			rn = aleatorio(&R);
			j = rn*L + 1;
			
			//Se o indivíduo for suscetível, então se tornará infectado
			if(populacao[i][j] == Sucetivel)
			{	populacao[i][j] = Infectado;
				quant.qi++;
				quant.qs--;
				
				k++;
			}
		}while(k<cTi);
	}
		
	//Inicializando a matriz auxiliar futuro
	for(i=1;i<=L;i++)
		for(j=1;j<=L;j++)
			futuro[i][j] = populacao[i][j];

	//Salvando as informações da população no dia=0
	Suscetivel_Total[Simulacao][0] = quant.qs/(1.0*N);
	Infectado_Total[Simulacao][0] = quant.qi/(1.0*N);
	
	//Salvando as informações da população no dia=0
	Suscetivel_Soma[0] += Suscetivel_Total[Simulacao][0];
	Infectado_Soma[0] += Infectado_Total[Simulacao][0];
	
	//Salvando as informações da população no dia=0
	Suscetivel_SomaQuad[0] += pow(Suscetivel_Total[Simulacao][0],2.0);		
	Infectado_SomaQuad[0] += pow(Infectado_Total[Simulacao][0],2.0);
}

//Ação para os Agentes Suscetíveis
void FuncSucetivel(int i, int j)
{	int l, k;
	int contTi = 0;
	
	double ProbLocalS;			//Probabilidades locais para cada tipo de bactéria
	
	rn = aleatorio(&R);
	if(rn<m) //Verificando a possibilidade de morte por causas naturais
		futuro[i][j] = Morto;
	else
	{	//Contando a quantidade pessoas doentes transmissíveis por perto
		for(l=-1;l<=1;l++)
			for(k=-1;k<=1;k++)
				if(populacao[i+l][j+k]==Infectado)
					contTi++;
			
		//Probabilidade local de transmissão
		ProbLocalS = 1.0 - pow(1.0 - Beta, 1.0*contTi);
			
		rn = aleatorio(&R);
		if(rn<ProbLocalS) //Se ocorrer a infecção
		{	rn = aleatorio(&R);
			
			//Temos que testar se vai direto para Ts 
		
			if(rn<Beta) //Desenvolve a doença e vai para Ts
			{	futuro[i][j] = Infectado;
			
			}
		
		}
		else //Se não acontecer
			futuro[i][j] = Sucetivel;	
	}
}

//Ação para os Agentes Infectados
void FuncInfectado(int i, int j)
{	rn = aleatorio(&R);

	if(rn<m) //Verificando a possibilidade de morte por causas naturais
		futuro[i][j] = Morto;
	else //Se não morreu
	{	rn = aleatorio(&R);
		
		if(rn < Gamma) //Verificando a probabilidade de recuperação
			futuro[i][j] = Sucetivel;
	}
}

//Função para atualizar a população no final do dia
void atualiza()
{	int i, j;
	int cTi=0;
	
	quant.qs = 0;
	quant.qi = 0;
	
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	populacao[i][j] = futuro[i][j]; //Salvando as atualizações da população
			
			if(populacao[i][j]==Morto) //Verificando as mortes e retorando o indivíduo morto com sucetível
				populacao[i][j] = Sucetivel;
		}
	}
	
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	//Calculando as quantidades gerais de cada Agente
			switch(populacao[i][j])
			{	case Sucetivel	:	quant.qs++;	
				break;
				
				case Infectado  :	quant.qi++;
				break;
					
				default :	break;				
			}
		}
	}
	
	cDias++;
	
	//Registrando a quantidade daqule dia
	Suscetivel_Total[Simulacao][cDias] = quant.qs/(1.0*N);
	Infectado_Total[Simulacao][cDias] = quant.qi/(1.0*N);

	//Calculos necessários para o Desvio Padrão
	Suscetivel_Soma[cDias] += Suscetivel_Total[Simulacao][cDias];
	Infectado_Soma[cDias] += Infectado_Total[Simulacao][cDias];
	
	//Calculos necessários para o Desvio Padrão
	Suscetivel_SomaQuad[cDias] += pow(Suscetivel_Total[Simulacao][cDias],2.0);		
	Infectado_SomaQuad[cDias] += pow(Infectado_Total[Simulacao][cDias],2.0);
			
}

FILE *pS;
FILE *pI;

int main()
{	int i, j, t;
	double rn1, rn2;
	
	R = 893221891;
	s = 888121;
	
	//Inicializando as matrizes 
	for(Simulacao=1;Simulacao<=MaxSimu;Simulacao++)
	{	for(t=0;t<=Tempo;t++)
		{	Suscetivel_Total[Simulacao][t] = 0;
			Infectado_Total[Simulacao][t] = 0;
		}
	}
	
	pS = fopen("Sucetivel.dat","w");
	pI = fopen("Infectado.dat","w");
	
	//Dando início as simulações
	for(Simulacao=1; Simulacao<=MaxSimu; Simulacao++)
	{	Inicial();
	
		cDias = 0;
	
		for(t=0;t<=Tempo;t++)
		{	//Condições de contorno	
			for(i=1;i<=L;i++)
			{	//Fechando as laterais da malha
				populacao[0][i] = populacao[0][L];
				populacao[L+1][i] = populacao[1][i];
			
				//Fechando a parte de cima com a de baixo
				populacao[i][0] = populacao[i][L];
				populacao[i][L+1] = populacao[i][1];		
			}
				//Ligando as pontas na malha
				populacao[0][0] = populacao[L][L];
				populacao[0][L+1] = populacao[L][1];
				populacao[L+1][L+1] = populacao[1][1];
				populacao[L+1][0] = populacao[1][L];
		
			//Percorrendo a população e chamando cada função respectiva
			for(i=1;i<=L;i++)
			{	for(j=1;j<=L;j++)
				{	switch(populacao[i][j])
					{	case Sucetivel	:	FuncSucetivel(i,j);	
						break;
					
						case Infectado :	FuncInfectado(i,j);
						break;
						
						default :	break;				
					}
				
				}
			}
			
			//Atualizando a população depois do fim do dia	
			atualiza();		
		}
	}
	
	//Cálculo do desvio padrão e registro das quantidades médias de cada grupo em cada dia.
	for(t=0;t<=Tempo;t++)
	{	//Cálculo de variáveis do desvio padrão
		Suscetivel_Medio[t] = Suscetivel_Soma[t]/((double)MaxSimu);
		Infectado_Medio[t] = Infectado_Soma[t]/((double)MaxSimu);
	
		//Cálculo de variáveis do desvio padrão
		Suscetivel_MedioQuad[t] = Suscetivel_SomaQuad[t]/((double)MaxSimu);
		Infectado_MedioQuad[t] = Infectado_SomaQuad[t]/((double)MaxSimu);
	
		//Cálculo do desvio padrão
		Dev_Suscetivel = sqrt( Suscetivel_MedioQuad[t] - pow(Suscetivel_Medio[t],2.0) );
		Dev_Infectado = sqrt( Infectado_MedioQuad[t] - pow(Infectado_Medio[t],2.0) );
		
		//Registrando no arquivo as quantidades médias
		fprintf(pS,"%d\t%lf\t%lf\n", t, Suscetivel_Medio[t], Dev_Suscetivel);
		fprintf(pI,"%d\t%lf\t%lf\n", t, Infectado_Medio[t], Dev_Infectado);
	}
		
	fclose(pS);
	fclose(pI);
		
	return 0; 
}
