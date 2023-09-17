#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define L 317
#define N L*L	
#define Tempo 150
#define MaxSimu 10	//Número de Simulações

#define Sucetivel 0	//Sucetível
#define Exposto 1	//Está na fase incubada da doença
#define Infectado 2	//Infectado
#define Recuperado 3	//Se curou através do Tratamento
#define Morto 4

#define Beta 0.181269/*1.0/5.0*/ 

#define Mu 1.0/100.0
#define Gamma 1.0/15.0 
#define m 1.0/(75.0*365.)		//Morte por qualquer outro motivo (mi)
#define Kappa 1.0/10.0

#define PropI 0.2

int Simulacao;		//Variável para controlar em qual simulação estamos

//Variáveis para salvar a quantidade de cada grupo em determinado tempo
double Suscetivel_Total[MaxSimu+2][Tempo+2];
double Exposto_Total[MaxSimu+2][Tempo+2];
double Infectado_Total[MaxSimu+2][Tempo+2];
double Recuperado_Total[MaxSimu+2][Tempo+2];

//Variáveis para o calculo do desvio padrão
double Suscetivel_Soma[Tempo+2];
double Exposto_Soma[Tempo+2];
double Infectado_Soma[Tempo+2];
double Recuperado_Soma[Tempo+2];
//Variáveis para o calculo do desvio padrão
double Suscetivel_SomaQuad[Tempo+2];
double Exposto_SomaQuad[Tempo+2];
double Infectado_SomaQuad[Tempo+2];
double Recuperado_SomaQuad[Tempo+2];
//Variáveis para o calculo do desvio padrão
double Suscetivel_Medio[Tempo+2];
double Exposto_Medio[Tempo+2];
double Infectado_Medio[Tempo+2];
double Recuperado_Medio[Tempo+2];
//Variáveis para o calculo do desvio padrão
double Suscetivel_MedioQuad[Tempo+2];
double Exposto_MedioQuad[Tempo+2];
double Infectado_MedioQuad[Tempo+2];
double Recuperado_MedioQuad[Tempo+2];
//Variáveis para o calculo do desvio padrão
double Dev_Suscetivel;
double Dev_Exposto;
double Dev_Infectado;
double Dev_Recuperado;

//Variáveis para o processo de número aleatório
#define max 4294967295.0
unsigned s;	//Semente 
unsigned R;	//Raíz
double rn;

struct grupos
{	int qs;
	int qe;
	int qi;
	int qr;
};
struct grupos quant;

int populacao[L+2][L+2];
int futuro[L+2][L+2];

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
	
	cTi = N*PropI;
	
	quant.qs = 0;
	quant.qe = 0;
	quant.qi = 0;
	quant.qr = 0;
	
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	populacao[i][j] = Sucetivel;
			quant.qs++;
		}
	}
	
	k=0;
	if(cTi>0)
	{	do
		{	rn = aleatorio(&R);
			i = rn*L + 1;
			rn = aleatorio(&R);
			j = rn*L + 1;
			
			if(populacao[i][j] == Sucetivel)
			{	populacao[i][j] = Infectado;
				quant.qi++;
				quant.qs--;
				
				k++;
			}
		}while(k<cTi);
	}
		
	for(i=1;i<=L;i++)
		for(j=1;j<=L;j++)
			futuro[i][j] = populacao[i][j];

	//Salvando as informações da população no dia=0
	Suscetivel_Total[Simulacao][0] = quant.qs/(1.0*N);
	Exposto_Total[Simulacao][0] = quant.qe/(1.0*N);
	Infectado_Total[Simulacao][0] = quant.qi/(1.0*N);
	Recuperado_Total[Simulacao][0] = quant.qr/(1.0*N);
	
	//Salvando as informações da população no dia=0
	Suscetivel_Soma[0] += Suscetivel_Total[Simulacao][0];
	Exposto_Soma[0] += Exposto_Total[Simulacao][0];
	Infectado_Soma[0] += Infectado_Total[Simulacao][0];
	Recuperado_Soma[0] += Recuperado_Total[Simulacao][0];	
	
	//Salvando as informações da população no dia=0
	Suscetivel_SomaQuad[0] += pow(Suscetivel_Total[Simulacao][0],2.0);		
	Exposto_SomaQuad[0] += pow(Exposto_Total[Simulacao][0],2.0);
	Infectado_SomaQuad[0] += pow(Infectado_Total[Simulacao][0],2.0);
	Recuperado_SomaQuad[0] += pow(Recuperado_Total[Simulacao][0],2.0);
}

