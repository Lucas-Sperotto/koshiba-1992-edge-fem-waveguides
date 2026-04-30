# TODO — roteiro pós-baseline documental

Este arquivo é o mapa mestre de pendências do projeto depois do fechamento documental. Ele registra o que já tem evidência técnica, o que ainda falta validar cientificamente e quais dúvidas devem permanecer visíveis até serem resolvidas contra o artigo original.

---

## Status atual

- [x] **Fase 1 — Tradução e documentação científica básica:** baseline documental fechada em `docs/00` a `docs/08`.
- [x] **Fase 2 — Derivação matemática e contrato numérico:** contrato matemático fechado em `docs/09` a `docs/16`.
- [x] **Fase 3A — Estrutura C++17 mínima com CMake:** biblioteca, CMake, `Node` e `Triangle`.
- [x] **Fase 3B — Geometria, malha e Gmsh:** `Edge`, `Mesh`, conectividade e leitor MSH 4.1 ASCII.
- [x] **Fase 4 — Funções de forma nodais e de aresta:** núcleo implementado e testado, incluindo continuidade tangencial em aresta compartilhada.
- [x] **Fase 5 — Matrizes locais do Apêndice:** A1-A8 implementadas e comparadas com quadratura independente; A11/A12 cobertas como auxiliares.
- [x] **Fase 6 — Montagem global parcial:** blocos geométricos esparsos implementados e testados, incluindo simetria esperada.
- [x] **Fase 7A — Solver físico mínimo:** combinação beta para material diagonal e `phi=E/H` implementada para casos pequenos.
- [x] **Fase 7B — Reprodutibilidade mínima:** mini caso sintético com entrada, CSV, runner e plot.
- [ ] **Fase 7C — Validação das figuras do artigo:** dossiê, exemplos Gmsh, configs, scripts e CSVs de smoke criados; validação quantitativa das Figuras 3, 5 e 7 ainda pendente.

---

## Auditoria de 2026-04-28

### Problemas encontrados e corrigidos

- [x] `src/fem/global_assembly.cpp` quebrava o build por usar APIs inexistentes; corrigido para a API real de `Mesh`, `Triangle` e `compute_local_integrals(...)`.
- [x] Testes de montagem/solver foram mantidos no estilo assert-based do repositório, sem GoogleTest.
- [x] O fixture versionado `tests/fixtures/two_triangles_msh41.msh` passou a ser reutilizado.
- [x] O README foi realinhado com o estado do projeto após a regressão de build.
- [x] B0 — build limpo e CTest completo restaurados após reconfiguração.

### Nova evidência desta etapa

- [x] Material diagonal genérico criado em `include/koshiba/physics/material.hpp`.
- [x] `FieldKind::Electric` e `FieldKind::Magnetic` convertem corretamente para coeficientes diagonais `[p]` e `[q]`.
- [x] Solver beta mínimo criado em `include/koshiba/fem/beta_solver.hpp`.
- [x] Redução da Equação (35) usa solução de sistemas lineares para `K_zz`, sem inversão explícita.
- [x] Mini caso reprodutível criado em `data/input/mini_case.cfg`.
- [x] CSV gerado em `data/output/mini_case_modes.csv` pelo runner/teste.
- [x] Plot simples gerado em `out/mini_case_modes.svg`.
- [x] Política essencial PEC/PMC registrada em `docs/18_politica_pec_pmc.md`.
- [x] Auditoria algébrica dos sinais da redução registrada em `docs/19_auditoria_sinais_acoplamentos.md`.
- [x] Dossiê dos casos de validação das Figuras 3, 5 e 7 registrado em `docs/20_dossie_casos_validacao_figuras.md`.

---

## Auditoria de 2026-04-29

### Problemas encontrados e corrigidos

- [x] Marcador cru de correção manual removido de `docs/16_contrato_para_implementacao_cpp.md`.
- [x] `CITATION.cff` criado com referência ao artigo original.
- [x] PDF e PNGs do artigo removidos do versionamento local.
- [x] Figuras geométricas substituídas por SVGs próprios em `docs/img/`.
- [x] README e índice dos docs atualizados para refletir o estado real.

