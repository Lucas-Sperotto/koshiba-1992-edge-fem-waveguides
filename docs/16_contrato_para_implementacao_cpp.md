# 16. Contrato para implementação C++17

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md)

> **Documento complementar da Fase 2.**
> Este arquivo consolida as decisões matemáticas necessárias para iniciar a futura implementação C++17 da formulação de elementos finitos de Koshiba e Inoue.
> Não contém código. Seu objetivo é servir como ponte entre a documentação matemática e a arquitetura computacional da Fase 3.

---

## 1. Finalidade do contrato

A Fase 2 organizou a base matemática da reprodução do artigo a partir de cinco frentes principais:

1. derivação da Equação (1) a partir das equações de Maxwell;
2. passagem da Equação (1) para o funcional da Equação (6);
3. análise do fator $j$ na Equação (7);
4. estudo das funções nodais e das funções de forma de aresta;
5. revisão das integrais do Apêndice e dos termos cruzados ausentes ou derivados.

Este documento transforma essas decisões em um **contrato de implementação**. Isso significa que, antes de escrever a montagem C++17, cada entidade matemática precisa ter:

- significado físico e geométrico claro;
- relação explícita com as equações do artigo;
- entradas e saídas definidas;
- riscos de implementação conhecidos;
- testes mínimos obrigatórios.

O contrato não decide detalhes de biblioteca, interface gráfica, formato final de entrada ou otimizações. Ele define apenas o que a implementação deve preservar matematicamente.

---

## 2. Escopo matemático inicial

A implementação inicial deve permanecer fiel ao núcleo do artigo:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2[q]\phi = 0.
$$

A variável $\phi$ representa genericamente o campo elétrico $\mathbf{E}$ ou o campo magnético $\mathbf{H}$, dependendo da escolha das matrizes $[p]$ e $[q]$.

Para $\phi=E$:

$$
[p] = [I],
\qquad
[q] =
\begin{bmatrix}
\varepsilon_{rx} & 0 & 0 \\
0 & \varepsilon_{ry} & 0 \\
0 & 0 & \varepsilon_{rz}
\end{bmatrix}.
$$

Para $\phi=H$:

$$
[p] =
\begin{bmatrix}
1/\varepsilon_{rx} & 0 & 0 \\
0 & 1/\varepsilon_{ry} & 0 \\
0 & 0 & 1/\varepsilon_{rz}
\end{bmatrix},
\qquad
[q] = [I].
$$

O escopo inicial assume a forma diagonal usada no artigo para $[p]$ e $[q]$. Extensões para tensores não diagonais devem permanecer documentadas, mas não devem entrar na primeira implementação.

---

## 3. Convenções globais obrigatórias

### 3.1 Convenção temporal e propagação modal

O artigo adota campos com dependência:

$$
\exp[j(\omega t - \beta z)].
$$

Portanto, a dependência longitudinal pode ser isolada como:

$$
\exp(-j\beta z),
$$

e a derivada longitudinal deve ser tratada como:

$$
\frac{\partial}{\partial z} \longrightarrow -j\beta.
$$

A unidade imaginária deve ser representada conceitualmente por $j$, seguindo a convenção usual da engenharia elétrica.

> **Nota de verificação:** a convenção acima está explícita na seção de equações básicas do artigo. Porém, a consistência de sinais na matriz $[B]$, especialmente no termo envolvendo $j\{N_x\}$, deve ser auditada antes da recuperação da componente axial $\{\phi_z\}$.

### 3.2 Uso do fator $j$ na componente axial

A Equação (7) aproxima a componente axial como:

$$
\phi_z = j\{N\}^T\{\phi_z\}_e.
$$

Na implementação, essa decisão deve ser tratada como uma **convenção estrutural da formulação**, não como um detalhe cosmético. Ela altera os sinais intermediários na expansão do rotacional e influencia a montagem de blocos acoplados.

### 3.3 Orientação local do triângulo

Todo triângulo deve possuir orientação local consistente. A convenção inicial recomendada é:

- vértices locais $1$, $2$, $3$ em sentido anti-horário;
- área assinada positiva para elementos válidos;
- rejeição, correção ou normalização explícita de triângulos com área negativa.

A área geométrica usada nas integrais deve ser positiva:

$$
A_e > 0.
$$

