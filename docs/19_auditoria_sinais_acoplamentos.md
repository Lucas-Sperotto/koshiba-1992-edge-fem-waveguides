# 19. Auditoria de sinais e acoplamentos

> **Navegação:** [Índice](README.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md)

Este documento registra a primeira auditoria objetiva dos sinais dos blocos acoplados. Ele não substitui a validação física contra as Figuras 3, 5 e 7; seu papel é proteger a álgebra reduzida das Equações (31)–(35).

---

## Convenção preservada

A convenção documental do projeto continua sendo:

$$
\exp[j(\omega t-\beta z)].
$$

Com essa convenção, os fatores imaginários introduzidos na aproximação da componente axial não devem ser reaplicados na montagem real dos blocos beta.

---

## Sistema particionado auditado

A auditoria usa as Equações (31a) e (31b):

$$
K_{tt}\phi_t
-
\beta K_{tz}\phi_z
-
\beta^2 M_{tt}\phi_t
=
0,
$$

$$
-
\beta K_{zt}\phi_t
+
K_{zz}\phi_z
=
0.
$$

Da segunda equação:

$$
\phi_z
=
\beta K_{zz}^{-1}K_{zt}\phi_t.
$$

Substituindo na primeira:

$$
K_{tt}\phi_t
-
\beta^2
\left(
M_{tt}
+
K_{tz}K_{zz}^{-1}K_{zt}
\right)
\phi_t
=
0.
$$

Isso confirma o sinal positivo em:

$$
\hat{M}_{tt}
=
M_{tt}
+
K_{tz}K_{zz}^{-1}K_{zt}.
$$

---

## Teste implementado

O teste `reduced_eigenproblem` agora executa uma verificação de ciclo fechado:

1. monta matrizes pequenas `Ktt`, `Ktz`, `Kzz` e `Mtt`;
2. resolve o problema reduzido em $\beta^2$;
3. recupera $\phi_z$ por solve linear, sem formar $K_{zz}^{-1}$;
4. substitui $\phi_t$ e $\phi_z$ nas duas equações particionadas;
5. exige resíduos numéricos próximos de zero.

Esse teste protege:

- o sinal de $\phi_z = \beta K_{zz}^{-1}K_{zt}\phi_t$;
- o sinal positivo de $K_{tz}K_{zz}^{-1}K_{zt}$ em $\hat{M}_{tt}$;
- o uso de $K_{zt}=K_{tz}^T$ na redução real simétrica;
- a ausência de inversão explícita de $K_{zz}$.

---

## Limitação preservada

Essa auditoria é algébrica. Ela ainda não prova que uma geometria, uma condição PEC/PMC ou uma escolha de campo reproduz corretamente uma curva do artigo.

As próximas validações precisam comparar resultados numéricos contra casos físicos, começando por uma geometria simples antes das Figuras 3, 5 e 7.
