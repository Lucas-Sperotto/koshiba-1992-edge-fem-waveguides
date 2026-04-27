# Apêndice

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
