<h1>Modelagem de Doenças Infecciosas: ABM x EDO</h1>

Objetivo: Implementar modelos de doenças infecciosas em Equações Diferenciais Ordinárias (EDO) e Modelagem Baseada em Agentes (em inglês, ABM). Utilizamos três tipos de modelos: 

<p>Suscetível-Infectado-Suscetível: depois da infecção, o indivíduo retorna para suscetível, onde pode ser infectado novamente;</p>
<p>Suscetível-Infectado-Recuperado: após a infecção, o indivíduo vai para um estágio de recuperado, não desenvolvendo mais a doença. </p>
<p>Suscetível-Exposto-Infectado-Recuperado: o indivíduo suscetível, ao ser infectado, vai para um estágio exposto, possuindo o patógeno, porém não desenvolvendo a forma ativa da doença e nem transmitindo. Após este estágio, ele migra para infectado, e depois da infecção vai para o estágio recuperado.</p>

<h1>Status</h1>

 O projeto encontra-se com o seu propósito alcançado. Onde conseguimos implementar as duas ferramentas neste caso, e conseguimos comparar as duas simulações.

 <h1>Tecnologias Utilizadas</h1>
 Linguagem: C, Shell Script
 Compilador: GCC 9.4.0
 Gnuplot: 5.2