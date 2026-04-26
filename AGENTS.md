# AGENTS.md

Este arquivo orienta agentes de IA, assistentes de código e fluxos automatizados neste repositório.

Ele complementa, mas não substitui, o `README.md`, o `TODO.md`, os arquivos em `docs/` e as instruções de contribuição. Para contribuidores humanos, o guia principal continua sendo o `README.md`; para agentes, este arquivo registra prioridades, restrições e critérios técnicos que não devem ser inferidos automaticamente.

## Objetivo do repositório

Reproduzir, documentar e implementar de forma didática o artigo:

> M. Koshiba and K. Inoue,  
> “Simple and Efficient Finite-Element Analysis of Microwave and Optical Waveguides,”  
> IEEE Transactions on Microwave Theory and Techniques, vol. 40, no. 2, pp. 371–377, Feb. 1992.  
> DOI: https://doi.org/10.1109/22.120111

O objetivo central é construir uma implementação em C++17 do método dos elementos finitos com elementos de aresta triangulares para análise de guias de onda de micro-ondas e ópticos, conectando a formulação física, a formulação variacional, a discretização por elementos finitos, o problema de autovalores e a validação numérica.

## Princípios gerais

1. Priorizar fidelidade ao artigo original.
2. Priorizar clareza didática antes de otimização prematura.
3. Não inventar fórmulas, hipóteses ou resultados que não estejam no artigo ou em referências explicitamente indicadas.
4. Quando houver dúvida, registrar a incerteza em `TODO.md` ou em comentário técnico, sem mascarar a lacuna.
5. Preservar a ligação entre equação, implementação e validação numérica.
6. Separar claramente física, matemática, malha, montagem FEM, álgebra linear, entrada/saída e visualização.
7. Toda saída numérica relevante deve ser exportável para CSV, JSON ou formato equivalente.
8. Todo gráfico deve ser gerado por scripts externos, preferencialmente em Python.
9. O código principal deve permanecer em C++17.
10. O repositório deve ser reprodutível por linha de comando.

## Escopo científico

O projeto deve reproduzir a formulação de Koshiba e Inoue baseada em:

- equação vetorial de onda;
- uso de três componentes do campo elétrico ou magnético;
- tensores diagonais de permissividade;
- formulação variacional;
- elementos de aresta triangulares;
- continuidade tangencial dos campos;
- eliminação de soluções espúrias;
- problema de autovalores para a constante de propagação;
- exemplos numéricos com linhas microstrip e guias dielétricos.

## Escopo computacional

A implementação deve ser organizada para permitir:

- definição de geometrias 2D;
- geração ou leitura de malhas triangulares;
- identificação de vértices, arestas e elementos;
- cálculo das funções de forma nodais e de aresta;
- montagem de matrizes locais;
- montagem de matrizes globais;
- aplicação de condições de contorno;
- solução do problema de autovalores;
- exportação de autovalores, constantes de propagação, campos e curvas de dispersão;
- geração de gráficos comparáveis às figuras do artigo.

## Estrutura esperada

A estrutura recomendada do repositório é:

```text
.
├── README.md
├── AGENTS.md
├── TODO.md
├── LICENSE
├── CITATION.cff
├── CMakeLists.txt
├── docs/
│   ├── 01_artigo_traduzido.md
│   ├── 02_introducao.md
│   ├── 03_equacoes_basicas.md
│   ├── 04_formulacao_elementos_finitos.md
│   ├── 05_exemplos_numericos.md
│   ├── 06_conclusao.md
│   ├── 07_apendice.md
│   ├── 08_referencias.md
│   └── img/
├── include/
│   └── koshiba/
├── src/
│   ├── main.cpp
│   ├── mesh/
│   ├── fem/
│   ├── physics/
│   ├── algebra/
│   └── io/
├── examples/
│   ├── microstrip/
│   ├── rectangular_dielectric_waveguide/
│   └── triangular_core_waveguide/
├── scripts/
│   ├── run/
│   └── plot/
├── data/
│   ├── input/
│   └── output/
├── tests/
└── out/
````

Essa estrutura pode evoluir, mas qualquer alteração deve manter a separação conceitual entre documentação, código-fonte, exemplos, scripts, dados de entrada, dados de saída e testes.

## Regras para documentação

Ao editar arquivos em `docs/`:

1. Preservar títulos, equações, numeração e notação já estabelecidos.
2. Usar Markdown compatível com GitHub.
3. Usar `$...$` para equações inline.
4. Usar `$$...$$` para equações em bloco.
5. Não usar `\tag{}` nas equações.
6. Registrar a numeração das equações em texto separado, por exemplo: `**Equação (34)**`.
7. Preservar os nomes originais de autores, periódicos e referências bibliográficas.
8. Manter imagens em `docs/img/`.
9. Referenciar imagens com caminho relativo, por exemplo: `![Descrição](img/fig1.png)`.
10. Quando acrescentar explicações próprias, deixar claro que são comentários didáticos, não texto original do artigo.

## Regras para tradução

A tradução do artigo deve:

* preservar o sentido técnico do texto original;
* evitar simplificações excessivas;
* manter termos técnicos importantes, como `edge elements`, `spurious solutions`, `propagation constant`, `finite-element method`, `waveguide`, `anisotropic substrate`;
* traduzir de forma consistente:

  * `edge elements` como `elementos de aresta`;
  * `spurious solutions` como `soluções espúrias`;
  * `propagation constant` como `constante de propagação`;
  * `waveguide` como `guia de onda`;
  * `finite-element method` como `método dos elementos finitos`;
  * `eigenvalue problem` como `problema de autovalores`.

Quando houver ambiguidade de OCR, registrar como:

```md
<!-- TODO: verificar trecho no artigo original devido a possível erro de OCR. -->
```

## Regras para implementação

Ao editar código C++:

1. Usar C++17.
2. Evitar dependências desnecessárias.
3. Separar cabeçalhos em `include/koshiba/`.
4. Separar implementações em `src/`.
5. Usar nomes de variáveis próximos à formulação matemática sempre que isso melhorar a rastreabilidade.
6. Comentar blocos de código relacionando-os às equações do artigo.
7. Evitar código monolítico.
8. Evitar valores fixos escondidos no código.
9. Preferir entrada por arquivos JSON, YAML ou texto estruturado.
10. Preferir saída em CSV ou JSON para validação.

Comentários técnicos devem indicar a origem matemática, por exemplo:

```cpp
// Monta a contribuição local associada à Equação (32a).
```

ou:

```cpp
// Calcula a matriz de massa transversal M_tt conforme a Equação (33).
```

## Regras para matrizes e autovalores

A montagem deve respeitar a formulação do artigo.

Atenção especial deve ser dada a:

* matrizes locais do elemento triangular;
* funções de forma nodais;
* funções de forma de aresta;
* orientação das arestas;
* continuidade tangencial;
* montagem global;
* aplicação de condições de contorno;
* distinção entre as matrizes das Equações (29)–(30) e das Equações (32)–(33);
* redução do problema para a forma final da Equação (34);
* cálculo direto da constante de propagação.

Não substituir a formulação de elementos de aresta por uma formulação nodal convencional sem registrar explicitamente que se trata de outro método.

Não introduzir método de penalidade como solução principal. O artigo usa elementos de aresta justamente para evitar soluções espúrias sem depender de coeficiente de penalidade arbitrário.

## Regras para validação

Todo resultado numérico relevante deve ter:

* arquivo de entrada;
* comando de execução;
* arquivo de saída;
* script de visualização, quando aplicável;
* comparação com figura, tabela ou referência do artigo;
* registro de erro, desvio ou limitação.

Casos de validação esperados:

1. linha microstrip blindada;
2. microstrip com substrato isotrópico;
3. microstrip com substrato anisotrópico;
4. guia retangular dielétrico;
5. guia com núcleo triangular equilátero.

As figuras de referência do artigo são:

* Figura 2: linha microstrip blindada;
* Figura 3: características de propagação da microstrip;
* Figura 4: guia retangular dielétrico;
* Figura 5: características de propagação do guia retangular dielétrico;
* Figura 6: guia com núcleo triangular equilátero;
* Figura 7: características de propagação do guia triangular.

## Regras para scripts

Scripts devem ficar em `scripts/`.

Scripts de execução devem ficar em:

```text
scripts/run/
```

Scripts de plotagem devem ficar em:

```text
scripts/plot/
```

Scripts devem:

* ser executáveis por terminal;
* ter mensagens claras;
* criar diretórios de saída quando necessário;
* não depender de caminhos absolutos da máquina do desenvolvedor;
* registrar arquivos gerados em `out/` ou `data/output/`.

## Regras para saída de dados

Arquivos gerados não devem ser misturados com código-fonte.

Usar preferencialmente:

```text
out/
data/output/
```

Exemplos de arquivos esperados:

```text
out/microstrip/isotropic_dispersion.csv
out/microstrip/anisotropic_dispersion.csv
out/rectangular_waveguide/fig5_data.csv
out/triangular_core/fig7_data.csv
out/validation/validation_summary.csv
```

## Regras para testes

Testes devem verificar, sempre que possível:

* montagem correta das matrizes locais;
* simetria das matrizes esperadas;
* orientação consistente das arestas;
* valores básicos de área e coordenadas baricêntricas;
* ausência de autovalores espúrios em casos simples;
* reprodução aproximada das curvas do artigo;
* estabilidade de resultados para refinamento de malha.

Não marcar uma etapa como concluída em `TODO.md` sem teste, script ou evidência correspondente.

## Regras para TODO.md

O `TODO.md` deve funcionar como mapa de pendências.

Ao atualizar o `TODO.md`:

* registrar o que foi feito;
* registrar o que falta;
* separar pendências científicas, técnicas e editoriais;
* não apagar pendências sem justificativa;
* mover itens concluídos para uma seção de concluídos, quando apropriado;
* apontar arquivos, scripts e testes relacionados.

Formato sugerido:

```md
## Pendência

