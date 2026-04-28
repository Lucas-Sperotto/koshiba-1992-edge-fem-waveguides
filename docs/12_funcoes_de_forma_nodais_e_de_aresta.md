# 12. Funções de forma nodais e de aresta

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md)

Este documento complementa a Fase 2 do projeto de reprodução do artigo de Koshiba e Inoue. O objetivo é explicar, de forma didática e tecnicamente cuidadosa, as funções de forma usadas no elemento triangular do artigo: as funções nodais lineares usadas para a componente axial $\phi_z$ e as funções de forma de aresta usadas para as componentes transversais $\phi_x$ e $\phi_y$.

A intenção aqui não é substituir a formulação original, mas criar uma ponte entre as equações do artigo, a geometria do elemento triangular e a futura implementação numérica.

---

## 1. O triângulo linear local

O elemento triangular considerado no artigo possui três vértices locais, numerados como $1$, $2$ e $3$.

Cada vértice possui coordenadas cartesianas $(x_k, y_k)$:

$$
(x_1,y_1), \qquad (x_2,y_2), \qquad (x_3,y_3).
$$

Esses três vértices são usados para interpolar a componente axial do campo:

$$
\phi_z,
$$

isto é, $E_z$ ou $H_z$, dependendo de a variável genérica $\phi$ representar o campo elétrico ou o campo magnético.

Além dos três vértices, o elemento de aresta triangular do artigo também possui três pontos laterais, numerados como $4$, $5$ e $6$. Esses pontos, localizados nas arestas, estão associados às componentes tangenciais do campo. Assim, o elemento usado no artigo mistura dois tipos de graus de liberdade:

| Tipo de ponto local | Pontos | Campo associado | Tipo de grau de liberdade |
| --- | --- | --- | --- |
| Vértices | $1$, $2$, $3$ | $\phi_z$ | nodal axial |
| Pontos laterais | $4$, $5$, $6$ | $\phi_t$ | tangencial de aresta |

Essa separação é essencial: o campo axial é interpolado como uma grandeza nodal escalar, enquanto o campo transversal é interpolado por funções que controlam diretamente a componente tangencial nas arestas.

Pela convenção cíclica usada nas fórmulas do artigo, o ponto lateral $k+3$ está associado à aresta entre os vértices locais $k$ e $l$. Assim, no mapeamento local adotado para implementação:

| Ponto lateral | Aresta local associada |
| --- | --- |
| $4$ | $1$–$2$ |
| $5$ | $2$–$3$ |
| $6$ | $3$–$1$ |

---

## 2. Convenção cíclica $k$, $l$, $m$

O artigo usa uma convenção cíclica para os índices $k$, $l$ e $m$. Isso significa que, quando um índice principal $k$ é escolhido, os outros dois índices seguem a ordem cíclica dos vértices do triângulo.

A convenção usual é:

| $k$ | $l$ | $m$ |
| --- | --- | --- |
| $1$ | $2$ | $3$ |
| $2$ | $3$ | $1$ |
| $3$ | $1$ | $2$ |

Assim, quando $k=1$, os outros vértices são $l=2$ e $m=3$; quando $k=2$, os outros vértices são $l=3$ e $m=1$; e quando $k=3$, os outros vértices são $l=1$ e $m=2$.

Essa convenção aparece diretamente nas expressões dos coeficientes $a_k$, $b_k$ e $c_k$ das funções nodais e também nas expressões das funções de aresta.

> **Nota de verificação:** na implementação, será necessário confirmar se a malha será armazenada sempre com orientação anti-horária dos vértices. Caso elementos apareçam com orientação horária, o sinal da área orientada e de alguns coeficientes poderá mudar.

---

## 3. Área do triângulo $A_e$

A área do elemento triangular é definida no artigo por:

$$
2A_e =
\begin{vmatrix}
1 & 1 & 1 \\
x_1 & x_2 & x_3 \\
y_1 & y_2 & y_3
\end{vmatrix}.
$$

Expandindo esse determinante, obtém-se:

$$
2A_e =
(x_2y_3-x_3y_2)
+
(x_3y_1-x_1y_3)
+
(x_1y_2-x_2y_1).
$$