Quando a malha vier com orientação arbitrária, a implementação deve decidir explicitamente entre:

1. reordenar os vértices para orientação anti-horária; ou
2. manter a orientação original e usar uma política rigorosa de sinais.

Para a primeira versão, recomenda-se a opção 1.

### 3.4 Convenção cíclica local

O artigo usa a convenção cíclica $k,l,m$ ao redor dos três vértices do triângulo. Para cada $k$, os índices $l$ e $m$ avançam ciclicamente.

A convenção recomendada é:

| $k$ | $l$ | $m$ |
| --- | --- | --- |
| 1 | 2 | 3 |
| 2 | 3 | 1 |
| 3 | 1 | 2 |

Essa tabela deve orientar o cálculo de:

$$
a_k = x_l y_m - x_m y_l,
$$

$$
b_k = y_l - y_m,
$$

$$
c_k = x_m - x_l.
$$

### 3.5 Orientação de arestas

As variáveis transversais $\{\phi_t\}_e$ são associadas aos pontos laterais/arestas do elemento. A orientação de cada aresta deve ser consistente globalmente, porque a componente tangencial depende de direção.

A implementação deve separar:

- orientação local da aresta dentro do triângulo;
- orientação global da aresta na malha;
- sinal de acoplamento entre o grau de liberdade global e a contribuição local.

> **Nota de verificação:** o artigo apresenta os pontos laterais 4 a 6 e os ângulos $\theta_{k+3}$, mas a tradução documental ainda não fechou uma política computacional definitiva para orientação global de arestas compartilhadas.

### 3.6 Indexação local

A indexação matemática do artigo usa índices locais iniciando em 1:

- vértices: $1$, $2$, $3$;
- pontos laterais/arestas: $4$, $5$, $6$.

Na documentação e nos testes matemáticos, essa indexação deve ser preservada. Na implementação interna C++17 poderá haver indexação iniciando em 0, mas a documentação de cada entidade deve mapear explicitamente a equivalência.

### 3.7 Tolerâncias numéricas iniciais

As tolerâncias iniciais sugeridas para testes matemáticos são:

| Tipo de teste | Tolerância inicial sugerida |
| --- | --- |
| Área do triângulo | $10^{-12}$ em escala normalizada |
| Soma das funções nodais | $10^{-12}$ |
| Derivadas nodais constantes | $10^{-12}$ |
| Integrais locais por fórmula fechada versus quadratura | $10^{-10}$ |
| Simetria de matrizes locais esperadas | $10^{-10}$ |
| Comparação de autovalores em casos pequenos | $10^{-8}$ a $10^{-6}$ |

Esses valores são apenas ponto de partida. Devem ser ajustados conforme escala geométrica, condicionamento do problema e precisão das bibliotecas lineares usadas na Fase 3.

---

## 4. Entidades matemáticas que orientarão classes ou funções

### 4.1 `Node`

#### Papel matemático

Representa um ponto geométrico da seção transversal. Na formulação do artigo, os nós de vértice são usados para a componente axial $\phi_z$.

#### Equações relacionadas

- Equação (8): funções nodais $\{N\}$;
- Equação (9): área do triângulo a partir dos vértices;
- Equações (10)–(12): coeficientes $a_k$, $b_k$, $c_k$;
- Equação (22): expansão elementar do campo;
- Equação (23): vetor elementar com graus de liberdade transversais e axiais.

#### Dados de entrada

- coordenada $x$;
- coordenada $y$;
- identificador global;
- marcador físico opcional, como interior, fronteira PEC, fronteira PMC, interface material.

#### Dados de saída

- coordenadas para cálculo geométrico;
- identificação para conectividade de triângulos;
- identificação para graus de liberdade axiais, quando aplicável.

#### Riscos de implementação

- confundir nó geométrico com grau de liberdade;
- aplicar condição de contorno diretamente no nó sem considerar se ela afeta componente axial, transversal ou ambas;
- perder escala geométrica em problemas com dimensões muito pequenas ou muito grandes.

#### Testes mínimos obrigatórios

- verificar armazenamento e recuperação de coordenadas;
- verificar unicidade de identificadores globais;
- verificar se triângulos conectados compartilham o mesmo nó global;
- testar classificação de fronteira em malhas simples.

---

