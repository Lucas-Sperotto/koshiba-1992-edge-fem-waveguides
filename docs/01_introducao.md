# 1. Introdução

> **Navegação dos docs:** [Índice](README.md) | [00](00_resumo.md) | [01](01_introducao.md) | [02](02_equacoes_basicas.md) | [03](03_formulacao_elementos_finitos.md) | [04](04_exemplos_numericos.md) | [05](05_conclusao.md) | [06](06_apendice.md) | [07](07_referencias.md) | [08](08_notas_editoriais_e_cientificas.md) | [09](09_maxwell_para_equacao_01.md) | [10](10_equacao_01_para_funcional_06.md) | [11](11_origem_do_fator_j_equacao_07.md) | [12](12_funcoes_de_forma_nodais_e_de_aresta.md) | [13](13_revisao_das_integrais_do_apendice.md) | [14](14_integrais_cruzadas_e_termos_ausentes.md) | [15](15_testes_matematicos_minimos.md) | [16](16_contrato_para_implementacao_cpp.md) | [17](17_implementacao_fase7_solver_beta.md) | [18](18_politica_pec_pmc.md)

Para avaliar rigorosamente as características de propagação de guias de onda de micro-ondas e ópticos com seções transversais de formato arbitrário, é necessária uma análise vetorial de ondas. Por essa razão, diferentes tipos de métodos dos elementos finitos vetoriais têm sido desenvolvidos.

Entre as várias formulações existentes, o método dos elementos finitos que utiliza o campo vetorial magnético completo, isto é, o campo $\mathbf{H}$ completo, é bastante adequado para uma ampla variedade de problemas práticos e complexos [1]–[10].

Essa abordagem tem sido amplamente utilizada em diversas estruturas dielétricas de guiamento de ondas nas regiões de micro-ondas, ondas milimétricas e comprimentos de onda ópticos. Recentemente, ela também tem sido utilizada como solucionador de guias de onda em pacotes de CAD [7].

O problema mais sério associado a essa abordagem é o aparecimento de soluções espúrias. O método da função de penalidade [3], [4], [6], [7] tem sido utilizado para corrigir esse problema. Entretanto, nessa técnica é introduzida uma constante positiva arbitrária, chamada coeficiente de penalidade, e a precisão das soluções depende do valor escolhido para essa constante.

Além disso, na formulação vetorial completa [1]–[10], a constante de propagação é inicialmente fornecida como dado de entrada e, em seguida, a frequência de operação é obtida como solução.

Mais recentemente, vários métodos para resolver diretamente a constante de propagação foram desenvolvidos. Contudo, cada um deles apresenta alguma limitação, como, por exemplo, o uso de um grande número de componentes de campo [11]–[13], a consideração de um campo adjunto que não corresponde ao campo eletromagnético real [14], ou a necessidade de estimar a integral de linha na expressão variacional [15].

Neste artigo, é formulado um método dos elementos finitos simples e eficiente para a análise de problemas de guiamento de ondas em micro-ondas e em guias ópticos, utilizando três componentes do campo elétrico ou magnético.

Com o objetivo de eliminar soluções espúrias e tratar guias de onda com geometrias arbitrárias, são introduzidos elementos de aresta triangulares.

A equação de autovalores derivada neste trabalho envolve apenas as variáveis de aresta no plano transversal e permite obter uma solução direta para a constante de propagação.

Para demonstrar a validade e a utilidade dessa abordagem, são calculados exemplos envolvendo linhas de transmissão microstrip sobre substratos isotrópicos ou anisotrópicos, guias de onda retangulares dielétricos e guias de onda com núcleo triangular equilátero.
