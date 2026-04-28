# 13. Revisão das integrais do Apêndice

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md)

> **Documento complementar da Fase 2.**
> Este arquivo revisa as integrais do Apêndice do artigo de Koshiba e Inoue, com foco em sua futura tradução para matrizes locais do método dos elementos finitos. O objetivo não é reescrever o artigo, mas transformar as Equações (A1) a (A10) em um guia seguro de leitura matemática e implementação posterior.

---

## 1. Papel do Apêndice na formulação

Na formulação do artigo, a discretização por elementos finitos leva a matrizes elementares construídas por integrais sobre cada triângulo $e$. Essas integrais aparecem depois que as componentes do campo são aproximadas por:

- funções nodais lineares $\{N\}$, associadas à componente axial $\phi_z$;
- funções de forma de aresta $\{U\}$ e $\{V\}$, associadas às componentes transversais $\phi_x$ e $\phi_y$;
- derivadas dessas funções, como $\{N_x\}$, $\{N_y\}$, $\{U_y\}$ e $\{V_x\}$.

O Apêndice fornece fórmulas fechadas para essas integrais, evitando que a implementação inicial precise usar quadratura numérica para termos polinomiais simples.

No artigo, essas integrais alimentam principalmente as submatrizes elementares associadas a:

$$
[K_{tt}], \quad [K_{tz}], \quad [K_{zt}], \quad [K_{zz}], \quad [M_{tt}], \quad [M_{zz}].
$$

---

## 2. Convenções geométricas usadas no Apêndice

Considere um elemento triangular $e$ com vértices locais $1$, $2$ e $3$, coordenadas:

$$
(x_1,y_1), \quad (x_2,y_2), \quad (x_3,y_3).
$$

A área do elemento é indicada por:

$$
A_e.
$$

O centroide do triângulo é dado por:

$$
x_c = \frac{x_1+x_2+x_3}{3}
$$

e

$$
y_c = \frac{y_1+y_2+y_3}{3}.
$$

As funções nodais lineares são escritas em termos dos coeficientes $a_k$, $b_k$ e $c_k$. Suas derivadas em relação a $x$ e $y$ são constantes dentro de cada elemento, o que explica por que as integrais envolvendo $\{N_x\}$ e $\{N_y\}$ dependem de fatores como $1/(4A_e)$.

As funções de aresta usadas pelo artigo são:

$$
\{U\} =
\begin{bmatrix}
\bar{a}_1+\bar{c}_1y\\
\bar{a}_2+\bar{c}_2y\\
\bar{a}_3+\bar{c}_3y
\end{bmatrix}
$$

e

$$
\{V\} =
\begin{bmatrix}
\bar{b}_1-\bar{c}_1x\\
\bar{b}_2-\bar{c}_2x\\
\bar{b}_3-\bar{c}_3x
\end{bmatrix}.
$$

Portanto,

$$
\{U_y\} =
\begin{bmatrix}
\bar{c}_1\\
\bar{c}_2\\
\bar{c}_3
\end{bmatrix}
$$

e

$$
\{V_x\} =
\begin{bmatrix}
-\bar{c}_1\\
-\bar{c}_2\\
-\bar{c}_3
\end{bmatrix}.
$$

Essa relação explica a estrutura da Equação (A3).

---

## 3. Equação (A1): integral $\{U\}\{U\}^T$

A primeira integral do Apêndice é:

$$
\left[
\iint_e
\{U\}\{U\}^T
dxdy
\right]_{kl} =
A_e \bar{a}_k \bar{a}_l
+
A_e y_c
\left(
\bar{a}_k \bar{c}_l
+
\bar{c}_k \bar{a}_l
\right)
+
\frac{1}{12}
A_e \bar{c}_k \bar{c}_l
\left(
y_1^2+y_2^2+y_3^2+9y_c^2
\right).
$$

Essa integral calcula o produto entre funções de forma de aresta associadas à aproximação de $\phi_x$.

Como $\{U\}$ depende linearmente de $y$, o produto $\{U\}\{U\}^T$ gera termos constantes, termos lineares em $y$ e termos quadráticos em $y$. Por isso aparecem:

- a área $A_e$;
- o centroide $y_c$;
- o termo quadrático envolvendo $y_1^2+y_2^2+y_3^2+9y_c^2$;
- o fator $1/12$, típico da integração exata de polinômios quadráticos em triângulos.

