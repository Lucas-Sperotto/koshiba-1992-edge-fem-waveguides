# 9. Das equações de Maxwell à Equação (1)

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | **09** | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md)

Este arquivo é uma explicação complementar para a Fase 2 do projeto. O objetivo é detalhar, de forma didática e rastreável, como a Equação (1) do artigo de Koshiba e Inoue pode ser compreendida a partir das equações de Maxwell no domínio da frequência.

A Equação (1) do artigo é:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0.
$$

Ela é uma forma compacta de escrever a equação vetorial de onda tanto para o campo elétrico quanto para o campo magnético, desde que as matrizes $[p]$ e $[q]$ sejam escolhidas de acordo com o campo usado.

---

## 1. Convenção temporal e forma harmônica dos campos

O artigo assume que o campo eletromagnético no guia varia como:

$$
\exp\left[j(\omega t - \beta z)\right].
$$

Essa expressão fixa uma convenção importante:

- $j$ é a unidade imaginária, com $j^2 = -1$;
- $\omega$ é a frequência angular, em rad/s;
- $t$ é o tempo;
- $z$ é a direção longitudinal de propagação;
- $\beta$ é a constante de propagação na direção $z$.

Assim, um campo vetorial físico pode ser entendido como a parte real de uma quantidade complexa do tipo:

$$
\mathbf{E}(x,y,z,t)
= \Re\left\{\mathbf{E}(x,y)\exp\left[j(\omega t - \beta z)\right]\right\}.
$$

De modo análogo:

$$
\mathbf{H}(x,y,z,t)
= \Re\left\{\mathbf{H}(x,y)\exp\left[j(\omega t - \beta z)\right]\right\}.
$$

Com essa convenção, as derivadas harmônicas são substituídas por fatores algébricos:

$$
\frac{\partial}{\partial t} \longrightarrow j\omega,
$$

$$
\frac{\partial}{\partial z} \longrightarrow -j\beta.
$$

A primeira substituição é usada para escrever as equações de Maxwell no domínio da frequência. A segunda será importante mais adiante, quando o operador rotacional for expandido na formulação por elementos finitos e aparecerem termos proporcionais a $\beta$.

> **Nota de verificação:** O artigo explicita a dependência $\exp[j(\omega t - \beta z)]$, mas não desenvolve detalhadamente a passagem das equações no tempo para as equações fasoriais. A interpretação acima é a convenção fasorial compatível com essa dependência temporal.

---

## 2. Equações de Maxwell no domínio da frequência

Em uma região sem fontes impressas, isto é, sem corrente elétrica externa explicitada e sem densidade de carga livre explicitada, as equações rotacionais de Maxwell no domínio do tempo são:

$$
\nabla \times \mathbf{E} = -\frac{\partial \mathbf{B}}{\partial t},
$$

$$
\nabla \times \mathbf{H} = \frac{\partial \mathbf{D}}{\partial t}.
$$

Usando apenas a parte temporal da convenção, $\exp(j\omega t)$, obtém-se:

$$
\nabla \times \mathbf{E} = -j\omega \mathbf{B},
$$

$$
\nabla \times \mathbf{H} = j\omega \mathbf{D}.
$$

As relações constitutivas conectam os campos auxiliares $\mathbf{D}$ e $\mathbf{B}$ aos campos $\mathbf{E}$ e $\mathbf{H}$:

$$
\mathbf{D} = \varepsilon_0 [\varepsilon_r]\mathbf{E},
$$

$$
\mathbf{B} = \mu_0\mathbf{H}.
$$

Como $\mu_0$ é escalar constante no espaço para o escopo do artigo, ela pode ser fatorada para fora dos operadores diferenciais nas derivações abaixo. Essa simplificação não seria válida sem revisão se a permeabilidade magnética fosse tensorial ou espacialmente variável.

Aqui:

- $\varepsilon_0$ é a permissividade elétrica do espaço livre;
- $\mu_0$ é a permeabilidade magnética do espaço livre;
- $[\varepsilon_r]$ é a matriz de permissividade relativa do material;
- $k_0$ é o número de onda no espaço livre, definido por:

$$
k_0 = \omega\sqrt{\mu_0\varepsilon_0}.
$$

Portanto:

$$
k_0^2 = \omega^2\mu_0\varepsilon_0.
$$

> **Nota de verificação:** A hipótese $\mathbf{B}=\mu_0\mathbf{H}$ corresponde a meio não magnético. O artigo não destaca essa hipótese como uma seção separada, mas ela fica implícita na forma das matrizes $[p]$ e $[q]$, que envolvem apenas permissividade relativa.

