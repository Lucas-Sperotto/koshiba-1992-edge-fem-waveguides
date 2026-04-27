# 14. Integrais cruzadas e termos ausentes do Apêndice

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md)

> **Documento complementar da Fase 2.**
> Este arquivo documenta integrais cruzadas, termos obtidos por simetria, termos derivados a partir das funções de forma e termos ainda pendentes para a futura implementação das matrizes locais da formulação de Koshiba e Inoue (1992).
> O objetivo é evitar que uma integral necessária seja implementada como se fosse explicitamente fornecida pelo artigo quando, na verdade, ela é uma consequência algébrica ou uma extensão derivada neste projeto.

---

## 1. Contexto

O Apêndice do artigo apresenta as integrais necessárias para construir as matrizes elementares da formulação com tensores diagonais $[p]$ e $[q]$. Essas integrais aparecem nas matrizes das Equações (29a)–(30b) e, depois da reorganização algébrica em termos de $\beta^2$, nas Equações (32a)–(33).

A formulação usa:

- funções nodais lineares $\{N\}$ para a componente axial $\phi_z$;
- funções de aresta $\{U\}$ e $\{V\}$ para as componentes transversais $\phi_x$ e $\phi_y$;
- derivadas $\{N_x\}$, $\{N_y\}$, $\{U_y\}$ e $\{V_x\}$;
- matrizes constitutivas diagonais $[p]$ e $[q]$.

A pergunta central deste documento é:

**o Apêndice é suficiente para a implementação do artigo, ou existem termos que precisam ser obtidos por simetria, derivados separadamente ou deixados pendentes?**

Para responder, os termos serão separados em quatro grupos:

1. **Termos presentes no artigo** — aparecem explicitamente nas Equações (A1)–(A10).
2. **Termos obtidos por simetria** — não aparecem como equações independentes, mas decorrem de transposição, igualdade ou cadeia algébrica apresentada no artigo.
3. **Termos derivados a partir das funções de forma** — não são dados explicitamente no artigo, mas podem ser obtidos por integração exata a partir das definições de $\{N\}$, $\{U\}$ e $\{V\}$.
4. **Termos ainda pendentes** — exigem conferência no artigo original, validação numérica independente ou derivação futura.

> **Nota de verificação:** este documento não altera a formulação original. Ele organiza o que é necessário para a implementação e separa claramente o que vem do artigo daquilo que é inferido, transposto ou derivado neste projeto.

---

## 2. Termos presentes no artigo

As integrais explicitamente fornecidas no Apêndice cobrem os seguintes blocos básicos:

| Integral | Equação do Apêndice | Papel principal |
| --- | --- | --- |
| $\displaystyle \iint_e \{U\}\{U\}^T\,dxdy$ | (A1) | Blocos transversais envolvendo $\phi_x$ |
| $\displaystyle \iint_e \{V\}\{V\}^T\,dxdy$ | (A2) | Blocos transversais envolvendo $\phi_y$ |
| $\displaystyle \iint_e \{U_y\}\{U_y\}^T\,dxdy$ | (A3) | Termo derivativo transversal associado ao rotacional |
| $\displaystyle \iint_e \{V_x\}\{V_x\}^T\,dxdy$ | (A3) | Termo derivativo transversal associado ao rotacional |
| $\displaystyle \iint_e \{U_y\}\{V_x\}^T\,dxdy$ | (A3) | Termo cruzado derivativo transversal |
| $\displaystyle \iint_e \{V_x\}\{U_y\}^T\,dxdy$ | (A3) | Termo cruzado derivativo transversal |
| $\displaystyle \iint_e \{U\}\{N_x\}^T\,dxdy$ | (A4) | Acoplamento aresta–nó no bloco $[K_{tz}]$ |
| $\displaystyle \iint_e \{V\}\{N_y\}^T\,dxdy$ | (A5) | Acoplamento aresta–nó no bloco $[K_{tz}]$ |
| $\displaystyle \iint_e \{N\}\{N\}^T\,dxdy$ | (A6) | Bloco axial de massa, associado a $\phi_z$ |
| $\displaystyle \iint_e \{N_x\}\{N_x\}^T\,dxdy$ | (A7) | Termo axial derivativo em $x$ |
| $\displaystyle \iint_e \{N_y\}\{N_y\}^T\,dxdy$ | (A8) | Termo axial derivativo em $y$ |
| $x_c$ | (A9) | Coordenada $x$ do centróide |
| $y_c$ | (A10) | Coordenada $y$ do centróide |