> **Risco de orientação:** todos os termos de A1 são proporcionais a $A_e$ com sinal linear. Para triângulos com vértices em ordem horária ($A_e < 0$), a fórmula inteira troca de sinal. Use $|A_e|$ ou garanta ordem anti-horária.

Essa integral aparece em blocos transversais que envolvem produtos de funções associadas a $\phi_x$. Ela é usada, por exemplo, na montagem de termos do tipo $\{U\}\{U\}^T$ em $[M_{tt}]$, em $[K_{tt}]$ reorganizado e em termos proporcionais a $\beta^2$ na formulação direta.

> **Nota de verificação:** a associação exata de cada ocorrência de $\{U\}\{U\}^T$ com $p_y$, $q_x$ ou outros coeficientes depende da etapa da formulação considerada, pois o artigo reutiliza símbolos como $[K_{tt}]$ em formas algébricas diferentes. A implementação deve separar os blocos por equação de origem.

---

## 4. Equação (A2): integral $\{V\}\{V\}^T$

A segunda integral é:

$$
\left[
\iint_e
\{V\}\{V\}^T
dxdy
\right]_{kl} =
A_e \bar{b}_k \bar{b}_l
-
A_e x_c
\left(
\bar{b}_k \bar{c}_l
+
\bar{c}_k \bar{b}_l
\right)
+
\frac{1}{12}
A_e \bar{c}_k \bar{c}_l
\left(
x_1^2+x_2^2+x_3^2+9x_c^2
\right).
$$

Essa integral calcula o produto entre funções de forma de aresta associadas à aproximação de $\phi_y$.

Como $\{V\}$ depende linearmente de $x$ na forma $\bar{b}_k-\bar{c}_kx$, aparece um sinal negativo no termo linear em $x_c$. O termo quadrático depende das coordenadas $x_1$, $x_2$, $x_3$ e do centroide $x_c$.

Ela aparece nos blocos transversais que envolvem produtos de funções associadas a $\phi_y$, como termos de $[M_{tt}]$, termos de $[K_{tt}]$ reorganizado e parcelas com $\beta^2$ da formulação direta.

> **Risco de orientação:** idêntico ao de A1. Todos os termos são proporcionais a $A_e$; a fórmula assume $A_e > 0$. Use $|A_e|$ ou garanta ordem anti-horária.

---

## 5. Equação (A3): integrais com derivadas de funções de aresta

A terceira equação reúne quatro integrais:

$$
\left[
\iint_e
\{U_y\}\{U_y\}^T
 dxdy
\right]_{kl}
=
\left[
\iint_e
\{V_x\}\{V_x\}^T
 dxdy
\right]_{kl}
=
-
\left[
\iint_e
\{U_y\}\{V_x\}^T
 dxdy
\right]_{kl}
=
-
\left[
\iint_e
\{V_x\}\{U_y\}^T
 dxdy
\right]_{kl}
=
A_e \bar{c}_k\bar{c}_l.
$$

Essa identidade é consequência direta de:

$$
U_{y,k}=\bar{c}_k
$$

e

$$
V_{x,k}=-\bar{c}_k.
$$

Assim, os produtos $\{U_y\}\{U_y\}^T$ e $\{V_x\}\{V_x\}^T$ têm o mesmo sinal, enquanto os produtos cruzados $\{U_y\}\{V_x\}^T$ e $\{V_x\}\{U_y\}^T$ têm sinal oposto.

Essa integral é essencial nos termos de rotacional envolvendo as componentes transversais. Em particular, ela está associada à parte do operador que contém combinações como:

$$
-\{U_y\}+\{V_x\}.
$$

Ao expandir essa combinação, os sinais da Equação (A3) são decisivos.

> **Nota de verificação:** na matriz $[B]$ do artigo aparece o termo $-\{U_y\}+\{V_x\}$. Como $\{V_x\}=-\{\bar{c}\}$ e $\{U_y\}=\{\bar{c}\}$, essa combinação resulta em $-2\bar{c}_k$ por componente. O fator $4\bar{c}_k\bar{c}_l$ que aparece nos blocos de $[K_{tt}]$ não vem de A3 isolado, mas da expansão do produto $(-\{U_y\}+\{V_x\})(-\{U_y\}+\{V_x\})^T$ na montagem de $[B]^T[p][B]$: cada fator $A_e\bar{c}_k\bar{c}_l$ de A3 recebe um coeficiente numérico 4. Esse é um ponto crítico para conferência contra as Equações (29a) e (32a).
>
> **Risco de orientação:** o resultado $A_e\bar{c}_k\bar{c}_l$ assume $A_e > 0$. Para triângulos em ordem horária, $A_e < 0$ e os coeficientes $\bar{c}_k = c_{l(k)}+c_{m(k)}$ também trocam de sinal. Os dois fatores $\bar{c}$ se cancelam em sinal, mas $A_e$ permanece negativo. A diagonal $A_e\bar{c}_k^2$ ficaria negativa — fisicamente incorreta. Use $|A_e|$ ou garanta ordem anti-horária.

