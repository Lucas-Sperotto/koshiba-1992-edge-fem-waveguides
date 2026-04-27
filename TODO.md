# TODO — Roteiro de desenvolvimento pós-baseline documental

Este arquivo é o roteiro mestre do projeto após o fechamento documental das Fases 1 e 2. Ele deve funcionar como mapa de pendências científicas, técnicas e de validação, sem apagar dúvidas que ainda precisam ser auditadas contra o artigo original.

---

## Status atual

- [x] **Fase 1 — Tradução e documentação científica básica:** baseline documental fechada em `docs/00` a `docs/08`.
- [x] **Fase 2 — Derivação matemática e contrato numérico:** complementos matemáticos fechados em `docs/09` a `docs/16`.
- [x] **Fase 3A — Estrutura C++17 mínima com CMake:** biblioteca inicial, `Node`, `Triangle` e teste geométrico criados.
- [x] **Fase 3B — Geometria, malha e Gmsh:** núcleo implementado e testado.
- [x] **Fase 4 — Funções de forma nodais e de aresta:** núcleo implementado e testado.
- [x] **Fase 5 — Matrizes locais do Apêndice:** A1-A8 implementadas e testadas contra valores documentados.
- [x] **Fase 6 — Montagem global parcial:** blocos geométricos esparsos implementados e testados.
- [ ] **Fase 7 — Autovalores e validação contra exemplos do artigo:** redução densa mínima implementada; validação científica das figuras segue pendente.

---

## Histórico documental

### Fase 1 — concluída como baseline documental

**Entregas concluídas**

- Tradução técnica organizada em `docs/00_resumo.md` a `docs/07_referencias.md`.
- Notas editoriais e científicas consolidadas em `docs/08_notas_editoriais_e_cientificas.md`.
- Navegação entre arquivos da documentação criada em `docs/README.md` e nos capítulos complementares.
- Problemas de renderização das equações e legendas revisados durante o fechamento documental.

**Observação**

O fechamento da Fase 1 não significa ausência de dúvidas científicas. Ele define apenas o baseline documental mínimo para iniciar a ponte matemática e a implementação.

### Fase 2 — concluída como contrato matemático

**Entregas concluídas**

- Maxwell para a Equação (1): `docs/09_maxwell_para_equacao_01.md`.
- Equação (1) para o funcional variacional: `docs/10_equacao_01_para_funcional_06.md`.
- Origem do fator `j` na Equação (7): `docs/11_origem_do_fator_j_equacao_07.md`.
- Funções nodais e de aresta: `docs/12_funcoes_de_forma_nodais_e_de_aresta.md`.
- Revisão das integrais do Apêndice: `docs/13_revisao_das_integrais_do_apendice.md`.
- Integrais cruzadas, termos ausentes e escopo de extensões: `docs/14_integrais_cruzadas_e_termos_ausentes.md`.
- Testes matemáticos mínimos: `docs/15_testes_matematicos_minimos.md`.
- Contrato para implementação C++17: `docs/16_contrato_para_implementacao_cpp.md`.

**Justificativa de limpeza**

O bloco antigo `TODO — Fase 2: Derivação matemática e contrato numérico` foi removido deste arquivo porque apontava para nomes de arquivos obsoletos e suas entregas foram absorvidas pelos documentos atuais `docs/09` a `docs/16`. As pendências científicas relevantes permanecem listadas abaixo.

---

## Fase 3A — Estrutura C++17 mínima com CMake

**Status:** concluída.

**Entregas**

- [x] `CMakeLists.txt` com biblioteca C++17.
- [x] `include/koshiba/mesh/node.hpp`.
- [x] `include/koshiba/mesh/triangle.hpp`.
- [x] `src/mesh/triangle.cpp`.
- [x] `tests/triangle_geometry_tests.cpp`.

**Evidência**

- Teste `triangle_geometry` executável por `/usr/bin/ctest --test-dir build --output-on-failure`.

---

## Fase 3B — Geometria, malha e Gmsh

**Status:** núcleo concluído.

**Decisões**

- Eigen deve ser introduzido já nesta fase.
- A orientação global de aresta deve usar o par canônico `(min(node_id), max(node_id))`.
- O sinal local-global deve ser armazenado separadamente para cada elemento.
- Arestas locais seguem a convenção do artigo: ponto 4 = aresta `(1,2)`, ponto 5 = `(2,3)`, ponto 6 = `(3,1)`.
- `Mesh` deve rejeitar triângulos degenerados e triângulos horários na primeira versão.
- Parser Gmsh inicial deve aceitar apenas MSH 4.1 ASCII.

**Tarefas**

- [x] Adicionar `find_package(Eigen3 REQUIRED)` ao CMake.
- [x] Criar `mesh::Edge`.
- [x] Criar `mesh::Mesh` com nós, triângulos, arestas globais, conectividade local-global e sinais de orientação.
- [x] Implementar identificação de arestas de fronteira.
- [x] Implementar leitura Gmsh MSH 4.1 ASCII com nós, triângulos lineares, linhas de fronteira e physical tags via `$Entities`.
- [x] Rejeitar explicitamente formatos binários, elementos de alta ordem e elementos 2D não triangulares.

**Critério de conclusão**

- [x] Teste de malha com um triângulo.
- [x] Teste de malha com dois triângulos compartilhando uma aresta.
- [x] Teste de contagem de nós, arestas, elementos e fronteiras.
- [x] Teste de sinais local-global.
- [x] Teste de rejeição de triângulo horário.
- [x] Teste de rejeição de triângulo degenerado.
- [x] Fixture Gmsh 4.1 ASCII mínimo com dois triângulos e tags físicas.

