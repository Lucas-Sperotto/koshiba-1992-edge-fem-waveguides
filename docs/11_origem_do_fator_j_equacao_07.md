# 11. Origem do fator $j$ na Equação (7)

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md)

Este arquivo complementa a documentação da Fase 2 do projeto de reprodução do artigo de Koshiba e Inoue. O objetivo é explicar, com cuidado matemático, a presença do fator $j$ na aproximação da componente axial apresentada na Equação (7):

$$
\phi_z = j\{N(x,y)\}^T\{\phi_z\}_e = j\{N\}^T\{\phi_z\}_e.
$$

A explicação abaixo não substitui a formulação original do artigo. Ela serve para orientar a leitura, evitar erros de sinal e preparar a futura implementação numérica.

---

## 1. Convenção de campos harmônicos no tempo

O artigo assume que o campo eletromagnético no guia varia como:

$$
\exp\left[j(\omega t - \beta z)\right].
$$

Isso significa que uma componente genérica do campo pode ser entendida, no domínio físico real, como a parte real de uma grandeza complexa:

$$
\Phi(x,y,z,t)
=
\operatorname{Re}\left\{
\phi(x,y)\exp\left[j(\omega t - \beta z)\right]
\right\}.
$$

Nessa escrita:

- $t$ é o tempo;
- $z$ é a direção longitudinal de propagação;
- $\omega$ é a frequência angular;
- $\beta$ é a constante de propagação;
- $j$ é a unidade imaginária, com $j^2=-1$;
- $\phi(x,y)$ representa a amplitude modal complexa no plano transversal.

A escolha

$$
\exp\left[j(\omega t - \beta z)\right]
$$

corresponde a uma onda que se propaga no sentido positivo de $z$, pois a fase

$$
\omega t - \beta z
$$

permanece constante quando $z$ cresce com $t$.

---

## 2. Por que a engenharia elétrica usa $j$ em vez de $i$

Em matemática pura, a unidade imaginária é geralmente indicada por $i$:

$$
i^2=-1.
$$

Em engenharia elétrica e eletromagnetismo aplicado, usa-se com frequência a letra $j$:

$$
j^2=-1.
$$

A razão prática é evitar confusão com a corrente elétrica, que tradicionalmente é representada por $i$ ou $I$.

Assim, neste projeto:

$$
j \equiv \sqrt{-1}.
$$

Todo fator $j$ deve ser tratado como parte da álgebra complexa do problema modal. Ele não é um índice, não é uma variável geométrica e não deve ser confundido com a numeração de nós, arestas ou elementos.

---

## 3. Derivadas em $z$ na convenção do artigo

Como a dependência longitudinal adotada é

$$
\exp\left[j(\omega t - \beta z)\right],
$$

a derivada em relação a $z$ atua apenas sobre o fator exponencial quando a amplitude modal depende somente de $x$ e $y$:

$$
\frac{\partial}{\partial z}
\left(
\phi(x,y)\exp[j(\omega t - \beta z)]
\right)
=
-j\beta\phi(x,y)\exp[j(\omega t - \beta z)].
$$

Portanto, no domínio modal usado pelo artigo:

$$
\frac{\partial}{\partial z}
\longrightarrow
-j\beta.
$$

Esse é um dos pontos centrais para entender por que fatores imaginários aparecem na formulação.

Se, em outro texto, fosse adotada a convenção longitudinal oposta,

$$
\exp\left[j(\omega t + \beta z)\right],
$$

teríamos:

$$
\frac{\partial}{\partial z}
\longrightarrow
+j\beta.
$$

E se fosse adotada uma convenção temporal do tipo

$$
\exp[-j\omega t],
$$

as equações de Maxwell no domínio da frequência também mudariam sinais intermediários. O resultado físico pode continuar equivalente, mas apenas se toda a formulação for alterada de forma consistente.

> **Nota de verificação:** o artigo informa explicitamente a dependência $\exp[j(\omega t-\beta z)]$, mas não apresenta uma derivação passo a passo do sinal de todos os termos da matriz $[B]$. Por isso, a implementação deve seguir uma convenção única e testável, em vez de misturar sinais de fontes diferentes.

---

## 4. Como o rotacional mistura componentes transversais e longitudinal

Considere um campo vetorial modal genérico:

$$
\boldsymbol{\phi}(x,y,z)
=
\begin{bmatrix}
\phi_x(x,y) \\
\phi_y(x,y) \\
\phi_z(x,y)
\end{bmatrix}
\exp(-j\beta z),
$$

omitindo temporariamente o fator temporal $\exp(j\omega t)$.

O rotacional é dado por:

$$
\nabla \times \boldsymbol{\phi}
=
\begin{bmatrix}
\dfrac{\partial \phi_z}{\partial y}
-
\dfrac{\partial \phi_y}{\partial z}
\\[1.0em]
\dfrac{\partial \phi_x}{\partial z}
-
\dfrac{\partial \phi_z}{\partial x}
\\[1.0em]
\dfrac{\partial \phi_y}{\partial x}
-
\dfrac{\partial \phi_x}{\partial y}
\end{bmatrix}.
$$

Depois da substituição modal,

$$
\frac{\partial}{\partial z}\rightarrow -j\beta,
$$

os termos que envolvem derivadas longitudinais tornam-se imaginários:

$$
-\frac{\partial \phi_y}{\partial z}
\rightarrow
+j\beta\phi_y,
$$

$$
\frac{\partial \phi_x}{\partial z}
\rightarrow
-j\beta\phi_x.
$$

Assim, as componentes transversais do rotacional contêm naturalmente fatores $j\beta$ ou $-j\beta$.

Já a terceira componente do rotacional,

$$
\frac{\partial \phi_y}{\partial x}
-
\frac{\partial \phi_x}{\partial y},
$$

não envolve derivada em $z$. Por isso, ela não recebe automaticamente um fator $j\beta$.

Essa diferença é uma fonte frequente de erro: algumas parcelas do rotacional vêm da propagação longitudinal e carregam $j$; outras vêm apenas de derivadas no plano $xy$ e podem permanecer reais.

---

## 5. O que aconteceria sem o fator $j$ em $\phi_z$

Se a componente axial fosse interpolada diretamente como

$$
\phi_z = \{N\}^T\{\phi_z\}_e,
$$

então as derivadas transversais da componente axial seriam reais, no sentido algébrico da amplitude modal:

$$
\frac{\partial \phi_z}{\partial x}
=
\{N_x\}^T\{\phi_z\}_e,
$$

$$
\frac{\partial \phi_z}{\partial y}
=
\{N_y\}^T\{\phi_z\}_e.
$$

Ao mesmo tempo, os termos provenientes das derivadas longitudinais das componentes transversais envolveriam $j\beta$.

Consequentemente, nas componentes transversais do rotacional haveria uma mistura entre:

- termos imaginários vindos de $\partial/\partial z$;
- termos reais vindos de $\partial\phi_z/\partial x$ e $\partial\phi_z/\partial y$.

Essa mistura não está errada fisicamente. Campos modais guiados podem, de fato, apresentar defasagem entre componentes transversais e longitudinais. Porém, ela torna a álgebra discreta menos conveniente, pois as submatrizes de acoplamento entre variáveis transversais e axiais tendem a carregar fatores imaginários explícitos.

---

## 6. O papel do fator $j$ na Equação (7)

O artigo escolhe aproximar a componente axial como:

$$
\phi_z = j\{N\}^T\{\phi_z\}_e.
$$

Com essa escolha, as derivadas transversais da componente axial também recebem o fator $j$:

$$
\frac{\partial \phi_z}{\partial x}
=
j\{N_x\}^T\{\phi_z\}_e,
$$

$$
\frac{\partial \phi_z}{\partial y}
=
j\{N_y\}^T\{\phi_z\}_e.
$$

Agora, nas componentes transversais do rotacional, os termos associados à propagação longitudinal e os termos associados à componente axial passam a ter uma estrutura de fase compatível. De forma esquemática, aparecem parcelas do tipo:

$$
j\beta\phi_y + j\frac{\partial \phi_z^{(r)}}{\partial y},
$$

ou

$$
-j\beta\phi_x - j\frac{\partial \phi_z^{(r)}}{\partial x},
$$

em que $\phi_z^{(r)}$ representa a parte interpolada por coeficientes nodais antes da multiplicação por $j$.

Assim, o fator $j$ pode ser entendido como uma escolha de fase para os graus de liberdade axiais. Essa escolha desloca a fase da componente longitudinal em relação às componentes transversais e permite organizar a formulação matricial de modo mais conveniente.

No artigo, essa organização aparece novamente na matriz de interpolação da Equação (24):

$$
[N]
=
\begin{bmatrix}
\{U\} & \{V\} & \{0\} \\
\{0\} & \{0\} & j\{N\}
\end{bmatrix},
$$

isto é, o fator $j$ não é um detalhe isolado da Equação (7). Ele acompanha a forma como o campo axial entra na aproximação vetorial completa.

Também aparece uma estrutura compatível na matriz $[B]$ da Equação (28), na qual diversos blocos carregam fatores $j$ associados a $\beta$, $\{N_x\}$ e $\{N_y\}$.