### 4.2 `Edge`

#### Papel matemático

Representa uma aresta geométrica do triângulo e o suporte natural do grau de liberdade tangencial $\phi_t$.

A componente tangencial é definida por:

$$
\phi_t = \phi_x\cos\theta + \phi_y\sin\theta.
$$

#### Equações relacionadas

- Equações (13)–(16): aproximações de $\phi_x$ e $\phi_y$ por funções de aresta;
- Equações (17)–(21): coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$ e ângulos $\theta_{k+3}$;
- Equação (24): matriz de interpolação $[N]$ contendo funções transversais e axiais;
- Equação (28): matriz $[B]$, na qual as componentes transversais entram no rotacional.

#### Dados de entrada

- par de nós extremos;
- identificador global;
- orientação global;
- orientação local em cada triângulo incidente;
- ponto lateral associado, quando usado conforme a notação do artigo;
- marcador físico de fronteira, quando aplicável.

#### Dados de saída

- direção tangencial unitária;
- comprimento geométrico;
- sinal local-global do grau de liberdade de aresta;
- identificação do grau de liberdade transversal.

#### Riscos de implementação

- inverter sinal da componente tangencial em uma aresta compartilhada;
- duplicar uma aresta global que deveria ser compartilhada por dois triângulos;
- confundir ponto lateral geométrico com grau de liberdade de aresta;
- aplicar condição PEC/PMC no grau de liberdade errado.

#### Testes mínimos obrigatórios

- duas células compartilhando uma aresta devem apontar para a mesma aresta global;
- a inversão da orientação local deve alterar apenas o sinal esperado do grau de liberdade local;
- a componente tangencial constante ao longo de uma aresta deve ser reproduzida corretamente;
- arestas de fronteira devem ser identificadas sem ambiguidades.

---

### 4.3 `Triangle`

#### Papel matemático

Representa o elemento triangular de aresta usado na formulação. Ele contém três vértices e três arestas/pontos laterais.

#### Equações relacionadas

- Equação (8): coordenadas de área $L_k$;
- Equação (9): área $A_e$;
- Equações (10)–(12): coeficientes geométricos $a_k$, $b_k$, $c_k$;
- Equações (13)–(21): funções de aresta e seus coeficientes;
- Equações (A1)–(A10): integrais elementares.

#### Dados de entrada

- três nós de vértice;
- três arestas associadas;
- região material do elemento;
- permissividades relativas $\varepsilon_{rx}$, $\varepsilon_{ry}$, $\varepsilon_{rz}$;
- escolha de campo $\phi=E$ ou $\phi=H$.

#### Dados de saída

- área $A_e$;
- centroide $(x_c,y_c)$;
- coeficientes $a_k$, $b_k$, $c_k$;
- coeficientes de aresta $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$;
- matrizes geométricas locais necessárias à montagem.

#### Riscos de implementação

- área negativa por orientação horária;
- degeneração geométrica, com $A_e \approx 0$;
- cálculo incorreto dos índices cíclicos;
- inconsistência entre orientação local do triângulo e orientação global das arestas;
- instabilidade no cálculo de $\theta_{k+3}$ se for usado arco-tangente sem tratamento de quadrante.

#### Testes mínimos obrigatórios

- triângulo de referência com vértices $(0,0)$, $(1,0)$, $(0,1)$;
- verificação de $A_e = 1/2$ no triângulo de referência;
- verificação de $L_1+L_2+L_3=1$;
- verificação de $L_k=1$ no vértice $k$ e $0$ nos demais vértices;
- verificação de derivadas nodais constantes;
- rejeição de triângulos degenerados.

---

### 4.4 `Mesh`

#### Papel matemático

Representa a discretização da seção transversal $\Omega$ do guia de onda.

A integração do funcional ocorre sobre $\Omega$:

$$
F = \iint_{\Omega}
\left[
(\nabla\times\phi)^*\cdot([p]\nabla\times\phi)
-
k_0^2[q]\phi^*\cdot\phi
\right]dxdy.
$$

No método dos elementos finitos, essa integral global é decomposta em soma de integrais elementares:

$$
\iint_{\Omega} (\cdot)\,dxdy
= \sum_e \iint_e (\cdot)\,dxdy.
$$

#### Equações relacionadas