---

## 6. Equação (A4): integral $\{U\}\{N_x\}^T$ (acoplamento $\phi_x – \phi_z$)

A quarta integral é:

$$
\left[
\iint_e
\{U\}\{N_x\}^T
 dxdy
\right]_{kl}
=
\frac{1}{2}
\left(
\bar{a}_k+\bar{c}_k y_c
\right)b_l.
$$

Aqui aparece o produto entre:

- uma função de forma de aresta, associada à componente transversal $\phi_x$;
- a derivada em $x$ da função nodal linear, associada à componente axial $\phi_z$.

Como $N_l$ é uma função nodal linear, sua derivada em $x$ é constante dentro do elemento. Na notação do artigo,

$$
N_{x,l} = \frac{b_l}{2A_e}.
$$

A função $U_k$ é linear em $y$. Sua integral média sobre o triângulo depende do valor no centroide:

$$
\bar{a}_k+\bar{c}_k y_c.
$$

Multiplicando a contribuição da derivada nodal pelo integral da função de aresta, o fator $A_e$ cancela com o denominador $2A_e$, restando o fator $1/2$.

Essa integral aparece na matriz de acoplamento entre graus de liberdade transversais e axiais, especialmente no termo $\{U\}\{N_x\}^T$ de $[K_{tz}]$.

---

## 7. Equação (A5): integral $\{V\}\{N_y\}^T$ (acoplamento $\phi_y – \phi_z$)

A quinta integral é:

$$
\left[
\iint_e
\{V\}\{N_y\}^T
 dxdy
\right]_{kl}
=
\frac{1}{2}
\left(
\bar{b}_k-\bar{c}_k x_c
\right)c_l.
$$

Ela é análoga à Equação (A4), mas envolve:

- a função de forma de aresta $V_k$, associada à componente transversal $\phi_y$;
- a derivada em $y$ da função nodal $N_l$, associada à componente axial.

Na notação do artigo,

$$
N_{y,l} = \frac{c_l}{2A_e}.
$$

A média de $V_k$ sobre o triângulo depende de seu valor no centroide:

$$
\bar{b}_k-\bar{c}_k x_c.
$$

Assim, novamente o fator $A_e$ da integral cancela com o denominador $2A_e$ da derivada nodal, produzindo o fator final $1/2$.

Essa integral também aparece na matriz de acoplamento $[K_{tz}]$, agora na parcela $\{V\}\{N_y\}^T$.

---

## 8. Equação (A6): integral $\{N\}\{N\}^T$ (matriz de massa axial)

A sexta integral é:

$$
\iint_e
\{N\}\{N\}^T
 dxdy
=
\frac{A_e}{12}
\begin{bmatrix}
2 & 1 & 1\\
1 & 2 & 1\\
1 & 1 & 2
\end{bmatrix}.
$$

Essa é a matriz de massa local clássica do elemento triangular linear.

Ela integra o produto entre funções nodais lineares. Como $\{N\}$ está associada à componente axial $\phi_z$, essa integral aparece em termos como $[M_{zz}]$ e também nos termos reorganizados em que há contribuição proporcional a $q_z k_0^2\{N\}\{N\}^T$.

O fator $A_e/12$ é característico da matriz de massa consistente para triângulos lineares. A diagonal recebe peso $2$ e os termos fora da diagonal recebem peso $1$.

> **Risco de orientação:** o fator $A_e/12$ é proporcional a $A_e$. Para triângulos em ordem horária ($A_e < 0$), toda a matriz de massa local trocaria de sinal, tornando os autovalores negativos. Use $|A_e|$ ou garanta ordem anti-horária.

---

## 9. Equação (A7): integral $\{N_x\}\{N_x\}^T$ (matriz de rigidez axial em $x$)

A sétima integral é:

$$
\iint_e
\{N_x\}\{N_x\}^T
 dxdy
