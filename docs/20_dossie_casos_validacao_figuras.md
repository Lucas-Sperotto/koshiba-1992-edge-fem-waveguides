# 20. Dossiê dos casos de validação das Figuras 3, 5 e 7

> **Navegação:** [Índice](README.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md)

Este documento define o contrato documental dos casos de validação das Figuras 3, 5 e 7 do artigo. Ele prepara a próxima fase de implementação, mas não declara nenhuma curva como reproduzida.

---

## 1. Convenções gerais

### 1.1 Escopo

Este dossiê cobre:

- Figura 3(a): microstrip isotrópica;
- Figura 3(b): microstrip anisotrópica;
- Figura 5(a): guia retangular dielétrico, modos $E^x_{11}$ e $E^x_{21}$;
- Figura 5(b): guia retangular dielétrico, modos $E^y_{11}$ e $E^y_{21}$;
- Figura 5(c): guia retangular dielétrico, modos $E^x_{11}$ e $E^y_{11}$;
- Figura 7(a): guia com núcleo triangular equilátero, modo $E^y_{11}$;
- Figura 7(b): guia com núcleo triangular equilátero, modo $E^y_{11}$.

Não fazem parte deste documento:

- geração final das malhas;
- execução do solver;
- extração de modos;
- comparação quantitativa;
- digitalização das imagens do artigo.

### 1.2 Identificadores estáveis

Os casos devem usar identificadores estáveis em entradas, saídas e scripts:

| Figura | `case_id` | Descrição |
| --- | --- | --- |
| 3(a) | `fig3a_microstrip_isotropic` | Microstrip com substrato isotrópico |
| 3(b) | `fig3b_microstrip_anisotropic` | Microstrip com substrato anisotrópico |
| 5(a) | `fig5a_rect_ex_low_contrast` | Guia retangular, modos $E^x$, $n_1=1.05$ |
| 5(b) | `fig5b_rect_ey_low_contrast` | Guia retangular, modos $E^y$, $n_1=1.05$ |
| 5(c) | `fig5c_rect_ex_ey_high_contrast` | Guia retangular, modos $E^x_{11}$ e $E^y_{11}$, $n_1=1.5$ |
| 7(a) | `fig7a_triangular_low_contrast` | Núcleo triangular, $n_1=1.5085$, $n_2=1.50$ |
| 7(b) | `fig7b_triangular_high_contrast` | Núcleo triangular, $n_1=1.5$, $n_2=1.0$ |

### 1.3 Política de escala `t`

Para os casos normalizados das Figuras 5 e 7:

```text
t_default = 1.0
```

O valor de `t` deve ser configurável nas entradas futuras. O teste de invariância esperado é:

1. executar o mesmo caso com `t=1`;
2. executar o mesmo caso com `t=2`;
3. comparar as curvas normalizadas $b(v)$;
4. exigir que as curvas coincidam dentro da tolerância numérica definida para a malha e o solver.

Como $v$ e $b$ são normalizados, alterar `t` deve reescalar a geometria e o número de onda físico, mas não deve alterar a curva $b(v)$.

---

## 2. Eixos e grades

| Figura | Eixo x | Grade x | Eixo y | Grade y |
| --- | --- | --- | --- | --- |
| 3(a,b) | $\beta$ de 0 a 2 rad/m | 0.1 rad/m | frequência de 0 a 50 GHz | 5 GHz |
| 5(a,b,c) | $v$ de 0 a 1.5 | 0.1 | $b$ de 0 a 1 | 0.1 |
| 7(a,b) | $v$ de 0 a 5 | 0.5 | $b$ de 0 a 1 | 0.1 |

> **Nota de controle:** a unidade `rad/m` para o eixo $\beta$ da Figura 3 deve ser preservada conforme a especificação atual do projeto. Se a comparação física futura indicar incompatibilidade de escala, a correção deve ser registrada como nova auditoria, não ajustada silenciosamente.

---

## 3. Figura 3 — microstrip

### 3.1 Geometria comum

Parâmetros extraídos de `docs/04_exemplos_numericos.md`:

| Parâmetro | Valor |
| --- | --- |
| `W` | 1.27 mm |
| `strip_thickness` | 0 |
| `h` | 1.27 mm |
| `X` | 12.7 mm |
| `Y` | 12.7 mm |
| Simetria usada no artigo | metade da seção transversal |
| Contagem do artigo | `NE=364`, `NC=210`, `NS=573` |