- Equação (6): funcional sobre $\Omega$;
- Equação (25): problema matricial global;
- Equações (26)–(27): matrizes globais como soma de contribuições elementares;
- Equações (29)–(35): blocos globais usados no problema final.

#### Dados de entrada

- lista de nós;
- lista de triângulos;
- conectividade elemento-vértice;
- conectividade elemento-aresta;
- regiões materiais;
- marcadores de fronteira.

#### Dados de saída

- conectividade global de graus de liberdade;
- mapa de nós para graus axiais;
- mapa de arestas para graus transversais;
- lista de elementos válidos para montagem;
- informações de fronteira para aplicação de condições essenciais.

#### Riscos de implementação

- duplicação de arestas globais;
- inconsistência de orientação entre elementos vizinhos;
- mistura de graus de liberdade nodais e de aresta no mesmo mapa;
- contagem errada de variáveis transversais e axiais;
- fronteiras artificiais mal identificadas nos exemplos numéricos.

#### Testes mínimos obrigatórios

- malha de um triângulo;
- malha de dois triângulos compartilhando uma aresta;
- contagem esperada de nós, arestas e elementos;
- verificação de fronteiras;
- verificação de orientação global das arestas.

---

### 4.5 Funções nodais

#### Papel matemático

A componente axial $\phi_z$ é aproximada por funções nodais lineares:

$$
\phi_z = j\{N\}^T\{\phi_z\}_e.
$$

O vetor de funções nodais é:

$$
\{N\} =
\begin{bmatrix}
L_1 \\
L_2 \\
L_3
\end{bmatrix}.
$$

Cada função nodal tem a forma:

$$
L_k(x,y)=\frac{a_k+b_kx+c_ky}{2A_e}.
$$

#### Equações relacionadas

- Equação (7): aproximação de $\phi_z$;
- Equação (8): definição de $\{N\}$;
- Equações (9)–(12): $A_e$, $a_k$, $b_k$, $c_k$;
- Equações (A6)–(A8): integrais nodais e integrais de derivadas nodais.

#### Dados de entrada

- coordenadas do triângulo;
- ponto de avaliação $(x,y)$;
- coeficientes $a_k$, $b_k$, $c_k$;
- área $A_e$.

#### Dados de saída

- valores $L_k(x,y)$;
- derivadas $N_x$ e $N_y$;
- matrizes elementares nodais associadas a A6, A7 e A8.

#### Riscos de implementação

- usar $A_e$ assinado sem controle;
- trocar $b_k$ e $c_k$;
- esquecer o fator $1/(2A_e)$ nas funções;
- esquecer que as derivadas são constantes no triângulo linear;
- aplicar o fator $j$ duas vezes na componente axial.

#### Testes mínimos obrigatórios

- propriedade de partição da unidade:

$$
L_1+L_2+L_3=1;
$$

- propriedade interpolatória:

$$
L_k(x_i,y_i)=\delta_{ki};
$$

- soma das derivadas nula:

$$
\sum_k \frac{\partial L_k}{\partial x}=0,
\qquad
\sum_k \frac{\partial L_k}{\partial y}=0;
$$

- comparação das integrais A6, A7 e A8 com quadratura numérica independente.

---

### 4.6 Funções de aresta

#### Papel matemático

As componentes transversais são aproximadas por funções associadas às arestas:

$$
\phi_x = \{U\}^T\{\phi_t\}_e,
$$

$$
\phi_y = \{V\}^T\{\phi_t\}_e.
$$

com:

$$
\{U\}=\begin{bmatrix}
\bar{a}_1+\bar{c}_1y\\
\bar{a}_2+\bar{c}_2y\\
\bar{a}_3+\bar{c}_3y
\end{bmatrix},
$$

$$
\{V\}=\begin{bmatrix}
\bar{b}_1-\bar{c}_1x\\
\bar{b}_2-\bar{c}_2x\\
\bar{b}_3-\bar{c}_3x
\end{bmatrix}.
$$

#### Equações relacionadas

- Equações (13)–(16): definição de $\phi_x$, $\phi_y$, $\{U\}$ e $\{V\}$;
- Equações (17)–(21): coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$;
- Equação (24): interpolação completa do campo;
- Equação (28): matriz do rotacional;
- Equações (A1)–(A5): integrais envolvendo funções de aresta e acoplamentos aresta-nó.

