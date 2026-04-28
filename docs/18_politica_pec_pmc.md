# 18. Política inicial para PEC/PMC

> **Navegação:** [Índice](README.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md)

Este documento registra a política inicial de condições essenciais usada pelo solver beta. A intenção é transformar a pendência PEC/PMC em uma regra de implementação testável, sem declarar ainda validação das Figuras 3, 5 e 7.

---

## Regra adotada

A formulação resolve um campo vetorial genérico $\phi$, que pode representar $\mathbf{E}$ ou $\mathbf{H}$.

Para uma fronteira extrudada ao longo de $z$, as direções tangenciais à parede incluem:

- a direção tangente à curva de fronteira no plano transversal;
- a direção axial $z$.

Por isso, quando a condição essencial se aplica ao próprio campo resolvido, a implementação elimina:

- os graus de liberdade de aresta sobre a physical tag da fronteira;
- os graus de liberdade nodais axiais dos nós tocados por essas arestas.

---

## Mapeamento essencial

| Campo resolvido | Condição de fronteira | DOFs eliminados nesta camada |
| --- | --- | --- |
| $\phi=\mathbf{E}$ | PEC | arestas da tag e nós tocados |
| $\phi=\mathbf{H}$ | PMC | arestas da tag e nós tocados |
| $\phi=\mathbf{E}$ | PMC | nenhum DOF essencial nesta camada |
| $\phi=\mathbf{H}$ | PEC | nenhum DOF essencial nesta camada |

Nos pares cruzados, a condição física atua naturalmente sobre o campo dual. A implementação atual não a converte em uma restrição de Dirichlet artificial para evitar impor a condição no grau de liberdade errado.

---

## API

A função:

```cpp
essential_boundary_constraints(field_kind, boundary_conditions)
```

converte uma lista de physical tags PEC/PMC em `BoundaryDofConstraints`.

Para `PEC + phi=E` e `PMC + phi=H`, a mesma lista de tags é colocada em:

```cpp
edge_physical_tags
node_physical_tags
```

O campo legado `constrain_nodes_touching_edges` continua disponível para testes manuais e compatibilidade com o mini caso anterior, mas a política física nova usa explicitamente `node_physical_tags`.

---

## Limitação preservada

Essa política fecha apenas a eliminação essencial de DOFs para o campo resolvido. Ainda falta validar, contra casos físicos do artigo, se os sinais dos blocos acoplados e a escolha de fronteiras artificiais reproduzem corretamente as curvas das Figuras 3, 5 e 7.
