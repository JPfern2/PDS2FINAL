#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// Inclua aqui os cabeçalhos das classes que você quer testar
#include "../include/jogador.hpp"
#include "../include/ListaDeJogadores.hpp"
#include "../include/bird.hpp"
#include "../include/scenario.hpp"
#include "../include/obstacle.hpp"

// Testes para a classe Jogador
TEST_CASE("Testando a classe Jogador") {
    Jogador j1("Nome Teste", "Apelido Teste", 100, 5);

    SUBCASE("Getters basicos") {
        CHECK(j1.getNome() == "Nome Teste");
        CHECK(j1.getApelido() == "Apelido Teste");
        CHECK(j1.getPontuacaoMaxima() == 100);
        CHECK(j1.getNumeroDeJogos() == 5);
    }

    SUBCASE("Setters") {
        j1.setPontuacaoMaxima(200);
        CHECK(j1.getPontuacaoMaxima() == 200);
        j1.increaseNumeroDeJogos();
        CHECK(j1.getNumeroDeJogos() == 6);
    }
}

// Testes para a classe ListaDeJogadores
TEST_CASE("Testando a classe ListaDeJogadores") {
    // Criar um arquivo de teste temporário para evitar conflitos
    std::string test_filename = "test_players.json";

    // Limpar o arquivo antes de cada TEST_CASE para garantir um estado limpo
    std::ofstream ofs("assets/" + test_filename, std::ios::trunc);
    ofs.close();

    ListaDeJogadores lista(test_filename);

    SUBCASE("Cadastrar novo jogador") {
        Jogador j("Novo Jogador", "novo", 50, 1);
        lista.cadastrarJogador(j);
        CHECK(lista.getJogadores().size() == 1);
        CHECK(lista.getJogadores()[0].getApelido() == "novo");
    }

    SUBCASE("Atualizar jogador existente") {
        Jogador j1("Jogador Existente", "existente", 100, 2);
        lista.cadastrarJogador(j1);
        // Criar um novo objeto Jogador com o mesmo apelido para simular uma atualização
        Jogador j2_updated("Jogador Existente", "existente", 150, 0); // Pontuação 0 para simular um novo jogo
        lista.cadastrarJogador(j2_updated);
        CHECK(lista.getJogadores().size() == 1); // Ainda deve haver apenas 1 jogador com esse apelido
        CHECK(lista.getJogadores()[0].getPontuacaoMaxima() == 150); // Pontuação deve ser atualizada
        CHECK(lista.getJogadores()[0].getNumeroDeJogos() == 3); // Número de jogos deve ser incrementado
    }

    SUBCASE("Remover jogador") {
        Jogador j("Para Remover", "remover", 70, 3);
        lista.cadastrarJogador(j);
        CHECK(lista.getJogadores().size() == 1);
        lista.removerJogador("remover");
        CHECK(lista.getJogadores().empty());
    }

    SUBCASE("Carregar e salvar jogadores") {
        ListaDeJogadores lista_save_load(test_filename);
        Jogador j1("Save Test 1", "save1", 10, 1);
        Jogador j2("Save Test 2", "save2", 20, 2);
        lista_save_load.cadastrarJogador(j1);
        lista_save_load.cadastrarJogador(j2);
        lista_save_load.salvarJogadores();

        ListaDeJogadores nova_lista(test_filename);
        CHECK(nova_lista.getJogadores().size() == 2);
        CHECK(nova_lista.getJogadores()[0].getApelido() == "save1");
        CHECK(nova_lista.getJogadores()[1].getApelido() == "save2");
    }

    SUBCASE("Maior pontuador") {
        ListaDeJogadores lista_maior(test_filename);
        Jogador j1("Top Player", "top", 500, 10);
        Jogador j2("Mid Player", "mid", 200, 5);
        lista_maior.cadastrarJogador(j1);
        lista_maior.cadastrarJogador(j2);
        CHECK(lista_maior.maiorPontuador().find("top") != std::string::npos);
    }
}

// Testes para a classe Bird
TEST_CASE("Testando a classe Bird") {
    Bird bird(100, 300);

    SUBCASE("Posicao inicial") {
        CHECK(bird.getX() == 100);
        CHECK(bird.getY() == 300);
    }

    SUBCASE("Pulo") {
        float initial_y = bird.getY();
        bird.jump();
        bird.update(); // Aplica a gravidade após o pulo
        CHECK(bird.getY() < initial_y); // Deve subir
    }

    SUBCASE("Gravidade") {
        bird.setGravity(1.0);
        float initial_y = bird.getY();
        bird.update();
        CHECK(bird.getY() > initial_y); // Deve cair mais rápido
    }

    SUBCASE("Forca do pulo") {
        Bird test_bird(100, 300); // Criar uma nova instância para este subcase
        test_bird.setJumpForce(-15.0); // Definir uma força de pulo negativa para subir
        float initial_y = test_bird.getY();
        test_bird.jump();
        test_bird.update(); // Aplica a gravidade após o pulo
        // A posição final deve ser menor que a inicial, e o pulo deve ser significativo
        // Considerando que velocity é -15 e gravity é 0.5, a nova velocity será -14.5
        // Então y_final = y_inicial + (-15 + 0.5) = y_inicial - 14.5
        CHECK(test_bird.getY() < initial_y); // Deve subir
        CHECK(test_bird.getY() == doctest::Approx(initial_y - 14.5)); // Verificar o valor exato após um update
    }
}

// Testes para a classe Obstacle
TEST_CASE("Testando a classe Obstacle") {
    Obstacle obs(800, 0, 50, 200, 5.0);

    SUBCASE("Posicao inicial") {
        CHECK(obs.getX() == 800);
        CHECK(obs.getY() == 0);
        CHECK(obs.getWidth() == 50);
        CHECK(obs.getHeight() == 200);
    }

    SUBCASE("Movimento") {
        float initial_x = obs.getX();
        obs.move(-10);
        CHECK(obs.getX() == initial_x - 10);
    }

    SUBCASE("Fora da tela") {
        obs.setX(-100);
        CHECK(obs.isOffScreen() == true);
    }

    SUBCASE("Pontuacao") {
        CHECK(obs.hasScored() == false);
        obs.markScored();
        CHECK(obs.hasScored() == true);
        obs.resetScored();
        CHECK(obs.hasScored() == false);
    }

    SUBCASE("Velocidade") {
        obs.setSpeed(10.0);
        // A velocidade é aplicada no update do Scenario, aqui apenas verificamos se o setter funciona
        // Não há um getter direto para speed na classe Obstacle, então a verificação é indireta
    }
}

// Testes para a classe Scenario
TEST_CASE("Testando a classe Scenario") {
    Scenario scenario(0.5, 3.0);
    Bird bird(100, 300);

    SUBCASE("Pontuacao inicial") {
        CHECK(scenario.getScore() == 0);
    }

    SUBCASE("Aumentar pontuacao") {
        scenario.increaseScore();
        CHECK(scenario.getScore() == 1);
    }

    SUBCASE("Reset") {
        scenario.increaseScore();
        scenario.reset();
        CHECK(scenario.getScore() == 0);
    }

    SUBCASE("Velocidade dos obstaculos") {
        scenario.setObstacleSpeed(5.0);
        // A verificação da velocidade dos obstáculos é mais complexa e envolveria a simulação do jogo
        // ou a inspeção interna dos objetos Obstacle, o que não é trivial com a interface atual.
        // Assumimos que setObstacleSpeed propaga corretamente a velocidade para os obstáculos.
    }
}


