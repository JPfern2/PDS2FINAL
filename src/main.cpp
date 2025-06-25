#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>

#include "../include/bird.hpp"
#include "../include/scenario.hpp"
#include "../include/ListaDeJogadores.hpp"
#include "../include/jogador.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    CADASTRO,
    RANKING
};

void drawMenu(ALLEGRO_FONT* font, int selectedOption) {
    al_clear_to_color(al_map_rgb(135, 206, 235));

    // Título
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "FLAPPY BIRD");

    // Opções
    al_draw_text(font, selectedOption == 0 ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0),
        SCREEN_WIDTH / 2, 200, ALLEGRO_ALIGN_CENTER, "1. Iniciar Jogo");
    al_draw_text(font, selectedOption == 1 ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0),
        SCREEN_WIDTH / 2, 250, ALLEGRO_ALIGN_CENTER, "2. Cadastrar Jogador");
    al_draw_text(font, selectedOption == 2 ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0),
        SCREEN_WIDTH / 2, 300, ALLEGRO_ALIGN_CENTER, "3. Ver Ranking");
    al_draw_text(font, selectedOption == 3 ? al_map_rgb(255, 255, 0) : al_map_rgb(0, 0, 0),
        SCREEN_WIDTH / 2, 350, ALLEGRO_ALIGN_CENTER, "4. Sair");

    al_flip_display();
}

void drawCadastroScreen(ALLEGRO_FONT* font, const std::string& nome, const std::string& apelido, bool apelido_existente) {
    al_clear_to_color(al_map_rgb(135, 206, 235));

    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "CADASTRO DE JOGADOR");

    if (apelido_existente) {
    al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, 300, ALLEGRO_ALIGN_CENTER,
        "Esse apelido já está cadastrado. Tente outro.");
    }

    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 200, ALLEGRO_ALIGN_CENTER,
        ("Nome: " + nome).c_str());
    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 250, ALLEGRO_ALIGN_CENTER,
        ("Apelido: " + apelido).c_str());

    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 350, ALLEGRO_ALIGN_CENTER,
        "Pressione ENTER para confirmar ou ESC para voltar");

    al_flip_display();
}