void FuncSucetivel(int i, int j)
{	int l, k;
	int contTi = 0;
	
	double ProbLocalS;			//Probabilidades locais para cada tipo de bactéria
	
	rn = aleatorio(&R);
	if(rn<m)
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
		{	//rn = aleatorio(&R);
			
			//Temos que testar se vai direto para Ts 
			//if(rn<Beta) //Desenvolve a doença e vai para Ts
				//futuro[i][j] = Infectado;
			//else
				futuro[i][j] = Exposto;
		}
		else //Se não acontecer
			futuro[i][j] = Sucetivel;
	}
}

void FuncExposto(int i, int j)
{	rn = aleatorio(&R);

	if(rn<m)
		futuro[i][j] = Morto;
	else
	{	rn = aleatorio(&R);
		if(rn<Kappa)
			futuro[i][j] = Infectado;
	}
}

void FuncInfectado(int i, int j)
{	rn = aleatorio(&R);

	if(rn<m)
		futuro[i][j] = Morto;
	else
	{	rn = aleatorio(&R);
		if(rn < Gamma)
			futuro[i][j] = Recuperado;
	}
}

void FuncRecuperado(int i, int j)
{	rn = aleatorio(&R);

	if(rn<m)
		futuro[i][j] = Morto;
	else
		futuro[i][j] = Recuperado;
}

void atualiza()
{	int i, j;
	int cTi=0;
	
	quant.qs = 0;
	quant.qe = 0;
	quant.qi = 0;
	quant.qr = 0;
	
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	populacao[i][j] = futuro[i][j];
			
			if(populacao[i][j] == Morto)
				populacao[i][j] = Sucetivel;
		}
	}
	
	for(i=1;i<=L;i++)
	{	for(j=1;j<=L;j++)
		{	switch(populacao[i][j])
			{	case Sucetivel	:	quant.qs++;	
				break;
				
				case Exposto  :	quant.qe++;
				break;
				
				case Infectado  :	quant.qi++;
				break;
				
				case Recuperado  :	quant.qr++;
				break;
					
				default :	break;				
			}
		}
	}
	
	cDias++;
	
	//Registrando a quantidade daqule dia
	Suscetivel_Total[Simulacao][cDias] = quant.qs/(1.0*N);
	Exposto_Total[Simulacao][cDias] = quant.qe/(1.0*N);
	Infectado_Total[Simulacao][cDias] = quant.qi/(1.0*N);
	Recuperado_Total[Simulacao][cDias] = quant.qr/(1.0*N);
	
	//Calculos necessários para o Desvio Padrão
	Suscetivel_Soma[cDias] += Suscetivel_Total[Simulacao][cDias];
	Exposto_Soma[cDias] += Exposto_Total[Simulacao][cDias];
	Infectado_Soma[cDias] += Infectado_Total[Simulacao][cDias];
	Recuperado_Soma[cDias] += Recuperado_Total[Simulacao][cDias];	
	
	//Calculos necessários para o Desvio Padrão
	Suscetivel_SomaQuad[cDias] += pow(Suscetivel_Total[Simulacao][cDias],2.0);		
	Exposto_SomaQuad[cDias] += pow(Exposto_Total[Simulacao][cDias],2.0);
	Infectado_SomaQuad[cDias] += pow(Infectado_Total[Simulacao][cDias],2.0);
	Recuperado_SomaQuad[cDias] += pow(Recuperado_Total[Simulacao][cDias],2.0);		
}