Essa expressão representa a área orientada do triângulo multiplicada por $2$. Se os vértices forem percorridos no sentido anti-horário, $A_e$ tende a ser positivo. Se forem percorridos no sentido horário, o determinante muda de sinal.

Na formulação matemática, $A_e$ aparece no denominador das funções nodais e das derivadas nodais. Portanto, um triângulo degenerado, com $A_e \approx 0$, torna o elemento inválido ou numericamente instável.

---

## 4. Funções nodais lineares

A componente axial $\phi_z$ é aproximada por um polinômio completo de primeira ordem. O artigo escreve:

$$
\phi_z = j\{N(x,y)\}^T\{\phi_z\}_e
= j\{N\}^T\{\phi_z\}_e.
$$

O vetor de funções de forma nodais é:

$$
\{N\}
=
\begin{bmatrix}
L_1 \\
L_2 \\
L_3
\end{bmatrix}.
$$

Cada $L_k$ é uma função linear em $x$ e $y$:

$$
L_k(x,y)=\frac{a_k+b_kx+c_ky}{2A_e}.
$$

Na forma matricial usada no artigo:

$$
\{N\}
=
\frac{1}{2A_e}
\begin{bmatrix}
a_1 & b_1 & c_1 \\
a_2 & b_2 & c_2 \\
a_3 & b_3 & c_3
\end{bmatrix}
\begin{bmatrix}
1 \\
x \\
y
\end{bmatrix}.
$$

Essas funções são as coordenadas de área, também chamadas de coordenadas baricêntricas do triângulo.

---

## 5. Derivação dos coeficientes $a_k$, $b_k$, $c_k$

As funções nodais lineares devem satisfazer a propriedade interpolatória:

$$
L_k(x_i,y_i)=\delta_{ki},
$$

em que $\delta_{ki}$ é o delta de Kronecker:

$$
\delta_{ki}=\begin{cases}
1, & k=i, \\
0, & k\ne i.
\end{cases}
$$

Para cada vértice $k$, a função $L_k$ deve valer $1$ no vértice $k$ e $0$ nos outros dois vértices. Como $L_k$ é linear, ela pode ser escrita como:

$$
L_k(x,y)=\alpha_k+\beta_kx+\gamma_ky.
$$

Ao impor as três condições nodais, obtém-se um sistema linear. A solução desse sistema pode ser escrita em termos de $a_k$, $b_k$ e $c_k$:

$$
a_k=x_ly_m-x_my_l,
$$

$$
b_k=y_l-y_m,
$$

$$
c_k=x_m-x_l.
$$

Assim:

$$
\alpha_k=\frac{a_k}{2A_e},
\qquad
\beta_k=\frac{b_k}{2A_e},
\qquad
\gamma_k=\frac{c_k}{2A_e}.
$$

Logo:

$$
L_k(x,y)=\frac{a_k+b_kx+c_ky}{2A_e}.
$$

As derivadas dessas funções são constantes dentro de cada elemento:

$$
\frac{\partial L_k}{\partial x}=\frac{b_k}{2A_e},
$$

$$
\frac{\partial L_k}{\partial y}=\frac{c_k}{2A_e}.
$$

Essas derivadas aparecem posteriormente nas matrizes elementares, por exemplo nos termos envolvendo $\{N_x\}$ e $\{N_y\}$.

---

## 6. Papel das funções nodais na formulação

As funções nodais lineares interpolam a componente axial:

$$
\phi_z.
$$

Em termos práticos, isso significa que o vetor elementar:

$$
\{\phi_z\}_e
=
\begin{bmatrix}
\phi_{z1} \\
\phi_{z2} \\
\phi_{z3}
\end{bmatrix}
$$

armazena os valores nodais axiais nos três vértices do triângulo.

A aproximação completa da componente axial fica:

$$
\phi_z(x,y)
=
j\left(
L_1\phi_{z1}+L_2\phi_{z2}+L_3\phi_{z3}
\right).
$$

O fator $j$ não pertence às funções nodais em si. Ele é uma escolha de fase adotada no artigo para a componente axial, já discutida no documento específico sobre a origem do fator $j$.

---

## 7. Funções de forma de aresta

