# G33_Dividir-e-Conquistar_PA-26.1

# UniCidade

*Conteúdo da Disciplina*: Dividir e Conquistar<br>

## Alunos

|Matrícula | Aluno |
| -- | -- |
| 23/2014404  |  [Carlos Henrique Brasil de Souza](https://github.com/Carlos-UCH) |
| 23/2014576  |  [Yogi Nam de Souza Barbosa](https://github.com/oyogi)


## Sobre

Projeto desenvolvido por alunos da Universidade de Brasília (UnB) para a disciplina de Projeto de Algoritmos. 

O projeto consiste em encontrar 20 pares de municípios mais próximos com a ideia de unir as cidades para diminuir custos e aumentar arrecadação.

Será utilizado o algoritmo de par de pontos mais próximos, que encontra os dois pontos em um conjunto que possuem a menor distância entre si.  

Para extrair a localização de cada município do Brasil, acessamos a base de dados do IBGE disponível em [IBGE](https://www.ibge.gov.br/geociencias/organizacao-do-territorio/malhas-territoriais/15774-malhas.html). O ideal era que viesse um .csv com os nomes e as coordenadas, mas ela vem em um arquivo .zip que extraímos e inserimos os arquivos no site [mapshaper](https://mapshaper.org). Daí pelo console do site rodamos o comando `-each "LATITUDE=this.y, LONGITUDE=this.x", depois na opção Export selecionamos CSV. Assim, conseguimos um arquivo .csv com os nomes e as coordenadas de cada município do Brasil de uma fonte confiável. O arquivo está no repositório, com nome BR_Municipios_2025.csv.

Porém, notamos que nessa base de dados as coordenadas não estavam exatamente no centro urbano, mas sim na região do município. Para uma melhor estimativa, buscamos outra base de dados, disponível no GitHub [municipios-brasileiros](https://github.com/kelvins/municipios-brasileiros). Esta se mostrou bem precisa em relação as coordenadas dos centros urbanos e também confiável, condizente com os dados do IBGE.

## Motivação e Aplicação Prática

A viabilidade financeira de municípios pequenos é um problema estrutural no Brasil. Uma grande parcela das cidades não gera receita própria suficiente para custear a própria máquina pública, dependendo quase que exclusivamente de repasses federais e estaduais.

Este projeto fundamenta-se em dados e propostas reais sobre a otimização do pacto federativo:
* Histórico Legislativo: A [PEC 188/2019](https://www25.senado.leg.br/web/atividade/materias/-/materia/139704) propôs a extinção e fusão de municípios com menos de 5.000 habitantes que não atingissem o limite mínimo de 10% de arrecadação própria.
* Impacto Econômico (2025): Um [estudo](https://periodicos.fgv.br/cgpc/article/view/92857) de pesquisadores da Universidade Federal da Paraíba (UFPB), publicado em 2025, concluiu que a fusão de municípios próximos geograficamente aumenta a capacidade das prefeituras de custear seus serviços em 36% e eleva a arrecadação de impostos locais em até 40%.

**Aplicação do Programa:**
Para a fusão ser viável, a proximidade geográfica é o critério principal. O algoritmo resolve o desafio espacial ao calcular os 20 pares de municípios mais próximos do Brasil. Identificar esses pares fornece os candidatos matematicamente ideais para unificação, garantindo:
1. A viabilidade da unificação de secretarias e prédios públicos com o menor custo de transição.
2. O menor impacto logístico no deslocamento da população para acesso a serviços essenciais centralizados, como saúde e educação.

## Detalhes Técnicos

Ao executar o algoritmo notamos alguns problemas.

Para descobrir a distância entre duas cidades possuindo a latitude e longitude não é trivial, mas pesquisamos e achamos uma fórmula chamada Fórmula de Haversine, com ela conseguimos a distância entre duas cidades.

Porém, ao pesquisar isso vimos que, no algoritmo de par de pontos mais próximo para cada ponto só precisamos checar os 7 próximos pontos, mas como nossa superfície é o planeta Terra, não é uma superfície Euclidiana, portanto não é garantido que basta checar os 7. Daí pesquisamos para resolver esse problema e decidimos fazer uma projeção equirretangular baseada na latitude média do conjunto de dados, para converter as coordenadas esféricas em um plano Euclidiano em quilômetros.

## Link do Vídeo da Apresentação

## Screenshots



## Instalação 
*Linguagem*: C++<br>

## Clone o repositório  
```sh 
git clone git@github.com:projeto-de-algoritmos-2026/G33_Dividir-e-Conquistar_PA-26.1.git
cd G33_Dividir-e-Conquistar_PA-26.1
```

### Pre-requisitos
- Ter o C++20 instalado.

## Uso

Como rodar a aplicação

```sh 
g++ -O2 main.cpp csv_parser.cpp -o unicidade.out
./unicidade.out
```