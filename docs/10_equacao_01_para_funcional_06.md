# 10. Da Equação (1) ao funcional da Equação (6)

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md)

Este documento complementa a tradução técnica do artigo de Koshiba e Inoue (1992). O objetivo é explicar, de forma didática e tecnicamente cuidadosa, como a equação vetorial de onda apresentada como Equação (1) conduz ao funcional variacional apresentado como Equação (6), preparando a base conceitual para a formulação por elementos finitos.

Não se trata de uma reescrita da tradução do artigo. Trata-se de uma ponte de estudo entre a forma diferencial forte, a forma variacional e a futura implementação numérica.

---

## 1. Ponto de partida: a Equação (1)

O artigo considera um guia de onda dielétrico com tensor de permissividade diagonal e dependência harmônica dos campos na forma:

$$
\exp\left[j(\omega t - \beta z)\right].
$$

A partir das equações de Maxwell, o artigo apresenta a seguinte equação vetorial de onda:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0.
$$

Essa é a Equação (1). Nela:

- $\phi$ representa genericamente o campo elétrico $\mathbf{E}$ ou o campo magnético $\mathbf{H}$;
- $[p]$ e $[q]$ são matrizes diagonais que mudam conforme a escolha de $\phi$;
- $k_0$ é o número de onda no espaço livre;
- $\nabla \times$ é o operador rotacional;
- o primeiro termo representa a parte diferencial do operador eletromagnético;
- o segundo termo representa a resposta constitutiva do meio ponderada por $k_0^2$.

A forma compacta da Equação (1) é conveniente porque permite tratar, em uma única notação, a formulação em campo elétrico e a formulação em campo magnético.

---

## 2. Forma forte e forma variacional

A Equação (1) é uma equação diferencial vetorial. Em linguagem de métodos numéricos, ela é a **forma forte** do problema, pois exige que a equação seja satisfeita ponto a ponto no domínio considerado.

De maneira abstrata, podemos escrever:

$$
\mathcal{L}(\phi) = 0,
$$

com:

$$
\mathcal{L}(\phi)
=
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi.
$$

Na formulação por elementos finitos, raramente se trabalha diretamente com a forma forte. Em vez disso, constrói-se uma **forma fraca** ou **variacional**, na qual a equação diferencial é testada contra funções auxiliares admissíveis.

Se $\boldsymbol{\psi}$ representa uma função teste vetorial, a ideia básica é impor:

$$
\iint_{\Omega}
\psi^* \cdot
\left[
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi
\right]
\,dx\,dy
= 0.
$$
Aqui, $\boldsymbol{\psi}^*$ representa o conjugado complexo da função teste. O conjugado aparece porque os campos, sob a convenção harmônica adotada, podem ser grandezas complexas. Assim, o produto interno adequado para espaços complexos deve considerar conjugação complexa. No método de Galerkin, as funções teste $\boldsymbol{\psi}$ são escolhidas do mesmo espaço de funções que a solução $\phi$.

> **Nota de verificação:** o artigo apresenta diretamente o funcional da Equação (6), sem detalhar a passagem intermediária pela forma fraca. A expressão acima explicita uma etapa padrão de formulações variacionais complexas, mas a convenção exata do produto interno deve ser conferida com a notação do artigo original e com as referências variacionais citadas pelos autores.

---

## 3. Por que aparece um funcional?

Um funcional é uma expressão que recebe uma função como argumento e retorna um número. No caso do artigo, o funcional associado à Equação (1) é:

$$
F =
\iint_{\Omega}
\left[
\left(\nabla \times \phi\right)^* \cdot
\left([p]\nabla \times \phi\right)
-
k_0^2 [q]\phi^* \cdot \phi
\right]
\,dx\,dy.
$$

Essa é a Equação (6) na forma preservada pela tradução.

Para fins de implementação e interpretação como produto interno matricial, é útil ler o segundo termo como:

$$
F =
\iint_{\Omega}
\left[
\left(\nabla \times \phi\right)^* \cdot
\left([p]\nabla \times \phi\right)
-
k_0^2
\phi^* \cdot \left([q]\phi\right)
\right]
\,dx\,dy.
$$

Quando $[q]$ é diagonal real, como na formulação principal do artigo para meios dielétricos sem perdas, as duas leituras são compatíveis como forma escalar de energia. Em uma extensão para meios com perdas, meios ativos ou tensores complexos, a ordem algébrica e o uso de transposta, conjugada ou adjunta devem ser tratados com mais cuidado.

