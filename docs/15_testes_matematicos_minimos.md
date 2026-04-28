# 15. Testes matemáticos mínimos

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md)

> **Documento complementar da Fase 2.**
> Este arquivo define o contrato dos testes matemáticos que deverão ser implementados futuramente em C++ para validar a base geométrica, as funções nodais e as funções de forma de aresta usadas na reprodução do artigo de Koshiba e Inoue.
> Não contém código.

---

## 1. Objetivo deste documento

Antes de implementar a montagem das matrizes locais e globais, é necessário garantir que a geometria elementar e as funções de forma estejam corretas. Pequenos erros em área, orientação, coeficientes nodais, gradientes ou sinais de aresta podem produzir matrizes inconsistentes, autovalores errados ou campos com descontinuidade tangencial.

Este documento define os testes mínimos para validar:

1. triângulo linear de referência;
2. orientação positiva e negativa;
3. centroide;
4. coeficientes $a_k$, $b_k$, $c_k$;
5. funções nodais lineares $L_k$;
6. partição da unidade;
7. valores nodais nos vértices;
8. gradientes das funções nodais;
9. funções de forma de aresta $\{U\}$ e $\{V\}$;
10. orientação de aresta;
11. continuidade tangencial em aresta compartilhada.

Esses testes são pré-requisitos para a implementação segura das matrizes locais associadas às Equações (29), (30), (32) e (33) do artigo.

---

## 2. Triângulo de referência adotado

O triângulo principal para os testes mínimos será:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

A ordenação local $(1,2,3)$ é anti-horária. Logo, a área assinada deve ser positiva:

$$
2A_e =
\begin{vmatrix}
1 & 1 & 1 \\
x_1 & x_2 & x_3 \\
y_1 & y_2 & y_3
\end{vmatrix}
= 1,
$$

portanto:

$$
A_e = \frac{1}{2}.
$$

Para esse triângulo, as funções nodais esperadas são:

$$
L_1(x,y)=1-x-y,
$$

$$
L_2(x,y)=x,
$$

$$
L_3(x,y)=y.
$$

E seus gradientes são:

$$
\nabla L_1=(-1,-1),
\qquad
\nabla L_2=(1,0),
\qquad
\nabla L_3=(0,1).
$$

---

## 3. Convenção cíclica usada nos testes

A convenção local segue o artigo. Para cada índice $k$, os índices $l$ e $m$ avançam ciclicamente ao redor dos três vértices:

| $k$ | $l$ | $m$ |
| --- | --- | --- |
| 1 | 2 | 3 |
| 2 | 3 | 1 |
| 3 | 1 | 2 |

Com essa convenção, os coeficientes das funções nodais são:

$$
a_k = x_l y_m - x_m y_l,
$$

$$
b_k = y_l - y_m,
$$

$$
c_k = x_m - x_l.
$$

As funções nodais lineares são dadas por:

$$
L_k(x,y)=\frac{a_k+b_kx+c_ky}{2A_e}.
$$

Os gradientes são constantes dentro do elemento:

$$
\frac{\partial L_k}{\partial x}=\frac{b_k}{2A_e},
\qquad
\frac{\partial L_k}{\partial y}=\frac{c_k}{2A_e}.
$$

---

## 4. Tolerâncias numéricas iniciais

Para a primeira implementação, recomenda-se começar com tolerâncias rígidas, pois os testes usam triângulos simples e resultados exatos em aritmética real.

| Tipo de teste | Tolerância sugerida |
| --- | --- |
| Área, centroide e coeficientes inteiros | $10^{-14}$ |
| Valores de funções nodais | $10^{-13}$ |
| Gradientes nodais | $10^{-13}$ |
| Propriedades de soma/partição da unidade | $10^{-13}$ |
| Funções de aresta em pontos de teste | $10^{-12}$ |
| Continuidade tangencial entre elementos | $10^{-12}$ |

> **Nota de verificação:** essas tolerâncias devem ser relaxadas se a implementação futura usar malhas muito distorcidas, coordenadas em escalas físicas muito pequenas/grandes ou quadratura numérica em vez de fórmulas fechadas.

---

## 5. Teste 1 — Cálculo da área do triângulo

### Objetivo

Verificar se a implementação calcula corretamente a área assinada e a área geométrica do triângulo.

### Entrada

Triângulo de referência:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

### Resultado esperado

Determinante:

$$
2A_e = 1.
$$

Área assinada:

$$
A_e^{\text{signed}}=\frac{1}{2}.
$$

Área geométrica:

$$
|A_e|=\frac{1}{2}.
$$

### Tolerância sugerida

$$
10^{-14}.
$$

### Equações relacionadas

Equação (9) do artigo.

---

## 6. Teste 2 — Orientação positiva e negativa

### Objetivo

Garantir que a implementação reconheça a orientação local do triângulo e trate corretamente triângulos em ordem anti-horária e horária.

### Entrada positiva

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

### Resultado esperado positivo

$$
2A_e=1,
\qquad
A_e^{\text{signed}}>0.
$$

### Entrada negativa

$$
P_1=(0,0),
\qquad
P_2=(0,1),
\qquad
P_3=(1,0).
$$

### Resultado esperado negativo

$$
2A_e=-1,
\qquad
A_e^{\text{signed}}<0,
\qquad
|A_e|=\frac{1}{2}.
$$

### Tolerância sugerida

$$
10^{-14}.
$$

### Equações relacionadas

Equação (9) do artigo.

### Observação para a implementação futura

A implementação deverá decidir uma das duas políticas:

1. rejeitar triângulos com orientação negativa;
2. reordenar automaticamente os vértices para orientação positiva;
3. armazenar a orientação e usar sinais de aresta de modo consistente.

Para a Fase 3 inicial, a opção mais segura é exigir triângulos locais anti-horários.

---

## 7. Teste 3 — Centroide

### Objetivo

Validar o cálculo do centroide do triângulo, usado nas integrais do Apêndice.

### Entrada

Triângulo de referência:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

### Resultado esperado

$$
x_c=\frac{x_1+x_2+x_3}{3}=\frac{1}{3},
$$

$$
y_c=\frac{y_1+y_2+y_3}{3}=\frac{1}{3}.
$$

### Tolerância sugerida

$$
10^{-14}.
$$

### Equações relacionadas

Equações (A9) e (A10) do Apêndice.

---

## 8. Teste 4 — Coeficientes $a_k$, $b_k$, $c_k$

### Objetivo

Verificar os coeficientes das funções nodais lineares.

### Entrada

Triângulo de referência:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

### Resultado esperado

Para $k=1$, com $(l,m)=(2,3)$:

$$
a_1=1,
\qquad
b_1=-1,
\qquad
c_1=-1.
$$