### Nova evidência desta etapa

- [x] `MaterialMap` por `physical tag` implementado.
- [x] Montagem heterogênea por elemento implementada para o solver em `beta`.
- [x] Erro explícito quando falta material para uma tag de triângulo.
- [x] Solver direto `beta -> k0/f` implementado para a Figura 3.
- [x] Normalizações `v -> k0`, `k0 -> v`, `beta/k0 -> b` e `k0 -> GHz` implementadas.
- [x] Rastreamento modal por overlap implementado como utilitário.
- [x] Exemplos versionados em `examples/microstrip/`, `examples/rectangular_dielectric_waveguide/` e `examples/triangular_core_waveguide/`.
- [x] `scripts/run/generate_meshes.sh` e `scripts/run/run_all_validation.sh` criados.
- [x] `scripts/plot/plot_validation.py` e `scripts/plot/compare_validation.py` criados.
- [x] CSVs de smoke gerados em `data/output/validation/`.
- [x] Gráficos próprios gerados em `out/validation/`.
- [x] `out/validation/validation_summary.csv` gerado com status `no_reference` enquanto não houver referência numérica versionada.
- [x] Relatório de validação atual registrado em `docs/21_relatorio_validacao_atual.md`.
- [x] Unidade da Figura 3 corrigida para `beta_rad_per_mm` no CSV, com conversão interna para `rad/m`.
- [x] Pontos com $b$ fora de $[0,1]$ classificados como `outside_guided_range`.
- [x] Suíte atual com `15/15` testes CTest passando.

### Limitações preservadas

- [ ] As malhas atuais são grossas para caber no solver denso e não têm as contagens finais do artigo.
- [ ] Os casos das Figuras 5 e 7 ainda precisam de seleção modal/PEC/PMC validada; resultados sem modo guiado físico ou com $b$ fora da faixa não devem ser tratados como reprodução.
- [ ] A Figura 3 ainda precisa de referência quantitativa em GHz para confirmar a escala corrigida.
- [ ] Nenhuma curva das Figuras 3, 5 ou 7 está declarada reproduzida.
- [ ] `status=no_reference` permanece esperado enquanto não houver CSV de referência conferido em `data/input/reference/`.

### Diagnóstico dos primeiros CSVs de smoke

- [ ] fig5 (todas as subfiguras): todos os pontos de smoke retornam `non_positive_beta2`; o solver não encontra nenhum modo guiado em v∈[0.3, 1.5]. Causa provável: paredes PEC do domínio truncado geram box modes que ocupam os primeiros autovalores; domínio de cladding pode ser insuficiente para baixo contraste (Δn ≈ 0.05 → comprimento evanescente elevado).
- [ ] fig7 (todas as subfiguras): todos os pontos de smoke retornam `outside_guided_range` com b ≈ −31 a −2.6 (beta² > 0 mas beta/k0 < n_cladding). Mesma causa provável; contraste ainda menor (Δn ≈ 0.0085, domínio de cladding de 3t × 5t provavelmente insuficiente).
- [ ] fig3a/fig3b: mode_0 não é monotônico — frequência recua de 16.73 para 16.72 GHz ao passar de beta=1.4 para 1.8 rad/mm. Possível crossing silencioso com mode_1 no solver denso.

---

## Histórico documental

### Fase 1 — concluída como baseline documental

- Tradução técnica organizada em `docs/00_resumo.md` a `docs/07_referencias.md`.
- Notas editoriais e científicas consolidadas em `docs/08_notas_editoriais_e_cientificas.md`.
- Navegação documental em `docs/README.md`.
- Problemas de renderização de equações e legendas corrigidos no fechamento documental.

### Fase 2 — concluída como contrato matemático