---

## Fase 4 — Funções de forma nodais e de aresta

**Status:** núcleo concluído.

**Tarefas**

- [x] Reusar as funções nodais já implementadas em `Triangle`.
- [x] Implementar coeficientes de aresta das Equações (17)–(21).
- [x] Implementar ângulos de aresta com `atan2` normalizado para `[0, pi)`.
- [x] Implementar avaliação de `{U}` e `{V}`.
- [x] Implementar derivadas constantes `{U_y}` e `{V_x}`.
- [x] Preservar a formulação de elementos de aresta; não introduzir formulação nodal alternativa.

**Critério de conclusão**

- [x] Teste 9 de `docs/15`: funções de forma de aresta no triângulo de referência.
- [x] Teste 10 de `docs/15`: orientação de aresta via sinais local-global.
- [ ] Teste 11 de `docs/15`: continuidade tangencial ao longo de mais de um ponto em aresta compartilhada.
- [x] Teste 15 de `docs/15`: `{U_y}` e `{V_x}`.

---

## Fase 5 — Matrizes locais do Apêndice

**Status:** núcleo concluído; quadratura independente ainda pendente.

**Tarefas**

- [x] Implementar A1-A8 como núcleo de produção fiel ao artigo.
- [x] Usar Eigen para matrizes locais pequenas.
- [ ] Comparar fórmulas fechadas com quadratura independente em testes.
- [x] Manter A11 e A12 como pendência/teste auxiliar, sem uso na montagem principal diagonal.

**Critério de conclusão**

- [x] Testes A1-A8 no triângulo de referência.
- [x] Testes A7-A8 no triângulo geral de `docs/15`.
- [ ] Verificação de simetria onde aplicável em testes dedicados.
- [x] Verificação do fator 4 associado à expansão do termo de rotacional na montagem geométrica.

---

## Fase 6 — Montagem global parcial

**Status:** núcleo parcial concluído.

**Tarefas**

- [x] Montar blocos globais com graus de liberdade de aresta para o campo transversal.
- [x] Montar blocos globais com graus de liberdade nodais para a componente axial.
- [x] Usar matrizes esparsas Eigen.
- [x] Preservar a separação conceitual dos blocos geométricos: `Ktt`, `Ktz`, `Kzz`, `Mtt` e `Mzz`.
- [x] Manter PEC/PMC como marcadores até a política matemática estar testada.

**Critério de conclusão**

- [x] Teste de dimensões dos blocos globais.
- [ ] Teste de simetria em blocos que devem ser simétricos.
- [x] Teste de montagem em malha de dois elementos.
- [x] Teste de consistência entre sinais de aresta e contribuição global.

---

## Fase 7 — Autovalores e validação contra o artigo

**Status:** iniciada; validação científica pendente.

**Tarefas**

- [x] Implementar redução sem inversão explícita de `[K_zz]`: resolver sistemas lineares fatorados.
- [x] Começar com solver denso Eigen para casos pequenos.
- [ ] Avaliar Spectra, ARPACK-NG ou LAPACK apenas se os exemplos reais exigirem.
- [ ] Criar casos Gmsh para as Figuras 3, 5 e 7.
- [ ] Exportar resultados para CSV.
- [ ] Criar scripts Python de plotagem e comparação.

**Critério de conclusão**

- [x] Autovalores pequenos reprodutíveis antes das curvas do artigo.
- [ ] Figura 3 reproduzida com erro quantitativo documentado.
- [ ] Figura 5 reproduzida com erro quantitativo documentado.
- [ ] Figura 7 reproduzida com erro quantitativo documentado.

---

## Pendências científicas preservadas

- [ ] **Sinais com fator `j`:** auditar a matriz `[B]`, principalmente termos com `j{N_x}`, `j{N_y}`, `{U}` e `{V}`.
- [ ] **PEC/PMC:** definir quais graus de liberdade são anulados para cada escolha `phi=E` ou `phi=H`.
- [x] **Inversão de `[K_zz]`:** implementar solução de sistemas lineares em vez de inversão explícita para o solver denso inicial.
- [ ] **Tensores não diagonais:** manter fora do escopo inicial até que integrais derivadas e acoplamentos estejam validados.
- [ ] **A11/A12:** testar numericamente como integrais complementares, mas não usar na montagem diagonal inicial do artigo.
- [ ] **Direitos de imagem:** verificar licença das imagens dos autores em `docs/00_resumo.md`.
- [ ] **Figura 1 anotada:** criar versão didática com ângulos `theta_4`, `theta_5`, `theta_6` se necessário.
- [ ] **Validação científica:** não declarar reprodução das curvas concluída sem arquivos de entrada, saída, script e erro quantitativo.

---

## Reprodutibilidade esperada

- [x] `cmake -S . -B build`
- [x] `cmake --build build`
- [x] `/usr/bin/ctest --test-dir build --output-on-failure`
- [ ] Entradas versionadas em `data/input/`.
- [ ] Saídas geradas em `data/output/` ou `out/`.
- [ ] Scripts de execução em `scripts/run/`.
- [ ] Scripts de plotagem em `scripts/plot/`.

---

## Critério geral de fechamento do projeto

O projeto só deve ser considerado fechado quando:

- todos os itens críticos e altos estiverem concluídos;
- as curvas das Figuras 3, 5 e 7 tiverem reprodução quantitativa documentada;
- os scripts gerarem os dados e gráficos por linha de comando;
- o README estiver sincronizado com o estado real do repositório;
- não houver `TODO` ou `FIXME` em código sem item correspondente neste arquivo.
