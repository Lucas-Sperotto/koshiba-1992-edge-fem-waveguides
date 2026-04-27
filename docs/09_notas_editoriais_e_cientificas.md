# Notas editoriais e científicas - baseline documental da Fase 1

> **Navegação dos docs:** [Resumo](00_resumo.md) | [Introdução](01_introducao.md) | [Equações básicas](02_equacoes_basicas.md) | [Formulação FEM](03_formulacao_elementos_finitos.md) | [Exemplos numéricos](04_exemplos_numericos.md) | [Conclusão](05_conclusao.md) | [Apêndice](06_apendice.md) | [Referências](07_referencias.md) | [Notas editoriais](08_notas_editoriais_e_cientificas.md) | [Baseline Fase 1](09_notas_editoriais_e_cientificas.md)

Este arquivo registra notas adotadas para fechar a Fase 1 como baseline documental. As observações abaixo não alteram a formulação matemática do artigo; elas apenas explicitam convenções, ambiguidades e cuidados que devem orientar as próximas fases.

## $\phi$ como campo vetorial

O artigo usa $\phi$ para representar $\mathbf{E}$ ou $\mathbf{H}$. Embora o símbolo apareça sem negrito, ele deve ser interpretado como campo vetorial em expressões como $\nabla \times \phi$. A documentação preserva a notação do artigo e registra esta convenção para evitar uma troca tipográfica que pudesse parecer mudança de formulação.

## Reutilização de $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$

As submatrizes $[K_{tt}]$, $[K_{tz}]$ e $[K_{zz}]$ aparecem com significados algébricos diferentes nas Equações (29) e (32). Essa reutilização vem do artigo original: primeiro elas surgem da substituição direta no funcional e depois reaparecem no problema reorganizado para autovalor em $\beta^2$. Na implementação futura, os nomes internos devem diferenciar esses dois conjuntos.

## Uso ambíguo de $t$

Na seção de exemplos, $t$ representa a espessura da fita metálica na microstrip, com $t = 0$, e também o parâmetro de escala geométrica dos guias dielétricos, como em $W = 2t$. A ambiguidade vem do artigo. A documentação preserva a notação original, mas a implementação deve usar nomes explícitos para cada grandeza.

## $[N]$ usado antes de ser definido

A Equação (22) usa $[N]$ antes da definição formal da Equação (24). Esta ordem segue o artigo. Para leitura, a Equação (22) deve ser interpretada junto com as Equações (23) e (24), onde a matriz de interpolação é finalmente exibida.

## Hipótese de meio não magnético

A formulação emprega meios não magnéticos, com $\mu = \mu_0$, hipótese implícita na definição das matrizes $[p]$ e $[q]$. Essa hipótese deve ser tratada como parte do escopo científico da reprodução, especialmente antes de qualquer extensão para materiais magnéticos.

## Funcional variacional

O funcional da Equação (6) é a forma variacional associada à equação vetorial de onda. A documentação ainda não inclui uma derivação didática completa; portanto, a ligação entre $\delta F = 0$, integração por partes e a equação diferencial permanece como pendência científica futura.

## Fator $j$ na Equação (7)

O fator $j$ na aproximação de $\phi_z$ está relacionado à convenção de fase $\exp[j(\omega t - \beta z)]$ e à organização das componentes axial e transversais no operador rotacional. A nota registra a origem esperada do fator sem modificar a equação original.

## Eliminação de soluções espúrias por elementos de aresta

Os elementos de aresta impõem continuidade tangencial de forma natural e usam um espaço discreto mais compatível com a estrutura vetorial das equações de Maxwell. Esta é a razão física e numérica central para a redução de soluções espúrias no artigo, mas uma explicação didática mais completa permanece como pendência futura.

## Inversão de $[K_{zz}]$

A eliminação de $\{\phi_z\}$ entre as Equações (31b) e (34) pressupõe o tratamento de $[K_{zz}]$. A inversão pode ser custosa, pode reduzir a esparsidade e requer cuidado quando a matriz for singular ou mal condicionada. A estratégia computacional ainda deve ser definida antes da implementação.

## Imagens e direitos autorais

As imagens usadas na documentação devem ter origem e direitos verificados. Para um repositório público, a preferência é redesenhar geometrias, regenerar gráficos a partir de dados próprios e citar o artigo como fonte, evitando a redistribuição de material protegido sem permissão.