- Maxwell para a Equação (1): `docs/09_maxwell_para_equacao_01.md`.
- Equação (1) para o funcional variacional: `docs/10_equacao_01_para_funcional_06.md`.
- Origem do fator `j` na Equação (7): `docs/11_origem_do_fator_j_equacao_07.md`.
- Funções nodais e de aresta: `docs/12_funcoes_de_forma_nodais_e_de_aresta.md`.
- Revisão das integrais do Apêndice: `docs/13_revisao_das_integrais_do_apendice.md`.
- Integrais cruzadas, termos ausentes e escopo de extensões: `docs/14_integrais_cruzadas_e_termos_ausentes.md`.
- Testes matemáticos mínimos: `docs/15_testes_matematicos_minimos.md`.
- Contrato para implementação C++17: `docs/16_contrato_para_implementacao_cpp.md`.
- Nota de implementação da Fase 7: `docs/17_implementacao_fase7_solver_beta.md`.
- Política inicial para PEC/PMC: `docs/18_politica_pec_pmc.md`.
- Auditoria de sinais e acoplamentos: `docs/19_auditoria_sinais_acoplamentos.md`.
- Dossiê dos casos de validação das Figuras 3, 5 e 7: `docs/20_dossie_casos_validacao_figuras.md`.

### Justificativa de limpeza

O bloco antigo `TODO — Fase 2: Derivação matemática e contrato numérico` foi removido porque apontava para nomes obsoletos. O conteúdo entregue está nos documentos `docs/09` a `docs/17`, e as dúvidas científicas preservadas continuam listadas abaixo.

---

## Fases 3A e 3B — infraestrutura e malha

**Status:** concluídas.

**Evidências**

- [x] `find_package(Eigen3 REQUIRED)` no CMake.
- [x] `mesh::Node`, `mesh::Triangle`, `mesh::Edge` e `mesh::Mesh`.
- [x] Convenção local do artigo: arestas 4 = `(1,2)`, 5 = `(2,3)`, 6 = `(3,1)`.
- [x] Orientação global por par canônico `(min(node_id), max(node_id))` e sinal local-global separado.
- [x] Rejeição de triângulos horários e degenerados.
- [x] Leitor Gmsh MSH 4.1 ASCII com nós, triângulos lineares, linhas de fronteira e physical tags via `$Entities`.
- [x] Testes de um triângulo, dois triângulos, fronteira, sinais, degeneração e orientação.

---

## Fase 4 — funções de forma nodais e de aresta

**Status:** concluída.

**Evidências**

- [x] Funções nodais reutilizadas de `Triangle`.
- [x] Coeficientes de aresta das Equações (17)–(21).
- [x] Ângulos com `atan2` normalizado para `[0, pi)`.
- [x] Avaliação de `{U}`, `{V}`, `{N}` e derivadas.
- [x] Teste 9 de `docs/15`: funções de aresta no triângulo de referência.
- [x] Teste 10 de `docs/15`: orientação de aresta via sinais local-global.
- [x] Teste 11 de `docs/15`: continuidade tangencial em mais de um ponto na aresta compartilhada.
- [x] Teste 15 de `docs/15`: `{U_y}` e `{V_x}`.

---

## Fase 5 — matrizes locais do Apêndice

**Status:** concluída para o núcleo diagonal inicial.

**Evidências**

- [x] A1-A8 implementadas como núcleo de produção.
- [x] Matrizes locais pequenas usam Eigen.
- [x] A1-A8 comparadas com quadratura independente em triângulo de referência, triângulo geral e triângulo escalado.
- [x] A11/A12 testadas por quadratura como integrais auxiliares.
- [x] A11/A12 permanecem fora da montagem diagonal principal.
- [x] Simetria dos blocos locais esperados coberta por teste.
- [x] Fator 4 do termo de rotacional preservado na montagem geométrica.

---

## Fase 6 — montagem global parcial

**Status:** concluída para blocos geométricos sem política física final de fronteira.

**Evidências**

