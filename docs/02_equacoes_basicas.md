# 2. Equações básicas

> **Navegação dos docs:** [Resumo](00_resumo.md) | [Introdução](01_introducao.md) | [Equações básicas](02_equacoes_basicas.md) | [Formulação FEM](03_formulacao_elementos_finitos.md) | [Exemplos numéricos](04_exemplos_numericos.md) | [Conclusão](05_conclusao.md) | [Apêndice](06_apendice.md) | [Referências](07_referencias.md) | [Notas editoriais](08_notas_editoriais_e_cientificas.md) | [Baseline Fase 1](09_notas_editoriais_e_cientificas.md)

Considera-se um guia de onda dielétrico com tensor de permissividade diagonal e assume-se que o campo eletromagnético no guia varia de acordo com:

$$
\exp\left[j(\omega t - \beta z)\right]
$$

em que $t$ é o tempo, $z$ é a direção de propagação, $\omega$ é a frequência angular e $\beta$ é a constante de propagação na direção $z$.

A partir das equações de Maxwell, obtém-se a seguinte equação vetorial de onda:

$$
\nabla \times \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi = 0
$$

**Equação (1)**

com:

$$
[p] =
\begin{bmatrix}
p_x & 0 & 0 \\
0 & p_y & 0 \\
0 & 0 & p_z
\end{bmatrix}
$$

**Equação (2)**

e:

$$
[q] =
\begin{bmatrix}
q_x & 0 & 0 \\
0 & q_y & 0 \\
0 & 0 & q_z
\end{bmatrix}
$$

**Equação (3)**

onde $k_0$ é o número de onda no espaço livre, $\phi$ representa $E$ ou $H$, e as componentes de $[p]$ e $[q]$ são dadas por:

$$
p_x = p_y = p_z = 1
$$

$$
q_x = \varepsilon_{rx} = n_x^2
$$

$$
q_y = \varepsilon_{ry} = n_y^2
$$

$$
q_z = \varepsilon_{rz} = n_z^2
\qquad \text{para } \phi = E
$$

**Equação (4)**

Para o caso em que $\phi = H$, tem-se:

$$
p_x = \frac{1}{\varepsilon_{rx}} = \frac{1}{n_x^2}
$$

$$
p_y = \frac{1}{\varepsilon_{ry}} = \frac{1}{n_y^2}
$$

$$
p_z = \frac{1}{\varepsilon_{rz}} = \frac{1}{n_z^2}
$$

$$
q_x = q_y = q_z = 1
\qquad \text{para } \phi = H
$$

**Equação (5)**

Aqui, $\varepsilon_{rx}$, $\varepsilon_{ry}$ e $\varepsilon_{rz}$ são as permissividades relativas nas direções $x$, $y$ e $z$, respectivamente, enquanto $n_x$, $n_y$ e $n_z$ são os índices de refração nas direções $x$, $y$ e $z$, respectivamente.

O funcional associado à Equação (1) é dado por:

$$
F =  \iint_{\Omega} \left[ \left(\nabla \times \phi\right)^* \cdot \left([p]\nabla \times \phi\right) - k_0^2 [q]\phi^*\cdot \phi \right] dx dy
$$

**Equação (6)**

onde $\Omega$ é a seção transversal do guia de onda e o asterisco indica o conjugado complexo.