As componentes transversais são aproximadas de forma diferente. O artigo escreve:

$$
\phi_x = \{U(y)\}^T\{\phi_t\}_e
= \{U\}^T\{\phi_t\}_e,
$$

$$
\phi_y = \{V(x)\}^T\{\phi_t\}_e
= \{V\}^T\{\phi_t\}_e.
$$

Aqui, $\{\phi_t\}_e$ representa os graus de liberdade tangenciais associados às arestas do elemento:

$$
\{\phi_t\}_e
=
\begin{bmatrix}
\phi_{t1} \\
\phi_{t2} \\
\phi_{t3}
\end{bmatrix}.
$$

> **Nota de verificação:** o artigo chama essas variáveis de variáveis de aresta no plano transversal. Pela descrição, elas estão associadas ao valor da componente tangencial nos pontos laterais das arestas. Deve-se conferir no artigo de Kikuchi [16] se a normalização é estritamente valor pontual tangencial, valor médio ou uma forma equivalente escalada.

Os vetores de funções de forma de aresta são:

$$
\{U\}
=
\begin{bmatrix}
\bar{a}_1+\bar{c}_1y \\
\bar{a}_2+\bar{c}_2y \\
\bar{a}_3+\bar{c}_3y
\end{bmatrix},
$$

$$
\{V\}
=
\begin{bmatrix}
\bar{b}_1-\bar{c}_1x \\
\bar{b}_2-\bar{c}_2x \\
\bar{b}_3-\bar{c}_3x
\end{bmatrix}.
$$

Assim, para cada grau de liberdade de aresta $k$, a contribuição vetorial transversal associada pode ser interpretada como:

$$
\mathbf{w}_k(x,y)
=
\begin{bmatrix}
\bar{a}_k+\bar{c}_ky \\
\bar{b}_k-\bar{c}_kx
\end{bmatrix}.
$$

Então:

$$
\begin{bmatrix}
\phi_x \\
\phi_y
\end{bmatrix}
=
\sum_{k=1}^{3}
\phi_{tk}\mathbf{w}_k(x,y).
$$

---

## 8. Componente tangencial na aresta

A componente tangencial do campo transversal ao longo de uma aresta é dada por:

$$
\phi_t=\phi_x\cos\theta+\phi_y\sin\theta.
$$

O vetor tangente unitário associado à aresta pode ser escrito como:

$$
\mathbf{t}=\begin{bmatrix}\cos\theta \\ \sin\theta\end{bmatrix}.
$$

Portanto:

$$
\phi_t=
\begin{bmatrix}\phi_x & \phi_y\end{bmatrix}
\begin{bmatrix}\cos\theta \\ \sin\theta\end{bmatrix}.
$$

A ideia central do elemento de aresta é escolher as funções $\mathbf{w}_k$ de modo que a componente tangencial seja controlada diretamente nas arestas. Para uma função de base $\mathbf{w}_k$, deseja-se que sua componente tangencial seja unitária na aresta associada ao grau de liberdade $k$ e nula nas demais arestas, de acordo com a orientação adotada.

Em forma abstrata, essa condição pode ser escrita como:

$$
\mathbf{w}_k \cdot \mathbf{t}_r = \delta_{kr} \quad \text{na aresta } r,
$$

onde $r$ é o índice da aresta. O artigo usa os pontos laterais $(x_{r+3}, y_{r+3})$ para impor essa condição.

A componente tangencial de cada função de base $\mathbf{w}_k$ é constante ao longo de cada aresta neste espaço de funções. Por isso, avaliar a condição no ponto lateral da aresta determina o valor tangencial em toda a aresta.

Usando:

$$
\mathbf{w}_k(x,y)
=
\begin{bmatrix}
\bar{a}_k+\bar{c}_ky \\
\bar{b}_k-\bar{c}_kx
\end{bmatrix}
$$

e:

$$
\mathbf{t}_{r+3}
=
\begin{bmatrix}
\cos\theta_{r+3} \\
\sin\theta_{r+3}
\end{bmatrix},
$$

a condição de interpolação tangencial fica:

$$
(\bar{a}_k+\bar{c}_k y_{r+3})\cos\theta_{r+3}
+
(\bar{b}_k-\bar{c}_k x_{r+3})\sin\theta_{r+3}
=\delta_{kr}.
$$