- [x] Blocos globais com DOFs de aresta para campo transversal.
- [x] Blocos globais com DOFs nodais para componente axial.
- [x] Matrizes esparsas Eigen.
- [x] Separação conceitual dos blocos `Ktt`, `Ktz`, `Kzz`, `Mtt` e `Mzz`.
- [x] Teste de dimensões dos blocos globais.
- [x] Teste de simetria em blocos globais esperados.
- [x] Teste de montagem em malha de dois elementos.
- [x] Teste de consistência entre sinais de aresta e contribuição global.

**Evidência adicional**

- [x] Política essencial PEC/PMC definida para o campo resolvido: `PEC + phi=E` e `PMC + phi=H`.
- [x] Restrições nodais axiais separadas de restrições de aresta via `node_physical_tags`.
- [ ] Validar a escolha de PEC/PMC por caso físico das Figuras 3, 5 e 7.

---

## Fase 7A — solver físico mínimo

**Status:** concluída para material diagonal e casos pequenos.

**Evidências**

- [x] `FieldKind {Electric, Magnetic}` implementado.
- [x] `DiagonalMaterial` implementado com `eps_rx`, `eps_ry`, `eps_rz`.
- [x] Conversão para `[p]` e `[q]`: para `E`, `p=I`, `q=eps`; para `H`, `p=1/eps`, `q=I`.
- [x] `Ktt_beta = k0^2(qx*Mtt_UU + qy*Mtt_VV) - pz*Ktt_curl`.
- [x] `Ktz_beta = py*Ktz_U_Nx + px*Ktz_V_Ny`.
- [x] `Kzz_beta = k0^2*qz*Mzz_NN - py*Kzz_NxNx - px*Kzz_NyNy`.
- [x] `Mtt_beta = py*Mtt_UU + px*Mtt_VV`.
- [x] `Mtt_hat = Mtt_beta + Ktz*Kzz^{-1}*Kzt` calculado por solve linear.
- [x] Problema `Ktt_beta * phi_t = beta^2 * Mtt_hat * phi_t` resolvido com Eigen denso para casos pequenos.
- [x] Recuperação axial `phi_z = beta*Kzz^{-1}*Kzt*phi_t` implementada por solve linear.
- [x] Resíduos das Equações (31a) e (31b) verificados após a redução.
- [x] Retorno de `beta2`, `beta`, autovetores transversais e status de filtragem.
- [x] Mecanismo genérico de restrição de DOFs de aresta por physical tag.
- [x] Mecanismo explícito de restrição de DOFs nodais axiais por physical tag.
- [x] Conversão de `PEC/PMC + phi=E/H` para restrições essenciais testada.

**Pendências**

- [ ] Validar o sinal físico dos termos acoplados contra os casos do artigo.
- [ ] Validar a política PEC/PMC nos exemplos reais antes de gerar curvas do artigo.
- [ ] Avaliar Spectra, ARPACK-NG ou LAPACK apenas se os exemplos reais exigirem.

---

## Fase 7B — reprodutibilidade mínima

**Status:** concluída para mini caso sintético.

**Evidências**

- [x] Entrada versionada em `data/input/mini_case.cfg`.
- [x] Executável `koshiba_mini_case`.
- [x] Runner `scripts/run/run_mini_case.sh`.
- [x] CSV em `data/output/mini_case_modes.csv` com cabeçalho `mode,beta2,beta,k0,field_kind,status`.
- [x] Script `scripts/plot/plot_mini_case.py`.
- [x] Gráfico simples em `out/mini_case_modes.svg`.
- [x] Teste de fumaça do mini caso no CTest.
- [x] Teste do cabeçalho CSV no CTest.
- [x] Nota documental em `docs/17_implementacao_fase7_solver_beta.md`.

---

## Fase 7C — validação das figuras do artigo

**Status:** pendente.

**Critérios de conclusão**

