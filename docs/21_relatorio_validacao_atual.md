# 21. Relatório de validação atual

> **Navegação:** [Índice](README.md) | [20](20_dossie_casos_validacao_figuras.md) | [21](21_relatorio_validacao_atual.md)

Este relatório registra o estado da validação após a primeira implementação da pipeline das Figuras 3, 5 e 7. Ele não declara reprodução científica das curvas do artigo.

---

## 1. Comandos executados

```bash
cmake -S . -B build
cmake --build build
/usr/bin/ctest --test-dir build --output-on-failure
scripts/run/run_all_validation.sh
scripts/plot/plot_validation.py
scripts/plot/compare_validation.py
```

Resultado local:

- a suíte atual passa com `/usr/bin/ctest --test-dir build --output-on-failure`;
- `15/15` testes passaram;
- Gmsh `4.12.1` gerou as três malhas de smoke;
- sete CSVs foram gerados em `data/output/validation/`;
- sete gráficos próprios foram gerados em `out/validation/`;
- `out/validation/validation_summary.csv` foi gerado com `status=no_reference`.

---

## 2. O que está validado

- A cadeia `config -> malha -> materiais por tag -> solver -> CSV` executa para os sete casos.
- O cabeçalho CSV mínimo é preservado:

```text
case_id,curve_id,x,y,x_quantity,y_quantity,mode_label,field_kind,status
```

- A Figura 3 usa o solver direto `beta -> k0/f`.
- A Figura 3 preserva o eixo em `beta_rad_per_mm` no CSV e converte internamente para `rad/m` antes do solver.
- As Figuras 5 e 7 usam o solver `k0 -> beta` e exportam $b(v)$ quando há modo com $\beta^2 > 0$.
- Pontos com $b < 0$ ou $b > 1$ agora são classificados como `outside_guided_range` e não entram nos plots/comparações.
- O comparador de validação reconhece a ausência de referência numérica versionada e não inventa erro.
- Os gráficos gerados são próprios do projeto.
- O PDF e os PNGs originais do artigo não são versionados por política de copyright.
- Os SVGs em `docs/img/` são esquemas próprios do projeto.

---

## 3. O que ainda não está validado

- Não há CSV de referência em `data/input/reference/`.
- As malhas atuais são grossas para caber no solver denso e não correspondem às contagens finais do artigo.
- As condições PEC/PMC e a seleção modal das Figuras 5 e 7 ainda precisam de ajuste físico.
- Alguns pontos de smoke das Figuras 5 e 7 não apresentam modo guiado físico ou continuam fora da faixa guiada.
- A Figura 3 ainda precisa de referência quantitativa em GHz antes de qualquer comparação.

---

## 4. Critério para declarar reprodução

Uma curva só deve ser declarada reproduzida quando houver:

- config versionado em `examples/`;
- malha gerada de forma reprodutível;
- CSV do solver;
- CSV de referência com origem documentada;
- gráfico comparativo próprio;
- erro quantitativo registrado em `out/validation/validation_summary.csv`.

Tolerâncias alvo:

- Figura 3: `max_rel <= 5%` e `mean_rel <= 2%`;
- Figuras 5 e 7: `max_abs <= 0.05` e `mean_abs <= 0.02`.