=
\frac{1}{4A_e}
\begin{bmatrix}
b_1^2 & b_1b_2 & b_1b_3\\
b_1b_2 & b_2^2 & b_2b_3\\
b_1b_3 & b_2b_3 & b_3^2
\end{bmatrix}.
$$

Ela integra o produto das derivadas das funções nodais em relação a $x$.

Como as funções nodais são lineares, suas derivadas são constantes dentro do elemento. A forma geral é:

$$
N_{x,k} = \frac{b_k}{2A_e}.
$$

Portanto,

$$
\iint_e N_{x,k}N_{x,l}\,dxdy
=
A_e
\left(
\frac{b_k}{2A_e}
\right)
\left(
\frac{b_l}{2A_e}
\right)
=
\frac{b_kb_l}{4A_e}.
$$

Essa integral aparece em blocos axiais relacionados ao operador diferencial sobre $\phi_z$, como $[K_{zz}]$, tanto na forma direta quanto na forma reorganizada.

---

## 10. Equação (A8): integral $\{N_y\}\{N_y\}^T$ (matriz de rigidez axial em $y$)

A oitava integral é:

$$
\iint_e
\{N_y\}\{N_y\}^T
 dxdy
=
\frac{1}{4A_e}
\begin{bmatrix}
c_1^2 & c_1c_2 & c_1c_3\\
c_1c_2 & c_2^2 & c_2c_3\\
c_1c_3 & c_2c_3 & c_3^2
\end{bmatrix}.
$$

Ela é análoga à Equação (A7), mas para derivadas em relação a $y$.

Como:

$$
N_{y,k}=\frac{c_k}{2A_e},
$$

temos:

$$
\iint_e N_{y,k}N_{y,l}\,dxdy
=
\frac{c_kc_l}{4A_e}.
$$

Essa integral também aparece em blocos axiais relacionados a $[K_{zz}]$ e, dependendo da forma algébrica usada, pode entrar com sinal positivo ou negativo.

> **Nota de verificação:** nas Equações (29c) e (32c), os termos com $\{N_x\}\{N_x\}^T$ e $\{N_y\}\{N_y\}^T$ aparecem em formas reorganizadas diferentes. A implementação deve preservar os sinais da equação específica que está sendo montada.
>
> **Risco crítico de orientação (vale para A7 e A8):** A fórmula $b_kb_l/(4A_e)$ pressupõe $A_e > 0$ (vértices em ordem anti-horária). Para triângulos com vértices em ordem horária, $A_e < 0$, e o fator $1/(4A_e)$ torna-se negativo. A entrada diagonal $b_k^2/(4A_e)$ ficaria negativa — fisicamente impossível para uma integral de Gram. A afirmação de que "os produtos $b_kb_l$ permanecem invariantes com o ajuste de sinal de $A_e$" é incorreta: quando a orientação é revertida, $b_k \to -b_k$ e $A_e \to -A_e$, de modo que $b_k^2/(4A_e) \to (-b_k)^2/(4(-A_e)) = -b_k^2/(4A_e) < 0$.
>
> Verificação numérica: triângulo $(0,0)\!\to\!(1,0)\!\to\!(0,1)$ anti-horário, $A_e=0.5$, $b_1=-1$; fórmula: $b_1^2/(4A_e)=0.5>0$ ✓. Mesmo triângulo horário $(0,0)\!\to\!(0,1)\!\to\!(1,0)$, $A_e=-0.5$, $b_1^{\mathrm{CW}}=1$; fórmula: $b_1^2/(4A_e)=-0.5<0$ ✗.
>
> **Mitigação obrigatória:** usar $|A_e|$ em vez de $A_e$ nas fórmulas A7 e A8 — e, pelo mesmo argumento, em A1, A2, A3 e A6 — ou reordenar os vértices para anti-horário antes de calcular qualquer integral elementar.

---

## 11. Equação (A9): coordenada $x_c$ do centroide

A nona equação define:

$$
x_c=\frac{x_1+x_2+x_3}{3}.
$$

O centroide aparece nas integrais de funções de aresta porque $\{V\}$ depende linearmente de $x$:

$$
V_k(x)=\bar{b}_k-\bar{c}_kx.
$$

Assim, integrais como:

$$
\iint_e V_k\,dxdy
$$

dependem da média de $x$ no triângulo, isto é, de $x_c$.

Na implementação futura, $x_c$ deve ser calculado uma única vez por elemento e reutilizado nas integrais que dependem da média geométrica em $x$.

---