#### Dados de entrada

- triângulo orientado;
- arestas locais;
- pontos laterais $4$, $5$, $6$;
- ângulos $\theta_{k+3}$;
- coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$;
- ponto de avaliação $(x,y)$.

#### Dados de saída

- valores de $\{U\}$;
- valores de $\{V\}$;
- derivadas $\{U_y\}$ e $\{V_x\}$;
- componente tangencial em cada aresta;
- blocos geométricos locais A1, A2, A3, A4 e A5.

#### Riscos de implementação

- erro de sinal em $\{V\}$, especialmente no termo $-\bar{c}_kx$;
- erro de quadrante no cálculo de $\theta$;
- inconsistência entre a orientação de aresta usada no cálculo dos coeficientes e a orientação usada no grau de liberdade global;
- confundir continuidade tangencial com continuidade componente a componente;
- implementar funções de aresta como se fossem funções nodais comuns.

#### Testes mínimos obrigatórios

- componente tangencial constante em cada lado do triângulo;
- continuidade tangencial entre dois triângulos compartilhando aresta;
- comparação de A1, A2, A3, A4 e A5 com quadratura independente;
- verificação do sinal quando a orientação local da aresta é invertida;
- teste de elemento de referência e elemento transladado/rotacionado.

---

### 4.7 Matrizes locais

#### Papel matemático

As matrizes locais representam a contribuição de cada elemento para os blocos globais da formulação.

A formulação direta do artigo envolve:

$$
[K]\{\phi\}-k_0^2[M]\{\phi\}=\{0\}.
$$

Depois, o problema é reorganizado para obter diretamente $\beta^2$:

$$
[K_{tt}]\{\phi_t\}-\beta^2[\hat{M}_{tt}]\{\phi_t\}=\{0\}.
$$

#### Equações relacionadas

- Equação (25): problema matricial inicial;
- Equação (26): matriz $[K]$;
- Equação (27): matriz $[M]$;
- Equações (29a)–(29c): blocos diretos;
- Equações (30a)–(30b): blocos de massa;
- Equações (32a)–(32c): blocos reorganizados para o problema em $\beta^2$;
- Equação (33): $[M_{tt}]$ reorganizada;
- Equações (A1)–(A10): integrais elementares.

#### Dados de entrada

- objeto matemático do triângulo;
- propriedades materiais locais;
- escolha de campo $\phi=E$ ou $\phi=H$;
- frequência ou número de onda $k_0$;
- convenção de montagem direta ou reorganizada;
- blocos geométricos elementares A1–A8.

#### Dados de saída

- blocos locais de $K_{tt}$;
- blocos locais de $K_{tz}$;
- blocos locais de $K_{zt}$;
- blocos locais de $K_{zz}$;
- blocos locais de $M_{tt}$;
- blocos locais de $M_{zz}$;
- versões diretas e reorganizadas claramente identificadas.

#### Riscos de implementação

- reutilizar o mesmo nome para matrizes diferentes, como ocorre no artigo com $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$;
- misturar os blocos das Equações (29) e (32);
- aplicar $k_0^2$ no bloco errado;
- aplicar $\beta$ ou $\beta^2$ em uma formulação que já foi reorganizada;
- usar inversa explícita de $[K_{zz}]$ sem necessidade;
- perder simetria esperada por erro de transposição;
- montar $[M]$ com blocos cruzados que o artigo não possui.

#### Testes mínimos obrigatórios

- teste de dimensão de todos os blocos locais;
- teste de simetria dos blocos que devem ser simétricos;
- comparação de matrizes locais com quadratura numérica independente;
- teste em material isotrópico, onde $p_x=p_y=p_z$ ou $q_x=q_y=q_z$ conforme o campo escolhido;
- teste em material anisotrópico diagonal;
- teste de consistência entre $K_{zt}$ e $K_{tz}^T$.

---

### 4.8 Montagem global

#### Papel matemático

A montagem global agrega as contribuições elementares e aplica as condições de contorno, produzindo os sistemas matriciais que serão usados para o problema de autovalores.

#### Equações relacionadas

- Equação (25): sistema global direto;
- Equações (31a)–(31b): sistema particionado em variáveis transversais e axiais;
- Equação (34): problema final de autovalores em $\beta^2$;
- Equação (35): definição de $[\hat{M}_{tt}]$.

