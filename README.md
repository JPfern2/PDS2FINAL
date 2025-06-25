Flappy Bird — Projeto Final de Programação e Desenvolvimento de Software II (PDS II)
1. Introdução
Este projeto tem como objetivo a implementação do jogo Flappy Bird em C++, utilizando a biblioteca Allegro 5. A proposta visa consolidar os conceitos de Programação Orientada a Objetos (POO), modularização de código, uso de controle de versão (Git/GitHub), testes unitários e documentação de software.

2. Visão Geral da Solução
O projeto é dividido em três partes principais:
2.1. Dinâmica do Jogo
Responsável por controlar o cenário, a pontuação e as regras da partida. A classe Scenario cuida da atualização dos obstáculos e da pontuação do jogador, além de oferecer métodos para modificar parâmetros como gravidade e velocidade dos obstáculos, permitindo diferentes configurações de jogo.
2.2. Elementos do Jogo
As entidades do jogo (como o Bird e os Obstacles) são modeladas por meio de uma hierarquia de classes, com uma superclasse abstrata GameObject. Essa estrutura promove reuso de código e facilita a manutenção. A classe Bird possui lógica específica para movimentação e permite configurar a força do pulo.
2.3. Cadastro e Ranking de Jogadores
O sistema registra o apelido do jogador e salva sua maior pontuação em um arquivo persistente. As classes jogador e ListadeDeJogares  gerenciam esses dados. A interface inclui uma tela de entrada de apelido e uma tela de ranking com os 10 melhores jogadores.

3. Execução do Jogo e Interface do Usuário
O jogo segue uma estrutura de estados, alternando entre as seguintes telas:
Tela de entrada: o jogador digita seu apelido.


Tela de jogo: o jogador controla o pássaro com a tecla ESPAÇO.


Game Over: a pontuação é salva automaticamente.


Ranking: os 10 jogadores com maior pontuação são exibidos.


Ao final da partida, o jogador pode reiniciar o jogo, trocar de jogador ou apenas visualizar o ranking.
4. Estrutura de Diretórios

assets/     → Arquivos de mídia e fontes (ex: fontes TTF).
bin/        → Executáveis gerados.
include/    → Arquivos de cabeçalho (.hpp).
obj/        → Arquivos objeto (.o) da compilação.
src/        → Código-fonte do projeto (.cpp).
tests/      → Testes unitários.
.gitignore  → Arquivos e pastas ignoradas pelo Git.
Makefile    → Regras de compilação.
README      → Descrição do projeto.

5. Como Compilar e Executar
1. Instale o Allegro 5
No Ubuntu/Debian:

sudo apt-get install liballegro5-dev

2. Compile o projeto
No terminal:
cd /caminho/para/TrabalhoFinalPDS2
make all

3. Execute o jogo

./bin/flappy_bird

6. Funcionalidades Extras
 Sistema de Ranking Persistente
 Armazena apelidos e pontuações dos jogadores entre sessões.


 Entrada de Apelido
 Tela interativa permite que o jogador insira seu nome antes da partida.


 Controle de Estados
 Sistema simples de estados de tela que alterna entre entrada, jogo, game over e ranking.

7. Dificuldades Encontradas
 Gerenciamento de Recursos Allegro
 A correta liberação de elementos como fontes, eventos e janelas exigiu atenção para evitar vazamentos de memória.


 Persistência de Dados
 A manipulação de arquivos exigiu cuidados com leitura e escrita de dados estruturados.

Configuração do Ambiente de Desenvolvimento
A instalação da Allegro 5 e a configuração de um ambiente de desenvolvimento funcional em diferentes sistemas operacionais foram tarefas trabalhosas. 