### 3.2 Casos

| Campo | Fig. 3(a) | Fig. 3(b) |
| --- | --- | --- |
| `case_id` | `fig3a_microstrip_isotropic` | `fig3b_microstrip_anisotropic` |
| Material do substrato | $\varepsilon_r=8.875$ | $\varepsilon_{rx}=\varepsilon_{rz}=9.4$, $\varepsilon_{ry}=11.6$ |
| Material externo | ar, $\varepsilon_r=1$ | ar, $\varepsilon_r=1$ |
| Campo preferencial inicial | `phi=E` | `phi=E` |
| Fronteiras esperadas | condutores e planos artificiais por physical tags | condutores e planos artificiais por physical tags |
| PEC/PMC | PEC para condutores; fronteiras artificiais a validar | PEC para condutores; fronteiras artificiais a validar |
| Variável varrida | $\beta$ | $\beta$ |
| Modos esperados | dois primeiros modos | dois primeiros modos |
| Saída futura | `data/output/validation/fig3a_microstrip_isotropic.csv` | `data/output/validation/fig3b_microstrip_anisotropic.csv` |

### 3.3 Pendências da Figura 3

- Definir a geometria operacional da metade da seção transversal.
- Definir physical tags para fita condutora, plano de terra, simetria e fronteiras artificiais.
- Definir se a varredura será feita em $\beta$ resolvendo frequência ou se o solver atual em $\beta^2$ deve receber uma camada alternativa.
- Confirmar a escala prática do eixo $\beta$ antes da comparação quantitativa.
- Criar malha Gmsh MSH 4.1 ASCII com contagens próximas às do artigo ou registrar desvio.

---

## 4. Figura 5 — guia retangular dielétrico

### 4.1 Geometria comum

Parâmetros extraídos de `docs/04_exemplos_numericos.md`:

| Parâmetro | Valor |
| --- | --- |
| `W` | $2t$ |
| `X` | $10t$ |
| `Y` | $5t$ |
| `t_default` | 1.0 |
| Simetria usada no artigo | um quarto da seção transversal |
| Contagem do artigo | `NE=320`, `NC=187`, `NS=506` |

Normalizações:

$$
v =
\frac{k_0 t \sqrt{n_1^2-n_2^2}}{\pi}
$$

$$
b =
\frac{
\left(\frac{\beta}{k_0}\right)^2-n_2^2
}{
n_1^2-n_2^2
}
$$

### 4.2 Casos

| Campo | Fig. 5(a) | Fig. 5(b) | Fig. 5(c) |
| --- | --- | --- | --- |
| `case_id` | `fig5a_rect_ex_low_contrast` | `fig5b_rect_ey_low_contrast` | `fig5c_rect_ex_ey_high_contrast` |
| Geometria | retangular dielétrica | retangular dielétrica | retangular dielétrica |
| Índices | $n_1=1.05$, $n_2=1.0$ | $n_1=1.05$, $n_2=1.0$ | $n_1=1.5$, $n_2=1.0$ |
| Modos | $E^x_{11}$, $E^x_{21}$ | $E^y_{11}$, $E^y_{21}$ | $E^x_{11}$, $E^y_{11}$ |
| Campo preferencial inicial | `phi=E` | `phi=E` | `phi=E` |
| Fronteiras esperadas | simetria e fronteiras artificiais | simetria e fronteiras artificiais | simetria e fronteiras artificiais |
| PEC/PMC | a validar por paridade modal | a validar por paridade modal | a validar por paridade modal |
| Variável varrida | $v$ | $v$ | $v$ |
| Saída futura | `data/output/validation/fig5a_rect_ex_low_contrast.csv` | `data/output/validation/fig5b_rect_ey_low_contrast.csv` | `data/output/validation/fig5c_rect_ex_ey_high_contrast.csv` |

### 4.3 Pendências da Figura 5

- Definir tags de simetria que selecionem modos $E^x$ e $E^y$.
- Definir fronteiras artificiais PEC/PMC sem restringir a componente dominante do campo.
- Criar malha de um quarto da seção transversal.
- Implementar conversão entre pontos de varredura em $v$ e valores físicos de $k_0$.
- Implementar cálculo de $b$ a partir de $\beta$, $k_0$, $n_1$ e $n_2$.
- Executar teste de invariância de escala com `t=1` e `t=2`.

---

## 5. Figura 7 — guia com núcleo triangular equilátero

