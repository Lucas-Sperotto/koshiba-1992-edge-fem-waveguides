# Índice da documentação

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md) | [19](19_auditoria_sinais_acoplamentos.md) | [20](20_dossie_casos_validacao_figuras.md) | [21](21_relatorio_validacao_atual.md)

Este diretório reúne a tradução técnica do artigo, notas editoriais, complementos matemáticos, contrato de implementação C++17 e dossiê dos casos de validação.

## Tradução e estrutura do artigo

- [00 — Resumo](00_resumo.md)
- [01 — Introdução](01_introducao.md)
- [02 — Equações básicas](02_equacoes_basicas.md)
- [03 — Formulação por elementos finitos](03_formulacao_elementos_finitos.md)
- [04 — Exemplos numéricos](04_exemplos_numericos.md)
- [05 — Conclusão](05_conclusao.md)
- [06 — Apêndice](06_apendice.md)
- [07 — Referências](07_referencias.md)

## Fechamento documental

- [08 — Notas editoriais e científicas](08_notas_editoriais_e_cientificas.md)

## Complementos matemáticos para implementação

- [09 — Das equações de Maxwell à Equação (1)](09_maxwell_para_equacao_01.md)
- [10 — Da Equação (1) ao funcional da Equação (6)](10_equacao_01_para_funcional_06.md)
- [11 — Origem do fator j na Equação (7)](11_origem_do_fator_j_equacao_07.md)
- [12 — Funções de forma nodais e de aresta](12_funcoes_de_forma_nodais_e_de_aresta.md)
- [13 — Revisão das integrais do Apêndice](13_revisao_das_integrais_do_apendice.md)
- [14 — Integrais cruzadas e termos ausentes do Apêndice](14_integrais_cruzadas_e_termos_ausentes.md)
- [15 — Testes matemáticos mínimos](15_testes_matematicos_minimos.md)
- [16 — Contrato para implementação C++17](16_contrato_para_implementacao_cpp.md)
- [17 — Implementação da Fase 7 — solver beta mínimo](17_implementacao_fase7_solver_beta.md)
- [18 — Política inicial para PEC/PMC](18_politica_pec_pmc.md)
- [19 — Auditoria de sinais e acoplamentos](19_auditoria_sinais_acoplamentos.md)
- [20 — Dossiê dos casos de validação das Figuras 3, 5 e 7](20_dossie_casos_validacao_figuras.md)
- [21 — Relatório de validação atual](21_relatorio_validacao_atual.md)

## Status

A fase documental está fechada como baseline científica e didática. A implementação já inclui materiais por região, solver `k0 -> beta`, solver direto `beta -> k0/f`, exemplos Gmsh e pipeline de CSV/plots para as Figuras 3, 5 e 7. A suíte atual possui 15 testes CTest e passa com `/usr/bin/ctest --test-dir build --output-on-failure`.

O resumo de comparação fica em `out/validation/validation_summary.csv`. O `status=no_reference` significa que ainda não há CSV de referência conferido em `data/input/reference/` para aquela curva, portanto a validação quantitativa contra referência externa segue pendente e não deve ser declarada concluída.

Na Figura 3, os CSVs preservam o eixo como `beta_rad_per_mm`, conforme a figura do artigo, e o valor é convertido internamente para `rad/m` antes do solver. Nas Figuras 5 e 7, apenas pontos com $0 \le b \le 1$ são tratados como modos guiados físicos; pontos fora dessa faixa recebem `status=outside_guided_range` e não entram nos gráficos ou comparações.

Por política de copyright, o PDF e as imagens originais do artigo não são versionados. Os SVGs em `docs/img/` são esquemas próprios do projeto, e os gráficos comparativos devem ser gerados em `out/validation/`.
