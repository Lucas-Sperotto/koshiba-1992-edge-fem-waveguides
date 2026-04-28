# 2. Equações básicas

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md)

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