> **Nota de verificação:** o artigo menciona, na conclusão, a possibilidade de aplicação a meios com perdas e/ou ativos, mas a Equação (6) é apresentada no corpo do texto para a formulação principal com tensor de permissividade diagonal. Antes de generalizar para materiais complexos, é necessário verificar a forma hermitiana correta do funcional.

A razão para introduzir $F$ é que a condição de estacionariedade:

$$
\delta F = 0
$$

produz a equação diferencial associada, juntamente com as condições de contorno naturais correspondentes, quando o termo de fronteira resultante da integração por partes se anula ou é tratado pelas condições admissíveis. Em outras palavras, a solução física é buscada como uma função que torna estacionária uma quantidade integral associada ao operador eletromagnético.

---

## 4. Da forma fraca ao funcional

Considere novamente o operador da Equação (1):

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0.
$$

Multiplicando por uma função teste vetorial conjugada $\boldsymbol{\psi}^*$ e integrando sobre a seção transversal $\Omega$, obtemos:

$$
\iint_{\Omega}
\boldsymbol{\psi}^* \cdot
\nabla \times \left([p]\nabla \times \phi\right)
\,dx\,dy
-
\iint_{\Omega}
\boldsymbol{\psi}^* \cdot
k_0^2 [q]\phi
\,dx\,dy
=0.
$$

O primeiro termo contém derivadas de ordem mais alta sobre $\phi$, pois há um rotacional aplicado a outro rotacional. Para chegar à forma fraca adequada ao método dos elementos finitos, usa-se uma identidade de integração por partes para o rotacional.

De modo formal, a identidade vetorial relevante é uma consequência do teorema do divergente para um produto vetorial:

$$
\iint_{\Omega} \nabla \cdot (\mathbf{A} \times \boldsymbol{\psi}^*) \,d\Omega = \oint_{\partial\Omega} (\mathbf{A} \times \boldsymbol{\psi}^*) \cdot \mathbf{n} \,d\Gamma.
$$

Expandindo o divergendo $\nabla \cdot (\mathbf{A} \times \boldsymbol{\psi}^*) = (\nabla \times \mathbf{A}) \cdot \boldsymbol{\psi}^* - \mathbf{A} \cdot (\nabla \times \boldsymbol{\psi}^*)$ e rearranjando, obtém-se a identidade de integração por partes:

$$
\iint_{\Omega} (\nabla \times \mathbf{A}) \cdot \boldsymbol{\psi}^* \,d\Omega = \iint_{\Omega} \mathbf{A} \cdot (\nabla \times \boldsymbol{\psi}^*) \,d\Omega + \oint_{\partial\Omega} (\mathbf{A} \times \boldsymbol{\psi}^*) \cdot \mathbf{n} \,d\Gamma.
$$

Aplicando essa identidade com:

$$
\mathbf{A} = [p]\nabla\times\phi.
$$

Assim, a forma fraca fica:

$$
\iint_{\Omega}
\left(\nabla\times\psi\right)^*\cdot
\left([p]\nabla\times\phi\right)
\,dx\,dy
-
\iint_{\Omega}
k_0^2
\psi^*\cdot\left([q]\phi\right)
\,dx\,dy,
$$

além de um termo de fronteira.

Pode-se verificar por cálculo direto de $\delta F$ que a primeira variação do funcional reproduz essa forma fraca após a integração por partes, desde que as variações admissíveis respeitem as condições de contorno essenciais:

$$
F(\phi)=
\iint_{\Omega}
\left[
\left(\nabla \times \phi\right)^* \cdot
\left([p]\nabla \times \phi\right)
-
k_0^2
\phi^*\cdot\left([q]\phi\right)
\right]
\,dx\,dy.
$$

Portanto, a Equação (6) pode ser entendida como a forma quadrática cuja estacionariedade gera a Equação (1), respeitadas as condições de contorno admissíveis e a anulação ou incorporação apropriada do termo de fronteira.

---

## 5. Papel de cada termo no funcional

### 5.1 Termo $\nabla \times \phi$

O termo:

$$
\nabla \times \phi
$$

representa o rotacional do campo escolhido. Em eletromagnetismo, o rotacional mede a circulação local do campo e aparece diretamente nas equações de Maxwell.

Quando $\phi = \mathbf{E}$, o rotacional de $\mathbf{E}$ está relacionado ao campo magnético pela lei de Faraday. Quando $\phi = \mathbf{H}$, o rotacional de $\mathbf{H}$ está relacionado ao campo elétrico pela lei de Ampère-Maxwell.

No funcional, o termo:

