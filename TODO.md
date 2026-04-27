# TODO — Koshiba & Inoue (1992) — Pendências editoriais e científicas

Este arquivo registra pendências identificadas na revisão editorial e científica dos arquivos em `docs/`.
Organizado por status: **baseline documental concluída**, **equações sem transição**, **comentários didáticos**, **imagens**, **implementação** e **validação**.

---

## Fase 1 — concluída como baseline documental

A Fase 1 documental foi fechada como baseline editorial e científica inicial. O objetivo desta fase foi corrigir problemas de renderização, registrar convenções de leitura, preservar incertezas científicas e preparar a documentação para as próximas fases sem implementar código C++.

**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md`
- `docs/04_exemplos_numericos.md`
- `docs/09_notas_editoriais_e_cientificas.md`

**Concluído nesta fase:**
- [x] **N1** — Convenção de leitura de `$\phi$` como campo vetorial registrada em `docs/09_notas_editoriais_e_cientificas.md`.
- [x] **N2** — Reutilização de `$[K_{tt}]$`, `$[K_{tz}]$` e `$[K_{zz}]$` registrada como ambiguidade do artigo, com orientação para nomes internos distintos na implementação futura.
- [x] **N3** — Uso ambíguo de `$t$` registrado como nota editorial, preservando a notação original.
- [x] **N4** — Uso de `$[N]$` antes de sua definição registrado como nota de leitura, sem alterar a ordem das equações.
- [x] **L1** — Equação (20) corrigida de `\left{ ... \right}` para `\left( ... \right)`.
- [x] **L2** — Equação (26) separada entre definição em blocos de `$[K]$` e forma integral.
- [x] **L3** — Legendas das Figuras 3, 5 e 7 corrigidas para evitar negrito Markdown quebrado.
- [x] Menu de navegação entre os arquivos Markdown de `docs/` criado no topo de cada arquivo documental.

**Pendências preservadas para fases futuras:**
- **E1–E5** permanecem abertas como pendências científicas/didáticas.
- **D1–D6** permanecem abertas como comentários didáticos a desenvolver.
- **I1–I2** permanecem abertas como pendências editoriais/legais de imagens.
- **P1–P2** permanecem abertas para antes da implementação C++.

---

## Equações sem transição explicada

### E1 — Maxwell → Equação (1): derivação omitida

A Seção 2 apresenta a equação vetorial de onda (Eq. 1) diretamente, sem mostrar como ela é obtida a partir das equações de Maxwell. O leitor não vê as substituições `$\nabla \times \mathbf{E} = -j\omega\mu_0\mathbf{H}$` e `$\nabla \times \mathbf{H} = j\omega\varepsilon_0[\varepsilon_r]\mathbf{E}$` combinadas em uma única equação.

A hipótese de meio **não magnético** (`$\mu = \mu_0$`, implícita em `$[p] = I$` para `$\phi = E$`) nunca é declarada.

**Tipo:** científica / didática
**Origem:** artigo
**Arquivos relacionados:**
- `docs/02_equacoes_basicas.md` (antes da Eq. 1)

**Critério de conclusão:**
- [ ] Adicionar comentário didático com os dois passos de substituição, ou ao menos a hipótese de meio não magnético
- [ ] Indicar explicitamente a hipótese `$\mu = \mu_0$`

---

### E2 — Equação (1) → Funcional (6): sem justificativa variacional

O funcional `$F$` da Equação (6) é apresentado sem qualquer explicação de que é o funcional de Rayleigh-Ritz associado ao operador hermitiano da Equação (1), nem de que sua estacionaridade (`$\delta F = 0$`) equivale à equação de onda.

**Tipo:** científica / didática
**Origem:** artigo
**Arquivos relacionados:**
- `docs/02_equacoes_basicas.md` (Eq. 6)

**Critério de conclusão:**
- [ ] Adicionar comentário didático explicando que `$\delta F = 0$` recupera a Eq. (1) via integração por partes, e que a simetria hermitiana garante autovalores reais `$k_0^2$`

---

### E3 — Fator `$j$` na Equação (7) sem explicação

`$\phi_z = j\{N\}^T\{\phi_z\}_e$` — o fator `$j = \sqrt{-1}$` multiplica a componente axial mas nunca é justificado no texto. Ele surge da convenção de fase `$e^{j(\omega t - \beta z)}$` e da necessidade de compatibilidade com as componentes transversais após a derivação em `$z$`, mas isso não é dito.

**Tipo:** científica / didática
**Origem:** artigo
**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md` (Eq. 7)

**Critério de conclusão:**
- [ ] Adicionar nota explicando a origem do fator `$j$` em relação à convenção de fase e à estrutura do operador rotacional

---