FILE *pS;
FILE *pE;
FILE *pI;
FILE *pR;

int main()
{	int i, j, t;
	
	R = 893221891;
	s = 888121;
	
	//Inicializando as matrizes 
	for(Simulacao=1;Simulacao<=MaxSimu;Simulacao++)
	{	for(t=0;t<=Tempo;t++)
		{	Suscetivel_Total[Simulacao][t] = 0;
			Exposto_Total[Simulacao][t] = 0;
			Infectado_Total[Simulacao][t] = 0;
			Recuperado_Total[Simulacao][t] = 0;
		}
	}
	
	pS = fopen("Sucetivel.dat","w");
	pE = fopen("Exposto.dat","w");	
	pI = fopen("Infectado.dat","w");
	pR = fopen("Recuperado.dat","w");

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
		
			for(i=1;i<=L;i++)
			{	for(j=1;j<=L;j++)
				{	switch(populacao[i][j])
					{	case Sucetivel	:	FuncSucetivel(i,j);	
						break;
					
						case Exposto :		FuncExposto(i,j);
						break;
						
						case Infectado :	FuncInfectado(i,j);
						break;
						
						case Recuperado :	FuncRecuperado(i,j);
						break;
						
						default :	break;				
					}
				
				}
			}
			
			atualiza();		
		}
	}
	
	//Cálculo do desvio padrão e registro das quantidades médias de cada grupo am cada dia.
	for(t=0;t<=Tempo;t++)
	{	//Cálculo de variáveis do desvio padrão
		Suscetivel_Medio[t] = Suscetivel_Soma[t]/((double)MaxSimu);
		Exposto_Medio[t] = Exposto_Soma[t]/((double)MaxSimu);
		Infectado_Medio[t] = Infectado_Soma[t]/((double)MaxSimu);
		Recuperado_Medio[t] = Recuperado_Soma[t]/((double)MaxSimu);
	
		//Cálculo de variáveis do desvio padrão
		Suscetivel_MedioQuad[t] = Suscetivel_SomaQuad[t]/((double)MaxSimu);
		Exposto_MedioQuad[t] = Exposto_SomaQuad[t]/((double)MaxSimu);
		Infectado_MedioQuad[t] = Infectado_SomaQuad[t]/((double)MaxSimu);
		Recuperado_MedioQuad[t] = Recuperado_SomaQuad[t]/((double)MaxSimu);
	
		//Cálculo do desvio padrão
		Dev_Suscetivel = sqrt( Suscetivel_MedioQuad[t] - pow(Suscetivel_Medio[t],2.0) );
		Dev_Exposto = sqrt( Exposto_MedioQuad[t] - pow(Exposto_Medio[t],2.0) );
		Dev_Infectado = sqrt( Infectado_MedioQuad[t] - pow(Infectado_Medio[t],2.0) );
		Dev_Recuperado = sqrt( Recuperado_MedioQuad[t] - pow(Recuperado_Medio[t],2.0) );
		
		//Registrando no arquivo as quantidades médias
		fprintf(pS,"%d\t%lf\t%lf\n", t, Suscetivel_Medio[t], Dev_Suscetivel);
		fprintf(pE,"%d\t%lf\t%lf\n", t, Exposto_Medio[t], Dev_Exposto);
		fprintf(pI,"%d\t%lf\t%lf\n", t, Infectado_Medio[t], Dev_Infectado);
		fprintf(pR,"%d\t%lf\t%lf\n", t, Recuperado_Medio[t], Dev_Recuperado);
	}
		
	fclose(pS);
	fclose(pE);
	fclose(pI);
	fclose(pR);
		
	return 0; 
}