## 12. Equação (A10): coordenada $y_c$ do centroide

A décima equação define:

$$
y_c=\frac{y_1+y_2+y_3}{3}.
$$

O centroide $y_c$ aparece porque $\{U\}$ depende linearmente de $y$:

$$
U_k(y)=\bar{a}_k+\bar{c}_ky.
$$

Assim, integrais como:

$$
\iint_e U_k\,dxdy
$$

dependem da média de $y$ no triângulo, isto é, de $y_c$.

Na implementação futura, $y_c$ também deve ser calculado uma única vez por elemento e reaproveitado nas integrais de aresta que dependem de $y$.

---

## 13. Tabela de revisão das integrais

| Equação | Tipo de termo | Dependência geométrica | Risco de implementação | Teste sugerido |
| --- | --- | --- | --- | --- |
| A1 | Produto de funções de aresta $\{U\}\{U\}^T$ | $A_e$, $y_c$, $y_1^2+y_2^2+y_3^2$, fator $1/12$ | Trocar $y$ por $x$; perder o termo misto; erro de sinal em $\bar{c}$; **sinal errado se $A_e < 0$** — usar $\|A_e\|$ | Comparar com quadratura numérica de alta ordem em triângulo simples |
| A2 | Produto de funções de aresta $\{V\}\{V\}^T$ | $A_e$, $x_c$, $x_1^2+x_2^2+x_3^2$, fator $1/12$ | Errar o sinal negativo do termo linear em $x_c$; **sinal errado se $A_e < 0$** — usar $\|A_e\|$ | Comparar com integração simbólica ou quadratura de referência |
| A3 | Produtos de derivadas $\{U_y\}$ e $\{V_x\}$ | $A_e\bar{c}_k\bar{c}_l$ | Errar sinais dos produtos cruzados; o fator 4 em blocos de $[K_{tt}]$ vem da expansão de $[B]^T[p][B]$, não de A3 sozinho; **diagonal negativa se $A_e < 0$** — usar $\|A_e\|$ | Verificar que $U_y=\bar{c}$ e $V_x=-\bar{c}$ para cada aresta; conferir fator 4 em Eq. (29a) |
| A4 | Produto aresta--derivada nodal $\{U\}\{N_x\}^T$ | Centroide $y_c$, coeficiente $b_l$, fator $1/2$ | Usar $c_l$ no lugar de $b_l$; confundir índice de linha e coluna; **A4 é imune à orientação** pois $A_e$ cancela | Comparar com integral direta de $U_k N_{x,l}$ |
| A5 | Produto aresta--derivada nodal $\{V\}\{N_y\}^T$ | Centroide $x_c$, coeficiente $c_l$, fator $1/2$ | Usar $b_l$ no lugar de $c_l$; errar o sinal em $-\bar{c}_kx_c$; **A5 é imune à orientação** pois $A_e$ cancela | Comparar com integral direta de $V_k N_{y,l}$ |
| A6 | Produto nodal $\{N\}\{N\}^T$ | $A_e/12$ | Montar matriz lumped em vez de consistente; trocar pesos da diagonal; **sinal errado se $A_e < 0$** — usar $\|A_e\|$ | Verificar matriz clássica de massa de triângulo linear |
| A7 | Produto de derivadas nodais em $x$ | $1/(4A_e)$ e produtos $b_kb_l$ | **Diagonal negativa se $A_e < 0$** (triângulo horário); usar $\|A_e\|$ no denominador; confundir $A_e/4$ com $1/(4A_e)$ | Verificar contra derivadas explícitas de $N_k$; checar positividade da diagonal |
| A8 | Produto de derivadas nodais em $y$ | $1/(4A_e)$ e produtos $c_kc_l$ | **Diagonal negativa se $A_e < 0$** (triângulo horário); usar $\|A_e\|$ no denominador; usar $b_k$ no lugar de $c_k$ | Verificar contra derivadas explícitas de $N_k$; checar positividade da diagonal |
| A9 | Centroide em $x$ | Média aritmética dos $x_k$ | Usar ponto lateral ou ponto médio de aresta no lugar do centroide | Testar triângulo conhecido e conferir valor médio de $x$ |
| A10 | Centroide em $y$ | Média aritmética dos $y_k$ | Usar ponto lateral ou ponto médio de aresta no lugar do centroide | Testar triângulo conhecido e conferir valor médio de $y$ |

---

## 14. Como essas integrais orientarão a classe `LocalElementMatrices`

