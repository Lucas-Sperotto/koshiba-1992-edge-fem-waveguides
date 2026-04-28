# Análise simples e eficiente de guias de onda de micro-ondas e ópticos pelo método dos elementos finitos

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md)

**Masanori Koshiba**, *Senior Member, IEEE*, e **Kazuhiro Inoue**

---

## Resumo

Um método de elementos finitos simples e eficiente para a análise de problemas de guiamento de ondas em micro-ondas e em guias ópticos é formulado utilizando as três componentes do campo elétrico ou magnético.

Com o objetivo de eliminar soluções espúrias, são introduzidos elementos de aresta. Na abordagem por elementos de aresta, os parâmetros nodais não ficam limitados ao campo magnético, como ocorre na formulação convencional de três componentes para o problema de guiamento em guias dielétricos.

A equação de autovalores derivada neste trabalho envolve apenas as variáveis de aresta no plano transversal e permite obter uma solução direta para a constante de propagação.

Para demonstrar a validade e a utilidade dessa abordagem, são apresentados resultados computacionais para linhas de transmissão microstrip e guias de onda dielétricos.

---

## Nota editorial do artigo original

Manuscrito recebido em **14 de maio de 1991**; revisado em **10 de setembro de 1991**.

Este trabalho recebeu apoio parcial de uma **Bolsa de Auxílio à Pesquisa Científica** do Ministério da Educação, Ciência e Cultura do Japão.

Os autores estavam vinculados ao **Departamento de Engenharia Eletrônica da Universidade de Hokkaido**, Sapporo, 060, Japão.

**IEEE Log Number:** 9104779.

## Direitos do artigo original

```text
0018-9480/92$03.00 © 1992 IEEE
```

---

## Sobre os autores

### Masanori Koshiba

**Masanori Koshiba** (*Senior Member, IEEE*) nasceu em Sapporo, Japão, em 23 de novembro de 1948. Recebeu os graus de B.S., M.S. e Ph.D. em Engenharia Eletrônica pela Universidade de Hokkaido, em Sapporo, Japão, nos anos de 1971, 1973 e 1976, respectivamente.

Em 1976, ingressou no Departamento de Engenharia Eletrônica do Instituto de Tecnologia de Kitami, em Kitami, Japão. De 1979 a 1987, foi professor associado de Engenharia Eletrônica na Universidade de Hokkaido e, em 1987, tornou-se professor nessa mesma instituição.

Sua atuação em pesquisa envolve tecnologia de ondas luminosas, ondas acústicas de superfície, ondas magnetostáticas, teoria de campos de micro-ondas e aplicações dos métodos dos elementos finitos e dos elementos de contorno a problemas de campo.

O Dr. Koshiba é membro do Institute of Electronics, Information and Communication Engineers (IEICE), do Institute of Television Engineers of Japan, do Institute of Electrical Engineers of Japan, da Japan Society for Simulation Technology e da Japan Society for Computational Methods in Engineering. Em 1987, recebeu o prêmio de melhor artigo de 1986 concedido pelo IEICE.

### Kazuhiro Inoue

**Kazuhiro Inoue** nasceu em Chitose, Hokkaido, Japão, em 10 de janeiro de 1965. Recebeu os graus de B.S. e M.S. em Engenharia Eletrônica pela Universidade de Hokkaido, em Sapporo, Japão, em 1987 e 1989, respectivamente.

Na época da publicação do artigo, estava realizando estudos de doutorado em Engenharia Eletrônica na Universidade de Hokkaido.

O Sr. Inoue é membro do Institute of Electronics, Information and Communication Engineers (IEICE).

> **Nota editorial:** fotos dos autores não são exibidas nesta documentação porque a licença das imagens anteriormente usadas no repositório não foi verificada. A biografia textual foi preservada.
