# Flappy Bird - Projeto Final PDS II

## 1. Introdução

Este projeto consiste na implementação do jogo Flappy Bird utilizando a linguagem C++ e a biblioteca Allegro. O objetivo principal é aplicar os conceitos de Programação Orientada a Objetos (POO), modularidade e corretude, além de utilizar ferramentas de controle de versão (Git/GitHub), testes unitários e documentação.

## 2. Visão Geral da Solução

O sistema é dividido em três partes principais:

### 2.1. Classes para a Dinâmica do Jogo

Responsável por gerenciar o cenário, a pontuação e as características do jogo. A classe `Scenario` lida com a atualização e renderização dos obstáculos, além de calcular a pontuação do jogador. Foram adicionados métodos para permitir a alteração da gravidade e da velocidade dos obstáculos, oferecendo maior flexibilidade para diferentes cenários de jogo.

### 2.2. Hierarquia de Classes para os Elementos do Jogo

Define os elementos interativos do jogo, como o pássaro (`Bird`) e os obstáculos (`Obstacle`). A classe base `GameObject` fornece funcionalidades comuns, enquanto as classes herdeiras especializam o comportamento de cada elemento. Métodos para detecção de colisão e atualização de movimento são implementados nessas classes. A classe `Bird` agora permite a alteração da força do pulo.

### 2.3. Módulo de Cadastro de Jogadores

Gerencia o registro e as estatísticas dos jogadores. A classe `ListaDeJogadores` é responsável por cadastrar, remover e listar jogadores, além de manter o registro da maior pontuação. Os dados dos jogadores são persistidos em um arquivo JSON (`assets/BaseJogadores.json`) entre as execuções do sistema, garantindo que o ranking seja mantido.

## 3. Execução de Partidas e Interface de Usuário

O jogo inicia com uma tela de entrada onde o jogador deve digitar seu apelido. A navegação entre as telas (entrada de apelido, jogo, game over e ranking) é controlada por um sistema de estados. Durante a partida, o pássaro é controlado pela tecla ESPAÇO. Ao final da partida, as estatísticas do jogador são atualizadas e salvas automaticamente. O ranking dos 10 melhores jogadores é exibido, e o jogador pode optar por jogar novamente, iniciar um novo jogador ou visualizar o ranking completo.

## 4. Estrutura do Diretório do Projeto

O projeto segue a seguinte estrutura de diretórios:

- `assets/`: Contém recursos como o arquivo JSON de jogadores.
- `bin/`: Armazena os executáveis gerados.
- `include/`: Contém os arquivos de cabeçalho (.hpp).
- `obj/`: Guarda os arquivos objeto (.o) gerados durante a compilação.
- `src/`: Contém os arquivos de código-fonte (.cpp).
- `tests/`: Abriga os arquivos de teste (a ser implementado).
- `.gitignore`: Lista arquivos e pastas a serem ignorados pelo Git.
- `makefile`: Contém instruções para automatizar a compilação do projeto.
- `README.md`: Este arquivo, com informações sobre o projeto.

## 5. Como Compilar e Executar

Para compilar e executar o projeto, siga os passos abaixo:

1. **Instalar Allegro 5:** Certifique-se de ter a biblioteca Allegro 5 instalada em seu sistema. No Ubuntu/Debian, você pode instalá-la com:
   ```bash
   sudo apt-get install liballegro5-dev
   ```

2. **Navegar até o diretório do projeto:**
   ```bash
   cd /home/ubuntu/TrabalhoFinalPDS2
   ```

3. **Compilar o projeto:**
   ```bash
   make all
   ```

4. **Executar o jogo:**
   ```bash
   ./bin/flappy_bird
   ```

## 6. Funcionalidades Extras Implementadas

- **Sistema de Ranking Persistente**: O jogo agora salva e carrega o ranking dos jogadores em um arquivo JSON, mantendo as pontuações e o número de jogos entre as sessões.
- **Entrada de Apelido**: Uma tela inicial permite que o jogador insira seu apelido antes de começar a jogar.
- **Controle de Estados do Jogo**: Implementação de um sistema de estados para gerenciar a transição entre as telas de entrada de apelido, jogo, game over e ranking.
- **Flexibilidade de Jogo**: Adição de métodos para alterar a gravidade e a força do pulo do pássaro, e a velocidade dos obstáculos, permitindo futuras customizações ou diferentes modos de jogo.

## 7. Dificuldades Encontradas

- **Gerenciamento de Memória com Allegro**: A integração com a biblioteca Allegro exigiu atenção especial ao gerenciamento de recursos (displays, timers, filas de eventos, fontes) para evitar vazamentos de memória e garantir o correto encerramento do programa.
- **Persistência de Dados JSON**: A manipulação de arquivos JSON para salvar e carregar os dados dos jogadores apresentou desafios na serialização e desserialização de objetos C++, que foram superados com a utilização da biblioteca `nlohmann/json`.
- **Compilação e Dependências**: A configuração do `makefile` para compilar o projeto com todas as dependências do Allegro e garantir a compatibilidade entre os diferentes arquivos-fonte foi um ponto de atenção.

## 8. Próximos Passos (Conforme Requisitos do Projeto)

- **Testes Unitários**: Implementar testes unitários utilizando a biblioteca `doctest` para garantir a corretude das classes e métodos.
- **Documentação Doxygen**: Gerar a documentação completa do código-fonte utilizando Doxygen.