Para $k=2$, com $(l,m)=(3,1)$:

$$
a_2=0,
\qquad
b_2=1,
\qquad
c_2=0.
$$

Para $k=3$, com $(l,m)=(1,2)$:

$$
a_3=0,
\qquad
b_3=0,
\qquad
c_3=1.
$$

### Tolerância sugerida

$$
10^{-14}.
$$

### Equações relacionadas

Equações (10), (11) e (12) do artigo.

---

## 9. Teste 5 — Funções nodais lineares

### Objetivo

Verificar se as funções nodais são calculadas corretamente a partir de $a_k$, $b_k$, $c_k$ e $A_e$.

### Entrada

Triângulo de referência e pontos internos:

$$
P=(0,0),
\qquad
P=\left(\frac{1}{2},0\right),
\qquad
P=\left(0,\frac{1}{2}\right),
\qquad
P=\left(\frac{1}{3},\frac{1}{3}\right).
$$

### Resultado esperado

As funções devem ser:

$$
L_1(x,y)=1-x-y,
\qquad
L_2(x,y)=x,
\qquad
L_3(x,y)=y.
$$

No centroide:

$$
L_1\left(\frac{1}{3},\frac{1}{3}\right)=\frac{1}{3},
$$

$$
L_2\left(\frac{1}{3},\frac{1}{3}\right)=\frac{1}{3},
$$

$$
L_3\left(\frac{1}{3},\frac{1}{3}\right)=\frac{1}{3}.
$$

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equação (8) do artigo.

---

## 10. Teste 6 — Partição da unidade

### Objetivo

Verificar a propriedade fundamental das funções nodais lineares:

$$
L_1(x,y)+L_2(x,y)+L_3(x,y)=1.
$$

### Entrada

Usar vários pontos dentro do triângulo de referência, por exemplo:

$$
(0,0),
\qquad
(1,0),
\qquad
(0,1),
\qquad
\left(\frac{1}{3},\frac{1}{3}\right),
\qquad
\left(\frac{1}{4},\frac{1}{4}\right).
$$

### Resultado esperado

Para todos os pontos testados:

$$
\sum_{k=1}^{3}L_k(x,y)=1.
$$

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equação (8) do artigo.

---

## 11. Teste 7 — Valor das funções nodais nos vértices

### Objetivo

Verificar a propriedade interpolatória nodal:

$$
L_i(P_j)=\delta_{ij}.
$$

### Entrada

Vértices do triângulo de referência:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(0,1).
$$

### Resultado esperado

Matriz de valores nodais:

$$
\begin{bmatrix}
L_1(P_1) & L_1(P_2) & L_1(P_3) \\
L_2(P_1) & L_2(P_2) & L_2(P_3) \\
L_3(P_1) & L_3(P_2) & L_3(P_3)
\end{bmatrix}
=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1
\end{bmatrix}.
$$

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equação (8) do artigo.

---

## 12. Teste 8 — Gradientes das funções nodais

### Objetivo

Verificar se os gradientes das funções nodais são constantes no elemento e compatíveis com $b_k$ e $c_k$.

### Entrada

Triângulo de referência.

### Resultado esperado

A fórmula geral é:

$$
\frac{\partial L_k}{\partial x}=\frac{b_k}{2A_e},
\qquad
\frac{\partial L_k}{\partial y}=\frac{c_k}{2A_e}.
$$

Para o triângulo de referência, $2A_e=1$, portanto $b_k/(2A_e)=b_k$. Substituindo explicitamente:

$$
\frac{\partial L_1}{\partial x}=\frac{b_1}{2A_e}=\frac{-1}{1}=-1,
\qquad
\frac{\partial L_1}{\partial y}=\frac{c_1}{2A_e}=\frac{-1}{1}=-1,
$$

$$
\frac{\partial L_2}{\partial x}=\frac{b_2}{2A_e}=\frac{1}{1}=1,
\qquad
\frac{\partial L_2}{\partial y}=\frac{c_2}{2A_e}=\frac{0}{1}=0,
$$

$$
\frac{\partial L_3}{\partial x}=\frac{b_3}{2A_e}=\frac{0}{1}=0,
\qquad
\frac{\partial L_3}{\partial y}=\frac{c_3}{2A_e}=\frac{1}{1}=1.
$$

> **Atenção para a implementação:** a igualdade $\partial L_k/\partial x = b_k$ é acidental neste triângulo porque $2A_e = 1$. Em geral, $\partial L_k/\partial x = b_k/(2A_e)$. Omitir o denominador produzirá erros em qualquer triângulo com área diferente de $1/2$.

Portanto:

$$
\nabla L_1=(-1,-1),
\qquad
\nabla L_2=(1,0),
\qquad
\nabla L_3=(0,1).
$$

Também deve valer:

$$
\nabla L_1+\nabla L_2+\nabla L_3=(0,0).
$$

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equações (8), (10), (11), (12), (A7) e (A8).

---

## 13. Teste 9 — Funções de forma de aresta

### Objetivo

Validar que as funções de forma de aresta reproduzem corretamente os graus de liberdade tangenciais nos pontos laterais e nas arestas do triângulo.

### Entrada

Triângulo de referência com pontos laterais adotados como pontos médios das arestas:

$$
P_4=\frac{P_1+P_2}{2}=\left(\frac{1}{2},0\right),
$$

$$
P_5=\frac{P_2+P_3}{2}=\left(\frac{1}{2},\frac{1}{2}\right),
$$

$$
P_6=\frac{P_3+P_1}{2}=\left(0,\frac{1}{2}\right).
$$

As funções de forma de aresta são escritas no artigo como:

$$
\phi_x = \{U(y)\}^T\{\phi_t\}_e,
$$

$$
\phi_y = \{V(x)\}^T\{\phi_t\}_e,
$$

com:

$$
\{U\}=\begin{bmatrix}
\bar{a}_1+\bar{c}_1y \\
\bar{a}_2+\bar{c}_2y \\
\bar{a}_3+\bar{c}_3y
\end{bmatrix},
$$

$$
\{V\}=\begin{bmatrix}
\bar{b}_1-\bar{c}_1x \\
\bar{b}_2-\bar{c}_2x \\
\bar{b}_3-\bar{c}_3x
\end{bmatrix}.
$$

### Resultado esperado

Para cada grau de liberdade de aresta local $r$, ao definir:

$$
\{\phi_t\}_e = \mathbf{e}_r,
$$

onde $\mathbf{e}_r$ é o vetor canônico local, a componente tangencial avaliada na aresta $s$ deve satisfazer:

$$
\phi_t^{(r)}\big|_{\text{aresta }s}=\delta_{rs}.
$$