Reorganizando:

$$
\bar{a}_k\cos\theta_{r+3}
+
\bar{b}_k\sin\theta_{r+3}
+
\bar{c}_k
\left(
 y_{r+3}\cos\theta_{r+3}
 -x_{r+3}\sin\theta_{r+3}
\right)
=\delta_{kr}.
$$

Para cada $k$, essa expressão fornece um sistema linear $3\times3$ para determinar $\bar{a}_k$, $\bar{b}_k$ e $\bar{c}_k$.

---

## 9. Coeficientes das funções de aresta

O artigo apresenta diretamente os coeficientes das funções de aresta. Para $k,l,m$ seguindo a convenção cíclica, tem-se:

$$
\bar{a}_k = \frac{
\left[
\left(
 y_{m+3}\cos\theta_{m+3}
 -x_{m+3}\sin\theta_{m+3}
\right)
\sin\theta_{l+3}
-
\left(
 y_{l+3}\cos\theta_{l+3}
 -x_{l+3}\sin\theta_{l+3}
\right)
\sin\theta_{m+3}
\right]
}{\Delta},
$$

$$
\bar{b}_k =
\frac{
\left[
\left(
 y_{l+3}\cos\theta_{l+3}
 -x_{l+3}\sin\theta_{l+3}
\right)
\cos\theta_{m+3}
-
\left(
 y_{m+3}\cos\theta_{m+3}
 -x_{m+3}\sin\theta_{m+3}
\right)
\cos\theta_{l+3}
\right]
}{\Delta},
$$

$$
\bar{c}_k = \frac{
\cos\theta_{l+3}\sin\theta_{m+3}
-
\cos\theta_{m+3}\sin\theta_{l+3}
}{\Delta}.
$$

A quantidade $\Delta$ é dada por:

$$
\Delta
=
\sum_{k=1}^{3}
\left(
 y_{k+3}\cos\theta_{k+3}
 -x_{k+3}\sin\theta_{k+3}
\right)
\left(
 \cos\theta_{l+3}\sin\theta_{m+3}
 -\cos\theta_{m+3}\sin\theta_{l+3}
\right).
$$

Esses coeficientes são a solução explícita do sistema de interpolação tangencial. Eles garantem que as funções $\{U\}$ e $\{V\}$ reproduzam os graus de liberdade tangenciais definidos nos pontos laterais das arestas.

> **Nota de verificação:** a expressão de $\Delta$ usa índices cíclicos dentro de um somatório. Para cada termo do somatório em $k$, os índices $l$ e $m$ devem ser atualizados pela mesma tabela cíclica usada nas funções nodais. A implementação deve testar essa fórmula com triângulos simples, pois erros de permutação de $k,l,m$ podem produzir funções de aresta com orientação incorreta.
>
> **Valor de referência para testes:** para o triângulo de referência $P_1=(0,0)$, $P_2=(1,0)$, $P_3=(0,1)$ com a convenção $\theta_4=0$ (arctan para aresta horizontal), $\theta_5=3\pi/4$, $\theta_6=\pi/2$, o determinante vale $\Delta=\sqrt{2}/2\approx0{,}707$. Esse valor **positivo** é esperado e correto — não indica erro. O sinal de $\Delta$ depende da convenção de $\theta$ e da orientação do triângulo; o que deve ser rejeitado é $|\Delta|\approx0$.

Também é necessário rejeitar elementos para os quais $\Delta=0$ ou $|\Delta|$ seja numericamente pequeno. Nesses casos, o sistema de interpolação tangencial fica singular ou mal condicionado e os coeficientes $\bar{a}_k$, $\bar{b}_k$ e $\bar{c}_k$ deixam de ser confiáveis.

---

## 10. Ângulo da aresta e orientação

O artigo define o ângulo associado ao ponto lateral $k+3$ como:

$$
0 \leq \theta_{k+3}
=
\tan^{-1}\left(\frac{y_k-y_l}{x_k-x_l}\right)
<\pi.
$$

Esse ângulo define a direção tangencial da aresta correspondente. Em termos geométricos, a direção tangente é:

$$
\mathbf{t}_{k+3}
=
\begin{bmatrix}
\cos\theta_{k+3} \\
\sin\theta_{k+3}
\end{bmatrix}.
$$

A orientação da aresta é crucial. Se a mesma aresta for percorrida no sentido oposto, o vetor tangente muda de sinal:

$$
\mathbf{t}\rightarrow -\mathbf{t}.
$$

Consequentemente, a componente tangencial também muda de sinal:

$$
\phi_t\rightarrow -\phi_t.
$$

Isso não é um erro físico. É uma consequência da orientação escolhida para o grau de liberdade de aresta. O que precisa ser garantido é a consistência global: uma aresta compartilhada por dois elementos deve ter uma única orientação global, e cada elemento local deve converter corretamente sua orientação local para essa orientação global.

> **Nota de verificação:** o artigo define $0\leq\theta<\pi$, o que elimina a distinção entre direções opostas em termos de ângulo módulo $\pi$. Para a implementação, recomenda-se calcular o ângulo com `atan2(y_k-y_l, x_k-x_l)` e projetar o resultado para $[0,\pi)$ somando ou subtraindo $\pi$ quando necessário. A orientação global da aresta deve ser tratada por um sinal separado.

---

## 11. Continuidade tangencial

Em eletromagnetismo, nas interfaces entre materiais, as componentes tangenciais dos campos eletromagnéticos obedecem condições de continuidade apropriadas, salvo presença de fontes superficiais ideais.

Para problemas de guias de onda dielétricos sem correntes superficiais impostas nas interfaces internas, a continuidade tangencial é uma condição física essencial para representar corretamente os campos.

Os elementos nodais comuns interpolam componentes cartesianas como valores em vértices. Isso pode impor continuidades inadequadas para campos vetoriais, especialmente em interfaces entre materiais. Já os elementos de aresta controlam diretamente a componente tangencial ao longo das arestas, tornando o espaço discreto mais compatível com a estrutura das equações de Maxwell.

No artigo, essa é uma das razões centrais para usar elementos de aresta: eles permitem impor de forma direta a continuidade tangencial e ajudam a eliminar soluções espúrias.

---

## 12. Relação entre funções nodais, funções de aresta e graus de liberdade

A formulação do artigo combina dois tipos de interpolação no mesmo elemento.

### 12.1 Parte axial

A parte axial usa funções nodais:

$$
\phi_z
=
j\{N\}^T\{\phi_z\}_e.
$$

Aqui:

- $\{N\}$ contém funções lineares $L_1$, $L_2$, $L_3$;
- $\{\phi_z\}_e$ contém graus de liberdade nodais;
- cada grau de liberdade está associado a um vértice do triângulo.

### 12.2 Parte transversal

A parte transversal usa funções de aresta:

$$
\phi_x = \{U\}^T\{\phi_t\}_e,
$$

$$
\phi_y = \{V\}^T\{\phi_t\}_e.
$$

Aqui:

- $\{U\}$ e $\{V\}$ contêm funções associadas às arestas;
- $\{\phi_t\}_e$ contém graus de liberdade tangenciais;
- cada grau de liberdade está associado a uma aresta ou ponto lateral do triângulo.

### 12.3 Vetor elementar completo

O vetor elementar completo é organizado como:

$$
\{\phi\}_e
=
\begin{bmatrix}
\{\phi_t\}_e \\
\{\phi_z\}_e
\end{bmatrix}.
$$

Essa organização separa claramente os graus de liberdade transversais de aresta e os graus de liberdade axiais nodais.

---

## 13. Ponte com a matriz de interpolação $[N]$

A formulação do artigo reúne as interpolações axial e transversal na matriz:

$$
[N]
=
\begin{bmatrix}
\{U\} & \{V\} & \{0\} \\
\{0\} & \{0\} & j\{N\}
\end{bmatrix}.
$$

Essa notação compacta deve ser lida com cuidado. O símbolo $[N]$ não representa apenas as funções nodais tradicionais. Ele reúne:

- funções de aresta para as componentes transversais;
- funções nodais lineares para a componente axial;
- o fator $j$ associado à convenção de fase da componente axial.