$$
\left(\nabla \times \phi\right)^* \cdot
\left([p]\nabla \times \phi\right)
$$

mede uma contribuição energética associada à variação espacial rotacional do campo.

---

### 5.2 Matriz $[p]$

A matriz $[p]$ pondera o termo rotacional. No artigo, ela é diagonal:

$$
[p] =
\begin{bmatrix}
 p_x & 0 & 0 \\
 0 & p_y & 0 \\
 0 & 0 & p_z
\end{bmatrix}.
$$

Quando $\phi = \mathbf{E}$, o artigo usa:

$$
p_x=p_y=p_z=1.
$$

Quando $\phi = \mathbf{H}$, o artigo usa:

$$
p_x = \frac{1}{\varepsilon_{rx}},
\qquad
p_y = \frac{1}{\varepsilon_{ry}},
\qquad
p_z = \frac{1}{\varepsilon_{rz}}.
$$

Assim, $[p]$ incorpora a resposta constitutiva necessária para que a mesma Equação (1) represente corretamente a formulação em $\mathbf{E}$ ou em $\mathbf{H}$.

---

### 5.3 Matriz $[q]$

A matriz $[q]$ pondera o termo de massa ou reação do campo:

$$
[q] =
\begin{bmatrix}
 q_x & 0 & 0 \\
 0 & q_y & 0 \\
 0 & 0 & q_z
\end{bmatrix}.
$$

Quando $\phi = \mathbf{E}$, tem-se:

$$
q_x = \varepsilon_{rx}=n_x^2,
\qquad
q_y = \varepsilon_{ry}=n_y^2,
\qquad
q_z = \varepsilon_{rz}=n_z^2.
$$

Quando $\phi = \mathbf{H}$, o artigo usa:

$$
q_x=q_y=q_z=1.
$$

O termo:

$$
\phi^*\cdot([q]\phi)
$$

é, portanto, uma forma quadrática que mede a intensidade do campo ponderada pelas propriedades materiais.

---

### 5.4 Termo $k_0^2$

O número de onda no espaço livre é usualmente definido por:

$$
k_0 = \omega \sqrt{\mu_0\varepsilon_0}.
$$

Logo:

$$
k_0^2 = \omega^2\mu_0\varepsilon_0.
$$

No funcional, $k_0^2$ escala o termo material associado a $[q]\phi$. Ele conecta a formulação eletromagnética ao regime de frequência de operação.

Na formulação posterior do artigo, ao substituir as aproximações por elementos finitos, aparece inicialmente um problema de autovalores envolvendo $k_0^2$:

$$
[K]\{\phi\} - k_0^2[M]\{\phi\}=\{0\}.
$$

Depois, por reorganização algébrica, o artigo chega ao problema final em $\beta^2$.

---

### 5.5 Produto interno vetorial

O ponto entre vetores indica produto interno. Para vetores complexos, a forma compatível com energia é baseada em conjugação complexa:

$$
\mathbf{a}^*\cdot\mathbf{b}
=
a_x^*b_x+a_y^*b_y+a_z^*b_z.
$$

Assim, termos como:

$$
\left(\nabla\times\phi\right)^*\cdot
\left([p]\nabla\times\phi\right)
$$

e:

$$
\phi^*\cdot([q]\phi)
$$

são escalares. Essa propriedade é essencial, pois o funcional $F$ deve retornar um número e não um vetor.

---

## 6. Integração sobre o domínio transversal $\Omega$

O artigo integra o funcional sobre $\Omega$, a seção transversal do guia de onda:

$$
F = \iint_{\Omega} (\cdots)\,dx\,dy.
$$

Isso ocorre porque a dependência longitudinal dos campos já foi separada na forma:

$$
\exp\left[j(\omega t-\beta z)\right].
$$

Desse modo, a variação em $z$ é incorporada por meio da constante de propagação $\beta$. Na prática, o problema tridimensional de propagação guiada é reduzido a um problema sobre a seção transversal $(x,y)$.

A integração em $\Omega$ soma a contribuição energética de todos os pontos da seção transversal. Quando o domínio for discretizado em elementos finitos, essa integral será decomposta como soma de integrais elementares:

$$
\iint_{\Omega}(\cdots)\,dx\,dy
=
\sum_e
\iint_{\Omega_e}(\cdots)\,dx\,dy.
$$

Essa é a ponte direta entre o funcional contínuo e a montagem das matrizes globais do método dos elementos finitos.

---

## 7. Condições de contorno naturais e essenciais

Na passagem da forma forte para a forma fraca, surge um termo de fronteira associado à integração por partes do rotacional:

$$
-
\int_{\partial\Omega}
\left(\mathbf{n}\times\psi^*\right)\cdot
\left([p]\nabla\times\phi\right)
\,d\Gamma.
$$

Esse termo indica onde as condições de contorno entram na formulação variacional.

De modo geral:

- **condições essenciais** são impostas diretamente no espaço de funções admissíveis, restringindo valores ou componentes do campo;
- **condições naturais** aparecem nos termos de fronteira da forma fraca e podem ser satisfeitas automaticamente quando o termo de fronteira é nulo ou especificado.

No contexto eletromagnético, condições envolvendo componentes tangenciais do campo são especialmente importantes. Isso se conecta à motivação do artigo para usar elementos de aresta, pois esses elementos impõem de maneira natural a continuidade tangencial entre elementos e através de interfaces de materiais.

> **Nota de verificação:** a seção de Equações Básicas do artigo apresenta a Equação (6), mas não detalha as condições de contorno usadas na derivação do funcional. Mais adiante, nos exemplos numéricos, o artigo menciona a imposição de condições de condutor elétrico perfeito ou condutor magnético perfeito em fronteiras artificiais. Ainda assim, é necessário conferir no artigo original e nas referências citadas como essas condições são incorporadas na forma variacional para cada caso de validação.

---

## 8. Relação com o problema de autovalores

O funcional da Equação (6) é a base para obter o problema matricial por elementos finitos.

A sequência conceitual é:

1. parte-se da Equação (1), escrita em forma diferencial forte;
2. constrói-se o funcional da Equação (6);
3. aproxima-se o campo $\phi$ por funções de forma em cada elemento;
4. impõe-se a estacionariedade do funcional;
5. obtém-se um sistema algébrico homogêneo;
6. esse sistema toma a forma de um problema de autovalores.

No artigo, após substituir a expansão por elementos finitos no funcional, obtém-se:

$$
[K]\{\phi\} - k_0^2[M]\{\phi\}=\{0\}.
$$

Esse problema ainda não é o resultado final do artigo. A formulação é reorganizada para separar as variáveis transversais e axiais e chegar a um problema final que fornece diretamente a constante de propagação $\beta$.

A ideia central é que a estacionariedade de $F$ transforma a busca por um campo contínuo satisfazendo a equação diferencial em uma busca por coeficientes discretos que satisfazem um sistema matricial.

---

## 9. Interpretação física

O funcional da Equação (6) tem estrutura de diferença entre duas contribuições:

$$
\text{termo rotacional}
-
\text{termo material}.
$$

O termo rotacional:

$$
\left(\nabla \times \phi\right)^*\cdot
\left([p]\nabla \times \phi\right)
$$

mede a energia associada à variação espacial do campo, ponderada pelas propriedades materiais adequadas à escolha de $\phi$.

O termo material:

$$
k_0^2\phi^*\cdot([q]\phi)
$$

mede a resposta do meio ao próprio campo, ponderada pelo número de onda no espaço livre.

A solução modal de um guia de onda corresponde a um equilíbrio entre essas duas contribuições. Esse equilíbrio não é, em geral, uma minimização simples como em problemas estáticos positivos definidos; trata-se de uma condição de estacionariedade associada a um problema de autovalores.

Fisicamente, isso significa que apenas certas distribuições de campo e certas constantes de propagação são compatíveis com a geometria, os materiais, a frequência e as condições de contorno do guia.

---

## 10. Interpretação numérica

Do ponto de vista numérico, o funcional fornece uma receita para construir matrizes.

O termo:

$$
\left(\nabla \times \phi\right)^*\cdot
\left([p]\nabla \times \phi\right)
$$

origina matrizes associadas ao operador rotacional, usualmente interpretadas como matrizes de rigidez eletromagnética.

O termo:

$$
\phi^*\cdot([q]\phi)
$$

origina matrizes associadas ao campo em si, usualmente interpretadas como matrizes de massa ou de reação material.

Quando o campo é aproximado por funções de forma, por exemplo:

$$
\phi \approx [N]^T\{\phi\}_e,
$$

as integrais do funcional passam a envolver produtos entre funções de forma, derivadas dessas funções e coeficientes materiais. É exatamente isso que aparece posteriormente nas matrizes elementares e nas integrais do Apêndice.

A formulação por elementos de aresta é numericamente importante porque o espaço discreto deve representar corretamente a continuidade tangencial dos campos. Caso o espaço discreto não seja compatível com a estrutura das equações de Maxwell, podem aparecer soluções espúrias.