As Equações (A9) e (A10) não são integrais de matriz, mas são fundamentais para avaliar as integrais que dependem do centróide.

---

## 3. Integrais exigidas diretamente pelas matrizes do artigo

A partir das Equações (29a)–(30b) e (32a)–(33), as integrais efetivamente necessárias para reproduzir a formulação diagonal do artigo são:

| Integral | Aparece em | Coeficiente associado |
| --- | --- | --- |
| $\{U\}\{U\}^T$ | (29a), (30a), (32a), (33) | $p_y\beta^2$, $q_x$, $q_xk_0^2$, $p_y$ |
| $\{V\}\{V\}^T$ | (29a), (30a), (32a), (33) | $p_x\beta^2$, $q_y$, $q_yk_0^2$, $p_x$ |
| $\{U_y\}\{U_y\}^T$ | (29a), (32a) | $4p_z$, $-4p_z$ |
| $\{U\}\{N_x\}^T$ | (29b), (32b) | $p_y\beta$, $p_y$ |
| $\{V\}\{N_y\}^T$ | (29b), (32b) | $p_x\beta$, $p_x$ |
| $\{N_x\}\{N_x\}^T$ | (29c), (32c) | $p_y$, $-p_y$ |
| $\{N_y\}\{N_y\}^T$ | (29c), (32c) | $p_x$, $-p_x$ |
| $\{N\}\{N\}^T$ | (30b), (32c) | $q_z$, $q_zk_0^2$ |

Portanto, para a formulação do artigo com $[p]$ e $[q]$ diagonais, o Apêndice é suficiente.

> **Nota de verificação:** essa conclusão vale para a formulação transcrita, na qual $[p]$ e $[q]$ são diagonais. Extensões para tensores cheios, meios bianisotrópicos, PML, condições absorventes ou termos de fronteira podem exigir integrais adicionais.

---

## 4. Termos obtidos por simetria

Alguns termos não precisam ser implementados como integrais independentes, pois podem ser obtidos por transposição ou por relações explícitas no próprio Apêndice.

### 4.1 Cadeia de igualdades da Equação (A3)

A Equação (A3) informa que:

$$
\left[
\iint_e
\{U_y\}\{U_y\}^T
dxdy
\right]_{kl}
=
\left[
\iint_e
\{V_x\}\{V_x\}^T
dxdy
\right]_{kl}
$$

$$
=
-
\left[
\iint_e
\{U_y\}\{V_x\}^T
dxdy
\right]_{kl}
=
-
\left[
\iint_e
\{V_x\}\{U_y\}^T
dxdy
\right]_{kl}
=
A_e\bar{c}_k\bar{c}_l.
$$

Logo:

$$
\left[
\iint_e
\{V_x\}\{V_x\}^T
dxdy
\right]_{kl}
=
A_e\bar{c}_k\bar{c}_l,
$$

mas:

$$
\left[
\iint_e
\{U_y\}\{V_x\}^T
dxdy
\right]_{kl}
=
-
A_e\bar{c}_k\bar{c}_l,
$$

e:

$$
\left[
\iint_e
\{V_x\}\{U_y\}^T
dxdy
\right]_{kl}
=
-
A_e\bar{c}_k\bar{c}_l.
$$

Essas igualdades são importantes porque explicam o fator $4$ que aparece nos termos com $\{U_y\}\{U_y\}^T$ nas matrizes do artigo.

De fato, a parcela longitudinal do rotacional transversal envolve a combinação:

$$
-\{U_y\}+\{V_x\}.
$$

Ao formar o produto:

$$
(-\{U_y\}+\{V_x\})
(-\{U_y\}+\{V_x\})^T,
$$

obtém-se:

$$
\{U_y\}\{U_y\}^T
-
\{U_y\}\{V_x\}^T
-
\{V_x\}\{U_y\}^T
+
\{V_x\}\{V_x\}^T.
$$

Usando a Equação (A3):

$$
A_e\bar{c}_k\bar{c}_l
+
A_e\bar{c}_k\bar{c}_l
+
A_e\bar{c}_k\bar{c}_l
+
A_e\bar{c}_k\bar{c}_l
=
4A_e\bar{c}_k\bar{c}_l.
$$

Assim, o fator $4$ não vem de uma nova integral, mas da combinação algébrica das quatro parcelas da Equação (A3).

---

### 4.2 Transpostas associadas a $[K_{zt}]$

O artigo informa que:

$$
[K_{tz}] = [K_{zt}]^T.
$$

Portanto, se o bloco $[K_{tz}]$ usa:

$$
\iint_e \{U\}\{N_x\}^T\,dxdy
$$

e:

$$
\iint_e \{V\}\{N_y\}^T\,dxdy,
$$

então o bloco $[K_{zt}]$ pode ser obtido pelas transpostas:

$$
\iint_e \{N_x\}\{U\}^T\,dxdy
=
\left(
\iint_e \{U\}\{N_x\}^T\,dxdy
\right)^T,
$$

e:

$$
\iint_e \{N_y\}\{V\}^T\,dxdy
=
\left(
\iint_e \{V\}\{N_y\}^T\,dxdy
\right)^T.
$$

Esses termos não devem ser tratados como novas fórmulas independentes, mas como transpostas das Equações (A4) e (A5).

> **Nota de verificação:** a transposição deve respeitar a ordem dos graus de liberdade locais. Um erro comum é montar a matriz aresta–nó diretamente no bloco nó–aresta sem transpor as dimensões.

---

## 5. Termos derivados a partir das funções de forma

Os termos desta seção **não devem ser apresentados como fórmulas explícitas do artigo**. Eles são derivados a partir das definições das funções de forma e só devem ser usados em extensões, testes independentes ou auditorias de consistência.

---

### 5.1 Integral $\{V\}\{N_x\}^T$

Esta integral não aparece na formulação diagonal do artigo. Ela pode surgir se uma extensão futura usar matriz $[p]$ com termos fora da diagonal, por exemplo acoplamento entre direções $x$ e $y$.

Pelas funções de forma do artigo:

$$
V_k(x) = \bar{b}_k - \bar{c}_k x.
$$

Além disso:

$$
N_l(x,y) = \frac{a_l+b_lx+c_ly}{2A_e}.
$$

Logo:

$$
\frac{\partial N_l}{\partial x}
=
\frac{b_l}{2A_e}.
$$

Então:

$$
\left[
\iint_e
\{V\}\{N_x\}^T
dxdy
\right]_{kl}
=
\iint_e
V_k
\frac{\partial N_l}{\partial x}
dxdy.
$$

Substituindo:

$$
=
\iint_e
(\bar{b}_k-\bar{c}_kx)
\frac{b_l}{2A_e}
dxdy.
$$

Como:

$$
\iint_e dxdy = A_e,
$$

e:

$$
\iint_e x\,dxdy = A_ex_c,
$$

tem-se:

$$
=
\frac{b_l}{2A_e}
\left(
\bar{b}_k A_e
-
\bar{c}_k A_ex_c
\right).
$$

Portanto:

$$
\left[
\iint_e
\{V\}\{N_x\}^T
dxdy
\right]_{kl}
=
\frac{1}{2}
(\bar{b}_k-\bar{c}_kx_c)b_l.
$$

> **Nota de verificação:** esta fórmula é derivada neste projeto. Ela não aparece explicitamente no artigo e não é necessária para a formulação diagonal usada por Koshiba e Inoue.

---

### 5.2 Integral $\{U\}\{N_y\}^T$

De modo análogo, com:

$$
U_k(y)=\bar{a}_k+\bar{c}_ky,
$$