### 5.1 Geometria comum

Parâmetros extraídos de `docs/04_exemplos_numericos.md`:

| Parâmetro | Valor |
| --- | --- |
| Núcleo | triangular equilátero |
| `X` | $6t$ |
| `Y` | $5t$ |
| `t_default` | 1.0 |
| Simetria usada no artigo | metade da seção transversal |
| Contagem do artigo | `NE=360`, `NC=208`, `NS=567` |

As mesmas definições normalizadas das Equações (36) e (37) devem ser usadas para $v$ e $b$, com os índices de cada subfigura.

### 5.2 Casos

| Campo | Fig. 7(a) | Fig. 7(b) |
| --- | --- | --- |
| `case_id` | `fig7a_triangular_low_contrast` | `fig7b_triangular_high_contrast` |
| Geometria | núcleo triangular equilátero | núcleo triangular equilátero |
| Índices | $n_1=1.5085$, $n_2=1.50$ | $n_1=1.5$, $n_2=1.0$ |
| Modo | $E^y_{11}$ | $E^y_{11}$ |
| Campo preferencial inicial | `phi=E` | `phi=E` |
| Fronteiras esperadas | simetria e fronteiras artificiais | simetria e fronteiras artificiais |
| PEC/PMC | a validar por paridade modal | a validar por paridade modal |
| Variável varrida | $v$ | $v$ |
| Saída futura | `data/output/validation/fig7a_triangular_low_contrast.csv` | `data/output/validation/fig7b_triangular_high_contrast.csv` |

### 5.3 Pendências da Figura 7

- Definir a orientação do triângulo equilátero dentro da janela $X \times Y$.
- Definir malha de metade da seção transversal.
- Definir tags de simetria para o modo $E^y_{11}$.
- Validar que a formulação por elementos de aresta não introduz soluções espúrias na seleção modal.
- Executar teste de invariância de escala com `t=1` e `t=2`.

---

## 6. Formato de saídas futuras

Os CSVs gerados pelo solver para validação devem conter, no mínimo:

```text
case_id,curve_id,x,y,x_quantity,y_quantity,mode_label,field_kind,status
```

Para a Figura 3:

- `x_quantity=beta_rad_per_m`;
- `y_quantity=frequency_ghz`.

Para as Figuras 5 e 7:

- `x_quantity=v_number`;
- `y_quantity=b_number`.

As curvas de referência externas devem usar o formato mínimo:

```text
case_id,curve_id,x,y,method,units
```

Onde `method` deve identificar a origem, por exemplo:

- `marcatili`;
- `goell`;
- `point_matching`;
- `external_validated`.

Os caminhos dos repositórios externos de Marcatili e Goell/Goel não são fixados neste dossiê. A integração futura deve aceitar CSVs exportados desses repositórios, mantendo este repositório independente.

---

## 7. Comparação e erro

### 7.1 Estratégia inicial

A primeira comparação após a geração dos gráficos será visual:

- conferir eixos e grades;
- conferir monotonicidade e faixa física;
- conferir separação modal;
- conferir ausência de modos espúrios óbvios.

### 7.2 Estratégia quantitativa

Quando houver CSVs de referência externos, o erro deve ser calculado por curva:

- interpolar a curva de referência nos pontos `x` do solver;
- calcular erro absoluto em `y`;
- calcular erro relativo quando `|y_ref|` for suficientemente maior que zero;
- registrar erro máximo, erro médio e número de pontos comparados.

Para as Figuras 5 e 7, a comparação preferencial é em $b(v)$. Para a Figura 3, a comparação preferencial é frequência em função de $\beta$.

### 7.3 Digitalização das figuras

Digitalizar as imagens do artigo não é parte desta etapa. A digitalização só deve ser considerada se:

- as referências externas de Marcatili/Goell não cobrirem o modo necessário;
- a inspeção visual indicar discrepância que exija quantificação contra a imagem;
- a origem dos pontos digitalizados for documentada.

---

## 8. Critério de avanço para implementação

A próxima etapa de código só deve começar quando houver, para cada caso:

- geometria operacional definida;
- physical tags planejadas;
- escolha inicial de `phi=E/H`;
- política PEC/PMC por tag;
- conversão de varredura para `k0` ou $\beta$;
- nome de CSV de saída;
- modo ou curvas a extrair.

Este dossiê fecha apenas o contrato documental inicial. Malhas, scripts e validação quantitativa continuam pendentes.