- [x] Criar dossiê documental dos casos das Figuras 3, 5 e 7.
- [x] Registrar eixos, grades, parâmetros físicos e normalizações por subfigura.
- [x] Registrar política `t_default=1.0`, override futuro e teste de invariância de escala.
- [x] Registrar plano de comparação por inspeção visual e referências externas Marcatili/Goell.
- [x] Criar casos Gmsh e materiais versionados para a Figura 3.
- [x] Criar casos Gmsh e materiais versionados para a Figura 5.
- [x] Criar casos Gmsh e materiais versionados para a Figura 7.
- [x] Implementar mapeamento essencial PEC/PMC por escolha `phi=E/H`.
- [ ] Validar PEC/PMC por caso de referência do artigo.
- [x] Exportar CSVs de smoke com o cabeçalho mínimo.
- [x] Criar scripts de plotagem comparáveis às figuras do artigo.
- [ ] Exportar curvas finais completas após seleção modal validada.
- [ ] Documentar erro quantitativo e limitações para a Figura 3.
- [ ] Documentar erro quantitativo e limitações para a Figura 5.
- [ ] Documentar erro quantitativo e limitações para a Figura 7.
- [ ] Diagnosticar ausência de modo guiado nas Figuras 5 e 7 antes de qualquer refinamento de malha: verificar se o domínio de cladding é grande o suficiente para que o campo evanescente esteja atenuado nas paredes PEC.
- [ ] Definir critério de tamanho do domínio externo: distância da fronteira do núcleo à parede PEC deve satisfazer d > 3/α, onde α = sqrt(n_cl²·k0² − β_target²); calcular para cada ponto representativo da varredura.
- [ ] Integrar `match_modes_by_overlap` no loop de sweep do `koshiba_validation_case` ou em pós-processamento Python para evitar crossing silencioso de modos entre pontos de varredura.
- [ ] Aumentar densidade da varredura de smoke para diagnóstico qualitativo: mínimo de 15–20 pontos por curva antes de comparar com referência.

---

## Pendências científicas preservadas

### Sinais com fator `j`

**Tipo:** científica  
**Origem:** artigo e formulação variacional  
**Arquivos relacionados:** `docs/11_origem_do_fator_j_equacao_07.md`, `docs/17_implementacao_fase7_solver_beta.md`, `src/fem/beta_solver.cpp`

**Critério de conclusão**

- [x] Auditar algebraicamente o sinal da redução das Equações (31)–(35).
- [x] Testar recuperação de `phi_z` sem inversa explícita.
- [ ] Auditar a matriz `[B]`, principalmente termos com `j{N_x}`, `j{N_y}`, `{U}` e `{V}`.
- [ ] Comparar o sinal dos blocos acoplados contra pelo menos um caso físico reproduzível.

### PEC/PMC

**Tipo:** científica e técnica  
**Origem:** condições de contorno do artigo  
**Arquivos relacionados:** `include/koshiba/fem/beta_solver.hpp`, `src/fem/beta_solver.cpp`

**Critério de conclusão**

- [x] Criar mecanismo genérico de restrição de DOFs por physical tag.
- [x] Definir quais DOFs são anulados para PEC com `phi=E`.
- [x] Definir quais DOFs são anulados para PMC com `phi=H`.
- [x] Cobrir o mapeamento essencial com teste e nota documental.
- [ ] Validar a escolha de PEC/PMC nas geometrias das Figuras 3, 5 e 7.

### Tensores não diagonais

**Tipo:** científica  
**Origem:** extensão além do núcleo inicial  
**Critério de conclusão**

- [ ] Manter fora do escopo inicial até que integrais derivadas e acoplamentos estejam validados.
- [ ] Criar nota separada antes de implementar termos não diagonais.

### A11/A12

**Tipo:** técnica e científica  
**Origem:** integrais auxiliares do Apêndice  
**Critério de conclusão**

- [x] Testar A11/A12 por quadratura independente.
- [x] Manter A11/A12 fora da montagem diagonal inicial.
- [ ] Reabrir apenas se uma extensão não diagonal exigir esses termos.

### Direitos de imagem e Figura 1

**Tipo:** documentação  
**Origem:** revisão editorial  
**Arquivos relacionados:** `docs/00_resumo.md`, `docs/img/fig_01_annotated.svg`