Como o artigo afirma que a componente tangencial é constante em cada lado do triângulo, esse teste deve ser feito não apenas no ponto médio da aresta, mas também em pelo menos outro ponto do mesmo lado.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equações (13), (14), (15), (16), (17), (18), (19), (20) e (21) do artigo.

### Nota de verificação

> A associação exata entre os pontos laterais $4$, $5$, $6$ e as arestas locais deve ser conferida contra a Figura 1 do artigo e contra a definição dos ângulos $\theta_{k+3}$. Para os testes iniciais, recomenda-se adotar explicitamente uma convenção local e mantê-la documentada no código e nos testes.

### Convenção de θ adotada por este projeto

O artigo define $0\leq\theta_{k+3}<\pi$ por meio de $\theta_{k+3}=\arctan\!\bigl((y_k-y_l)/(x_k-x_l)\bigr)$. Para a implementação, adota-se a seguinte regra computacional:

1. Calcular $\theta = \operatorname{atan2}(y_k-y_l,\, x_k-x_l)$.
2. Se $\theta < 0$, somar $\pi$: $\theta \leftarrow \theta+\pi$.
3. Se $\theta = \pi$ (exatamente, caso da aresta horizontal apontando para a esquerda), tratar como $\theta = 0$.

O passo 3 é necessário porque $\arctan(0)=0$ independentemente do sinal do denominador, enquanto $\operatorname{atan2}(0,-1)=\pi$. O limite superior do artigo é estrito ($\theta<\pi$), portanto $\theta=\pi$ deve ser mapeado para $0$.

### Valores esperados para o triângulo de referência

Para $P_1=(0,0)$, $P_2=(1,0)$, $P_3=(0,1)$ com pontos médios $P_4=(\tfrac{1}{2},0)$, $P_5=(\tfrac{1}{2},\tfrac{1}{2})$, $P_6=(0,\tfrac{1}{2})$:

**Ângulos das arestas:**

$$
\theta_4 = 0
\quad\text{(aresta }1\text{–}2\text{, horizontal; }\operatorname{atan2}(0,-1)=\pi\text{ mapeado para }0\text{)},
$$

$$
\theta_5 = \frac{3\pi}{4}
\quad\text{(aresta }2\text{–}3\text{; }\operatorname{atan2}(-1,1)=-\tfrac{\pi}{4}\to+\pi=\tfrac{3\pi}{4}\text{)},
$$

$$
\theta_6 = \frac{\pi}{2}
\quad\text{(aresta }3\text{–}1\text{, vertical; }\operatorname{atan2}(1,0)=\tfrac{\pi}{2}\text{)}.
$$

**Coeficientes das funções de aresta:**

$$
\bar{a}_1=1,\quad\bar{b}_1=0,\quad\bar{c}_1=-1
\quad\Rightarrow\quad
U_1(y)=1-y,\quad V_1(x)=x.
$$

$$
\bar{a}_2=0,\quad\bar{b}_2=0,\quad\bar{c}_2=-\sqrt{2}
\quad\Rightarrow\quad
U_2(y)=-\sqrt{2}\,y,\quad V_2(x)=\sqrt{2}\,x.
$$

$$
\bar{a}_3=0,\quad\bar{b}_3=1,\quad\bar{c}_3=1
\quad\Rightarrow\quad
U_3(y)=y,\quad V_3(x)=1-x.
$$

> **Nota:** $\bar{c}_2=-\sqrt{2}$ é irracional (comprimento da hipotenusa dividido por $\sqrt{2}$). Isso é correto e esperado.

**Determinante do sistema de interpolação:**

$$
\Delta = \frac{\sqrt{2}}{2} > 0.
$$

O valor positivo de $\Delta$ é esperado para este triângulo com a convenção $\theta_4=0$. A implementação deve rejeitar elementos com $|\Delta|\approx 0$.

**Derivadas das funções de aresta:**

$$
\{U_y\}
=
\begin{bmatrix}\bar{c}_1\\\bar{c}_2\\\bar{c}_3\end{bmatrix}
=
\begin{bmatrix}-1\\-\sqrt{2}\\1\end{bmatrix},
\qquad
\{V_x\}
=
\begin{bmatrix}-\bar{c}_1\\-\bar{c}_2\\-\bar{c}_3\end{bmatrix}
=
\begin{bmatrix}1\\\sqrt{2}\\-1\end{bmatrix}.
$$

**Verificação da condição de interpolação tangencial:**

Para $r=1$ ($\phi_t^{(1)}=1$ na aresta 4, $0$ nas demais), avaliar nos pontos médios:

$$
\phi_t^{(1)}\big|_{P_4}
=U_1\!\bigl(\tfrac{1}{2}\cdot 0\bigr)\cos 0+V_1\!\bigl(\tfrac{1}{2}\bigr)\sin 0
=1\cdot 1+\tfrac{1}{2}\cdot 0=1\;\checkmark,
$$

$$
\phi_t^{(1)}\big|_{P_5}
=U_1\!\bigl(\tfrac{1}{2}\bigr)\cos\tfrac{3\pi}{4}+V_1\!\bigl(\tfrac{1}{2}\bigr)\sin\tfrac{3\pi}{4}
=\tfrac{1}{2}\cdot\bigl(-\tfrac{\sqrt{2}}{2}\bigr)+\tfrac{1}{2}\cdot\tfrac{\sqrt{2}}{2}=0\;\checkmark,
$$

$$
\phi_t^{(1)}\big|_{P_6}
=U_1\!\bigl(\tfrac{1}{2}\bigr)\cos\tfrac{\pi}{2}+V_1\!\bigl(0\bigr)\sin\tfrac{\pi}{2}
=\tfrac{1}{2}\cdot 0+0\cdot 1=0\;\checkmark.
$$

---

## 14. Teste 10 — Orientação de aresta

### Objetivo

Garantir que a orientação local das arestas seja tratada de maneira consistente, especialmente quando uma mesma aresta global é compartilhada por dois triângulos.

### Entrada

Dois triângulos que compartilham uma aresta:

$$
T_1: (0,0), (1,0), (0,1),
$$

$$
T_2: (1,0), (1,1), (0,1).
$$

A aresta compartilhada é:

$$
(1,0) \leftrightarrow (0,1).
$$

### Resultado esperado

A implementação deve identificar que a mesma aresta global aparece em dois elementos. Se as orientações locais forem opostas, o sinal do grau de liberdade de aresta deve ser ajustado.

O teste deve verificar que uma grandeza tangencial orientada globalmente muda de sinal quando a orientação local da aresta é invertida:

$$
\phi_t^{\text{local}} = s_e \, \phi_t^{\text{global}},
\qquad
s_e\in\{-1,+1\}.
$$

