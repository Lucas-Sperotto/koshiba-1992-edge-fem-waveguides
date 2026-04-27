# TODO — Roteiro para Fechamento do Projeto

Este arquivo é o roteiro mestre para a conclusão do projeto. Ele substitui a lista de pendências da Fase 1 e organiza todas as tarefas restantes para que o projeto seja considerado **fechado**: implementado, validado, documentado e reprodutível.

---

## 0. Status Atual

- [x] **Fase 1 documental fechada**: tradução, notas editoriais e baseline científica registradas em `docs/00` a `docs/08`.
- [x] **Fase 2 documental complementar fechada**: pontes matemáticas para implementação registradas em `docs/09` a `docs/16`.
- [x] **Índice de documentação criado**: `docs/README.md` consolida a navegação entre os capítulos.
- [x] **Fase 3 iniciada**: núcleo C++17 de geometria triangular criado com CMake e teste mínimo.

---

## 1. Bloqueios Críticos

*Estas tarefas impedem o início da validação numérica. Devem ser resolvidas primeiro.*

- [x] **(Concluído) Derivar integrais complementares da família aresta–nó**: As integrais $\{V\}\{N_x\}^T$ e $\{U\}\{N_y\}^T$ foram derivadas analiticamente e registradas como Equações (A11) e (A12) em `docs/06_apendice.md`, com nota editorial explicando seu escopo.
  - **Correção científica:** As Eqs. (29b) e (32b) usam $\{V\}\{N_y\}^T$ (A5) e $\{U\}\{N_x\}^T$ (A4), que já constavam no apêndice. As novas (A11)–(A12) completam a família, mas só são necessárias para extensões com tensor $[p]$ não diagonal. A montagem de `[K_tz]` para o caso do artigo (diagonal) não estava bloqueada.
  - **Pendência restante:** Criar teste unitário em `tests/` que verifique numericamente (A11) e (A12) para um triângulo de geometria conhecida.

- [x] **(Concluído) Criar núcleo geométrico inicial**: `CMakeLists.txt`, `include/koshiba/mesh/`, `src/mesh/` e `tests/triangle_geometry_tests.cpp` foram criados para validar área, orientação, centroide, coeficientes nodais, funções nodais e gradientes no triângulo de referência.

- [ ] **(CRÍTICO) Implementar o núcleo do solver FEM**: Estruturas de dados e rotinas essenciais para a montagem das matrizes globais.
  - **Impacto:** Sem o solver, nenhuma simulação pode ser executada.
  - **Validação:**
    - [ ] Criar classes para malha (nós, elementos, arestas) em `src/mesh/`.
    - [ ] Implementar o cálculo das matrizes elementares (Eqs. A1-A8 e as derivadas acima) em `src/fem/`.
    - [ ] Implementar a montagem das matrizes globais `[K]` e `[M]` (Eqs. 26, 27).
    - [ ] Implementar a redução do sistema (Eq. 35), incluindo uma estratégia para a inversão de `[K_zz]`.
    - [ ] Integrar um solver de autovalores (ex: Eigen) para resolver a Eq. (34).

---

## 2. Pendências Científicas e de Implementação

*Tradução da física do artigo para código funcional e validado.*

- [ ] **(ALTO) Implementar os 3 casos de validação do artigo**:
  - [ ] **Microstrip (Fig. 3)**: Criar executável em `examples/microstrip/` que receba os parâmetros da geometria (Fig. 2) e do material (isotrópico e anisotrópico) e gere um arquivo `.csv` com a curva de dispersão (`β/k0` vs. frequência).
  - [ ] **Guia Retangular (Fig. 5)**: Criar executável em `examples/rectangular_dielectric_waveguide/` que reproduza as curvas de dispersão (`b` vs. `v`) para os modos `E^x` e `E^y`.
  - [ ] **Guia Triangular (Fig. 7)**: Criar executável em `examples/triangular_core_waveguide/` que reproduza as curvas de dispersão para o modo `E^y_11`.

- [ ] **(ALTO) Definir e implementar estratégia para a inversão de `[K_zz]`**: A Eq. (35) requer `[K_zz]⁻¹`. A implementação deve ser robusta a matrizes mal-condicionadas ou singulares (ver E5 da Fase 1).
  - **Ação:** Em `src/algebra/`, implementar a solução de `[K_zz] * X = [K_zt]` em vez da inversão explícita, usando uma fatoração estável (ex: LU com pivoteamento ou SVD). Adicionar tratamento de exceção para matrizes singulares.

- [ ] **(MÉDIO) Implementar estrutura para leitura de malha**: O código precisa ler malhas triangulares de um formato padrão (ex: Gmsh `.msh`).
  - **Ação:** Criar um parser em `src/io/` que leia nós, elementos e *physical groups* (para identificar materiais e contornos).