**Critério de conclusão**

- [x] Remover imagens de autores sem licença clara.
- [x] Registrar nota editorial em `docs/00_resumo.md`.
- [x] Criar `docs/img/fig_01_annotated.svg` como figura didática original com `theta_4`, `theta_5`, `theta_6`.

### Validação científica

**Tipo:** validação  
**Origem:** Figuras 3, 5 e 7 do artigo  
**Critério de conclusão**

- [x] Documentar o contrato inicial dos casos de validação em `docs/20_dossie_casos_validacao_figuras.md`.
- [ ] Não declarar reprodução das curvas sem arquivo de entrada, saída, script e erro quantitativo.
- [ ] Separar claramente mini casos sintéticos de validação científica contra o artigo.

### Domínio truncado e modos espúrios em guias dielétricos abertos

**Tipo:** física e numérica
**Origem:** Figuras 5 e 7 do artigo — diagnóstico pós-smoke
**Arquivos relacionados:** `examples/rectangular_dielectric_waveguide/`, `examples/triangular_core_waveguide/`

**Contexto:** guias dielétricos são estruturas abertas — o campo decai exponencialmente no cladding. Com paredes PEC a distância finita, o domínio truncado suporta box modes com beta²< n_cl²·k0², que ocupam os primeiros autovalores e mascaram os modos guiados reais. Isso explica o diagnóstico smoke de fig5 (non_positive_beta2 em toda a varredura) e fig7 (outside_guided_range com b << 0 em toda a varredura).

#### Critério de conclusão

- [ ] Para cada ponto representativo da varredura, calcular o comprimento de decaimento evanescente 1/α usando a solução analítica de referência (Marcatili ou modo analítico da laje).
- [ ] Verificar se a distância da interface do núcleo à parede PEC satisfaz d > 3/α; se não, aumentar o domínio nos arquivos `.geo` e recalibrar `lc`.
- [ ] Se o domínio for suficiente, auditar a ordenação dos autovalores: o modo guiado pode não ser o primeiro retornado pelo solver denso; filtrar autovalores na faixa n_cl²·k0² < beta² < n_co²·k0².
- [ ] Registrar o diagnóstico no dossiê (`docs/20`) antes de refinar malha ou ajustar PEC/PMC.

---

## Reprodutibilidade esperada

- [x] `cmake -S . -B build`
- [x] `cmake --build build`
- [x] `/usr/bin/ctest --test-dir build --output-on-failure`
- [x] Entradas versionadas em `data/input/`.
- [x] Saídas geradas em `data/output/`.
- [x] Scripts de execução em `scripts/run/`.
- [x] Scripts de plotagem em `scripts/plot/`.

---

## Próximo plano de resolução

1. Fechar sinais e acoplamentos contra caso físico:
   - auditar explicitamente a matriz `[B]` da Equação (28);
   - comparar contra um caso analítico ou referência controlada.
2. Preparar validação do artigo:
   - criar geometrias Gmsh das Figuras 3, 5 e 7 a partir do dossiê;
   - versionar entradas;
   - gerar CSVs e plots reproduzíveis;
   - registrar erro quantitativo.
3. Validar PEC/PMC nas figuras:
   - escolher `phi=E` ou `phi=H` por caso;
   - aplicar tags físicas de fronteira;
   - registrar o efeito das fronteiras artificiais.
4. Só então considerar solver esparso externo:
   - manter Eigen denso enquanto os casos forem pequenos;
   - avaliar Spectra/ARPACK-NG apenas se necessário para malhas reais.

---

## Critério geral de fechamento do projeto

O projeto só deve ser considerado fechado quando:

- as curvas das Figuras 3, 5 e 7 tiverem reprodução quantitativa documentada;
- scripts gerarem dados e gráficos por linha de comando;
- PEC/PMC estiverem validados para `phi=E` e `phi=H` nos casos do artigo;
- o README estiver sincronizado com o estado real;
- não houver `TODO` ou `FIXME` em código sem item correspondente neste arquivo.