**Valores esperados para a aresta compartilhada $(1,0)\leftrightarrow(0,1)$ entre $T_1$ e $T_2$:**

Usando a convenção deste projeto ($\operatorname{atan2}$ mapeado para $[0,\pi)$), o ângulo da aresta para ambos os elementos é $\theta=3\pi/4$, pois:

- Em $T_1$, aresta $5$ (k=2, l=3): $(x_2-x_3,y_2-y_3)=(1,-1)$, $\operatorname{atan2}(-1,1)=-\pi/4\to+\pi=3\pi/4$.
- Em $T_2$, aresta $6$ (k=3, l=1): $(x_3'-x_1',y_3'-y_1')=(-1,1)$, $\operatorname{atan2}(1,-1)=3\pi/4$.

Como ambos produzem o mesmo ângulo, não há inversão de sinal para este par de triângulos nesta convenção: $s_{e}=+1$. A componente tangencial calculada por $T_1$ e por $T_2$ deve ser idêntica para o mesmo grau de liberdade global.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equações (13) a (21), especialmente a definição:

$$
\phi_t=\phi_x\cos\theta+\phi_y\sin\theta.
$$

### Nota de verificação

> O artigo usa $0\leq\theta_{k+3}<\pi$, o que sugere que a direção da reta da aresta é tratada com uma convenção angular específica. Para uma implementação global em malhas arbitrárias, a orientação da aresta precisa ser definida de forma inequívoca, por exemplo, a partir do menor para o maior índice global do nó, ou por uma convenção geométrica fixa.

---

## 15. Teste 11 — Continuidade tangencial em aresta compartilhada

### Objetivo

Validar a propriedade fundamental dos elementos de aresta: continuidade da componente tangencial entre elementos vizinhos.

### Entrada

Usar os dois triângulos:

$$
T_1: (0,0), (1,0), (0,1),
$$

$$
T_2: (1,0), (1,1), (0,1).
$$

Selecionar a aresta compartilhada:

$$
E=(1,0)\leftrightarrow(0,1).
$$

Atribuir o mesmo grau de liberdade global à aresta compartilhada.

### Resultado esperado

A componente tangencial calculada pelo lado de $T_1$ e pelo lado de $T_2$ deve coincidir ao longo da aresta compartilhada:

$$
\phi_t^{T_1}(s)=\phi_t^{T_2}(s),
\qquad
s\in E.
$$

Se as orientações locais forem opostas, a igualdade deve ser obtida após aplicar o fator de sinal local-global:

$$
s_1\phi_t^{T_1}(s)=s_2\phi_t^{T_2}(s).
$$

### Pontos de avaliação sugeridos

Ponto médio da aresta:

$$
P_m=\left(\frac{1}{2},\frac{1}{2}\right).
$$

Outro ponto na aresta:

$$
P_q=\left(\frac{3}{4},\frac{1}{4}\right).
$$

### Valores esperados concretos

Para a aresta compartilhada $(1,0)\leftrightarrow(0,1)$ com $\theta=3\pi/4$, se o grau de liberdade global da aresta for $\phi_t^{\text{global}}=1$:

- Em $T_1$ (aresta local $5$, funções de $k=2$): $U_2^{T_1}(y)=-\sqrt{2}\,y$, $V_2^{T_1}(x)=\sqrt{2}\,x$.

  $$
  \phi_t^{T_1}\big|_{P_m}
  =(-\tfrac{\sqrt{2}}{2})\cos\tfrac{3\pi}{4}+(\tfrac{\sqrt{2}}{2})\sin\tfrac{3\pi}{4}
  =(-\tfrac{\sqrt{2}}{2})(-\tfrac{\sqrt{2}}{2})+(\tfrac{\sqrt{2}}{2})(\tfrac{\sqrt{2}}{2})
  =\tfrac{1}{2}+\tfrac{1}{2}=1.
  $$

  $$
  \phi_t^{T_1}\big|_{P_q}
  =(-\tfrac{\sqrt{2}}{4})(-\tfrac{\sqrt{2}}{2})+(\tfrac{3\sqrt{2}}{4})(\tfrac{\sqrt{2}}{2})
  =\tfrac{1}{4}+\tfrac{3}{4}=1.
  $$

- Em $T_2$ (aresta local $6$, funções de $k=3$): $U_3^{T_2}(y)=-\sqrt{2}+2\sqrt{2}\,y$, $V_3^{T_2}(x)=2\sqrt{2}-2\sqrt{2}\,x$.

  $$
  \phi_t^{T_2}\big|_{P_m}
  =(0)(-\tfrac{\sqrt{2}}{2})+(\sqrt{2})(\tfrac{\sqrt{2}}{2})
  =0+1=1.
  $$

  $$
  \phi_t^{T_2}\big|_{P_q}
  =(-\tfrac{\sqrt{2}}{2})(-\tfrac{\sqrt{2}}{2})+(\tfrac{\sqrt{2}}{2})(\tfrac{\sqrt{2}}{2})
  =\tfrac{1}{2}+\tfrac{1}{2}=1.
  $$

A componente tangencial é constante e igual a $1$ em toda a aresta, tanto por $T_1$ quanto por $T_2$. Isso confirma a continuidade tangencial e a constância da componente ao longo da aresta.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equações (13) a (21) do artigo e a afirmação de que a componente tangencial é contínua ao longo das fronteiras entre elementos.

---

## 16. Teste 12 — Consistência entre derivadas e integrais básicas

### Objetivo

Fazer uma ponte entre os testes de funções de forma e as futuras integrais locais do Apêndice.

### Entrada

Triângulo de referência.

### Resultado esperado

Como os gradientes nodais são constantes, as integrais de rigidez nodal devem satisfazer:

$$
\iint_e \{N_x\}\{N_x\}^T dxdy
= \frac{1}{4A_e}
\begin{bmatrix}
b_1^2 & b_1b_2 & b_1b_3 \\
b_1b_2 & b_2^2 & b_2b_3 \\
b_1b_3 & b_2b_3 & b_3^2
\end{bmatrix},
$$

$$
\iint_e \{N_y\}\{N_y\}^T dxdy
= \frac{1}{4A_e}
\begin{bmatrix}
c_1^2 & c_1c_2 & c_1c_3 \\
c_1c_2 & c_2^2 & c_2c_3 \\
c_1c_3 & c_2c_3 & c_3^2
\end{bmatrix}.
$$

Para o triângulo de referência, com $A_e=1/2$ e $(b_1,b_2,b_3)=(-1,1,0)$, $(c_1,c_2,c_3)=(-1,0,1)$:

$$
\frac{1}{4A_e}=\frac{1}{2}.
$$

Portanto, as matrizes numéricas explícitas para este triângulo são:

$$
\iint_e \{N_x\}\{N_x\}^T\,dxdy
=
\frac{1}{2}
\begin{bmatrix}
(-1)^2 & (-1)(1) & (-1)(0)\\
(1)(-1) & (1)^2 & (1)(0)\\
(0)(-1) & (0)(1) & (0)^2
\end{bmatrix}
=
\frac{1}{2}
\begin{bmatrix}
1 & -1 & 0\\
-1 & 1 & 0\\
0 & 0 & 0
\end{bmatrix}.
$$

$$
\iint_e \{N_y\}\{N_y\}^T\,dxdy
=
\frac{1}{2}
\begin{bmatrix}
(-1)^2 & (-1)(0) & (-1)(1)\\
(0)(-1) & (0)^2 & (0)(1)\\
(1)(-1) & (1)(0) & (1)^2
\end{bmatrix}
=
\frac{1}{2}
\begin{bmatrix}
1 & 0 & -1\\
0 & 0 & 0\\
-1 & 0 & 1
\end{bmatrix}.
$$

> **Nota:** a linha e coluna 3 de A7 são nulas porque $b_3=0$; a linha e coluna 2 de A8 são nulas porque $c_2=0$. Isso é esperado para este triângulo. Para testar entradas não nulas nessas posições, use o segundo triângulo de teste definido na Seção 22.

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equações (A7) e (A8) do Apêndice.

---

## 17. Teste 13 — Massa nodal consistente

### Objetivo

Validar a integral básica das funções nodais que futuramente será usada na montagem de $[M_{zz}]$ e em termos envolvendo $\{N\}\{N\}^T$.

### Entrada

Triângulo de referência.

### Resultado esperado

A matriz de massa nodal consistente deve ser:

$$
\iint_e \{N\}\{N\}^T dxdy
=
\frac{A_e}{12}
\begin{bmatrix}
2 & 1 & 1 \\
1 & 2 & 1 \\
1 & 1 & 2
\end{bmatrix}.
$$

Como $A_e=1/2$:

$$
\frac{A_e}{12}=\frac{1}{24}.
$$

Logo a matriz numérica explícita para o triângulo de referência é:

$$
\iint_e \{N\}\{N\}^T dxdy
=
\frac{1}{24}
\begin{bmatrix}
2 & 1 & 1 \\
1 & 2 & 1 \\
1 & 1 & 2
\end{bmatrix}.
$$

A soma de cada linha deve ser $4A_e/12 = A_e/3 = 1/6$: verificar que $(2+1+1)/24 = 4/24 = 1/6$. Isso valida a montagem sem precisar calcular o integral diretamente.

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equação (A6) do Apêndice.

---

## 18. Teste 14 — Degeneração geométrica

### Objetivo

Garantir que triângulos degenerados sejam detectados antes da montagem das funções de forma e matrizes locais.

### Entrada

Três pontos colineares:

$$
P_1=(0,0),
\qquad
P_2=(1,0),
\qquad
P_3=(2,0).
$$

### Resultado esperado

A área assinada deve ser:

$$
A_e=0.
$$

A implementação futura deve rejeitar o elemento antes de calcular:

$$
\frac{1}{2A_e},
\qquad
\frac{1}{4A_e},
\qquad
\nabla L_k.
$$

### Tolerância sugerida

Triângulo degenerado se:

$$
|2A_e| < 10^{-14}
$$

para coordenadas de ordem unitária.

### Equações relacionadas

Equações (8), (9), (A7) e (A8).

---

## 19. Teste 15 — Derivadas das funções de aresta $\{U_y\}$ e $\{V_x\}$

### Objetivo

Verificar que as derivadas das funções de aresta são calculadas corretamente e correspondem aos vetores $\bar{c}$ do elemento. Esses valores alimentam diretamente a montagem de $[B]$ e as integrais da Equação (A3).

### Entrada

Triângulo de referência com os coeficientes de aresta do Teste 9.

### Resultado esperado

$$
\{U_y\}
=
\begin{bmatrix}\bar{c}_1\\\bar{c}_2\\\bar{c}_3\end{bmatrix}
=
\begin{bmatrix}-1\\-\sqrt{2}\\1\end{bmatrix},
$$

$$
\{V_x\}
=
\begin{bmatrix}-\bar{c}_1\\-\bar{c}_2\\-\bar{c}_3\end{bmatrix}
=
\begin{bmatrix}1\\\sqrt{2}\\-1\end{bmatrix}.
$$

Verificar também a relação $\{V_x\}=-\{U_y\}$ componente a componente.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equações (13)–(16) do artigo; Equação (A3) do Apêndice.

---

## 20. Teste 16 — Integral A3 e fator 4 no rotacional

### Objetivo

Verificar a integral $\iint_e \{U_y\}\{U_y\}^T\,dxdy = A_e\bar{c}_k\bar{c}_l$ e confirmar que a combinação $(-\{U_y\}+\{V_x\})$ ao quadrado produz exatamente $4$ vezes essa integral.

### Entrada

Triângulo de referência.

### Resultado esperado

**Integral A3:**

$$
\iint_e \{U_y\}\{U_y\}^T\,dxdy
=
A_e\bar{c}\bar{c}^T
=
\frac{1}{2}
\begin{bmatrix}
1 & \sqrt{2} & -1 \\
\sqrt{2} & 2 & -\sqrt{2} \\
-1 & -\sqrt{2} & 1
\end{bmatrix}.
$$

**Fator 4:** como $-\{U_y\}+\{V_x\}=-\bar{c}-\bar{c}=-2\bar{c}$ componente a componente, tem-se:

$$
\iint_e(-\{U_y\}+\{V_x\})(-\{U_y\}+\{V_x\})^T\,dxdy
=
4A_e\bar{c}\bar{c}^T
=
\begin{bmatrix}
2 & 2\sqrt{2} & -2 \\
2\sqrt{2} & 4 & -2\sqrt{2} \\
-2 & -2\sqrt{2} & 2
\end{bmatrix}.
$$

Verificar também que $\iint_e \{V_x\}\{V_x\}^T\,dxdy = A_e\bar{c}\bar{c}^T$ (mesma matriz que A3).

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equação (A3) do Apêndice; Equações (29a) e (32a) do artigo.

---

## 21. Teste 17 — Integrais de acoplamento A4 e A5

### Objetivo

Verificar as integrais de acoplamento aresta–nó $\iint_e\{U\}\{N_x\}^T$ (A4) e $\iint_e\{V\}\{N_y\}^T$ (A5), que alimentam os blocos $[K_{tz}]$.

### Entrada