A futura classe `LocalElementMatrices` deve ser orientada por blocos matemáticos, não apenas por nomes de matrizes globais. A razão é que o artigo reutiliza símbolos como $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$ em momentos diferentes da formulação.

Uma organização segura deve separar, no mínimo, os seguintes objetos matemáticos locais:

- integral local de $\{U\}\{U\}^T$;
- integral local de $\{V\}\{V\}^T$;
- integral local de $\{U_y\}\{U_y\}^T$;
- integral local de $\{V_x\}\{V_x\}^T$;
- integrais cruzadas envolvendo $\{U_y\}$ e $\{V_x\}$ quando forem necessárias para conferência;
- integral local de $\{U\}\{N_x\}^T$;
- integral local de $\{V\}\{N_y\}^T$;
- integral local de $\{N\}\{N\}^T$;
- integral local de $\{N_x\}\{N_x\}^T$;
- integral local de $\{N_y\}\{N_y\}^T$.

A montagem posterior das matrizes do artigo deve combinar esses blocos com os coeficientes materiais:

$$
p_x,\quad p_y,\quad p_z,\quad q_x,\quad q_y,\quad q_z,
$$

e com os parâmetros físicos:

$$
k_0,\quad \beta.
$$

Essa separação ajuda a testar cada integral isoladamente antes de testar a matriz completa.

Também reduz o risco de confundir:

- integral geométrica;
- coeficiente material;
- sinal algébrico;
- fator físico;
- forma direta ou reorganizada da formulação.

---

## 15. Interpretação para montagem das matrizes locais

A partir das Equações (29), (30), (32) e (33), as integrais do Apêndice podem ser vistas como uma biblioteca de blocos locais.

De forma esquemática:

- A1 e A2 alimentam blocos transversais envolvendo $\phi_x$ e $\phi_y$;
- A3 alimenta termos de rotacional transversal;
- A4 e A5 alimentam o acoplamento entre componentes transversais e axiais;
- A6 alimenta termos de massa axial;
- A7 e A8 alimentam termos de rigidez axial;
- A9 e A10 fornecem médias geométricas necessárias para A1, A2, A4 e A5.

O caminho seguro é montar primeiro esses blocos elementares puros e, somente depois, formar as submatrizes físicas conforme a equação específica do artigo.

---

## 16. Pendências científicas

1. Conferir no PDF original se todas as transcrições de A1 a A10 preservam exatamente os sinais, índices e fatores numéricos.

2. Verificar cuidadosamente a Equação (A3), pois ela controla os sinais cruzados entre $\{U_y\}$ e $\{V_x\}$ e influencia diretamente o fator que aparece em termos de rotacional.

3. Confirmar a relação entre a Equação (A3) e o termo $4p_z\{U_y\}\{U_y\}^T$ que aparece em blocos posteriores da formulação.

4. Conferir se o artigo usa alguma convenção implícita de orientação de arestas que afete os sinais de $\bar{a}_k$, $\bar{b}_k$ e $\bar{c}_k$.

5. Verificar se as integrais A4 e A5 são suficientes para toda a formulação original, considerando que $[p]$ e $[q]$ são diagonais no artigo. As integrais complementares $\{V\}\{N_x\}^T$ (A11) e $\{U\}\{N_y\}^T$ (A12), derivadas por este projeto e registradas em `docs/06_apendice.md`, são necessárias apenas se $[p]$ tiver termos fora da diagonal.

6. Manter separadas, na documentação, as integrais originais do Apêndice e quaisquer integrais complementares derivadas futuramente pelo projeto.

7. Definir se a implementação inicial usará apenas as fórmulas analíticas do Apêndice ou se também incluirá uma rotina de quadratura numérica apenas para validação independente.

8. Confirmar como as condições de contorno afetarão a montagem local e a montagem global, especialmente em fronteiras artificiais com condição de condutor elétrico perfeito ou condutor magnético perfeito.

---

## 17. Síntese

As Equações (A1) a (A10) formam a ponte entre a formulação matemática contínua e a implementação local por elemento triangular. Elas transformam produtos de funções de forma e derivadas em expressões fechadas dependentes apenas da geometria do elemento e dos coeficientes das funções nodais e de aresta.

Para a implementação futura, o ponto central é não misturar responsabilidades: primeiro calcular corretamente as integrais geométricas locais; depois aplicar os coeficientes materiais e físicos; por fim, montar as matrizes globais respeitando a forma algébrica específica de cada equação do artigo.