---

## 3. Matriz de permissividade relativa

O artigo considera um guia de onda dielétrico com tensor de permissividade diagonal. Assim, a permissividade relativa pode ser escrita como:

$$
[\varepsilon_r]
=
\begin{bmatrix}
\varepsilon_{rx} & 0 & 0 \\
0 & \varepsilon_{ry} & 0 \\
0 & 0 & \varepsilon_{rz}
\end{bmatrix}.
$$

Como o artigo também escreve as permissividades relativas em termos dos índices de refração, tem-se:

$$
\varepsilon_{rx}=n_x^2,
$$

$$
\varepsilon_{ry}=n_y^2,
$$

$$
\varepsilon_{rz}=n_z^2.
$$

Logo:

$$
[\varepsilon_r]
=
\begin{bmatrix}
n_x^2 & 0 & 0 \\
0 & n_y^2 & 0 \\
0 & 0 & n_z^2
\end{bmatrix}.
$$

Quando a formulação é escrita para o campo magnético, aparece a matriz inversa:

$$
[\varepsilon_r]^{-1}
=
\begin{bmatrix}
\dfrac{1}{\varepsilon_{rx}} & 0 & 0 \\
0 & \dfrac{1}{\varepsilon_{ry}} & 0 \\
0 & 0 & \dfrac{1}{\varepsilon_{rz}}
\end{bmatrix}
=
\begin{bmatrix}
\dfrac{1}{n_x^2} & 0 & 0 \\
0 & \dfrac{1}{n_y^2} & 0 \\
0 & 0 & \dfrac{1}{n_z^2}
\end{bmatrix}.
$$

Essa inversa permanece dentro do operador rotacional na formulação em $\mathbf{H}$ porque $[\varepsilon_r]$ pode variar por região material na seção transversal. Para a reprodução inicial, assume-se que cada componente principal da permissividade relativa é positiva, de modo que $[\varepsilon_r]^{-1}$ exista em cada material.

---

## 4. Meio isotrópico e meio anisotrópico

### 4.1 Meio isotrópico

Em um meio isotrópico, a resposta elétrica do material é a mesma em todas as direções. Nesse caso:

$$
\varepsilon_{rx}=\varepsilon_{ry}=\varepsilon_{rz}=\varepsilon_r.
$$

Consequentemente:

$$
[\varepsilon_r]
=
\varepsilon_r[I],
$$

em que $[I]$ é a matriz identidade:

$$
[I]
=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1
\end{bmatrix}.
$$

Nesse caso, a anisotropia desaparece e as direções $x$, $y$ e $z$ têm o mesmo índice de refração:

$$
n_x=n_y=n_z=n.
$$

### 4.2 Meio anisotrópico diagonal

Em um meio anisotrópico diagonal, a resposta elétrica pode ser diferente nas direções $x$, $y$ e $z$, mas não há acoplamento direto entre componentes diferentes do campo. Assim:

$$
[\varepsilon_r]
=
\begin{bmatrix}
\varepsilon_{rx} & 0 & 0 \\
0 & \varepsilon_{ry} & 0 \\
0 & 0 & \varepsilon_{rz}
\end{bmatrix}.
$$

Isso significa que a componente $D_x$ depende de $E_x$, a componente $D_y$ depende de $E_y$, e a componente $D_z$ depende de $E_z$:

$$
D_x = \varepsilon_0\varepsilon_{rx}E_x,
$$

$$
D_y = \varepsilon_0\varepsilon_{ry}E_y,
$$

$$
D_z = \varepsilon_0\varepsilon_{rz}E_z.
$$

### 4.3 Meio anisotrópico geral

Um meio anisotrópico geral poderia envolver uma matriz cheia:

$$
[\varepsilon_r]
=
\begin{bmatrix}
\varepsilon_{xx} & \varepsilon_{xy} & \varepsilon_{xz} \\
\varepsilon_{yx} & \varepsilon_{yy} & \varepsilon_{yz} \\
\varepsilon_{zx} & \varepsilon_{zy} & \varepsilon_{zz}
\end{bmatrix}.
$$

Entretanto, essa não é a forma usada diretamente na Equação (1) do artigo, pois o artigo define $[p]$ e $[q]$ como matrizes diagonais.

> **Nota de verificação:** Extensões para matrizes constitutivas não diagonais devem ser tratadas como ampliação futura do projeto, não como parte da reprodução inicial da formulação de Koshiba e Inoue apresentada na Equação (1).