Triângulo de referência. Com $y_c=1/3$, $x_c=1/3$, $(b_1,b_2,b_3)=(-1,1,0)$, $(c_1,c_2,c_3)=(-1,0,1)$, $\bar{a}=(1,0,0)$, $\bar{b}=(0,0,1)$, $\bar{c}=(-1,-\sqrt{2},1)$.

### Resultado esperado

**A4** — $\left[\iint_e\{U\}\{N_x\}^T\,dxdy\right]_{kl} = \frac{1}{2}(\bar{a}_k+\bar{c}_ky_c)b_l$:

$$
\frac{1}{2}(\bar{a}_k+\bar{c}_ky_c)
=
\frac{1}{2}\begin{bmatrix}1+(-1)(1/3)\\0+(-\sqrt{2})(1/3)\\0+(1)(1/3)\end{bmatrix}
=
\begin{bmatrix}1/3\\-\sqrt{2}/6\\1/6\end{bmatrix}.
$$

$$
A_4
=
\begin{bmatrix}
-1/3 & 1/3 & 0\\
\sqrt{2}/6 & -\sqrt{2}/6 & 0\\
-1/6 & 1/6 & 0
\end{bmatrix}.
$$

A coluna 3 é nula porque $b_3=0$.

**A5** — $\left[\iint_e\{V\}\{N_y\}^T\,dxdy\right]_{kl} = \frac{1}{2}(\bar{b}_k-\bar{c}_kx_c)c_l$:

$$
\frac{1}{2}(\bar{b}_k-\bar{c}_kx_c)
=
\frac{1}{2}\begin{bmatrix}0-(-1)(1/3)\\0-(-\sqrt{2})(1/3)\\1-(1)(1/3)\end{bmatrix}
=
\begin{bmatrix}1/6\\\sqrt{2}/6\\1/3\end{bmatrix}.
$$

$$
A_5
=
\begin{bmatrix}
-1/6 & 0 & 1/6\\
-\sqrt{2}/6 & 0 & \sqrt{2}/6\\
-1/3 & 0 & 1/3
\end{bmatrix}.
$$

A coluna 2 é nula porque $c_2=0$.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equações (A4) e (A5) do Apêndice; Equações (29b) e (32b) do artigo.

---

## 22. Teste 18 — Triângulo geral com todos $b_k$, $c_k$ não nulos

### Objetivo

Verificar as integrais A7 e A8 em um triângulo onde nenhum $b_k$ ou $c_k$ é zero. O triângulo de referência tem $b_3=0$ e $c_2=0$, de modo que os Testes 12–13 não exercitam as entradas correspondentes. Bugs nessas posições passam silenciosamente.

### Entrada

$$
P_1=(0,0),\quad P_2=(2,1),\quad P_3=(1,3).
$$

Coeficientes (convenção $k,l,m$ cíclica):

$$
a_1=5,\ b_1=-2,\ c_1=-1;\quad
a_2=0,\ b_2=3,\ c_2=-1;\quad
a_3=0,\ b_3=-1,\ c_3=2.
$$

Área:

$$
2A_e = 5,\qquad A_e = \frac{5}{2}.
$$

Centroide:

$$
x_c=\frac{0+2+1}{3}=1,\qquad y_c=\frac{0+1+3}{3}=\frac{4}{3}.
$$

### Resultado esperado

**A7** com $1/(4A_e)=1/10$:

$$
\iint_e\{N_x\}\{N_x\}^T\,dxdy
=
\frac{1}{10}
\begin{bmatrix}
4 & -6 & 2\\
-6 & 9 & -3\\
2 & -3 & 1
\end{bmatrix}.
$$

**A8** com $1/(4A_e)=1/10$:

$$
\iint_e\{N_y\}\{N_y\}^T\,dxdy
=
\frac{1}{10}
\begin{bmatrix}
1 & 1 & -2\\
1 & 1 & -2\\
-2 & -2 & 4
\end{bmatrix}.
$$

Nenhuma linha ou coluna é nula; todas as entradas devem ser testadas.

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equações (A7) e (A8) do Apêndice.

---

## 23. Teste 19 — Efeito da orientação horária sobre A7 e A8

### Objetivo

Confirmar que usar $A_e$ com sinal negativo (triângulo em ordem horária) produz diagonal negativa nas matrizes A7 e A8 — resultado fisicamente incorreto. Este teste documenta por que a implementação deve usar $|A_e|$ ou reordenar vértices.

### Entrada

Triângulo em ordem **horária**:

$$
P_1=(0,0),\quad P_2=(0,1),\quad P_3=(1,0).
$$

### Resultado com sinal errado (usando $A_e$ com sinal)

Área assinada: $A_e^{\mathrm{CW}}=-1/2$.

Coeficientes:

$$
b_1^{\mathrm{CW}}=1,\ b_2^{\mathrm{CW}}=0,\ b_3^{\mathrm{CW}}=-1;\quad
c_1^{\mathrm{CW}}=1,\ c_2^{\mathrm{CW}}=-1,\ c_3^{\mathrm{CW}}=0.
$$

Com $1/(4A_e^{\mathrm{CW}})=1/(4\cdot(-1/2))=-1/2$:

$$
A_7^{\mathrm{CW}}\bigl[\text{sinal errado}\bigr]
=
-\frac{1}{2}
\begin{bmatrix}
1 & 0 & -1\\
0 & 0 & 0\\
-1 & 0 & 1
\end{bmatrix}.
$$

A diagonal é **negativa** ($-1/2$), o que é fisicamente impossível para uma integral de Gram. Este é o diagnóstico de erro.

### Resultado esperado correto

Usando $|A_e^{\mathrm{CW}}|=1/2$:

$$
A_7^{\mathrm{CW}}\bigl[\text{correto}\bigr]
=
\frac{1}{2}
\begin{bmatrix}
1 & 0 & -1\\
0 & 0 & 0\\
-1 & 0 & 1
\end{bmatrix}.
$$

> **Nota:** os $b_k$ e $c_k$ de um triângulo horário satisfazem $b_k^{\mathrm{CW}}=-b_k^{\mathrm{CCW}}$ e $c_k^{\mathrm{CW}}=-c_k^{\mathrm{CCW}}$. O quadrado cancela o sinal, mas o sinal de $A_e$ permanece — daí o resultado negativo. A mitigação obrigatória é usar $|A_e|$ em todas as integrais A1–A3 e A6–A8.

### Tolerância sugerida

$$
10^{-13}.
$$

### Equações relacionadas

Equações (A6)–(A8) do Apêndice; nota de orientação em `docs/13`.

---

## 24. Teste 20 — Integral A1 (massa transversal $\{U\}\{U\}^T$)

### Objetivo

Verificar a integral de massa transversal $\iint_e\{U\}\{U\}^T\,dxdy$ (A1), que alimenta o bloco diagonal $x$–$x$ de $[M_{tt}]$.