Descrição da pendência.

**Tipo:** científica | técnica | documentação | validação  
**Origem:** artigo | implementação | teste | revisão  
**Arquivos relacionados:**  
- `docs/...`
- `src/...`

**Critério de conclusão:**
- [ ] evidência 1
- [ ] evidência 2
```

## Regras para commits

Mensagens de commit devem ser claras e, preferencialmente, seguir Conventional Commits.

Exemplos:

```text
docs: add translated introduction and basic equations
docs: add finite element formulation notes
feat: add triangular mesh data structures
feat: implement local edge element matrices
test: add checks for triangular element integrals
scripts: add microstrip reproduction runner
fix: correct edge orientation in global assembly
```

## O que agentes não devem fazer

Agentes não devem:

1. Inventar resultados numéricos.
2. Inventar fórmulas ausentes no artigo.
3. Apagar documentação existente sem motivo técnico explícito.
4. Reescrever arquivos inteiros quando uma alteração localizada resolver.
5. Misturar código de protótipo com código final sem marcação.
6. Introduzir dependências pesadas sem justificativa.
7. Alterar a estrutura do projeto sem atualizar o `README.md`.
8. Declarar validação concluída sem comparação numérica.
9. Usar imagens, tabelas ou dados do artigo sem registrar sua origem.
10. Trocar a formulação de elementos de aresta por elementos nodais convencionais sem criar uma seção separada de comparação.

## Critério geral de qualidade

Uma contribuição é considerada adequada quando:

* respeita o artigo original;
* melhora a rastreabilidade entre equação e código;
* mantém o projeto compilável;
* mantém os scripts executáveis;
* gera saídas reprodutíveis;
* documenta limitações;
* não esconde dúvidas científicas;
* facilita a validação futura.

## Prioridade de desenvolvimento

A ordem recomendada de desenvolvimento é:

1. consolidar a tradução e documentação do artigo;
2. documentar a formulação matemática com comentários didáticos;
3. implementar estruturas básicas de malha triangular;
4. implementar funções de forma nodais;
5. implementar funções de forma de aresta;
6. montar matrizes locais;
7. montar matrizes globais;
8. resolver o problema de autovalores;
9. reproduzir casos simples;
10. reproduzir as figuras do artigo;
11. automatizar validações;
12. melhorar documentação e scripts de reprodução.

## Observação final

Este repositório deve servir tanto como reprodução científica quanto como material didático. Portanto, toda implementação deve ser compreensível para estudantes e pesquisadores que desejem estudar elementos de aresta, métodos de elementos finitos vetoriais e análise de guias de onda eletromagnéticos.