### E4 — Coeficientes `$\bar{a}_k$`, `$\bar{b}_k$`, `$\bar{c}_k$` (Eqs. 17–19) sem derivação ou contexto geométrico

Os coeficientes das funções de forma de aresta aparecem como resultado de um sistema não apresentado. O leitor não vê:
- qual sistema de equações foi imposto (três condições de continuidade tangencial, uma por aresta);
- o que `$\theta_{k+3}$` representa geometricamente (ângulo da aresta `$k+3$` com o eixo `$x$`);
- a relação entre esses coeficientes e os `$a_k, b_k, c_k$` das funções de forma nodais.

A Figura 1 não mostra os ângulos `$\theta_{k+3}$`, o que tornaria a Equação (20) autoexplicativa.

**Tipo:** científica / didática
**Origem:** artigo
**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md` (Eqs. 15–21, Figura 1)

**Critério de conclusão:**
- [ ] Adicionar comentário didático descrevendo o sistema linear que determina `$\bar{a}_k, \bar{b}_k, \bar{c}_k$`
- [ ] Indicar que `$\theta_{k+3}$` é o ângulo da aresta com o eixo horizontal
- [ ] Avaliar se a Figura 1 precisa ser complementada com anotação dos ângulos (ou adicionar figura auxiliar)

---

### E5 — Eliminação de `$\{\phi_z\}$` (Eq. 31b → Eq. 34): invertibilidade de `$[K_{zz}]$` não discutida

A passagem da Eq. (31b) para a Eq. (35) requer que `$[K_{zz}]$` seja invertível. Nenhuma condição sobre isso é mencionada. Na prática, `$[K_{zz}]$` pode ser singular em casos especiais (e.g., em ou próximo da frequência de corte de modos axiais).

**Tipo:** científica
**Origem:** artigo
**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md` (Eqs. 31b, 34, 35)

**Critério de conclusão:**
- [ ] Adicionar nota sobre as condições em que `$[K_{zz}]$` é regular
- [ ] Registrar o custo computacional da inversão e a perda de esparsidade como trade-off documentado (o texto menciona isso, mas merece mais destaque)

---

## Comentários didáticos faltantes

### D1 — Dualidade `$\phi = E$` vs. `$\phi = H$`: motivação não explicada

O artigo apresenta as duas formulações em paralelo mas não explica por que as duas existem, nem qual é preferida em prática. A formulação em `$\mathbf{H}$` é mais comum na literatura (refs. [1]–[10]) e a formulação em `$\mathbf{E}$` tem restrições de continuidade diferentes. Isso não é discutido.

**Tipo:** didática
**Arquivos relacionados:**
- `docs/02_equacoes_basicas.md` (Eqs. 4–5)

**Critério de conclusão:**
- [ ] Adicionar nota comparando as duas formulações: grau de liberdade dos parâmetros nodais, compatibilidade com condições de contorno elétrico/magnético

---

### D2 — Por que elementos de aresta eliminam espúrios: nunca explicado na Seção 3

A Seção 3 implementa os elementos de aresta mas não explica o mecanismo pelo qual eles eliminam soluções espúrias. A explicação (espaço de funções de forma que satisfaz automaticamente `$\nabla \cdot \mathbf{B} = 0$` no sentido fraco) é central para a contribuição do artigo, mas fica apenas implícita.

**Tipo:** didática / científica
**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md` (introdução da Seção 3)
- `docs/01_introducao.md`

**Critério de conclusão:**
- [ ] Adicionar parágrafo de comentário didático (fora do texto original) conectando a continuidade tangencial dos elementos de aresta à ausência de espúrios

---

### D3 — Apêndice, Equação (A3): identidade não óbvia sem derivação

A Equação (A3) mostra que quatro integrais distintas são todas iguais a `$A_e \bar{c}_k \bar{c}_l$`:

$$
\left[\iint_e \{U_y\}\{U_y\}^T\right]_{kl} =
\left[\iint_e \{V_x\}\{V_x\}^T\right]_{kl} =
-\left[\iint_e \{U_y\}\{V_x\}^T\right]_{kl} =
-\left[\iint_e \{V_x\}\{U_y\}^T\right]_{kl} =
A_e \bar{c}_k \bar{c}_l
$$

Esta identidade tem consequência direta na simplificação do termo `$4p_z\{U_y\}\{U_y\}^T$` em Eq. (29a), mas a ligação não é traçada explicitamente.

**Tipo:** didática / científica
**Arquivos relacionados:**
- `docs/06_apendice.md` (Eq. A3)
- `docs/03_formulacao_elementos_finitos.md` (Eq. 29a)

**Critério de conclusão:**
- [ ] Adicionar derivação esquemática de (A3) a partir das definições de `$\{U\}$ e `$\{V\}$`
- [ ] Traçar a ligação entre (A3) e o coeficiente 4 em (29a)