### Entrada

Triângulo de referência. $A_e=1/2$, $y_c=1/3$, $\bar{a}=(1,0,0)$, $\bar{c}=(-1,-\sqrt{2},1)$.

Soma de quadrados das ordenadas: $y_1^2+y_2^2+y_3^2+9y_c^2 = 0+0+1+1 = 2$.

### Fórmula simplificada para o triângulo de referência

$$
[A_1]_{kl}
= \frac{1}{2}\bar{a}_k\bar{a}_l
+ \frac{1}{6}(\bar{a}_k\bar{c}_l+\bar{c}_k\bar{a}_l)
+ \frac{1}{12}\bar{c}_k\bar{c}_l.
$$

### Resultado esperado

Calculando entrada por entrada:

| $(k,l)$ | Termo $\bar{a}\bar{a}$ | Termo $\bar{a}\bar{c}+\bar{c}\bar{a}$ | Termo $\bar{c}\bar{c}$ | $[A_1]_{kl}$ |
| --- | --- | --- | --- | --- |
| $(1,1)$ | $1/2$ | $2(-1)/6=-1/3$ | $1/12$ | $1/4$ |
| $(1,2)$ | $0$ | $(-\sqrt{2})/6$ | $\sqrt{2}/12$ | $-\sqrt{2}/12$ |
| $(1,3)$ | $0$ | $1/6$ | $-1/12$ | $1/12$ |
| $(2,2)$ | $0$ | $0$ | $2/12$ | $1/6$ |
| $(2,3)$ | $0$ | $0$ | $-\sqrt{2}/12$ | $-\sqrt{2}/12$ |
| $(3,3)$ | $0$ | $0$ | $1/12$ | $1/12$ |

$$
A_1
=
\begin{bmatrix}
1/4 & -\sqrt{2}/12 & 1/12 \\
-\sqrt{2}/12 & 1/6 & -\sqrt{2}/12 \\
1/12 & -\sqrt{2}/12 & 1/12
\end{bmatrix}.
$$

### Propriedades de verificação

1. **Simetria:** $A_1 = A_1^T$ (verificável de cada par $(k,l)$ acima).
2. **Traço:** $\mathrm{tr}(A_1) = 1/4+1/6+1/12 = 3/12+2/12+1/12 = 1/2 = A_e$.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equação (A1) do Apêndice; bloco $[q]_{xx}$ de $[M_{tt}]$ na Eq. (27).

---

## 25. Teste 21 — Integral A2 (massa transversal $\{V\}\{V\}^T$)

### Objetivo

Verificar a integral de massa transversal $\iint_e\{V\}\{V\}^T\,dxdy$ (A2), que alimenta o bloco diagonal $y$–$y$ de $[M_{tt}]$.

### Entrada

Triângulo de referência. $A_e=1/2$, $x_c=1/3$, $\bar{b}=(0,0,1)$, $\bar{c}=(-1,-\sqrt{2},1)$.

Soma de quadrados das abscissas: $x_1^2+x_2^2+x_3^2+9x_c^2 = 0+1+0+1 = 2$.

### Fórmula simplificada para o triângulo de referência

$$
[A_2]_{kl}
= \frac{1}{2}\bar{b}_k\bar{b}_l
- \frac{1}{6}(\bar{b}_k\bar{c}_l+\bar{c}_k\bar{b}_l)
+ \frac{1}{12}\bar{c}_k\bar{c}_l.
$$

### Resultado esperado

Calculando entrada por entrada:

| $(k,l)$ | Termo $\bar{b}\bar{b}$ | Termo $\bar{b}\bar{c}+\bar{c}\bar{b}$ | Termo $\bar{c}\bar{c}$ | $[A_2]_{kl}$ |
| --- | --- | --- | --- | --- |
| $(1,1)$ | $0$ | $0$ | $1/12$ | $1/12$ |
| $(1,2)$ | $0$ | $0$ | $\sqrt{2}/12$ | $\sqrt{2}/12$ |
| $(1,3)$ | $0$ | $-2(-1)(1)/6=1/3$... | $-1/12$ | $1/12$ |
| $(2,2)$ | $0$ | $0$ | $2/12$ | $1/6$ |
| $(2,3)$ | $0$ | $-(-\sqrt{2})(1)/6=\sqrt{2}/6$ | $-\sqrt{2}/12$ | $\sqrt{2}/12$ |
| $(3,3)$ | $1/2$ | $-2(1)/6=-1/3$ | $1/12$ | $1/4$ |

Detalhe de $(1,3)$: $-\frac{1}{6}[b_1\bar{c}_3+\bar{c}_1 b_3] = -\frac{1}{6}[(0)(1)+(-1)(1)] = +\frac{1}{6}$; soma com $\bar{c}\bar{c}$: $\frac{1}{6}-\frac{1}{12}=\frac{1}{12}$.

$$
A_2
=
\begin{bmatrix}
1/12 & \sqrt{2}/12 & 1/12 \\
\sqrt{2}/12 & 1/6 & \sqrt{2}/12 \\
1/12 & \sqrt{2}/12 & 1/4
\end{bmatrix}.
$$

### Propriedades de verificação

1. **Simetria:** $A_2 = A_2^T$ ✓.
2. **Traço:** $\mathrm{tr}(A_2) = 1/12+1/6+1/4 = 1/12+2/12+3/12 = 1/2 = A_e$.
3. **Simetria estrutural com A1:** $A_2$ é o espelho de $A_1$ com as linhas e colunas em ordem inversa ($1\leftrightarrow3$), refletindo a troca de papéis $\bar{a}\leftrightarrow\bar{b}$ e $y\leftrightarrow x$.

### Tolerância sugerida

$$
10^{-12}.
$$

### Equações relacionadas

Equação (A2) do Apêndice; bloco $[q]_{yy}$ de $[M_{tt}]$ na Eq. (27).

---

## 26. Checklist para testes C++