---

## 11. Cuidados para a implementação C++

A futura implementação deve preservar explicitamente a correspondência entre equação, funcional e matriz. Alguns cuidados são fundamentais:

1. **Separar a formulação em $\mathbf{E}$ e em $\mathbf{H}$**
   A variável genérica $\phi$ é elegante no artigo, mas no código convém representar claramente se o caso usa campo elétrico ou campo magnético, pois isso altera $[p]$ e $[q]$.

2. **Representar $[p]$ e $[q]$ como propriedades materiais**
   Mesmo sendo diagonais no artigo, essas matrizes devem ser associadas a cada região material do domínio. Em materiais anisotrópicos diagonais, os valores em $x$, $y$ e $z$ podem ser diferentes.

3. **Evitar ambiguidade no termo com $[q]$**
   Para meios reais diagonais, a implementação pode montar o termo escalar componente a componente. Para extensões com meios complexos, será necessário revisar se a forma correta usa transposta, conjugada ou adjunta hermitiana.

4. **Manter o domínio como seção transversal**
   O problema deve ser montado em $\Omega$, isto é, no plano $(x,y)$. A dependência em $z$ entra pela constante de propagação $\beta$ e pela convenção harmônica.

5. **Tratar condições de contorno como parte da formulação, não como detalhe posterior**
   PEC, PMC, simetria e fronteiras artificiais afetam os graus de liberdade admissíveis. Elas devem ser planejadas junto com a malha e com a montagem global.

6. **Preservar rastreabilidade entre termos do funcional e blocos matriciais**
   Cada termo integral deve ter correspondência clara com a matriz que ele gera. Isso será essencial para validar $[K]$, $[M]$, $[K_{tt}]$, $[K_{tz}]$, $[K_{zz}]$ e $[M_{tt}]$.

7. **Usar aritmética complexa quando necessário**
   A presença de $j$, de conjugado complexo e de possíveis extensões para meios com perdas sugere que o projeto deve ser arquitetado sem impedir o uso posterior de números complexos.

8. **Não formar inversas explicitamente sem necessidade**
   Embora essa preocupação apareça mais diretamente na passagem para a Equação (35), ela já deve orientar a arquitetura numérica desde a montagem variacional.

---

## 12. Pendências científicas

Antes de considerar esta passagem completamente fechada, as seguintes pendências devem ser investigadas:

1. **Forma do produto interno:** Qual a forma exata do produto interno no funcional da Equação (6) no artigo original, especialmente a posição de $[q]$ em relação a $\phi^*$ e $\phi$?

2. **Propriedades das matrizes:** A formulação assume explicitamente que $[p]$ e $[q]$ são reais, diagonais e simétricas na dedução do funcional?

3. **Tratamento das condições de contorno:** Como as condições de contorno naturais e essenciais são tratadas no artigo original ou nas referências citadas?

4. **Natureza do funcional:** A Equação (6) deve ser interpretada como um funcional hermitiano, um funcional complexo estacionário ou uma forma bilinear/sesquilinear associada ao operador?

5. **Expansão do rotacional:** Como a dependência longitudinal $\exp[j(\omega t-\beta z)]$ modifica explicitamente o operador rotacional antes da montagem da matriz $[B]$?

6. **Normalização dos modos:** A passagem da Equação (6) para a Equação (25) pressupõe alguma normalização específica dos modos ou apenas a estacionariedade do funcional?

7. **Extensão para meios com perdas:** Para meios com perdas ou ativos (mencionados na conclusão), a mesma estrutura variacional permanece válida ou requer uma formulação adjunta/generalizada?

8. **Graus de liberdade na fronteira:** Nos exemplos numéricos, quais graus de liberdade são eliminados ou preservados em fronteiras PEC, PMC e planos de simetria?

---

## 13. Síntese

A Equação (1) descreve o equilíbrio diferencial vetorial dos campos no guia. A Equação (6) reescreve esse equilíbrio como uma condição de estacionariedade de uma quantidade integral sobre a seção transversal.

Essa passagem é o núcleo conceitual do método dos elementos finitos: em vez de exigir que a equação seja satisfeita ponto a ponto, busca-se uma solução aproximada em um espaço finito de funções, impondo que o funcional seja estacionário em relação aos graus de liberdade escolhidos.

No artigo de Koshiba e Inoue, essa estratégia permite construir uma formulação vetorial com elementos de aresta, reduzir o aparecimento de soluções espúrias e reorganizar o problema até obter uma equação de autovalores diretamente relacionada à constante de propagação.