> **Nota de verificação:** o artigo não declara explicitamente “introduzimos $j$ para tornar as matrizes reais” ou frase equivalente. Essa interpretação é uma leitura matemática provável da organização algébrica. A confirmação rigorosa deve ser feita refazendo a expansão completa de $\nabla\times\phi$ e comparando termo a termo com a Equação (28). Por exemplo, a segunda linha da matriz $[B]$ contém os blocos $j\{N_y\}$ e $-j\{N_x\}$, que correspondem exatamente aos termos $\partial\phi_z/\partial y$ e $-\partial\phi_z/\partial x$ após a aplicação da convenção da Equação (7).

---

## 7. Mudança de convenção e sinais intermediários

Se a convenção longitudinal usada fosse

$$
\exp(+j\beta z),
$$

então:

$$
\frac{\partial}{\partial z}\rightarrow +j\beta.
$$

Nesse caso, sinais como $+j\beta\phi_y$ e $-j\beta\phi_x$ poderiam ser trocados. A mesma matriz final poderia ser recuperada somente se a definição dos campos, do operador rotacional, das funções de forma e dos graus de liberdade fosse ajustada de maneira consistente.

O problema não está em escolher uma convenção ou outra. O problema está em misturar convenções. Por exemplo, seria perigoso usar:

- derivadas em $z$ compatíveis com $\exp(-j\beta z)$;
- equações de Maxwell escritas como se a convenção temporal fosse $\exp(-j\omega t)$;
- matriz $[B]$ copiada do artigo sem preservar a definição de $\phi_z$ da Equação (7).

Essas misturas podem produzir sinais errados em $[K_{tz}]$, $[K_{zt}]$ ou em termos proporcionais a $\beta$.

> **Nota de verificação:** a reprodução deve adotar como convenção canônica a do artigo: $\exp[j(\omega t-\beta z)]$. Qualquer comparação com livros ou artigos que usem $\exp(-j\omega t)$ deve ser feita com uma tabela de conversão de sinais.

---

## 8. Relação com as componentes transversais e longitudinais

Em guias de onda, as componentes transversais e longitudinais não são independentes do ponto de vista físico. A propagação em $z$ conecta $\phi_x$, $\phi_y$ e $\phi_z$ por meio das equações de Maxwell e do operador rotacional.

Na formulação do artigo:

- as variáveis transversais são associadas às arestas do elemento triangular;
- a variável axial é associada aos vértices;
- a componente axial é multiplicada por $j$ antes de entrar na aproximação do campo.

Essa separação é coerente com o fato de que as componentes transversais e longitudinais podem estar em quadratura de fase, isto é, defasadas de $90^\circ$ em uma representação fasorial.

Multiplicar por $j$ equivale precisamente a introduzir uma rotação de fase de $90^\circ$:

$$
j = \exp\left(j\frac{\pi}{2}\right).
$$

Portanto, a Equação (7) pode ser lida como uma escolha de representação: os graus de liberdade nodais $\{\phi_z\}_e$ armazenam a amplitude axial em uma convenção real/auxiliar, enquanto o campo axial físico modal usado na interpolação recebe o fator complexo $j$.

> **Nota de verificação:** essa interpretação é útil para implementação, mas o artigo não explicita se os autores pretendiam armazenar internamente graus de liberdade reais. Deve-se verificar, na reprodução algébrica das matrizes, se essa escolha permite montar o problema final usando matrizes reais nos casos sem perdas e com tensores diagonais reais.

---

## 9. Relação direta com a Equação (7)

A Equação (7) deve ser preservada exatamente na documentação e na implementação conceitual:

$$
\phi_z = j\{N\}^T\{\phi_z\}_e.
$$

Ela não deve ser interpretada como:

$$
\phi_z = \{N\}^T\{\phi_z\}_e.
$$

Também não deve ser implementada implicitamente duas vezes. Se o fator $j$ for incluído na matriz de interpolação $[N]$, ele não deve ser novamente multiplicado em outro ponto da montagem como se ainda estivesse ausente.

O mesmo cuidado vale para a matriz $[B]$. Se $[B]$ for derivada simbolicamente a partir da aproximação dos campos, o fator $j$ deve aparecer naturalmente. Se $[B]$ for implementada diretamente a partir da Equação (28), então é necessário garantir que a definição de graus de liberdade seja compatível com a Equação (7).

---

## 10. Risco de erro de sinal no código

O maior risco na futura implementação C++ não é apenas esquecer o fator $j$. O risco maior é duplicar, cancelar ou deslocar esse fator para o lugar errado.