e:

$$
\frac{\partial N_l}{\partial y}
=
\frac{c_l}{2A_e},
$$

tem-se:

$$
\left[
\iint_e
\{U\}\{N_y\}^T
dxdy
\right]_{kl}
=
\iint_e
U_k
\frac{\partial N_l}{\partial y}
dxdy.
$$

Substituindo:

$$
=
\iint_e
(\bar{a}_k+\bar{c}_ky)
\frac{c_l}{2A_e}
dxdy.
$$

Usando:

$$
\iint_e dxdy = A_e,
$$

e:

$$
\iint_e y\,dxdy = A_ey_c,
$$

segue:

$$
=
\frac{c_l}{2A_e}
\left(
\bar{a}_kA_e+\bar{c}_kA_ey_c
\right).
$$

Assim:

$$
\left[
\iint_e
\{U\}\{N_y\}^T
dxdy
\right]_{kl}
=
\frac{1}{2}
(\bar{a}_k+\bar{c}_ky_c)c_l.
$$

> **Nota de verificação:** esta integral também é derivada neste projeto e deve permanecer separada das integrais explicitamente fornecidas no Apêndice original.

---

### 5.3 Integral $\{N_x\}\{N_y\}^T$

As derivadas nodais são constantes dentro do triângulo:

$$
\frac{\partial N_k}{\partial x}
=
\frac{b_k}{2A_e},
$$

e:

$$
\frac{\partial N_l}{\partial y}
=
\frac{c_l}{2A_e}.
$$

Então:

$$
\left[
\iint_e
\{N_x\}\{N_y\}^T
dxdy
\right]_{kl}
=
\iint_e
\frac{b_k}{2A_e}
\frac{c_l}{2A_e}
dxdy.
$$

Como:

$$
\iint_e dxdy=A_e,
$$

resulta:

$$
\left[
\iint_e
\{N_x\}\{N_y\}^T
dxdy
\right]_{kl}
=
\frac{b_kc_l}{4A_e}.
$$

A integral transposta é:

$$
\left[
\iint_e
\{N_y\}\{N_x\}^T
dxdy
\right]_{kl}
=
\frac{c_kb_l}{4A_e}.
$$

> **Nota de verificação:** esses termos não aparecem na formulação diagonal do artigo. Eles seriam necessários apenas em extensões com operadores contendo acoplamento entre derivadas em $x$ e $y$.

---

### 5.4 Integral $\{U\}\{V\}^T$

A integral cruzada entre funções de aresta:

$$
\iint_e
\{U\}\{V\}^T
dxdy
$$

não aparece no Apêndice do artigo. Ela pode ser necessária em extensões com matrizes constitutivas não diagonais.

Partindo de:

$$
U_k(y)=\bar{a}_k+\bar{c}_ky,
$$

e:

$$
V_l(x)=\bar{b}_l-\bar{c}_lx,
$$

tem-se:

$$
\left[
\iint_e
\{U\}\{V\}^T
dxdy
\right]_{kl}
=
\iint_e
(\bar{a}_k+\bar{c}_ky)
(\bar{b}_l-\bar{c}_lx)
dxdy.
$$

Expandindo:

$$
=
\bar{a}_k\bar{b}_l
\iint_e dxdy
-
\bar{a}_k\bar{c}_l
\iint_e x\,dxdy
+
\bar{c}_k\bar{b}_l
\iint_e y\,dxdy
-
\bar{c}_k\bar{c}_l
\iint_e xy\,dxdy.
$$

Usando:

$$
\iint_e dxdy=A_e,
$$

$$
\iint_e x\,dxdy=A_ex_c,
$$

$$
\iint_e y\,dxdy=A_ey_c,
$$

e a fórmula derivada para o termo misto:

$$
\iint_e xy\,dxdy
=
\frac{A_e}{12}
\left(
x_1y_1+x_2y_2+x_3y_3+9x_cy_c
\right),
$$

obtém-se:

$$
\left[
\iint_e
\{U\}\{V\}^T
dxdy
\right]_{kl}
=
A_e\bar{a}_k\bar{b}_l
-
A_ex_c\bar{a}_k\bar{c}_l
+
A_ey_c\bar{c}_k\bar{b}_l
-
\frac{A_e}{12}
\bar{c}_k\bar{c}_l
\left(
x_1y_1+x_2y_2+x_3y_3+9x_cy_c
\right).
$$

A transposta é:

$$
\iint_e \{V\}\{U\}^T dxdy
=
\left(
\iint_e \{U\}\{V\}^T dxdy
\right)^T.
$$

> **Nota de verificação:** a fórmula de $\iint_e xy\,dxdy$ não está no Apêndice do artigo. Antes de ser usada no código, deve ser validada numericamente por quadratura independente em triângulos de teste.

---

### 5.5 Integrais entre derivadas de aresta e derivadas nodais

Como:

$$
U_{y,k}=\frac{\partial U_k}{\partial y}=\bar{c}_k,
$$

e:

$$
V_{x,k}=\frac{\partial V_k}{\partial x}=-\bar{c}_k,
$$

podem ser derivadas integrais como:

$$
\left[
\iint_e
\{U_y\}\{N_x\}^T
dxdy
\right]_{kl}
=
\iint_e
\bar{c}_k
\frac{b_l}{2A_e}
dxdy
=
\frac{\bar{c}_kb_l}{2}.
$$

De modo semelhante:

$$
\left[
\iint_e
\{U_y\}\{N_y\}^T
dxdy
\right]_{kl}
=
\frac{\bar{c}_kc_l}{2},
$$

$$
\left[
\iint_e
\{V_x\}\{N_x\}^T
dxdy
\right]_{kl}
=
-\frac{\bar{c}_kb_l}{2},
$$

e:

$$
\left[
\iint_e
\{V_x\}\{N_y\}^T
dxdy
\right]_{kl}
=
-\frac{\bar{c}_kc_l}{2}.
$$

> **Nota de verificação:** essas integrais não aparecem nas matrizes do artigo. Estão registradas apenas para auditoria, testes independentes ou futuras extensões do operador.

---

## 6. Termos ainda pendentes

### 6.1 Sinal de termos com $j$ na matriz $[B]$

A matriz $[B]$ do artigo contém fatores $j$ associados à dependência modal longitudinal e à escolha:

$$
\phi_z=j\{N\}^T\{\phi_z\}_e.
$$

Como a convenção de propagação adotada é:

$$
\exp[j(\omega t-\beta z)],
$$

tem-se:

$$
\frac{\partial}{\partial z}
\rightarrow
-j\beta.
$$

Dependendo de como a componente axial é escrita e de como o rotacional é expandido, sinais intermediários podem mudar.

> **Nota de verificação:** antes de implementar a matriz $[B]$, é necessário conferir diretamente no PDF original todos os sinais dos termos com $j$, principalmente os associados a $\{N_x\}$, $\{N_y\}$, $\{U\}$ e $\{V\}$. O risco principal é obter $\beta^2$ correto, mas recuperar $\phi_z$ com fase ou sinal incompatível.

---

### 6.2 Integrais de contorno

A passagem para a formulação variacional pode gerar termos de contorno ao aplicar integração por partes. O artigo trabalha com condições de contorno adequadas para os exemplos tratados, mas não desenvolve explicitamente uma biblioteca de integrais de linha no Apêndice.

Possíveis integrais futuras, caso sejam necessárias condições não homogêneas, absorventes ou excitações de porta, incluem:

$$
\int_{\partial e}
\{N\}\,ds,
$$

$$
\int_{\partial e}
\{U\}\,ds,
$$

$$
\int_{\partial e}
\{V\}\,ds.
$$

> **Nota de verificação:** essas integrais não fazem parte da implementação inicial fiel ao artigo. Devem ser tratadas apenas em uma fase posterior, caso o projeto avance para PML, ABC, excitação de portas ou condições de contorno não homogêneas.

---

### 6.3 Integrais para tensores não diagonais