- [ ] **(MÉDIO) Implementar aplicação de Condições de Contorno**: O artigo usa condutores elétricos e magnéticos perfeitos.
  - **Ação:** Implementar em `src/fem/` a modificação das matrizes globais para impor `Dirichlet` (zerar linhas/colunas) ou `Neumann` (naturalmente satisfeitas) nas fronteiras externas.

---

## 3. Reprodutibilidade e Automação

*Garantir que qualquer pessoa possa clonar o repositório e reproduzir os resultados.*

- [ ] **(ALTO) Criar scripts de plotagem para validação**: Para cada caso de validação, deve haver um script Python que lê o `.csv` gerado pelo solver e plota o gráfico correspondente, sobrepondo os pontos do artigo original (digitalizados) para comparação quantitativa.
  - **Arquivos:** `scripts/plot/plot_fig3.py`, `scripts/plot/plot_fig5.py`, `scripts/plot/plot_fig7.py`.

- [ ] **(MÉDIO) Criar scripts de execução (`run`)**: Automatizar a execução dos exemplos.
  - **Arquivos:** `scripts/run/run_microstrip.sh`, `scripts/run/run_all.sh`, etc. Esses scripts devem chamar os executáveis C++ com os parâmetros corretos e garantir que os outputs sejam salvos em `data/output/`.

- [ ] **(MÉDIO) Padronizar entrada e saída dos executáveis**: Todos os executáveis em `examples/` devem seguir um padrão de linha de comando (ex: `--input <config.json> --output <results.csv>`).
  - **Ação:** Implementar um parser de argumentos em C++ e definir um formato de arquivo de configuração (ex: JSON) em `data/input/` para cada exemplo.

- [ ] **(BAIXO) Configurar pipeline de CI (GitHub Actions)**:
  - **Ação:** Criar um workflow `.github/workflows/build_and_test.yml` que compile o código e execute os testes (`tests/`) a cada push.

---

## 4. Documentação e Clareza Didática

*Melhorar a documentação para que ela sirva como material de estudo.*

- [ ] **(MÉDIO) Conectar equações ao código**: Adicionar comentários no código-fonte C++ referenciando as equações do artigo (ex: `// Implements Eq. (A1) from the Appendix`).

- [x] **(Concluído) Adicionar notas didáticas sobre a física**: Os pontos levantados na Fase 1 foram consolidados nos complementos `docs/09` a `docs/16`, incluindo Maxwell → Eq. (1), Eq. (1) → funcional, fator `j`, funções de forma, integrais do apêndice, testes mínimos e contrato C++17.

- [ ] **(BAIXO) Melhorar a Figura 1**: A Figura 1 não mostra os ângulos `θ` da Eq. (20).
  - **Ação:** Criar uma nova versão de `docs/img/fig_01.png` (ou uma `fig_01_annotated.svg`) que inclua os ângulos `θ_4, θ_5, θ_6`, facilitando o entendimento.

- [ ] **(BAIXO) Verificar direitos das imagens dos autores**: As fotos em `docs/00_resumo.md` precisam ter a licença verificada.
  - **Ação:** Se a licença não puder ser confirmada, remover as imagens.

---

## 5. Testes e Qualidade de Código

*Garantir a correção e robustez da implementação.*

- [ ] **(ALTO) Criar conjunto de testes unitários e de integração**:
  - **Ação:** Usando um framework como Google Test, criar testes em `tests/` para:
    - **Geometria:** Cálculo de área e coeficientes `a, b, c` de um triângulo.
    - **Funções de Forma:** Valores das funções de forma nodais e de aresta em pontos conhecidos.
    - **Matrizes Elementares:** Comparar a saída das funções de matrizes locais com resultados calculados à mão para um elemento simples (ex: triângulo retângulo unitário).
    - **Simetria:** Verificar se as matrizes que devem ser simétricas (`[K_zz]`, `[M_tt]`, etc.) de fato o são.

### Pendências identificadas pela auditoria de `docs/15_testes_matematicos_minimos.md`

Os itens abaixo corrigem ou completam `docs/15` antes da implementação dos testes em C++.

- [x] **(Concluído) Resolver a ambiguidade de θ₄ e documentar a convenção** (`docs/15`, Seção do Teste 9):
  Convenção adotada: usar `atan2(y_k-y_l, x_k-x_l)`, somar π se resultado < 0, tratar π como 0 (limite estrito). Isso dá θ₄=0, produzindo ā₁=+1, c̄₁=−1. Valores completos documentados no Teste 9 com verificação numérica.

- [x] **(Concluído) Adicionar valores esperados explícitos ao Teste 9 de `docs/15`** (funções de aresta):
  Valores adicionados: ā=(1,0,0), b̄=(0,0,1), c̄=(−1,−√2,1); θ₄=0, θ₅=3π/4, θ₆=π/2; Δ=√2/2>0. Verificação numérica da condição tangencial incluída.

