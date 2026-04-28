# 17. Implementação da Fase 7 — solver beta mínimo

> **Navegação:** [Índice](README.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md)

Este documento registra a implementação mínima da Fase 7. Ele não altera a formulação matemática do artigo: apenas explicita como os blocos geométricos já montados no código são combinados para resolver a forma reduzida das Equações (32)–(35).

---

## Escopo

A implementação inicial considera apenas materiais diagonais e meios não magnéticos, preservando o escopo do artigo. Tensores não diagonais, perdas, meios ativos, PML e integrais de contorno seguem fora do escopo.

A API aceita duas escolhas para o campo genérico $\phi$:

- `FieldKind::Electric`, com $[p]=[I]$ e $[q]=[\varepsilon_r]$;
- `FieldKind::Magnetic`, com $[p]=[\varepsilon_r]^{-1}$ e $[q]=[I]$.

---

## Composição dos blocos

Os blocos geométricos globais são combinados conforme a forma reorganizada em $\beta^2$:

$$
K_{tt}^{\beta}
=
k_0^2(q_x M_{UU} + q_y M_{VV})
-p_z K_{\mathrm{curl}},
$$

Aqui, \(K_{\mathrm{curl}}\) representa o bloco geométrico associado a
\((V_x - U_y)(V_x - U_y)^T\). Para as funções de aresta usadas no artigo,
esse termo equivale ao fator \(4\{U_y\}\{U_y\}^T\) que aparece na Equação (32a).


$$
K_{tz}^{\beta}
=
p_y K_{U N_x}
+p_x K_{V N_y},
$$

$$
K_{zz}^{\beta}
=
k_0^2 q_z M_{NN}
-p_y K_{N_x N_x}
-p_x K_{N_y N_y},
$$

$$
M_{tt}^{\beta}
=
p_y M_{UU}
+p_x M_{VV}.
$$

Esses nomes são computacionais. Eles correspondem aos blocos das Equações (32a), (32b), (32c) e (33), usando os blocos geométricos já testados contra as integrais A1–A8.

---

## Redução sem inversa explícita

A Equação (35) é implementada resolvendo sistemas lineares com $K_{zz}^{\beta}$:

$$
\hat{M}_{tt}
=
M_{tt}^{\beta}
+
K_{tz}^{\beta}
\left(K_{zz}^{\beta}\right)^{-1}
K_{zt}^{\beta}.
$$

O código não forma a inversa explicitamente. Ele resolve:

$$
K_{zz}^{\beta} X = K_{zt}^{\beta}
$$

e calcula:

$$
\hat{M}_{tt}
=
M_{tt}^{\beta}
+
K_{tz}^{\beta} X.
$$

O problema reduzido usado no mini caso é:

$$
K_{tt}^{\beta}\phi_t
=
\beta^2 \hat{M}_{tt}\phi_t.
$$

Depois de obter $\phi_t$, a componente axial pode ser recuperada pela Equação (31b):

$$
\phi_z
=
\beta
\left(K_{zz}^{\beta}\right)^{-1}
K_{zt}^{\beta}\phi_t.
$$

O código resolve esse sistema linear sem formar a inversa. A coerência dos sinais dessa recuperação está auditada em [19 — Auditoria de sinais e acoplamentos](19_auditoria_sinais_acoplamentos.md).

---

## Condições de contorno

O código possui um mecanismo genérico para restringir graus de liberdade por `physical tag` de fronteira. A política inicial está registrada em [18 — Política inicial para PEC/PMC](18_politica_pec_pmc.md).

Em resumo, `PEC + phi=E` e `PMC + phi=H` eliminam os graus de liberdade de aresta da fronteira e os graus nodais axiais tocados por essas arestas. Os pares cruzados são tratados como condições naturais nesta camada e não eliminam DOFs diretamente.

---

## Mini caso reprodutível

O mini caso versionado em `data/input/mini_case.cfg` usa uma malha sintética de dois triângulos e gera:

```text
data/output/mini_case_modes.csv
```

As colunas mínimas são:

```text
mode,beta2,beta,k0,field_kind,status
```

Esse caso valida o encadeamento computacional do solver beta. Ele não é uma reprodução de uma figura do artigo.