Erros comuns que devem ser evitados:

1. **Remover o fator $j$ de $\phi_z$** e continuar usando a matriz $[B]$ do artigo como se nada tivesse mudado.
2. **Aplicar $j$ em $\phi_z$ e também reaplicá-lo em $\{N_x\}$ ou $\{N_y\}$**, duplicando o fator imaginário.
3. **Trocar o sinal de $\partial/\partial z$**, usando $+j\beta$ em vez de $-j\beta$ sem alterar toda a formulação.
4. **Misturar convenções temporais**, por exemplo usando equações compatíveis com $\exp(-j\omega t)$ enquanto o artigo usa $\exp(+j\omega t)$.
5. **Assumir que todos os termos com $j$ desaparecem automaticamente**, sem verificar a montagem de $[K]$, $[M]$, $[K_{tz}]$ e $[K_{zt}]$.
6. **Alterar a orientação das arestas sem atualizar os sinais das variáveis tangenciais**, o que pode parecer erro do fator $j$, mas na verdade é erro de orientação geométrica.

A consequência desses erros pode ser séria:

- autovalores negativos ou fisicamente incompatíveis;
- modos duplicados ou ausentes;
- perda de simetria esperada;
- surgimento de componentes imaginárias em casos que deveriam produzir matrizes reais;
- curvas de dispersão deslocadas em relação às figuras do artigo.

---

## 11. Como testar essa convenção futuramente

Antes de implementar os exemplos completos do artigo, a convenção do fator $j$ deve ser testada em níveis progressivos.

### 11.1 Teste simbólico local do rotacional

Escolher funções simples para $\phi_x$, $\phi_y$ e $\phi_z$ em um único elemento e calcular manualmente:

$$
\nabla\times\phi.
$$

Depois, comparar os termos obtidos com a estrutura da matriz $[B]$ da Equação (28). Esse teste deve verificar especialmente:

- o sinal dos termos proporcionais a $\beta$;
- o sinal dos termos com $\{N_x\}$;
- o sinal dos termos com $\{N_y\}$;
- a presença do fator $j$ apenas onde ele deve aparecer.

### 11.2 Teste de consistência com $\beta=0$

Para $\beta=0$, os termos provenientes de derivadas longitudinais desaparecem:

$$
\frac{\partial}{\partial z}\rightarrow 0.
$$

Esse caso ajuda a separar erros associados à propagação longitudinal de erros associados às derivadas transversais.

### 11.3 Teste de troca de sinal de $\beta$

Comparar formalmente os resultados para $+\beta$ e $-\beta$. Em problemas sem perdas e com materiais reais, espera-se que a física de propagação em sentidos opostos seja relacionada por conjugação ou simetria apropriada.

> **Nota de verificação:** a forma exata dessa simetria deve ser conferida para cada condição de contorno e para cada escolha de campo $\phi=E$ ou $\phi=H$.

### 11.4 Teste de matriz real em caso sem perdas

Em meios sem perdas, com tensores diagonais reais e seguindo a convenção do artigo, deve-se verificar se a montagem final pode ser representada por matrizes reais ou por uma estrutura complexa com cancelamentos esperados.

Se aparecerem partes imaginárias residuais em termos que deveriam ser reais, isso pode indicar erro de sinal ou duplicação do fator $j$.

### 11.5 Teste de reprodução de uma curva simples

Após os testes locais, a convenção deve ser validada em um caso pequeno e conhecido, antes de reproduzir as figuras completas:

- um guia retangular dielétrico simples;
- uma malha muito pequena apenas para verificar tendências;
- comparação qualitativa da constante normalizada $b$ com o intervalo físico esperado.

Esse teste não substitui a validação final contra as Figuras 5 e 7 do artigo, mas ajuda a detectar erros grosseiros de convenção.

---

## 12. Decisão documental para o projeto

Para manter rastreabilidade, este projeto adotará como convenção canônica:

$$
\exp[j(\omega t-\beta z)].
$$

Consequentemente:

$$
\frac{\partial}{\partial t}\rightarrow j\omega,
$$

$$
\frac{\partial}{\partial z}\rightarrow -j\beta.
$$

E a componente axial será tratada conforme a Equação (7):

$$
\phi_z = j\{N\}^T\{\phi_z\}_e.
$$

A implementação futura deverá decidir explicitamente onde esse fator será representado:

- na matriz de interpolação;
- na matriz associada ao rotacional;
- ou na definição dos graus de liberdade axiais.

Mas essa decisão deve ser única. O fator $j$ não pode ser espalhado em vários pontos do código sem uma regra clara.

---