Na futura implementação, será importante evitar uma classe ou função chamada simplesmente `N` sem distinguir se ela representa as funções nodais escalares, a matriz de interpolação completa ou os vetores de forma de aresta.

---

## 14. Derivadas das funções de forma

As derivadas das funções nodais são constantes:

$$
\{N_x\}=\frac{\partial\{N\}}{\partial x},
\qquad
\{N_y\}=\frac{\partial\{N\}}{\partial y}.
$$

Com:

$$
\frac{\partial L_k}{\partial x}=\frac{b_k}{2A_e},
\qquad
\frac{\partial L_k}{\partial y}=\frac{c_k}{2A_e}.
$$

As derivadas das funções de aresta também são simples:

$$
\frac{\partial U_k}{\partial y}=\bar{c}_k,
$$

$$
\frac{\partial V_k}{\partial x}=-\bar{c}_k.
$$

Portanto:

$$
U_{k,y}=\bar{c}_k,
\qquad
V_{k,x}=-\bar{c}_k.
$$

Essa relação explica por que as integrais envolvendo $\{U_y\}$ e $\{V_x\}$ no Apêndice possuem uma estrutura particularmente simples.

> **Nota de verificação:** na Equação (28), aparece o termo $-\{U_y\}+\{V_x\}$. Com as definições acima, para cada componente de base, esse termo se torna $-2\bar{c}_k$. Por isso aparecem fatores compatíveis com $4\bar{c}_k\bar{c}_l$ em expressões posteriores. Esse ponto deve ser verificado cuidadosamente contra o sinal adotado para $\{V\}$.

---

## 15. Por que elementos de aresta reduzem soluções espúrias

Soluções espúrias aparecem quando o espaço discreto usado no método numérico permite campos que satisfazem artificialmente o problema algébrico, mas não representam modos eletromagnéticos físicos.

Em problemas eletromagnéticos vetoriais, isso está relacionado à escolha inadequada do espaço de aproximação. Usar apenas elementos nodais para todas as componentes pode forçar continuidades erradas ou permitir modos não físicos.

Os elementos de aresta são mais adequados porque aproximam campos vetoriais respeitando a continuidade tangencial. Assim, eles se alinham melhor à estrutura do operador rotacional:

> **Nota didática:** As funções de base vetoriais $\mathbf{w}_k$ usadas no artigo são uma forma de construir os chamados **elementos de Nédélec** de primeira ordem (ou elementos de Whitney), que são fundamentais em eletromagnetismo computacional.

$$
\nabla\times(\cdot).
$$

No artigo, essa compatibilidade é usada para construir uma formulação capaz de eliminar soluções espúrias sem depender de coeficiente de penalidade arbitrário.

---

## 16. Riscos de implementação

A futura implementação deve tratar com atenção os seguintes pontos:

1. **Orientação local dos vértices:** se a orientação do triângulo mudar, o sinal de $A_e$, $b_k$ e $c_k$ pode mudar. As integrais do Apêndice devem ser avaliadas com área positiva; elementos em orientação horária devem ser reordenados ou tratados explicitamente antes da montagem.

2. **Área quase nula:** triângulos degenerados ou muito finos podem gerar instabilidade nas funções nodais e nas matrizes elementares.

3. **Mapeamento entre arestas locais e globais:** cada aresta compartilhada por dois elementos deve possuir uma orientação global consistente.

4. **Sinal do grau de liberdade tangencial:** quando a orientação local da aresta não coincidir com a orientação global, o grau de liberdade deve receber o sinal correto.

5. **Uso de $\tan^{-1}$:** a fórmula do artigo usa $\tan^{-1}$ com intervalo $0\leq\theta<\pi$. Em computadores, funções como arctangente simples podem perder informação de quadrante. A decisão numérica deve ser documentada.

6. **Confusão entre $\{N\}$ nodal e $[N]$ matricial:** o artigo reutiliza a letra $N$ em contextos diferentes. O código deve usar nomes internos mais explícitos.

7. **Confusão entre ponto lateral e aresta:** os pontos $4$, $5$ e $6$ representam graus de liberdade de aresta, mas não devem ser tratados como vértices nodais comuns.

