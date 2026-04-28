# koshiba-1992-edge-fem-waveguides

Repositório em **C++17** para reproduzir, de forma didática e reprodutível, o artigo:

> M. Koshiba and K. Inoue,  
> “Simple and Efficient Finite-Element Analysis of Microwave and Optical Waveguides,”  
> *IEEE Transactions on Microwave Theory and Techniques*, vol. 40, no. 2, pp. 371–377, Feb. 1992.  
> DOI: https://doi.org/10.1109/22.120111

## Objetivo do projeto

Este projeto tem como objetivo estudar, documentar e reproduzir numericamente a formulação proposta por **Koshiba e Inoue (1992)** para análise de guias de onda de micro-ondas e guias ópticos por meio do **método dos elementos finitos com elementos de aresta triangulares**.

O foco principal é conectar:

- a formulação eletromagnética original;
- a discretização por elementos finitos;
- o problema de autovalores;
- a eliminação de soluções espúrias;
- a implementação computacional em C++;
- a validação por meio dos exemplos numéricos do artigo.

## Contexto científico

A análise rigorosa de guias de onda com seções transversais arbitrárias exige uma formulação vetorial das equações de Maxwell. Métodos baseados em elementos finitos nodais tradicionais podem produzir **soluções espúrias**, isto é, modos numéricos que não correspondem a campos eletromagnéticos físicos.

O artigo de Koshiba e Inoue propõe uma formulação eficiente usando três componentes do campo elétrico ou magnético e introduz **elementos de aresta triangulares** para impor corretamente a continuidade tangencial dos campos nas interfaces entre materiais.

A formulação final leva a um problema de autovalores que envolve apenas as variáveis de aresta no plano transversal e permite obter diretamente a **constante de propagação** do modo guiado.

## Problemas físicos abordados

O artigo apresenta aplicações para:

1. linhas de transmissão microstrip;
2. guias retangulares dielétricos;
3. guias com núcleo triangular equilátero;
4. substratos isotrópicos e anisotrópicos;
5. comparação com métodos clássicos da literatura.

## Estrutura atual do repositório

```text
koshiba-1992-edge-fem-waveguides/
├── README.md
├── AGENTS.md
├── TODO.md
├── LICENSE
├── CITATION.cff
├── CMakeLists.txt
├── docs/
│   ├── README.md
│   ├── 00_resumo.md
│   ├── ... (documentos de 01 a 17)
│   └── img/
├── include/
│   └── koshiba/
│       ├── algebra/
│       ├── fem/
│       ├── mesh/
│       ├── io/
│       └── physics/
├── src/
│   ├── algebra/
│   ├── fem/
│   ├── mesh/
│   ├── io/
│   └── physics/
├── app/
│   └── mini_case.cpp
├── scripts/
│   ├── run/
│   └── plot/
├── data/
│   ├── input/
│   └── output/
├── tests/
│   ├── fixtures/
│   └── ... (arquivos de teste)
└── out/
```

## Compilação, testes e mini caso

O núcleo atual usa C++17, CMake e Eigen. A suíte de testes cobre geometria triangular, conectividade de malha, leitura Gmsh MSH 4.1 ASCII, funções de forma de aresta, integrais locais A1-A8, montagem global geométrica parcial, redução densa sem inversão explícita de `K_zz` e o solver beta mínimo para material diagonal.

```bash
cmake -S . -B build
cmake --build build
/usr/bin/ctest --test-dir build --output-on-failure
```

Se o `ctest` do `PATH` apontar para um wrapper Python local sem o módulo `cmake`, use o binário do sistema diretamente.

O mini caso sintético da Fase 7B não reproduz ainda uma figura do artigo; ele serve para manter a cadeia entrada → solver → CSV → gráfico executável por linha de comando.

```bash
scripts/run/run_mini_case.sh
scripts/plot/plot_mini_case.py
```

Entradas versionadas ficam em `data/input/`. Saídas geradas ficam em `data/output/` e `out/`.

## Etapas previstas