---

## 5. Derivação da equação vetorial para o campo elétrico

Comecemos pelas equações fasoriais:

$$
\nabla \times \mathbf{E} = -j\omega\mu_0\mathbf{H},
$$

$$
\nabla \times \mathbf{H} = j\omega\varepsilon_0[\varepsilon_r]\mathbf{E}.
$$

Tomando o rotacional da primeira equação:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
=
\nabla \times \left(-j\omega\mu_0\mathbf{H}\right).
$$

Como $\omega$ e $\mu_0$ são constantes:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
=
-j\omega\mu_0\left(\nabla \times \mathbf{H}\right).
$$

Substituindo a equação de Maxwell para $\nabla \times \mathbf{H}$:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
=
-j\omega\mu_0\left(j\omega\varepsilon_0[\varepsilon_r]\mathbf{E}\right).
$$

Como:

$$
-j \cdot j = -j^2 = 1,
$$

segue que:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
=
\omega^2\mu_0\varepsilon_0[\varepsilon_r]\mathbf{E}.
$$

Usando $k_0^2=\omega^2\mu_0\varepsilon_0$:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
=
k_0^2[\varepsilon_r]\mathbf{E}.
$$

Reorganizando:

$$
\nabla \times \left(\nabla \times \mathbf{E}\right)
-
k_0^2[\varepsilon_r]\mathbf{E}
=0.
$$

Essa é a equação vetorial de onda do tipo `curl-curl` para o campo elétrico.

Comparando com a forma compacta do artigo:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0,
$$

identificamos, para $\phi=\mathbf{E}$:

$$
[p]=[I],
$$

$$
[q]=[\varepsilon_r].
$$

Ou seja:

$$
p_x=p_y=p_z=1,
$$

$$
q_x=\varepsilon_{rx}=n_x^2,
$$

$$
q_y=\varepsilon_{ry}=n_y^2,
$$

$$
q_z=\varepsilon_{rz}=n_z^2.
$$

---

## 6. Derivação da equação vetorial para o campo magnético

Agora partimos novamente das equações fasoriais:

$$
\nabla \times \mathbf{E} = -j\omega\mu_0\mathbf{H},
$$

$$
\nabla \times \mathbf{H} = j\omega\varepsilon_0[\varepsilon_r]\mathbf{E}.
$$

Da segunda equação, isolamos $\mathbf{E}$:

$$
[\varepsilon_r]\mathbf{E}
=
\frac{1}{j\omega\varepsilon_0}\nabla \times \mathbf{H}.
$$

Multiplicando por $[\varepsilon_r]^{-1}$:

$$
\mathbf{E}
=
\frac{1}{j\omega\varepsilon_0}[\varepsilon_r]^{-1}\nabla \times \mathbf{H}.
$$

Substituímos essa expressão na equação de Faraday:

$$
\nabla \times \mathbf{E} = -j\omega\mu_0\mathbf{H}.
$$

Logo:

$$
\nabla \times
\left(
\frac{1}{j\omega\varepsilon_0}[\varepsilon_r]^{-1}\nabla \times \mathbf{H}
\right)
=
-j\omega\mu_0\mathbf{H}.
$$

Como $\omega$ e $\varepsilon_0$ são constantes:

$$
\frac{1}{j\omega\varepsilon_0}
\nabla \times
\left(
[\varepsilon_r]^{-1}\nabla \times \mathbf{H}
\right)
=
-j\omega\mu_0\mathbf{H}.
$$

Multiplicando ambos os lados por $j\omega\varepsilon_0$:

$$
\nabla \times
\left(
[\varepsilon_r]^{-1}\nabla \times \mathbf{H}
\right)
=
(j\omega\varepsilon_0)(-j\omega\mu_0)\mathbf{H}.
$$

Como:

$$
j\cdot(-j)=-j^2=1,
$$

obtém-se:

$$
\nabla \times
\left(
[\varepsilon_r]^{-1}\nabla \times \mathbf{H}
\right)
=
\omega^2\mu_0\varepsilon_0\mathbf{H}.
$$

Usando novamente $k_0^2=\omega^2\mu_0\varepsilon_0$:

$$
\nabla \times
\left(
[\varepsilon_r]^{-1}\nabla \times \mathbf{H}
\right)
-
k_0^2\mathbf{H}
=0.
$$

Comparando com a forma compacta do artigo:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0,
$$

identificamos, para $\phi=\mathbf{H}$:

$$
[p]=[\varepsilon_r]^{-1},
$$

$$
[q]=[I].
$$

