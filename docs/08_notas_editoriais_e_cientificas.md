# Notas editoriais e científicas — baseline documental da Fase 1

> **Navegação:** [Índice](../README.md) | [Fase 1](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md)

Este arquivo registra as decisões editoriais e observações científicas adotadas para fechar a **Fase 1** como baseline documental do projeto de reprodução do artigo de Koshiba e Inoue (1992).

A Fase 1 teve como objetivo organizar a tradução técnica inicial, preservar a numeração das equações, estruturar as figuras, registrar pendências científicas e preparar o repositório para a implementação computacional.

As observações abaixo não alteram a formulação matemática do artigo original. Elas apenas explicitam convenções, ambiguidades, hipóteses e cuidados que devem orientar as próximas fases do projeto.

---

## Convenção para o campo $\phi$

No artigo original, o símbolo $\phi$ é usado para representar tanto o campo elétrico $\mathbf{E}$ quanto o campo magnético $\mathbf{H}$.

Embora o símbolo apareça sem negrito no texto original, ele deve ser interpretado como um campo vetorial sempre que aparecer em expressões como:

$$
\nabla \times \phi
$$

A documentação preserva a notação do artigo para evitar que uma troca tipográfica pareça mudança de formulação. No entanto, quando necessário para clareza didática, poderá ser usada a notação $\boldsymbol{\phi}$, desde que fique explícito que não houve alteração matemática.

---

## Reutilização de $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$

As submatrizes $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$ aparecem inicialmente nas Equações (29a)–(29c), derivadas da substituição direta da aproximação por elementos finitos no funcional variacional.

Posteriormente, nas Equações (32a)–(32c), os mesmos símbolos são reutilizados após uma reorganização algébrica do problema, agora voltada à formulação final do problema de autovalores em $\beta^2$.

Essa reutilização de símbolos vem do artigo original. Entretanto, na implementação futura, recomenda-se evitar ambiguidade usando nomes internos distintos, por exemplo:

- `Ktt_direct` para a forma associada à Equação (29a);
- `Ktz_direct` para a forma associada à Equação (29b);
- `Kzz_direct` para a forma associada à Equação (29c);
- `Ktt_beta` para a forma associada à Equação (32a);
- `Ktz_beta` para a forma associada à Equação (32b);
- `Kzz_beta` para a forma associada à Equação (32c).

Essa distinção ajudará a manter a rastreabilidade entre equação, código e validação numérica.

---

## Uso ambíguo do símbolo $t$

Na seção de exemplos numéricos, o símbolo $t$ aparece com significados diferentes.

No exemplo da linha microstrip, $t$ representa a espessura da fita metálica, sendo usado como:

$$
t = 0
$$

Já no exemplo do guia retangular dielétrico, $t$ é usado como parâmetro geométrico de escala, por exemplo:

$$
W = 2t
$$

$$
X = 10t
$$

$$
Y = 5t
$$

Essa ambiguidade vem do artigo original. A documentação preserva a notação original, mas a implementação deve usar nomes explícitos para cada grandeza, como:

- `strip_thickness` para a espessura metálica da microstrip;
- `scale_t` para o parâmetro geométrico usado nos guias dielétricos;
- `core_height` ou `core_half_height`, caso a formulação geométrica adotada justifique essa escolha.

---

## Definição da matriz $[N]$

A Equação (22) usa a matriz $[N]$ antes de sua definição formal na Equação (24). Essa ordem segue o artigo original.

Para leitura didática, a Equação (22) deve ser interpretada em conjunto com as Equações (23) e (24), nas quais são explicitados o vetor de graus de liberdade elementares e a matriz de interpolação.

Essa observação é importante porque $[N]$ não representa apenas as funções de forma nodais usuais. Na formulação vetorial do artigo, $[N]$ reúne a aproximação das componentes transversais por funções associadas às arestas e da componente axial por funções nodais.

---

## Hipótese de meio não magnético

A formulação emprega meios não magnéticos, com:

$$
\mu = \mu_0
$$

Essa hipótese está implícita na definição das matrizes $[p]$ e $[q]$.

Ela deve ser tratada como parte do escopo científico da reprodução, especialmente antes de qualquer extensão futura para materiais magnéticos, metamateriais ou meios com tensores de permeabilidade diferentes de $\mu_0$.

---

## Origem da Equação (1)

A Equação (1) é derivada das equações de Maxwell considerando campos harmônicos com dependência longitudinal:

$$
\exp[j(\omega t - \beta z)]
$$

Sob a hipótese de meio não magnético, a combinação das equações de Maxwell leva a uma equação vetorial de onda escrita de maneira compacta como:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2[q]\phi = 0
$$

Nesta forma, $\phi$ pode representar $\mathbf{E}$ ou $\mathbf{H}$, desde que as matrizes $[p]$ e $[q]$ sejam escolhidas conforme as Equações (4) e (5).

A documentação da Fase 1 preserva a apresentação direta do artigo. Uma derivação didática mais completa fica registrada como pendência científica para as próximas fases.

---

## Funcional variacional

O funcional da Equação (6) é a forma variacional associada à equação vetorial de onda.

Sua estacionaridade,

$$
\delta F = 0
$$

leva à equação diferencial correspondente após integração por partes e aplicação das condições de contorno apropriadas.

A documentação atual ainda não inclui uma derivação didática completa entre a Equação (1) e o funcional da Equação (6). Essa ligação deverá ser detalhada posteriormente, principalmente para esclarecer:

- o papel da forma fraca;
- a interpretação energética do funcional;
- a relação com o método de Rayleigh-Ritz;
- as hipóteses necessárias para a simetria ou hermiticidade do operador;
- as condições de contorno naturais e essenciais.

---

## Fator $j$ na Equação (7)

Na Equação (7), a componente axial é aproximada com um fator $j$:

$$
\phi_z = j\{N\}^T\{\phi_z\}_e
$$

Esse fator está relacionado à convenção temporal e longitudinal adotada:

$$
\exp[j(\omega t - \beta z)]
$$

Ele também organiza a fase relativa entre as componentes axiais e transversais quando o operador rotacional é expandido na formulação por elementos finitos.

A nota registra a origem esperada do fator sem modificar a equação original. Uma explicação mais detalhada deverá ser incluída futuramente na transição entre o operador rotacional contínuo e sua forma discreta.

---

## Coeficientes das funções de forma de aresta

Os coeficientes $\bar{a}_k$, $\bar{b}_k$ e $\bar{c}_k$, apresentados nas Equações (17)–(19), definem as funções associadas às arestas do elemento triangular.

Esses coeficientes surgem da imposição de condições sobre a componente tangencial do campo em cada aresta do triângulo. A formulação garante que a componente tangencial seja contínua ao longo das fronteiras entre elementos.

O artigo apresenta diretamente as expressões finais. Na documentação futura, recomenda-se acrescentar uma derivação didática mostrando o sistema linear que determina esses coeficientes e explicando geometricamente os ângulos $\theta_{k+3}$.

---

## Eliminação de soluções espúrias por elementos de aresta

Os elementos de aresta são usados porque impõem naturalmente a continuidade tangencial dos campos nas interfaces entre elementos e materiais.

Essa característica torna o espaço discreto mais compatível com a estrutura vetorial das equações de Maxwell, reduzindo ou eliminando modos numéricos não físicos, conhecidos como soluções espúrias.

Do ponto de vista computacional, essa é uma das contribuições centrais do artigo: evitar a dependência de métodos de penalidade, que introduzem coeficientes arbitrários e podem afetar a precisão das soluções.

Uma explicação didática mais completa sobre a relação entre elementos de aresta, continuidade tangencial, espaços vetoriais apropriados e eliminação de modos espúrios permanece como pendência para as próximas fases.

---

## Inversão de $[K_{zz}]$

A passagem da Equação (31b) para a Equação (34) exige a eliminação de $\{\phi_z\}$, o que envolve o tratamento de $[K_{zz}]$.

A operação leva à matriz:

$$
[\hat{M}_{tt}]
=
[M_{tt}]
+
[K_{tz}][K_{zz}]^{-1}[K_{zt}]
$$

Essa redução simplifica o problema final, deixando-o apenas em termos das variáveis de aresta transversais $\{\phi_t\}$.

Entretanto, há um custo computacional importante:

- a inversão pode destruir a esparsidade das matrizes;
- $[K_{zz}]$ pode ser singular ou mal condicionada em situações especiais;
- a implementação deve evitar formar inversas explícitas sempre que possível;
- a estratégia de solução deve considerar fatorações lineares ou resolução de sistemas auxiliares.

Antes da implementação, será necessário definir como tratar numericamente essa etapa.

---

## Integrais do apêndice e implementação futura