O artigo apresenta $[p]$ e $[q]$ como matrizes diagonais. Para tensores cheios, surgiriam termos adicionais envolvendo produtos cruzados como:

$$
\{U\}\{V\}^T,
$$

$$
\{V\}\{U\}^T,
$$

$$
\{N_x\}\{N_y\}^T,
$$

e:

$$
\{N_y\}\{N_x\}^T.
$$

> **Nota de verificação:** embora algumas dessas integrais tenham sido derivadas neste documento, elas não pertencem à reprodução inicial do artigo. Qualquer uso em código deve ser marcado como extensão.

---

## 7. Tabela geral de classificação

| Termo | Origem | Status | Risco | Teste futuro |
| --- | --- | --- | --- | --- |
| $\{U\}\{U\}^T$ | Artigo, Eq. (A1) | Presente | Trocar dependência em $y$ por $x$; orientação negativa do triângulo | Comparar fórmula fechada com quadratura numérica |
| $\{V\}\{V\}^T$ | Artigo, Eq. (A2) | Presente | Perder o sinal de $-\bar{c}_kx$; orientação negativa | Comparar fórmula fechada com quadratura numérica |
| $\{U_y\}\{U_y\}^T$ | Artigo, Eq. (A3) | Presente | Esquecer que vem da cadeia de quatro termos | Testar fator $4$ no termo $-\{U_y\}+\{V_x\}$ |
| $\{V_x\}\{V_x\}^T$ | Artigo, Eq. (A3) | Presente por igualdade | Sinal de $V_x=-\bar{c}_k$ | Verificar contra A3 |
| $\{U_y\}\{V_x\}^T$ | Artigo, Eq. (A3) | Presente por igualdade com sinal negativo | Perder o sinal negativo | Testar expansão completa do rotacional |
| $\{V_x\}\{U_y\}^T$ | Artigo, Eq. (A3) | Presente por igualdade com sinal negativo | Perder o sinal negativo | Testar expansão completa do rotacional |
| $\{U\}\{N_x\}^T$ | Artigo, Eq. (A4) | Presente | Trocar $b_l$ por $c_l$ | Comparar com quadratura de primeira ordem |
| $\{V\}\{N_y\}^T$ | Artigo, Eq. (A5) | Presente | Trocar $c_l$ por $b_l$ | Comparar com quadratura de primeira ordem |
| $\{N\}\{N\}^T$ | Artigo, Eq. (A6) | Presente | Usar massa lumped em vez de consistente | Conferir matriz $A_e/12$ |
| $\{N_x\}\{N_x\}^T$ | Artigo, Eq. (A7) | Presente | Usar área com sinal incorreto | Testar com triângulo unitário |
| $\{N_y\}\{N_y\}^T$ | Artigo, Eq. (A8) | Presente | Usar área com sinal incorreto | Testar com triângulo unitário |
| $\{N_x\}\{U\}^T$ | Transposta de (A4) | Obtido por simetria | Montar sem transpor | Testar dimensões nó–aresta |
| $\{N_y\}\{V\}^T$ | Transposta de (A5) | Obtido por simetria | Montar sem transpor | Testar dimensões nó–aresta |
| $\{V\}\{N_x\}^T$ | Derivado das funções de forma | Derivado, não presente no artigo | Usar como se fosse Eq. do artigo | Validar por quadratura |
| $\{U\}\{N_y\}^T$ | Derivado das funções de forma | Derivado, não presente no artigo | Usar como se fosse Eq. do artigo | Validar por quadratura |
| $\{N_x\}\{N_y\}^T$ | Derivado das funções nodais | Derivado, extensão | Erro de ordem $b_kc_l$ | Validar por quadratura |
| $\{N_y\}\{N_x\}^T$ | Transposta do termo anterior | Derivado, extensão | Erro de transposição | Validar por quadratura |
| $\{U\}\{V\}^T$ | Derivado das funções de aresta | Derivado, extensão | Fórmula de $\iint xy$ não testada | Validar $\iint xy$ numericamente |
| $\{V\}\{U\}^T$ | Transposta de $\{U\}\{V\}^T$ | Derivado, extensão | Assumir simetria indevida | Comparar com transposta |
| Derivadas de aresta $\times$ derivadas nodais | Derivado das funções de forma | Sem uso no artigo | Implementar sem necessidade | Deixar fora da implementação inicial |
| Integrais de contorno | Não fornecidas pelo artigo | Pendente | Introduzir termos de fronteira sem derivação | Derivar apenas se houver PML/ABC/BC não homogênea |
| Sinais com $j$ na matriz $[B]$ | Artigo, Eq. (28) | Pendente de auditoria | Erro de fase/sinal em $\phi_z$ | Conferir PDF e testar modo simples |

