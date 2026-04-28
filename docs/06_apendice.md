# Apêndice

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md)

As integrais necessárias para construir as matrizes elementares são calculadas da seguinte forma:

$$
\left[
\iint_e
\{U\}\{U\}^T
dxdy
\right]_{kl} =
A_e \bar{a}_k \bar{a}_l +
A_e y_c
\left(
\bar{a}_k \bar{c}_l +
\bar{c}_k \bar{a}_l \right) +
\frac{1}{12}
A_e \bar{c}_k \bar{c}_l
\left(
y_1^2 + y_2^2 + y_3^2 + 9y_c^2
\right)
$$

**Equação (A1)**

$$
\left[
\iint_e
\{V\}\{V\}^T
dxdy
\right]_{kl} =
A_e \bar{b}_k \bar{b}_l -
A_e x_c
\left(
\bar{b}_k \bar{c}_l +
\bar{c}_k \bar{b}_l \right) +
\frac{1}{12}
A_e \bar{c}_k \bar{c}_l
\left(
x_1^2 + x_2^2 + x_3^2 + 9x_c^2
\right)
$$

**Equação (A2)**

$$
\left[
\iint_e
\{U_y\}\{U_y\}^T
dxdy
\right]_{kl} =
\left[
\iint_e
\{V_x\}\{V_x\}^T
\,dx\,dy
\right]_{kl} = -
\left[
\iint_e
\{U_y\}\{V_x\}^T
dxdy
\right]_{kl} = -
\left[
\iint_e
\{V_x\}\{U_y\}^T
dxdy
\right]_{kl} = A_e \bar{c}_k \bar{c}_l
$$

**Equação (A3)**

$$
\left[
\iint_e
\{U\}\{N_x\}^T
dxdy \right]_{kl} =
\frac{1}{2}
\left(
\bar{a}_k + \bar{c}_k y_c
\right)
b_l
$$

**Equação (A4)**

$$
\left[
\iint_e
\{V\}\{N_y\}^T
dxdy
\right]_{kl} =
\frac{1}{2}
\left(
\bar{b}_k - \bar{c}_k x_c
\right)
c_l
$$

**Equação (A5)**

$$
\iint_e
\{N\}\{N\}^T
dxdy =
\frac{A_e}{12}
\begin{bmatrix}
2 & 1 & 1 \\
1 & 2 & 1 \\
1 & 1 & 2
\end{bmatrix}
$$

**Equação (A6)**

$$
\iint_e
\{N_x\}\{N_x\}^T
dxdy =
\frac{1}{4A_e}
\begin{bmatrix}
b_1^2 & b_1b_2 & b_1b_3 \\
b_1b_2 & b_2^2 & b_2b_3 \\
b_1b_3 & b_2b_3 & b_3^2
\end{bmatrix}
$$

**Equação (A7)**

$$
\iint_e
\{N_y\}\{N_y\}^T
dxdy =
\frac{1}{4A_e}
\begin{bmatrix}
c_1^2 & c_1c_2 & c_1c_3 \\
c_1c_2 & c_2^2 & c_2c_3 \\
c_1c_3 & c_2c_3 & c_3^2
\end{bmatrix}
$$

**Equação (A8)**

com:

$$
x_c = \frac{x_1 + x_2 + x_3}{3}
$$

**Equação (A9)**

$$
y_c = \frac{y_1 + y_2 + y_3}{3}
$$

**Equação (A10)**

onde $\left[\cdot\right]_{kl}$, com $kl = 11, 12, \cdots, 33$, indica o componente $(k,l)$ da matriz $\left[\cdot\right]$.

---

> **Nota editorial — Integrais complementares (não constam no artigo original):** As duas equações a seguir completam a família das integrais aresta–nó. Elas **não são necessárias** para a formulação do artigo, pois $[p]$ é sempre diagonal e $[K_{tz}]$ (Eqs. 29b e 32b) usa apenas $\{V\}\{N_y\}^T$ (A5) e $\{U\}\{N_x\}^T$ (A4). Tornam-se necessárias se o tensor $[p]$ possuir termos fora da diagonal (meios com acoplamento $xy$). As expressões são derivadas diretamente das Equações (8), (15) e (16) por integração exata, usando $\iint_e dxdy = A_e$, $\iint_e x\,dxdy = A_e x_c$ e $\iint_e y\,dxdy = A_e y_c$.

$$
\left[ \iint_e \{V\}\{N_x\}^T dxdy \right]_{kl} = \frac{1}{2} \left( \bar{b}_k - \bar{c}_k x_c \right) b_l
$$

**Equação (A11)**

$$
\left[ \iint_e \{U\}\{N_y\}^T dxdy \right]_{kl} = \frac{1}{2} \left( \bar{a}_k + \bar{c}_k y_c \right) c_l
$$

**Equação (A12)**

As quatro integrais aresta–nó seguem um padrão regular: o fator de linha depende apenas da função de forma de aresta avaliada no centróide, e o fator de coluna depende apenas da direção da derivada nodal ($x \to b_l$, $y \to c_l$):

| Integral | Fator de linha $(k)$ | Fator de coluna $(l)$ | Onde aparece |
| --- | --- | --- | --- |
| $\{U\}\{N_x\}^T$ (A4) | $\bar{a}_k + \bar{c}_k y_c$ | $b_l/2$ | $[K_{tz}]$, Eqs. (29b), (32b) |
| $\{V\}\{N_y\}^T$ (A5) | $\bar{b}_k - \bar{c}_k x_c$ | $c_l/2$ | $[K_{tz}]$, Eqs. (29b), (32b) |
| $\{V\}\{N_x\}^T$ (A11) | $\bar{b}_k - \bar{c}_k x_c$ | $b_l/2$ | extensões com $[p]$ não diagonal |
| $\{U\}\{N_y\}^T$ (A12) | $\bar{a}_k + \bar{c}_k y_c$ | $c_l/2$ | extensões com $[p]$ não diagonal |
