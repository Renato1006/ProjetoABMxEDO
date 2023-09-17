/*
Nome: Renato Maciel Félix

Lista 7. Questão 3.
*/

#include <stdio.h>
#include <math.h>

#define N 4

#define B 0.001
#define Mu 1.0/1000.0
#define Kappa 1.0/5.0
#define Beta 1.0/3.0
#define Gamma 1.0/20.0 

typedef double (*equacoes)();

//y[0] == Suscetível - y[1] == Exposto - y[2] == Infectado - y[3] == Recuperado

double f0(double x, double y[N]) //Sucetível/dt
{	return ( B -Beta*y[0]*y[2] - Mu*y[0]);
}

double f1(double x, double y[N]) //Exposto/dt
{	return (Beta*y[0]*y[2] - (Mu + Kappa)*y[1]);
}

double f2(double x, double y[N]) //Infectado/dt
{	return (Kappa*y[1] - (Mu + Gamma)*y[2]);
}

double f3(double x, double y[N]) //Recuperado/dt
{	return (Gamma*y[2] - Mu*y[3]);
}

void rk4(double x, double y[N], double h)
{	int i;	
	double k1[N], k2[N], k3[N], k4[N], yp[N], ypp[N];
    	equacoes f[] = {f0,f1,f2,f3};
    	
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
    	double a, b, h, x, y[N] = {0.80,0.0,0.20,0.0};
    
    	a = 0;
    	b = 365;
    	h = 0.1;
    	x = a;
    
    	for(x=a;x<b;x+=h)
    	{	printf("%g ", x);
        
        	for(i=0;i<N;i++)  printf("%g ", y[i]);
        
        	rk4(x,y,h);
        	puts("");
    	}
}