int main() {
    if (!al_init()) return -1;
    if (!al_init_primitives_addon()) return -1;
    if (!al_install_keyboard()) return -1;
    if (!al_init_font_addon()) return -1;
    if (!al_init_ttf_addon()) return -1;

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) return -1;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_FONT* font = al_load_ttf_font("assets/arial.ttf", 24, 0);
    if (!font) font = al_create_builtin_font();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    GameState gameState = MENU;
    int selectedOption = 0;
    std::string nomeJogador = "";
    std::string apelidoJogador = "";
    bool typingName = true;
    bool apelido_existente = false;
    ListaDeJogadores cadastro_jogadores("BaseJogadores.json");

    Bird bird(100, SCREEN_HEIGHT / 2);
    Scenario scenario(0.5, 3.0);
    bool redraw = true;

    al_start_timer(timer);

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            switch (gameState) {
            case MENU:
                if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    selectedOption = (selectedOption + 1) % 4;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                    selectedOption = (selectedOption - 1 + 4) % 4;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (selectedOption == 0) {
                        gameState = PLAYING;
                    }
                    else if (selectedOption == 1) {
                        gameState = CADASTRO;
                        nomeJogador = "";
                        apelidoJogador = "";
                        typingName = true;
                    }
                    else if (selectedOption == 2) {
                        gameState = RANKING;
                    }
                    else if (selectedOption == 3) {
                        goto end_game;
                    }
                }
                break;

            case CADASTRO:
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    gameState = MENU;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    if (typingName && !nomeJogador.empty()) {
                        nomeJogador.pop_back();
                    }
                    else if (!typingName && !apelidoJogador.empty()) {
                        apelidoJogador.pop_back();
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (typingName) {
                        typingName = false;
                    }
                    else {
                        if (!nomeJogador.empty() && !apelidoJogador.empty()) {

                            apelido_existente = false;
                            for(Jogador& j : cadastro_jogadores.getJogadores()) {
                                if (j.getApelido() == apelidoJogador) {
                                    apelido_existente = true;
                                    typingName = false;
                                    break;
                                }
                            }

                            if(!apelido_existente) {
                                Jogador novoJogador(nomeJogador, apelidoJogador, 0, 0);
                                cadastro_jogadores.cadastrarJogador(novoJogador);
                                gameState = MENU;
                            }
                        }
                    }
                }
                else if (ev.keyboard.unichar >= 32 && ev.keyboard.unichar <= 126) {
                    if (typingName) {
                        if(nomeJogador.length() < 20)
                            nomeJogador += ev.keyboard.unichar;
                    }
                    else {
                        if(apelidoJogador.length() < 20)
                            apelidoJogador += ev.keyboard.unichar;
                    }
                }
                break;

            case RANKING:
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
                    ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    gameState = MENU;
                }
                break;

            case PLAYING:
                if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    bird.jump();
                }
                break;

            case GAME_OVER:
                if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
                    bird = Bird(100, SCREEN_HEIGHT / 2);
                    scenario.reset();
                    gameState = PLAYING;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    gameState = MENU;
                }
                break;
            }
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (gameState == PLAYING) {
                bird.update();
                bool collision_detected = scenario.update(bird);
                if (collision_detected || bird.getY() < 0 || bird.getY() + 20 > SCREEN_HEIGHT) {
                    gameState = GAME_OVER;
                }
            }
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            switch (gameState) {
            case MENU: {
                drawMenu(font, selectedOption);
                break;
            }

            case CADASTRO: {
                drawCadastroScreen(font, nomeJogador, apelidoJogador, apelido_existente);
                break;
            }

            case RANKING: {
                al_clear_to_color(al_map_rgb(135, 206, 235));
                al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "RANKING DE JOGADORES");

                std::vector<Jogador> jog = cadastro_jogadores.getJogadores();
                std::sort(jog.begin(), jog.end(), [](Jogador& a, Jogador& b) {
                    return a.getPontuacaoMaxima() > b.getPontuacaoMaxima();
                    });

                int y = 100;
                for (size_t i = 0; i < jog.size() && i < 10; i++) {
                    std::string texto = std::to_string(i + 1) + ". " + jog[i].getNome() + " (" + jog[i].getApelido() + ") - " +
                        std::to_string(jog[i].getPontuacaoMaxima()) + " pontos";
                    al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, y, ALLEGRO_ALIGN_CENTER, texto.c_str());
                    y += 30;
                }

                al_draw_text(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50,
                    ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar");
                al_flip_display();
                break;
            }

            case PLAYING: {
                al_clear_to_color(al_map_rgb(135, 206, 235));
                scenario.draw();
                bird.draw();

                std::string score_text = "Pontuação: " + std::to_string(scenario.getScore());
                al_draw_text(font, al_map_rgb(0, 0, 0), 10, 10, 0, score_text.c_str());

                al_flip_display();
                break;
            }

            case GAME_OVER: {
                al_clear_to_color(al_map_rgb(135, 206, 235));

                // Desenha o retângulo do game over
                int retangulo_Larg = 480;
                int retangulo_Alt = 400;
                int retangulo_X = (SCREEN_WIDTH - retangulo_Larg) / 2;
                int retangulo_Y = (SCREEN_HEIGHT - retangulo_Alt) / 2;

                al_draw_filled_rectangle(retangulo_X, retangulo_Y, retangulo_X + retangulo_Larg, retangulo_Y + retangulo_Alt, al_map_rgba(0, 0, 0, 240));
                al_draw_rectangle(retangulo_X, retangulo_Y, retangulo_X + retangulo_Larg, retangulo_Y + retangulo_Alt, al_map_rgb(255, 255, 255), 2);

                // Mensagens e Ranking
                std::string frase = "VOCÊ PERDEU! - Pontuação: " + std::to_string(scenario.getScore());
                al_draw_text(font, al_map_rgb(255, 0, 0), retangulo_X + retangulo_Larg / 2, retangulo_Y + 10, ALLEGRO_ALIGN_CENTER, frase.c_str());
                al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + retangulo_Larg / 2, retangulo_Y + 40, ALLEGRO_ALIGN_CENTER, "Ranking dos Jogadores");

                std::vector<Jogador> jog = cadastro_jogadores.getJogadores();
                std::sort(jog.begin(), jog.end(), [](Jogador& a, Jogador& b) {
                    return a.getPontuacaoMaxima() > b.getPontuacaoMaxima();
                    });

                if (!jog.empty()) {
                    std::string campeaoTexto = "GOAT: " + jog[0].getNome() + " (" + jog[0].getApelido() + ") - Pontos: " + std::to_string(jog[0].getPontuacaoMaxima()) + " (" + std::to_string(jog[0].getNumeroDeJogos()) + " partidas)";
                    al_draw_text(font, al_map_rgb(255, 215, 0), retangulo_X + retangulo_Larg / 2, retangulo_Y + 70, ALLEGRO_ALIGN_CENTER, campeaoTexto.c_str());

                    int qtde_jogadores = jog.size();
                    int linhaAltura = 12;
                    for (int i = 1; i < qtde_jogadores; i++) {
                        retangulo_Y += linhaAltura;
                        std::string texto = std::to_string(i + 1) + ". " + jog[i].getNome() + " (" + jog[i].getApelido() + ") - Pontos: " + std::to_string(jog[i].getPontuacaoMaxima()) + " (" + std::to_string(jog[i].getNumeroDeJogos()) + " partidas)";
                        al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + 20, retangulo_Y + 70 + i * linhaAltura + 10, ALLEGRO_ALIGN_LEFT, texto.c_str());
                    }
                }
                else {
                    al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + retangulo_Larg / 2, retangulo_Y + retangulo_Alt / 2, ALLEGRO_ALIGN_CENTER, "Nenhum jogador cadastrado");
                }
                al_draw_text(font, al_map_rgb(200, 200, 200), retangulo_X + retangulo_Larg / 2, retangulo_Y - 10, ALLEGRO_ALIGN_CENTER, "Pressione R para Reiniciar ou ESC para Menu");

                al_flip_display();
                break;
            }
            }
        }
    }

end_game:
    cadastro_jogadores.salvarJogadores();
    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;
}