---

## 8. Critérios para aceitar uma integral derivada no código

Uma integral derivada neste projeto só deve entrar no código quando cumprir todos os critérios abaixo:

1. **Origem documentada:** deve estar claro se a integral vem do artigo, de simetria, de transposição ou de derivação própria.
2. **Derivação escrita:** os passos intermediários devem estar registrados em Markdown, como feito neste documento.
3. **Teste por quadratura independente:** a fórmula fechada deve ser comparada com integração numérica em pelo menos três triângulos:
   - triângulo unitário;
   - triângulo escalado;
   - triângulo deslocado e não alinhado aos eixos.
4. **Teste de orientação:** deve ficar claro se a implementação exige orientação anti-horária ou se usará $|A_e|$.
5. **Teste de transposição:** integrais do tipo aresta–nó e nó–aresta devem ser comparadas por transposição.
6. **Separação de escopo:** integrais não presentes no artigo não devem ser usadas na reprodução inicial, salvo se houver justificativa científica explícita.
7. **Rastreabilidade:** cada bloco local deve apontar para a equação ou seção documental correspondente.
8. **Consistência dimensional:** a dimensão da matriz local deve ser conferida:
   - aresta–aresta: $3\times3$;
   - nó–nó: $3\times3$;
   - aresta–nó: $3\times3$;
   - nó–aresta: $3\times3$, mas obtida por transposição quando aplicável.

---

## 9. Pendências para auditoria antes da implementação

Antes de implementar as matrizes locais, é necessário auditar os seguintes pontos:

1. Conferir no PDF original todos os sinais da matriz $[B]$, principalmente os termos com $j$.
2. Verificar se a transcrição da Equação (A3) está correta em todos os sinais.
3. Confirmar se o fator $4$ nas Equações (29a) e (32a) realmente decorre da combinação completa:

$$
-\{U_y\}+\{V_x\}.
$$

4. Definir se a malha será normalizada para orientação anti-horária ou se as integrais usarão $|A_e|$.
5. Validar numericamente as integrais A1–A8 por quadratura independente antes de montar $[K]$ e $[M]$.
6. Validar separadamente as integrais A4 e A5, pois elas alimentam diretamente $[K_{tz}]$.
7. Manter $\{V\}\{N_x\}^T$, $\{U\}\{N_y\}^T$, $\{U\}\{V\}^T$ e $\{N_x\}\{N_y\}^T$ fora da implementação inicial, salvo se houver extensão formal para tensores não diagonais.
8. Definir como as condições de contorno PEC/PMC serão aplicadas sem depender de integrais de linha ausentes no Apêndice.
9. Criar testes de regressão para verificar que as matrizes locais são simétricas quando teoricamente devem ser.
10. Criar testes para verificar que blocos transpostos, como $[K_{zt}]$, são montados como transpostas reais dos blocos correspondentes.

---

## 10. Conclusão operacional

Para a reprodução inicial do artigo de Koshiba e Inoue, a implementação deve usar apenas as integrais explicitamente presentes no Apêndice e as transpostas necessárias para montar os blocos correspondentes.

As integrais derivadas neste documento são úteis para auditoria e para extensões futuras, mas não devem ser confundidas com fórmulas originais do artigo.

A regra de segurança é:

> **Implementar primeiro o núcleo diagonal fiel ao artigo; validar; somente depois introduzir integrais derivadas ou extensões para tensores não diagonais.**