#### Dados de entrada

- malha;
- matrizes locais;
- mapas de graus de liberdade;
- condições de contorno essenciais;
- propriedades materiais por elemento;
- frequência ou $k_0$.

#### Dados de saída

- matrizes globais dos blocos transversais e axiais;
- sistema global direto, se usado para auditoria;
- sistema global reduzido em $\{\phi_t\}$;
- autovalores $\beta^2$ em fase futura;
- autovetores transversais em fase futura;
- componente axial recuperada em fase futura.

#### Riscos de implementação

- aplicar a mesma condição de contorno a graus nodais e de aresta sem distinguir sua natureza;
- formar $[K_{zz}]^{-1}$ explicitamente;
- destruir esparsidade cedo demais;
- erro de sinal em $K_{tz}$ ou $K_{zt}$;
- recuperar $\{\phi_z\}$ com convenção de sinal inconsistente;
- resolver autovalores nulos ou espúrios por montar o problema errado.

#### Testes mínimos obrigatórios

- montagem de uma malha com um único triângulo;
- montagem de dois triângulos compartilhando uma aresta;
- verificação de dimensões globais;
- verificação de simetria global quando esperada;
- verificação de preservação de esparsidade antes da redução;
- comparação do problema direto com o problema reduzido em casos pequenos;
- teste de recuperação de $\{\phi_z\}$ após resolver $\{\phi_t\}$.

---

### 4.9 Testes matemáticos

#### Papel matemático

Os testes matemáticos devem proteger a implementação contra erros silenciosos de sinal, orientação, transposição, fator geométrico e convenção complexa.

#### Equações relacionadas

- Equações (7)–(12): funções nodais e fator $j$;
- Equações (13)–(21): funções de aresta;
- Equação (28): rotacional discreto;
- Equações (29)–(35): blocos matriciais;
- Equações (A1)–(A10): integrais locais.

#### Dados de entrada

- triângulos de referência;
- triângulos escalados;
- triângulos rotacionados;
- malhas mínimas;
- materiais isotrópicos e anisotrópicos diagonais;
- valores controlados de $k_0$ e $\beta$.

#### Dados de saída

- aprova/reprova por propriedade matemática;
- erros absolutos e relativos;
- relatório de tolerâncias;
- rastreamento de qual equação do artigo cada teste protege.

#### Riscos de implementação

- testar apenas casos simétricos, mascarando erros de sinal;
- não testar triângulos com coordenadas não inteiras;
- não testar orientação invertida;
- não comparar fórmulas fechadas contra quadratura independente;
- não isolar erro geométrico de erro material.

#### Testes mínimos obrigatórios

- área e centroide;
- coeficientes $a_k$, $b_k$, $c_k$;
- funções nodais $L_k$;
- derivadas nodais;
- funções de aresta $\{U\}$ e $\{V\}$;
- derivadas $\{U_y\}$ e $\{V_x\}$;
- integrais A1–A8;
- transpostas A4/A5 para $K_{zt}$;
- combinação da cadeia A3 produzindo o fator 4;
- consistência de sinal do fator $j$;
- montagem local e global mínima.

---

## 5. Mapa de entidades, equações, entradas, saídas, riscos e testes

| Entidade | Equações relacionadas | Entradas principais | Saídas principais | Riscos centrais | Testes mínimos |
| --- | --- | --- | --- | --- | --- |
| `Node` | (8)–(12), (22), (23) | coordenadas, id, marcador | coordenadas, conectividade, grau axial | confundir nó com grau de liberdade | unicidade, conectividade, fronteira |
| `Edge` | (13)–(21), (24), (28) | nós extremos, orientação, marcador | tangente, comprimento, grau transversal | sinal de orientação | aresta compartilhada, inversão de sinal |
| `Triangle` | (8)–(21), (A1)–(A10) | três nós, três arestas, material | área, centroide, coeficientes | área negativa, degeneração | triângulo de referência, partição da unidade |
| `Mesh` | (6), (25)–(27), (29)–(35) | nós, arestas, triângulos, regiões | mapas globais, fronteiras | duplicação de arestas | malha mínima, contagens globais |
| Funções nodais | (7)–(12), (A6)–(A8) | triângulo, ponto $(x,y)$ | $L_k$, $N_x$, $N_y$ | trocar $b_k$ e $c_k$ | $L_k(x_i)=\delta_{ki}$, soma 1 |
| Funções de aresta | (13)–(21), (A1)–(A5) | triângulo, orientação, ponto | $U$, $V$, $U_y$, $V_x$ | erro de sinal e orientação | continuidade tangencial |
| Matrizes locais | (25)–(33), (A1)–(A10) | triângulo, material, $k_0$ | blocos locais | misturar Eq. (29) e Eq. (32) | simetria, quadratura independente |
| Montagem global | (31)–(35) | malha, blocos locais, BCs | blocos globais, sistema reduzido | inversa explícita, BC errada | malhas mínimas, dimensões, simetria |
| Testes matemáticos | todas as anteriores | casos controlados | erro e aprovação | cobertura incompleta | suíte por equação |