O Apêndice apresenta as integrais necessárias para construir as matrizes elementares. Essas expressões serão fundamentais para a implementação das matrizes locais.

Algumas identidades, como a Equação (A3), merecem explicação adicional, pois conectam diretamente as derivadas das funções de forma de aresta com termos que aparecem nas matrizes da formulação FEM.

Além disso, algumas integrais cruzadas necessárias para a montagem de $[K_{tz}]$ devem ser verificadas cuidadosamente antes da implementação. Caso o artigo não apresente todas explicitamente, elas deverão ser derivadas e registradas como complemento didático, sempre separando o que vem do artigo original do que foi acrescentado pelo projeto.

---

## Frequência normalizada $v$ e constante normalizada $b$

Na seção de exemplos numéricos, a frequência normalizada $v$ e a constante de propagação normalizada $b$ são definidas pelas Equações (36) e (37).

Essas normalizações permitem comparar modos guiados em diferentes geometrias e contrastes de índice de refração.

Para modos guiados típicos em guias dielétricos, a constante normalizada $b$ costuma ser interpretada no intervalo físico:

$$
0 \leq b \leq 1
$$

De forma qualitativa:

- $b \to 0$ indica operação próxima ao corte;
- $b \to 1$ indica maior confinamento no núcleo.

A documentação futura deverá detalhar essa interpretação e sua relação com as curvas das Figuras 5 e 7.

---

## Conclusão e referências [14] e [15]

A conclusão do artigo menciona que a abordagem pode ser aplicada a guias com perdas, meios ativos e guias anisotrópicos com simetria de reflexão, citando as referências [14] e [15].

Essas referências não são discutidas em detalhe no corpo do texto, mas indicam possíveis extensões da formulação para problemas mais gerais.

Na documentação futura, recomenda-se acrescentar uma nota contextual explicando brevemente:

- o papel da análise variacional em guias anisotrópicos;
- a relação com formulações sem soluções espúrias;
- os limites da reprodução atual, que inicialmente deve permanecer fiel ao caso tratado por Koshiba e Inoue.

---

## Imagens e direitos autorais

As imagens usadas na documentação devem ter origem e direitos verificados.

Para um repositório público, recomenda-se evitar o uso direto de figuras, fotos ou páginas extraídas do PDF original, salvo quando houver autorização explícita.

A alternativa recomendada é:

- redesenhar esquemas geométricos;
- gerar novamente gráficos a partir dos dados reproduzidos;
- citar o artigo original como fonte;
- não incluir o PDF original no repositório público;
- evitar redistribuição de fotos dos autores sem confirmação de licença.

No caso específico das figuras geométricas, recomenda-se futuramente criar versões próprias em SVG, Python/Matplotlib ou TikZ. Para os gráficos de validação, a melhor prática será gerar novas figuras a partir dos dados calculados pelo código do repositório.

---

## Status da Fase 1

A Fase 1 é considerada fechada como **baseline documental**, com as seguintes entregas:

- tradução técnica inicial organizada em arquivos Markdown;
- numeração das equações preservada;
- figuras referenciadas nos arquivos de documentação;
- referências bibliográficas estruturadas;
- pendências científicas e editoriais registradas;
- arquivo `TODO.md` atualizado com pontos para as próximas fases;
- arquivo `AGENTS.md` criado para orientar fluxos assistidos por IA;
- notas editoriais e científicas consolidadas neste arquivo.

O fechamento da Fase 1 não significa que todas as lacunas matemáticas foram resolvidas. Significa que o projeto possui agora uma base documental suficiente para iniciar a próxima etapa.

---

## Próxima fase recomendada

A próxima fase deve tratar da preparação científica e computacional para a implementação.

Sugestão de escopo da **Fase 2**:

1. revisar a derivação da Equação (1) a partir das equações de Maxwell;
2. detalhar a passagem da Equação (1) para o funcional da Equação (6);
3. explicar a origem do fator $j$ na Equação (7);
4. derivar ou complementar as funções de forma de aresta;
5. revisar as integrais do Apêndice;
6. derivar integrais cruzadas ausentes, se necessário;
7. definir a arquitetura C++ inicial;
8. criar testes matemáticos para área, coeficientes do triângulo, funções nodais e funções de aresta;
9. preparar a estrutura mínima de compilação com CMake;
10. iniciar a implementação sem ainda reproduzir os exemplos completos.