Ou seja:

$$
p_x=\frac{1}{\varepsilon_{rx}}=\frac{1}{n_x^2},
$$

$$
p_y=\frac{1}{\varepsilon_{ry}}=\frac{1}{n_y^2},
$$

$$
p_z=\frac{1}{\varepsilon_{rz}}=\frac{1}{n_z^2},
$$

$$
q_x=q_y=q_z=1.
$$

---

## 7. Por que o artigo usa a variável genérica $\phi$

A Equação (1) é escrita com uma variável genérica $\phi$ porque a mesma estrutura matemática serve para duas formulações:

1. formulação em termos do campo elétrico $\mathbf{E}$;
2. formulação em termos do campo magnético $\mathbf{H}$.

A diferença entre as duas formulações não está na forma do operador, mas na escolha das matrizes $[p]$ e $[q]$.

Para o campo elétrico:

$$
\phi=\mathbf{E},
\qquad
[p]=[I],
\qquad
[q]=[\varepsilon_r].
$$

Para o campo magnético:

$$
\phi=\mathbf{H},
\qquad
[p]=[\varepsilon_r]^{-1},
\qquad
[q]=[I].
$$

Essa escolha tem uma consequência importante para o projeto: a implementação pode ser organizada em torno de uma formulação genérica `curl-curl`, na qual o tipo de campo selecionado define os coeficientes materiais usados nas matrizes.

> **Nota de verificação:** O artigo usa o símbolo $\phi$ sem negrito, mas a Equação (1) envolve rotacional. Portanto, no contexto da equação vetorial, $\phi$ deve ser interpretado como campo vetorial. A documentação do projeto preserva a notação original para manter rastreabilidade com o artigo.

---

## 8. Forma matricial usada pelo artigo

O artigo escreve:

$$
[p]
=
\begin{bmatrix}
p_x & 0 & 0 \\
0 & p_y & 0 \\
0 & 0 & p_z
\end{bmatrix},
$$

$$
[q]
=
\begin{bmatrix}
q_x & 0 & 0 \\
0 & q_y & 0 \\
0 & 0 & q_z
\end{bmatrix}.
$$

Assim, a Equação (1) pode ser lida como uma equação vetorial unificada:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0.
$$

Essa escrita é compacta, mas possui significado físico bem definido:

- o termo $\nabla \times \phi$ representa a rotação do campo escolhido;
- a matriz $[p]$ pondera o termo rotacional conforme o tipo de campo e o material;
- o termo $k_0^2[q]\phi$ representa a parte associada à resposta constitutiva do meio;
- $k_0$ introduz a escala de comprimento de onda no espaço livre;
- $[q]$ indica como o campo escolhido é ponderado pela permissividade relativa ou pela identidade.

No caso elétrico, o material aparece no termo de massa, por meio de $[q]=[\varepsilon_r]$. No caso magnético, o material aparece no termo rotacional, por meio de $[p]=[\varepsilon_r]^{-1}$.

---

## 9. Relação com a dependência longitudinal em $z$

A Equação (1) ainda aparece em forma vetorial compacta. Como o campo varia segundo:

$$
\exp\left[j(\omega t - \beta z)\right],
$$

a derivada em relação a $z$ é substituída por:

$$
\frac{\partial}{\partial z}\longrightarrow -j\beta.
$$

Essa substituição é o que faz a constante de propagação $\beta$ aparecer na formulação discreta posterior. Na seção de elementos finitos, ao expandir o operador rotacional para as componentes transversais e axial do campo, surgem termos proporcionais a $j\beta$, a $\beta$ e a $\beta^2$.

> **Nota de verificação:** A derivação detalhada da matriz $[B]$ a partir da expansão componente a componente do rotacional não está incluída neste arquivo. Ela deve ser desenvolvida em documento próprio, ligado à passagem da Equação (1) para as Equações (22)–(35).

---

## 10. O que foi assumido e o que não foi assumido aqui

Esta derivação usa apenas o necessário para recuperar a forma da Equação (1):

- campos harmônicos no tempo;
- dependência longitudinal $\exp(-j\beta z)$ dentro da convenção completa $\exp[j(\omega t-\beta z)]$;
- ausência de fontes impressas na região de análise modal;
- meio dielétrico com tensor de permissividade relativo diagonal;
- meio não magnético, com $\mu=\mu_0$;
- uso de $\mathbf{E}$ ou $\mathbf{H}$ como campo vetorial principal.

Não foram introduzidos neste arquivo:

- tensores de permeabilidade magnética;
- acoplamentos magnetoelétricos;
- anisotropia não diagonal;
- perdas materiais complexas em detalhe;
- meios ativos em detalhe;
- condições de contorno específicas;
- forma fraca ou funcional variacional;
- discretização por elementos finitos.

Esses tópicos podem ser relevantes em extensões futuras, mas não devem ser misturados com a reprodução inicial da Equação (1).

---

## 11. Orientações para a implementação em C++

1. A implementação deve tratar $[p]$ e $[q]$ como matrizes materiais diagonais no escopo inicial.

2. O código deve permitir selecionar se a formulação será em $\mathbf{E}$ ou em $\mathbf{H}$ por meio de um parâmetro de configuração.

3. Para formulação em $\mathbf{E}$, os coeficientes materiais devem obedecer:

$$
[p]=[I],
\qquad
[q]=[\varepsilon_r].
$$

4. Para formulação em $\mathbf{H}$, os coeficientes materiais devem obedecer:

$$
[p]=[\varepsilon_r]^{-1},
\qquad
[q]=[I].
$$

5. O número de onda no espaço livre deve ser tratado como parâmetro físico derivado de $\omega$, $\mu_0$ e $\varepsilon_0$:

$$
k_0 = \omega\sqrt{\mu_0\varepsilon_0}.
$$

6. A dependência longitudinal deve ser representada pela constante de propagação $\beta$, lembrando que:

$$
\frac{\partial}{\partial z}\longrightarrow -j\beta.
$$

7. A estrutura de dados de materiais deve distinguir claramente:

- material isotrópico;
- material anisotrópico diagonal;
- extensões futuras para material não diagonal.

8. A implementação inicial não deve aceitar silenciosamente tensores não diagonais se as integrais e a formulação ainda não tiverem sido validadas para esse caso.

9. As variáveis internas devem evitar ambiguidade entre permissividade absoluta $\varepsilon$ e permissividade relativa $\varepsilon_r$.

10. A documentação do código deve manter rastreabilidade explícita entre:

- Equação (1);
- escolha $\phi=\mathbf{E}$ ou $\phi=\mathbf{H}$;
- definição de $[p]$;
- definição de $[q]$;
- montagem posterior das matrizes de elementos finitos.

---

## 12. Pendências científicas e de verificação

1. **Hipótese de meio não magnético:** Conferir no artigo original se a hipótese $\mu=\mu_0$ é apenas implícita ou se aparece em alguma passagem não destacada da tradução atual.

2. **Interpretação do operador $\nabla$:** Verificar se o operador $\nabla$ da Equação (1) deve ser interpretado inicialmente como operador tridimensional completo antes da substituição $\partial/\partial z\to -j\beta$.

3. **Meios com perdas:** Conferir se o artigo assume materiais sem perdas na derivação principal e apenas menciona perdas e meios ativos como extensão posterior na conclusão.

4. **Variação espacial de $[\varepsilon_r]$:** Verificar se $[\varepsilon_r]$ pode variar espacialmente por regiões materiais na implementação sem alterar a forma forte da Equação (1), desde que a forma fraca trate corretamente interfaces e continuidade tangencial.

5. **Compatibilidade de sinais:** Conferir a compatibilidade de sinais entre a convenção $\exp[j(\omega t-\beta z)]$ e todos os sinais da matriz $[B]$ (Eq. 28) apresentada posteriormente.

6. **Expansão do rotacional:** Desenvolver em arquivo separado a expansão componente a componente de $\nabla \times \left([p]\nabla \times \phi\right)$ até a forma discreta usada nas matrizes da formulação FEM.

7. **Escolha da formulação:** Conferir se a formulação em $\mathbf{E}$ e a formulação em $\mathbf{H}$ produzem exatamente a mesma estrutura algébrica para todos os exemplos numéricos ou se há escolhas específicas de campo nos casos apresentados.

8. **Condições de contorno:** Verificar como as condições de contorno de condutor elétrico perfeito (PEC) e condutor magnético perfeito (PMC) interagem com a escolha $\phi=\mathbf{E}$ ou $\phi=\mathbf{H}$.

9. **Anisotropia não diagonal:** Conferir se extensões para anisotropia não diagonal exigem apenas novas integrais cruzadas ou também mudanças na estrutura variacional e nos graus de liberdade.

10. **Tabela de convenções:** Registrar, antes da implementação, uma tabela definitiva com os nomes computacionais dos parâmetros físicos: $\omega$, $\beta$, $k_0$, $\varepsilon_r$, $n$, $[p]$ e $[q]$.