### 1. Tradução e documentação do artigo

A primeira etapa consiste em traduzir o artigo para arquivos Markdown, preservando a estrutura original e acrescentando notas explicativas quando necessário.

Arquivos sugeridos:

```text
docs/01_artigo_traduzido.md
docs/02_contexto_fisico.md
docs/03_equacoes_basicas.md
docs/04_elemento_de_aresta_triangular.md
docs/05_formulacao_fem.md
docs/06_problema_de_autovalores.md
docs/07_exemplos_numericos.md
```

### 2. Formulação matemática

Nesta etapa serão detalhados:

* equações de Maxwell;
* equação vetorial de onda;
* tensores de permissividade;
* campos elétrico e magnético;
* dependência longitudinal `exp[j(ωt - βz)]`;
* formulação variacional;
* aproximação por elementos finitos;
* montagem das matrizes;
* problema generalizado de autovalores.

### 3. Implementação computacional

A implementação será feita em C++17, com separação entre:

* leitura de parâmetros;
* geração ou importação de malhas;
* montagem local dos elementos;
* montagem global das matrizes;
* aplicação de condições de contorno;
* solução do problema de autovalores;
* exportação dos resultados.

Dependências inicialmente previstas:

* C++17;
* CMake;
* Eigen;
* Python para visualização simples.

Dependências futuras possíveis:

* Matplotlib;
* Pandas;
* LAPACK;
* ARPACK-NG;
* VTK ou ParaView para visualização dos campos.

O leitor atual aceita arquivos Gmsh MSH 4.1 ASCII sem depender da biblioteca do Gmsh em tempo de compilação.

### 4. Casos de validação

Os exemplos numéricos do artigo serão usados como referência:

* linha microstrip blindada;
* guia retangular dielétrico;
* guia com núcleo triangular equilátero.

A validação deverá comparar:

* constante de propagação;
* frequência normalizada;
* curvas de dispersão;
* modos guiados;
* ausência de soluções espúrias.

## Referência principal

```bibtex
@article{Koshiba1992EdgeFEMWaveguides,
  author  = {Koshiba, Masanori and Inoue, Kazuhiro},
  title   = {Simple and Efficient Finite-Element Analysis of Microwave and Optical Waveguides},
  journal = {IEEE Transactions on Microwave Theory and Techniques},
  volume  = {40},
  number  = {2},
  pages   = {371--377},
  year    = {1992},
  month   = {February},
  doi     = {10.1109/22.120111}
}
```

## Status do projeto

Fase documental fechada e núcleo C++17 em desenvolvimento.

* [x] Criar estrutura documental do repositório
* [x] Traduzir e organizar o artigo em Markdown
* [x] Documentar a formulação física e matemática
* [x] Documentar o elemento de aresta triangular
* [x] Implementar `Node`, `Triangle`, `Edge`, `Mesh` e leitura Gmsh MSH 4.1 ASCII inicial
* [x] Implementar funções de forma nodais e de aresta
* [x] Implementar integrais locais A1-A8
* [x] Implementar montagem global geométrica parcial
* [x] Implementar redução densa mínima sem inversão explícita de `K_zz`
* [x] Implementar material diagonal genérico para `phi=E` e `phi=H`
* [x] Implementar combinação beta mínima dos blocos das Equações (32)–(35)
* [x] Criar mini caso reprodutível com entrada, CSV, runner e plot
* [x] Definir política essencial inicial para PEC/PMC por `phi=E/H`
* [x] Auditar algebraicamente a redução e a recuperação de `phi_z`
* [ ] Validar sinais e acoplamentos contra caso físico de referência
* [ ] Validar e aplicar PEC/PMC por caso de validação do artigo
* [ ] Reproduzir os exemplos numéricos
* [ ] Comparar resultados com as figuras do artigo

## Licença

Este repositório contém uma implementação didática e independente baseada no estudo do artigo original.

O código-fonte será disponibilizado sob licença MIT, salvo indicação em contrário.

O artigo original pertence aos seus respectivos autores e editor.