---

### D4 — Apêndice: integrais cruzadas `$\{V\}\{N_x\}^T$` e `$\{U\}\{N_y\}^T$` ausentes

As Eqs. (A4) e (A5) fornecem `$\{U\}\{N_x\}^T$` e `$\{V\}\{N_y\}^T$`, mas **não** as integrais `$\{V\}\{N_x\}^T$` e `$\{U\}\{N_y\}^T$`. Estas aparecem em `$[K_{tz}]$` (Eq. 29b / 32b) e serão necessárias na implementação.

**Tipo:** científica / técnica
**Arquivos relacionados:**
- `docs/06_apendice.md`
- `docs/03_formulacao_elementos_finitos.md` (Eqs. 29b, 32b)

**Critério de conclusão:**
- [ ] Verificar no artigo original se as integrais cruzadas estão presentes
- [ ] Se ausentes, derivá-las e registrá-las como complemento ao apêndice com nota editorial

---

### D5 — Definição de `$v$` e `$b$` (Eqs. 36–37): faixa física não explicada

As equações (36–37) definem frequência normalizada `$v$` e constante de propagação normalizada `$b$` sem:
- indicar a origem da normalização (Goell, 1969 — ref. [25]);
- explicitar o intervalo físico `$0 \leq b \leq 1$` para modos guiados;
- explicar o significado dos limites `$b \to 0$` (corte) e `$b \to 1$` (confinamento total).

**Tipo:** didática
**Arquivos relacionados:**
- `docs/04_exemplos_numericos.md` (Eqs. 36–37)

**Critério de conclusão:**
- [ ] Adicionar nota didática sobre o intervalo de `$b$` e o significado físico dos extremos

---

### D6 — Conclusão cita referências [14] e [15] sem contexto no corpo do texto

A Seção 5 menciona que a abordagem pode ser estendida a "guias de onda anisotrópicos com simetria de reflexão [14], [15]", mas [14] e [15] não são discutidas no corpo do artigo. Para o leitor, a citação surge sem contexto.

**Tipo:** editorial
**Arquivos relacionados:**
- `docs/05_conclusao.md`

**Critério de conclusão:**
- [ ] Adicionar nota editorial identificando [14] (Chew & Nasir, 1989) e [15] (Fernandez & Lu, 1990) e descrevendo brevemente o tipo de extensão sugerida

---

## Imagens

Todas as 13 imagens referenciadas nos docs existem em `docs/img/`. Nenhuma imagem está faltando.

Pendências relacionadas a imagens são de **conteúdo**, não de arquivo:

### I1 — Figura 1 não mostra ângulos `$\theta_{k+3}$`

A Figura 1 (elemento triangular de aresta) não exibe os ângulos `$\theta_{k+3}$` das arestas, que são centrais na definição dos coeficientes das Eqs. (17–20).

**Tipo:** editorial / didática
**Arquivos relacionados:**
- `docs/03_formulacao_elementos_finitos.md` (Figura 1, Eq. 20)

**Critério de conclusão:**
- [ ] Complementar ou substituir `img/fig_01.png` por versão anotada com os ângulos `$\theta_4$`, `$\theta_5$`, `$\theta_6$`

---

### I2 — Fotos dos autores (`koshiba.png`, `inoue.png`) precisam de verificação de direitos

As fotos dos autores são exibidas no arquivo `00_resumo.md`. Devem ter origem verificada quanto a direitos de uso.

**Tipo:** editorial / legal
**Arquivos relacionados:**
- `docs/00_resumo.md` (linhas 41, 53)

**Critério de conclusão:**
- [ ] Confirmar origem e licença das fotos ou substituir por texto descritivo

---

## Implementação (pendências derivadas da revisão)

### P1 — Derivar e documentar as integrais cruzadas do apêndice antes de implementar `$[K_{tz}]$`

Ver pendência **D4**. A montagem de `$[K_{tz}]$` requer integrais não documentadas no artigo.

**Tipo:** técnica
**Critério de conclusão:**
- [ ] Equações derivadas e registradas em `docs/06_apendice.md`
- [ ] Implementação de `$[K_{tz}]$` verificada contra resultado analítico para elemento unitário

---

### P2 — Documentar tratamento de `$[K_{zz}]$` singular ou mal condicionado

Ver pendência **E5**. A inversão de `$[K_{zz}]$` deve ser implementada com tratamento de casos singulares.

**Tipo:** técnica
**Critério de conclusão:**
- [ ] Estratégia de inversão definida (fatoração LU, pseudo-inversa, ou regularização)
- [ ] Teste com guia próximo à frequência de corte

---

## Concluído

Ver a seção **Fase 1 — concluída como baseline documental**, que registra os itens N1–N4 e L1–L3 como concluídos nesta etapa.