8. **Sinal de $\{V_x\}$:** como $V_k=\bar{b}_k-\bar{c}_kx$, sua derivada em relação a $x$ é negativa. Esse sinal é crítico na montagem do operador rotacional.

9. **Consistência com o fator $j$:** o fator $j$ pertence à aproximação de $\phi_z$ e influencia a forma discreta do rotacional. Ele não deve ser duplicado nem omitido.

10. **Interpretação de $\phi_t$:** deve-se confirmar se o grau de liberdade é valor tangencial pontual, valor constante na aresta ou grandeza escalada por comprimento de aresta.

---

## 17. Testes matemáticos futuros

Antes da montagem completa das matrizes FEM, recomenda-se criar testes matemáticos independentes para as funções de forma.

### 17.1 Testes para funções nodais

Verificar se:

$$
L_k(x_i,y_i)=\delta_{ki}.
$$

Verificar também a partição da unidade:

$$
L_1+L_2+L_3=1.
$$

E as derivadas constantes:

$$
\frac{\partial L_k}{\partial x}=\frac{b_k}{2A_e},
\qquad
\frac{\partial L_k}{\partial y}=\frac{c_k}{2A_e}.
$$

### 17.2 Testes para funções de aresta

Verificar se as funções de base tangenciais satisfazem:

$$
\mathbf{w}_k(x_{r+3},y_{r+3})\cdot\mathbf{t}_{r+3}
=\delta_{kr}.
$$

Esse é o teste mais importante para confirmar os coeficientes $\bar{a}_k$, $\bar{b}_k$ e $\bar{c}_k$.

### 17.3 Testes de orientação

Usar o mesmo triângulo com duas orientações de vértices:

$$
(1,2,3)
$$

e:

$$
(1,3,2).
$$

O teste deve revelar quais sinais mudam e quais grandezas devem permanecer invariantes após correção por orientação global.

### 17.4 Testes de derivadas

Confirmar numericamente que:

$$
U_{k,y}=\bar{c}_k,
$$

$$
V_{k,x}=-\bar{c}_k.
$$

E que:

$$
-U_{k,y}+V_{k,x}=-2\bar{c}_k.
$$

### 17.5 Testes das integrais do Apêndice

Após validar as funções de forma, comparar integrais calculadas numericamente por quadratura com as fórmulas analíticas do Apêndice, especialmente:

$$
\iint_e\{U\}\{U\}^T\,dxdy,
$$

$$
\iint_e\{V\}\{V\}^T\,dxdy,
$$

$$
\iint_e\{U\}\{N_x\}^T\,dxdy,
$$

$$
\iint_e\{V\}\{N_y\}^T\,dxdy.
$$

---

## 18. Pendências científicas

1. Conferir no artigo original de Kikuchi [16] a definição exata das variáveis de aresta usadas por Koshiba e Inoue.

2. Confirmar se os pontos laterais $4$, $5$ e $6$ são pontos médios das arestas ou pontos laterais definidos de outra forma.

3. Verificar se a grandeza $\phi_t$ é tratada como valor pontual da componente tangencial, valor constante ao longo da aresta ou grau de liberdade escalado.

4. Confirmar a relação entre a definição de $\theta_{k+3}$ e a orientação local das arestas na Figura 1.

5. Verificar se a fórmula de $\Delta$ deve ser implementada literalmente como no artigo ou se é mais seguro resolver o sistema linear de interpolação tangencial para cada elemento.

6. Conferir se há convenções adicionais de sinal nas referências [5], [8] e [16] que afetam a montagem global.

7. Verificar se a formulação do artigo é equivalente a uma forma particular de elemento de Nédélec/Whitney ou se utiliza uma construção própria baseada em pontos laterais e valores tangenciais constantes.

8. Confirmar como as condições de contorno PEC/PMC afetam os graus de liberdade de aresta e os graus de liberdade nodais na implementação dos exemplos numéricos.

9. Avaliar se a formulação pode ser generalizada para tensores não diagonais ou se as funções de aresta e integrais do Apêndice precisam ser complementadas.

10. Conferir se a notação $[N]$ da Equação (24) deve ser reinterpretada na implementação para evitar conflito com $\{N\}$ das funções nodais.
