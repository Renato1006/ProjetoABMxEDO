/*
Autor: Renato Maciel Félix

Programa que gera uma simulação de uma doença fictícia do modelo SIR (Suscetível-Infectado-Recuperado) utilizando EDOs pelo método de Runge-Kutta de quarta ordem.

Última atualização: 06/02/2023
*/

#include <stdio.h>
#include <math.h>

#define N 3	//Número de equações

#define Mu 1.0/100.0 	//Taxa de mortalidade por causas naturais//Taxa de mortalidade por causas naturais
#define Beta 1.0/5.0 	//Taxa de infecciosidade
#define Gamma 1.0/15.0 //Taxa de recuperação

#define Ti 0.01 //Definindo a quantidade incial de infectados

typedef double (*equacoes)(); //Criando uma estrutura que contem as EDOs

//y[0] == Suscetível   ----  y[1] == Infectado    ----  y[3] == Recuperado 

double f0(double x, double y[N]) //Sucetível
{	return (-Beta*y[0]*y[1] + Mu*(y[1] + y[2]));
}

double f1(double x, double y[N]) //Infectado
{	return (Beta*y[0]*y[1] - Gamma*y[1] - Mu*y[1]);
}

double f2(double x, double y[N]) //Recuperado
{	return (Gamma*y[1] - Mu*y[2]);
}

//Método de Runge-Kutta de quarta ordem 
void rk4(double x, double y[N], double h)
{	int i;	
	double k1[N], k2[N], k3[N], k4[N], yp[N], ypp[N];
    	equacoes f[] = {f0,f1,f2};
    	
    	for(i=0;i<N;i++)
    	{	k1[i] = f[i](x,y);
        	yp[i] = y[i] +h/2*k1[i];
    	}
    
    	for(i=0;i<N;i++)
    	{	k2[i] = f[i](x+h/2, yp);
        	ypp[i] = y[i] + h/2*k2[i];
    	}
    
    	for(i=0;i<N;i++)
    	{	k3[i] = f[i](x+h/2, ypp);
        	yp[i] = y[i] + h*k3[i];
    	}
    
    	for(i=0;i<N;i++)
    	{	k4[i] = f[i](x+h, yp);
        	y[i] += h/6*(k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
    	}
}

int main()
{	int i;
    	double a, b, h, x;
    	double y[N] = {1.0 - Ti, Ti, 0.0}; //Definindo as quantidades iniciais
    
    	a = 0;
    	b = 365; //Quantidade de dia simulado
    	h = 0.1;
    	x = a;
    
    	//Inicializando os pontos   
    	for(x=a;x<b;x+=h)
    	{	//Imprimindo o passo
    		printf("%g ", x);
        
        	for(i=0;i<N;i++)  printf("%g ", y[i]); //Imprimindo cada quantidade
        	
        	rk4(x,y,h); //Atualizando as quantidades 
        	puts("");
    	}
}
