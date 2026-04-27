# 15. Testes matemáticos mínimos

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md)

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

Como $2A_e=1$, tem-se:

$$
\frac{\partial L_1}{\partial x}=b_1=-1,
\qquad
\frac{\partial L_1}{\partial y}=c_1=-1,
$$

$$
\frac{\partial L_2}{\partial x}=b_2=1,
\qquad
\frac{\partial L_2}{\partial y}=c_2=0,
$$

$$
\frac{\partial L_3}{\partial x}=b_3=0,
\qquad
\frac{\partial L_3}{\partial y}=c_3=1.
$$

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

Para o triângulo de referência, com $A_e=1/2$:

$$
\frac{1}{4A_e}=\frac{1}{2}.
$$

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

Logo:

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

## 19. Checklist para testes C++

| Teste | Entidade validada | Entrada mínima | Resultado esperado | Tolerância | Equações relacionadas |
| --- | --- | --- | --- | --- | --- |
| Área positiva | `Triangle` | $(0,0),(1,0),(0,1)$ | $A_e=1/2$ | $10^{-14}$ | (9) |
| Área negativa | `Triangle` | $(0,0),(0,1),(1,0)$ | $A_e^{signed}=-1/2$ | $10^{-14}$ | (9) |
| Centroide | `Triangle` | Triângulo de referência | $(x_c,y_c)=(1/3,1/3)$ | $10^{-14}$ | (A9), (A10) |
| Coeficientes nodais | Funções nodais | Triângulo de referência | $(a,b,c)_1=(1,-1,-1)$, $(0,1,0)$, $(0,0,1)$ | $10^{-14}$ | (10)–(12) |
| Funções nodais | $L_k$ | Pontos internos | $L_1=1-x-y$, $L_2=x$, $L_3=y$ | $10^{-13}$ | (8) |
| Partição da unidade | $L_k$ | Vértices e pontos internos | $L_1+L_2+L_3=1$ | $10^{-13}$ | (8) |
| Kronecker nodal | $L_k(P_j)$ | Vértices | $L_i(P_j)=\delta_{ij}$ | $10^{-13}$ | (8) |
| Gradientes nodais | $\nabla L_k$ | Triângulo de referência | $(-1,-1)$, $(1,0)$, $(0,1)$ | $10^{-13}$ | (8), (10)–(12) |
| Soma dos gradientes | $\nabla L_k$ | Triângulo de referência | $\sum_k\nabla L_k=(0,0)$ | $10^{-13}$ | (8) |
| Funções de aresta | $\{U\}$, $\{V\}$ | Pontos laterais | $\phi_t^{(r)}=\delta_{rs}$ na aresta $s$ | $10^{-12}$ | (13)–(21) |
| Orientação de aresta | `Edge` | Dois triângulos vizinhos | Sinal local-global consistente | $10^{-12}$ | (13)–(21) |
| Continuidade tangencial | `Mesh` + `Edge` | Dois triângulos vizinhos | $\phi_t^{T_1}=\phi_t^{T_2}$ após sinal | $10^{-12}$ | (13)–(21) |
| Massa nodal | Integrais locais | Triângulo de referência | $A_e/12$ vezes matriz consistente | $10^{-13}$ | (A6) |
| Rigidez nodal | Integrais locais | Triângulo de referência | Matrizes de (A7), (A8) | $10^{-13}$ | (A7), (A8) |
| Degeneração | `Triangle` | Pontos colineares | Rejeitar elemento | $10^{-14}$ | (9) |

---

## 20. Testes que ainda não são validação do artigo

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

## 21. Pendências científicas e técnicas antes da implementação

1. Confirmar a associação definitiva entre pontos laterais $4$, $5$, $6$ e arestas locais na Figura 1 do artigo.
2. Definir oficialmente se a implementação aceitará triângulos horários ou se todos serão convertidos para orientação anti-horária.
3. Definir uma convenção global de orientação de arestas.
4. Conferir como o ângulo $\theta_{k+3}$ deve ser calculado numericamente quando a aresta for vertical ou horizontal.
5. Decidir se será usado `atan2` com normalização para $0\leq\theta<\pi$.
6. Verificar se a mudança de orientação local altera os coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$ conforme esperado.
7. Validar as funções de aresta contra uma derivação independente, antes de montar as matrizes do artigo.
8. Confirmar se os testes de continuidade tangencial devem usar pontos laterais, pontos arbitrários na aresta ou ambos.