- [x] **(Concluído) Adicionar segundo triângulo de teste** (`docs/15`, Seção 22 — Teste 18):
  Triângulo P1=(0,0), P2=(2,1), P3=(1,3) adicionado com A_e=5/2 e todos b_k, c_k não-nulos. Matrizes A7 e A8 explícitas calculadas.

- [x] **(Concluído) Adicionar testes para integrais A3, A4, A5** (`docs/15`, Seções 20 e 21 — Testes 16 e 17):
  A3 com fator 4 no rotacional documentado. A4 e A5 com matrizes numéricas completas para o triângulo de referência.

- [x] **(Concluído) Adicionar valores esperados concretos aos Testes 10 e 11 de `docs/15`**:
  Teste 10: sinal s_e=+1, ambos elementos dão θ=3π/4. Teste 11: φ_t=1 em P_m e P_q calculados explicitamente para T1 e T2.

- [x] **(Concluído) Adicionar teste de {U_y} e {V_x}** (`docs/15`, Seção 19 — Teste 15):
  {U_y}=(−1,−√2,1) e {V_x}=(1,√2,−1) documentados com relação {V_x}=−{U_y}.

- [x] **(Concluído) Adicionar matrizes numéricas explícitas aos Testes 12 e 13 de `docs/15`**:
  A7=(1/2)\[[1,−1,0],[−1,1,0],[0,0,0]\], A8=(1/2)\[[1,0,−1],[0,0,0],[−1,0,1]\], A6=(1/24)\[[2,1,1],[1,2,1],[1,1,2]\] adicionados.

- [x] **(Concluído) Corrigir notação do Teste 8 em `docs/15`**:
  Reescrito mostrando ∂L_k/∂x = b_k/(2A_e) com substituição explícita e nota de atenção para o implementador.

- [x] **(Concluído) Adicionar teste de orientação horária** (`docs/15`, Seção 23 — Teste 19):
  Diagnóstico completo: b_k^CW = −b_k^CCW, diagonal de A7 negativa com A_e < 0, correção com |A_e|.

- [x] **(Concluído) Documentar Δ para o triângulo de referência** (`docs/12`, Seção 9):
  Nota adicionada: Δ=√2/2 > 0 para o triângulo de referência com θ₄=0. Sinal de Δ depende da convenção; o que invalida o elemento é |Δ|≈0.

- [x] **(Concluído) Adicionar testes para integrais A1 e A2** (`docs/15`, Seções 24 e 25 — Testes 20 e 21):
  A1 (massa {U}{U}^T) e A2 (massa {V}{V}^T) com matrizes numéricas completas para o triângulo de referência. Propriedades de verificação: simetria e tr(A1)=tr(A2)=A_e. Simetria estrutural A1↔A2 documentada.

- [ ] **(MÉDIO) Refatorar código duplicado**: Após a implementação inicial, identificar e refatorar código repetido entre os diferentes exemplos.

---

## 6. Melhorias (Não Bloqueantes)

*Tarefas para melhorar a qualidade do projeto, mas que não impedem seu fechamento.*

- [ ] **(BAIXO) Otimização de performance**: Analisar gargalos de performance (ex: montagem de matrizes, solução de autovalores) e otimizar se necessário.
- [ ] **(BAIXO) Visualização de campos**: Criar scripts para exportar os autovetores (campos `E` ou `H`) para um formato legível por Paraview/VTK.

---

## 7. Critério de Conclusão do Projeto

O projeto será considerado **"fechado"** quando todos os itens a seguir forem concluídos:

1. **Validação Completa**:
    - [ ] Todas as curvas das Figuras 3, 5 e 7 do artigo foram reproduzidas com desvio quantitativo documentado e inferior a um limiar aceitável (ex: 5%).
    - [ ] Os scripts em `scripts/plot/` geram gráficos que sobrepõem os resultados do código aos do artigo.

2. **Reprodutibilidade Total**:
    - [ ] O comando `git clone ... && cd ... && cmake . && make` compila o projeto sem erros.
    - [ ] O script `scripts/run/run_all.sh` executa todas as simulações e gera todos os arquivos de dados em `data/output/`.
    - [ ] O script `scripts/plot/plot_all.sh` (ou similar) gera todos os gráficos de validação em `out/`.

3. **Qualidade e Documentação**:
    - [ ] Todos os itens de prioridade **Crítica** e **Alta** deste `TODO.md` foram resolvidos.
    - [ ] O `README.md` foi atualizado com instruções claras de compilação, execução e reprodução dos resultados.
    - [ ] Não há mais pendências marcadas como `TODO` ou `FIXME` no código-fonte sem um item correspondente neste arquivo.

---

### Histórico (Fase 1 - Concluída)

*A Fase 1 (baseline documental) foi fechada. Suas pendências foram migradas para as seções acima.*
