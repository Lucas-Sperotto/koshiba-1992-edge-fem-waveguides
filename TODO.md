# TODO — Roteiro para Fechamento do Projeto

Este arquivo é o roteiro mestre para a conclusão do projeto. Ele substitui a lista de pendências da Fase 1 e organiza todas as tarefas restantes para que o projeto seja considerado **fechado**: implementado, validado, documentado e reprodutível.

---

## 0. Status Atual

- [x] **Fase 1 documental fechada**: tradução, notas editoriais e baseline científica registradas em `docs/00` a `docs/08`.
- [x] **Fase 2 documental complementar fechada**: pontes matemáticas para implementação registradas em `docs/09` a `docs/16`.
- [x] **Índice de documentação criado**: `docs/README.md` consolida a navegação entre os capítulos.
- [ ] **Fase 3 em andamento**: iniciar a codificação C++17 por geometria triangular, orientação e testes matemáticos mínimos.

---

## 1. Bloqueios Críticos

*Estas tarefas impedem o início da validação numérica. Devem ser resolvidas primeiro.*

- [x] **(Concluído) Derivar integrais complementares da família aresta–nó**: As integrais $\{V\}\{N_x\}^T$ e $\{U\}\{N_y\}^T$ foram derivadas analiticamente e registradas como Equações (A11) e (A12) em `docs/06_apendice.md`, com nota editorial explicando seu escopo.
  - **Correção científica:** As Eqs. (29b) e (32b) usam $\{V\}\{N_y\}^T$ (A5) e $\{U\}\{N_x\}^T$ (A4), que já constavam no apêndice. As novas (A11)–(A12) completam a família, mas só são necessárias para extensões com tensor $[p]$ não diagonal. A montagem de `[K_tz]` para o caso do artigo (diagonal) não estava bloqueada.
  - **Pendência restante:** Criar teste unitário em `tests/` que verifique numericamente (A11) e (A12) para um triângulo de geometria conhecida.

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

- [ ] **(CRÍTICO) Resolver a ambiguidade de θ₄ para a aresta horizontal do triângulo de referência** (`docs/15`, Teste 9):
  O artigo define $0 \le \theta < \pi$, mas para k=1, l=2 no triângulo P1=(0,0), P2=(1,0), P3=(0,1): o vetor $(x_k-x_l, y_k-y_l) = (-1,0)$ dá $\text{atan2}(0,-1)=\pi$, que está **excluído** pelo intervalo estrito. Com arctan simples, $\theta_4=0$, produzindo $\bar{a}_1=+1$, $\bar{c}_1=-1$. Com atan2, $\theta_4=\pi$, produzindo $\bar{a}_1=-1$, $\bar{c}_1=+1$. Ambos satisfazem as condições tangenciais, mas com sinais opostos. **Ação:** definir a convenção usada pelo projeto (`atan2` mapeado para $[0,\pi)$), documentar em `docs/15`, e adicionar teste explícito dos ângulos $\theta_4=\pi$, $\theta_5=3\pi/4$, $\theta_6=\pi/2$ antes de qualquer teste de $\bar{a}_k$, $\bar{b}_k$, $\bar{c}_k$.

- [ ] **(CRÍTICO) Adicionar valores esperados explícitos ao Teste 9 de `docs/15`** (funções de aresta):
  O Teste 9 descreve a propriedade $\phi_t^{(r)}|_s = \delta_{rs}$ mas não fornece os coeficientes do triângulo de referência. Os valores corretos (usando atan2, $\theta_4=\pi$) são:
  - $\bar{a}_1=-1$, $\bar{b}_1=0$, $\bar{c}_1=1$ → $U_1(y)=-1+y$, $V_1(x)=-x$
  - $\bar{a}_2=0$, $\bar{b}_2=0$, $\bar{c}_2=-\sqrt{2}$ → $U_2(y)=-\sqrt{2}\,y$, $V_2(x)=\sqrt{2}\,x$
  - $\bar{a}_3=0$, $\bar{b}_3=1$, $\bar{c}_3=1$ → $U_3(y)=y$, $V_3(x)=1-x$

  Nota: $\bar{c}_2=-\sqrt{2}$ é irracional (comprimento da hipotenusa). $\Delta=-\sqrt{2}/2<0$ para este triângulo — isso é correto e não indica erro. Adicionar estes valores ao documento e à checklist da Seção 19.