---

## 6. Contrato específico para matrizes e nomes internos

O artigo reutiliza símbolos como $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$ em contextos diferentes. Para evitar ambiguidade, a implementação futura deve adotar nomes conceituais distintos.

| Bloco matemático | Origem | Nome conceitual recomendado |
| --- | --- | --- |
| $[K_{tt}]$ da Eq. (29a) | formulação direta | `Ktt_direct` |
| $[K_{tz}]$ da Eq. (29b) | formulação direta | `Ktz_direct` |
| $[K_{zz}]$ da Eq. (29c) | formulação direta | `Kzz_direct` |
| $[M_{tt}]$ da Eq. (30a) | massa direta | `Mtt_direct` |
| $[M_{zz}]$ da Eq. (30b) | massa direta | `Mzz_direct` |
| $[K_{tt}]$ da Eq. (32a) | problema em $\beta^2$ | `Ktt_beta` |
| $[K_{tz}]$ da Eq. (32b) | problema em $\beta^2$ | `Ktz_beta` |
| $[K_{zz}]$ da Eq. (32c) | problema em $\beta^2$ | `Kzz_beta` |
| $[M_{tt}]$ da Eq. (33) | problema em $\beta^2$ | `Mtt_beta` |
| $[\hat{M}_{tt}]$ da Eq. (35) | matriz reduzida | `Mtt_hat` |

Esses nomes são apenas rótulos de contrato. A Fase 3 poderá ajustar nomes finais, desde que preserve a distinção conceitual.

---

## 7. Condições de contorno no contrato inicial

O artigo menciona o uso de condições de condutor elétrico perfeito e condutor magnético perfeito nas fronteiras artificiais dos exemplos. Para a implementação inicial, o tratamento deve ser conservador:

- condições essenciais devem ser aplicadas removendo, fixando ou restringindo graus de liberdade compatíveis com a componente afetada;
- condições naturais devem emergir da formulação variacional quando apropriado;
- nenhuma integral de linha adicional deve ser introduzida sem documentação matemática prévia.

> **Nota de verificação:** a documentação atual ainda não possui uma derivação completa das condições PEC/PMC para cada uma das escolhas $\phi=E$ e $\phi=H$ dentro desta formulação. Antes dos exemplos completos, será necessário definir exatamente quais graus de liberdade são anulados em cada fronteira.

---

## 8. O que ainda não deve ser implementado

A Fase 3 não deve começar por extensões ou otimizações. Os seguintes itens devem permanecer fora da primeira implementação:

1. tensores $[p]$ e $[q]$ não diagonais;
2. materiais magnéticos com $\mu \neq \mu_0$;
3. meios com perdas ou ativos;
4. PML, ABC ou integrais de contorno não presentes no artigo;
5. acoplamento com excitação externa ou portas;
6. malhas curvas ou elementos de ordem superior;
7. elementos quadrilaterais;
8. otimização de desempenho antes da validação matemática;
9. inversão explícita global de $[K_{zz}]$ como solução definitiva;
10. reprodução direta das figuras completas antes de validar as matrizes locais;
11. pós-processamento avançado de campos antes da convenção de sinal de $\phi_z$ estar auditada;
12. suporte simultâneo a muitas formulações alternativas sem testes isolados.

A primeira implementação deve priorizar:

- geometria triangular;
- funções nodais lineares;
- funções de aresta triangulares conforme o artigo;
- materiais isotrópicos e anisotrópicos diagonais;
- matrizes locais A1–A8;
- montagem global mínima;
- testes matemáticos e validação incremental.

---

## 9. Critérios para abrir a Fase 3

A Fase 3 só deve ser aberta quando os seguintes critérios estiverem satisfeitos:

### 9.1 Critérios matemáticos

- a convenção $\exp[j(\omega t-\beta z)]$ deve estar fixada;
- a transformação $\partial/\partial z \to -j\beta$ deve estar aceita;
- a interpretação do fator $j$ na Equação (7) deve estar documentada;
- o sinal do termo $j\{N_x\}$ na matriz $[B]$ deve estar auditado no PDF original;
- os coeficientes $a_k$, $b_k$, $c_k$ devem estar validados em triângulos de teste;
- os coeficientes $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$ devem ter teste geométrico mínimo;
- as integrais A1–A8 devem estar verificadas contra quadratura independente;
- a cadeia A3 deve explicar e testar o fator 4 das Equações (29a) e (32a).

### 9.2 Critérios de arquitetura

- o mapa entre entidade matemática e entidade computacional deve estar aprovado;
- os nomes conceituais para blocos diretos e reorganizados devem estar definidos;
- a política de orientação de triângulos deve estar definida;
- a política de orientação global de arestas deve estar definida;
- a política inicial de condições de contorno deve estar definida;
- a estrutura mínima de diretórios da implementação deve estar definida.

### 9.3 Critérios de teste

- deve existir uma lista de testes matemáticos obrigatórios antes do solver;
- cada teste deve indicar qual equação protege;
- deve haver pelo menos um triângulo de referência documentado;
- deve haver pelo menos uma malha mínima de dois triângulos;
- deve haver critério de tolerância inicial;
- deve haver decisão sobre comparação por fórmula fechada e quadratura independente.

### 9.4 Critérios de validação incremental

A implementação deve avançar na seguinte ordem:

1. geometria básica;
2. funções nodais;
3. funções de aresta;
4. integrais locais;
5. matrizes locais;
6. montagem global sem autovalores;
7. problema direto para casos pequenos;
8. problema reduzido em $\beta^2$;
9. recuperação de campo axial;
10. reprodução de exemplos simples;
11. reprodução das figuras do artigo.

A Fase 3 não deve começar pela reprodução das figuras. As figuras são consequência da validação matemática, não ponto de partida.

---

## 10. Pendências antes da implementação efetiva

1. Verificar no PDF original o sinal exato do termo $j\{N_x\}$ na Equação (28).
2. Definir a política computacional definitiva para orientação global das arestas.
3. Definir a aplicação de PEC/PMC para cada escolha $\phi=E$ e $\phi=H$.
4. Confirmar se a primeira implementação resolverá inicialmente $\phi=E$, $\phi=H$ ou ambas.
5. Confirmar se a primeira malha será gerada manualmente, importada de arquivo ou produzida por ferramenta externa.
6. Decidir se a validação inicial usará apenas triângulos sintéticos ou também geometrias simplificadas dos exemplos do artigo.
7. Registrar quais integrais do projeto, como A11 e A12, ficam fora da primeira implementação fiel ao artigo.
8. Definir como resolver sistemas envolvendo $[K_{zz}]$ sem formar inversa explícita.
9. Definir formato de saída mínimo para auditoria: matrizes, autovalores, autovetores e campos.
10. Preparar um roteiro de testes antes de qualquer reprodução numérica completa.

---

## 11. Resumo executivo do contrato

A implementação C++17 deve nascer pequena, auditável e fiel ao artigo. O núcleo mínimo é:

- triângulos lineares orientados;
- graus de liberdade nodais para $\phi_z$;
- graus de liberdade de aresta para $\phi_t$;
- funções nodais $\{N\}$;
- funções de aresta $\{U\}$ e $\{V\}$;
- integrais A1–A8;
- matrizes locais separadas por significado;
- montagem global com orientação de arestas controlada;
- problema final em $\beta^2$ apenas depois dos testes locais.

A prioridade científica é evitar erros de sinal, orientação e interpretação das matrizes. A prioridade computacional é manter rastreabilidade direta entre equação, entidade e teste.