| Teste | Entidade validada | Entrada mínima | Resultado esperado | Tolerância | Equações relacionadas |
| --- | --- | --- | --- | --- | --- |
| Área positiva | `Triangle` | $(0,0),(1,0),(0,1)$ | $A_e=1/2$ | $10^{-14}$ | (9) |
| Área negativa | `Triangle` | $(0,0),(0,1),(1,0)$ | $A_e^{signed}=-1/2$ | $10^{-14}$ | (9) |
| Centroide | `Triangle` | Triângulo de referência | $(x_c,y_c)=(1/3,1/3)$ | $10^{-14}$ | (A9), (A10) |
| Coeficientes nodais | Funções nodais | Triângulo de referência | $(a,b,c)_1=(1,-1,-1)$, $(0,1,0)$, $(0,0,1)$ | $10^{-14}$ | (10)–(12) |
| Funções nodais | $L_k$ | Pontos internos | $L_1=1-x-y$, $L_2=x$, $L_3=y$ | $10^{-13}$ | (8) |
| Partição da unidade | $L_k$ | Vértices e pontos internos | $L_1+L_2+L_3=1$ | $10^{-13}$ | (8) |
| Kronecker nodal | $L_k(P_j)$ | Vértices | $L_i(P_j)=\delta_{ij}$ | $10^{-13}$ | (8) |
| Gradientes nodais | $\nabla L_k$ | Triângulo de referência | $b_k/(2A_e)$: $(-1,-1)$, $(1,0)$, $(0,1)$ | $10^{-13}$ | (8), (10)–(12) |
| Soma dos gradientes | $\nabla L_k$ | Triângulo de referência | $\sum_k\nabla L_k=(0,0)$ | $10^{-13}$ | (8) |
| Ângulos de aresta | $\theta_{k+3}$ | Triângulo de referência | $\theta_4=0$, $\theta_5=3\pi/4$, $\theta_6=\pi/2$ | $10^{-12}$ | (20) |
| Coeficientes de aresta | $\bar{a},\bar{b},\bar{c}$ | Triângulo de referência | $\bar{a}=(1,0,0)$, $\bar{b}=(0,0,1)$, $\bar{c}=(-1,-\sqrt{2},1)$ | $10^{-12}$ | (17)–(19) |
| Funções de aresta | $\{U\}$, $\{V\}$ | Pontos laterais | $\phi_t^{(r)}=\delta_{rs}$ na aresta $s$ | $10^{-12}$ | (13)–(21) |
| Derivadas de aresta | $\{U_y\}$, $\{V_x\}$ | Triângulo de referência | $(-1,-\sqrt{2},1)$ e $(1,\sqrt{2},-1)$ | $10^{-12}$ | (A3) |
| Orientação de aresta | `Edge` | Dois triângulos vizinhos | $s_e=+1$ para aresta $(1,0)-(0,1)$ | $10^{-12}$ | (13)–(21) |
| Continuidade tangencial | `Mesh` + `Edge` | Dois triângulos vizinhos | $\phi_t^{T_1}=\phi_t^{T_2}=1$ em $P_m$ e $P_q$ | $10^{-12}$ | (13)–(21) |
| Integral A3 | Integrais locais | Triângulo de referência | $(1/2)[[1,\sqrt{2},-1],...]$ | $10^{-12}$ | (A3) |
| Fator 4 no rotacional | Integrais locais | Triângulo de referência | $4\times$A3 | $10^{-12}$ | (29a), (32a) |
| Integral A4 | Integrais locais | Triângulo de referência | Matriz $3\times3$ (col 3 nula) | $10^{-12}$ | (A4) |
| Integral A5 | Integrais locais | Triângulo de referência | Matriz $3\times3$ (col 2 nula) | $10^{-12}$ | (A5) |
| Massa nodal A6 | Integrais locais | Triângulo de referência | $(1/24)[[2,1,1],[1,2,1],[1,1,2]]$ | $10^{-13}$ | (A6) |
| Rigidez A7 | Integrais locais | Triângulo de referência | $(1/2)[[1,-1,0],[-1,1,0],[0,0,0]]$ | $10^{-13}$ | (A7) |
| Rigidez A8 | Integrais locais | Triângulo de referência | $(1/2)[[1,0,-1],[0,0,0],[-1,0,1]]$ | $10^{-13}$ | (A8) |
| A7/A8 triângulo geral | Integrais locais | $(0,0),(2,1),(1,3)$ | Nenhuma entrada nula | $10^{-13}$ | (A7), (A8) |
| Orientação horária (diagnóstico) | Integrais locais | $(0,0),(0,1),(1,0)$ | Diagonal negativa com $A_e<0$; corrigir com $\|A_e\|$ | $10^{-13}$ | (A7), (A8) |
| Degeneração | `Triangle` | Pontos colineares | Rejeitar elemento | $10^{-14}$ | (9) |
| Massa A1 ($\{U\}\{U\}^T$) | Integrais locais | Triângulo de referência | $\mathrm{diag}=(1/4,1/6,1/12)$; $\mathrm{tr}=1/2$ | $10^{-12}$ | (A1) |
| Massa A2 ($\{V\}\{V\}^T$) | Integrais locais | Triângulo de referência | $\mathrm{diag}=(1/12,1/6,1/4)$; $\mathrm{tr}=1/2$ | $10^{-12}$ | (A2) |

---

## 27. Testes que ainda não são validação do artigo

Os testes deste documento são necessários, mas ainda não validam a reprodução numérica do artigo. Eles apenas garantem que a fundação geométrica e interpolatória está coerente.

Ainda não são validação do artigo:

1. reproduzir as curvas de dispersão das Figuras 3, 5 e 7;
2. validar a ausência de soluções espúrias;
3. validar o problema de autovalores da Equação (34);
4. validar a recuperação da componente axial $\phi_z$;
5. validar as condições PEC/PMC nas fronteiras artificiais;
6. validar a montagem global completa;
7. validar a matriz $[\hat{M}_{tt}]$;
8. comparar resultados com Goell, Marcatili, Yeh ou Koshiba-Hayata-Suzuki;
9. testar materiais anisotrópicos reais;
10. testar malhas grandes e refinamento de convergência.

Essas validações pertencem às fases posteriores, depois que as classes geométricas, funções de forma e matrizes locais forem aprovadas.

---

## 28. Pendências científicas e técnicas antes da implementação

1. Confirmar a associação definitiva entre pontos laterais $4$, $5$, $6$ e arestas locais na Figura 1 do artigo.
2. Definir oficialmente se a implementação aceitará triângulos horários ou se todos serão convertidos para orientação anti-horária.
3. Definir uma convenção global de orientação de arestas.
4. ~~Conferir como o ângulo $\theta_{k+3}$ deve ser calculado numericamente quando a aresta for vertical ou horizontal.~~ **Resolvido:** usar `atan2(y_k−y_l, x_k−x_l)` + somar π se resultado < 0 + tratar π como 0. Ver Seção 13, Teste 9.
5. ~~Decidir se será usado `atan2` com normalização para $0\leq\theta<\pi$.~~ **Resolvido:** convenção adotada e documentada no Teste 9.
6. Verificar se a mudança de orientação local altera os coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$ conforme esperado.
7. Validar as funções de aresta contra uma derivação independente, antes de montar as matrizes do artigo.
8. Confirmar se os testes de continuidade tangencial devem usar pontos laterais, pontos arbitrários na aresta ou ambos.