- [ ] **(GRAVE) Adicionar segundo triângulo de teste sem zeros em $b_k$ e $c_k$** (`docs/15`, Seções 12 e 13):
  No triângulo de referência, $b_3=0$ e $c_2=0$, zerando linha/coluna 3 de A7 e linha/coluna 2 de A8. Bugs nessas entradas passam silenciosamente. **Ação:** adicionar um segundo triângulo geral a `docs/15`, por exemplo P1=(0,0), P2=(3,0), P3=(1,2) ($A_e=3$, todos $b_k,c_k$ não-nulos), com os valores esperados de A7 e A8 calculados explicitamente.

- [ ] **(GRAVE) Adicionar testes para integrais de aresta A1, A2, A3, A4, A5 em `docs/15`**:
  Os Testes 12–13 cobrem apenas A6, A7, A8 (funções nodais). As integrais de aresta — A1 e A2 com termos em $y_c$, $\sum y_k^2$ e fator 1/12; A3 com o fator-4 no rotacional; A4 e A5 de acoplamento aresta–nó — são mais complexas e mais sujeitas a erro. **Ação:** acrescentar pelo menos 3 testes a `docs/15`:
  - A3: verificar que $A_e\bar{c}_k\bar{c}_l$ vale $(1/2)[[1,-\sqrt{2},1],[-\sqrt{2},2,-\sqrt{2}],[1,-\sqrt{2},1]]$ para o triângulo de referência;
  - A4/A5: verificar os valores escalares $(1/2)(\bar{a}_k+\bar{c}_ky_c)b_l$ e $(1/2)(\bar{b}_k-\bar{c}_kx_c)c_l$;
  - A1 ou A2: verificar a matriz $3\times3$ completa (requer $\sum y_k^2$ e $y_c$).

- [ ] **(GRAVE) Adicionar valores esperados concretos aos Testes 10 e 11 de `docs/15`**:
  O Teste 10 (orientação de aresta) e o Teste 11 (continuidade tangencial) descrevem o comportamento sem fornecer resultados numéricos específicos. Para o par de triângulos T1=(0,0),(1,0),(0,1) e T2=(1,0),(1,1),(0,1) com aresta compartilhada (1,0)↔(0,1): calcular explicitamente o sinal de aresta local de cada elemento e o valor de $\phi_t$ em pelo menos dois pontos da aresta.

- [ ] **(GRAVE) Adicionar teste de $U_{y,k}=\bar{c}_k$ e $V_{x,k}=-\bar{c}_k$ em `docs/15`**:
  Para o triângulo de referência: $\{U_y\}=(1,-\sqrt{2},1)$ e $\{V_x\}=(-1,\sqrt{2},-1)$. Esses valores são críticos para a montagem de $[B]$ e para a integral A3. Acrescentar teste explícito na Seção 14 de `docs/15` com esses resultados como contrato numérico.

- [ ] **(MODERADO) Adicionar matrizes numéricas explícitas aos Testes 12 e 13 de `docs/15`**:
  O documento apresenta as fórmulas mas não as matrizes do triângulo de referência. Completar com:
  - A7 = $(1/2)[[1,-1,0],[-1,1,0],[0,0,0]]$ (linha 3 nula pois $b_3=0$)
  - A8 = $(1/2)[[1,0,-1],[0,0,0],[-1,0,1]]$ (linha 2 nula pois $c_2=0$)
  - A6 = $(1/24)[[2,1,1],[1,2,1],[1,1,2]]$

- [ ] **(MODERADO) Corrigir notação do Teste 8 em `docs/15`**:
  O documento escreve $\partial L_1/\partial x = b_1 = -1$, o que é verdadeiro apenas porque $2A_e=1$ para o triângulo de referência. A fórmula geral é $\partial L_k/\partial x = b_k/(2A_e)$. Reescrever mostrando a substituição explícita para evitar que o implementador ache que $\partial L_k/\partial x = b_k$ sem o denominador.

- [ ] **(MODERADO) Adicionar teste de $b_k$ e $c_k$ sob inversão de orientação em `docs/15`**:
  Para o triângulo (0,0),(0,1),(1,0) (ordem horária do mesmo triângulo de referência), verificar explicitamente que $b_k^{\text{CW}} = -b_k^{\text{CCW}}$, $c_k^{\text{CW}} = -c_k^{\text{CCW}}$, e que A7/A8 calculadas com $A_e^{\text{CW}}<0$ dão sinal errado. Este teste documenta por que a implementação deve usar $|A_e|$.

- [ ] **(MODERADO) Documentar que $\Delta<0$ é válido para o triângulo de referência em `docs/15`**:
  $\Delta = -\sqrt{2}/2 \approx -0.707$ para o triângulo P1=(0,0), P2=(1,0), P3=(0,1). O sinal negativo é correto e não indica erro. Adicionar nota na Seção 9 de `docs/12` e na Seção 13 de `docs/15` alertando que $\Delta < 0$ é possível e não invalida o elemento.

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
