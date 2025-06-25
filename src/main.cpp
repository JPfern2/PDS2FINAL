//  bibliotecas 
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

// nossas classes
#include "../include/bird.hpp"
#include "../include/scenario.hpp"
#include "../include/ListaDeJogadores.hpp"
#include "../include/jogador.hpp"

//  constantes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Estados do jogo
enum GameState {
    NICKNAME_INPUT,
    PLAYING,
    GAME_OVER,
    RANKING
};

int main() {
    //  iniciando allegro
    if (!al_init()) {
        return -1;
    }

    if (!al_init_primitives_addon()) {
        return -1;
    }

    if (!al_install_keyboard()) {
        return -1;
    }

    if (!al_init_font_addon()) {
        return -1;
    }

    if (!al_init_ttf_addon()) {
        return -1;
    }

    //  preparando allegro 
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        return -1;
    }

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

    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    //  preparando eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Variáveis do jogo
    GameState current_state = NICKNAME_INPUT;
    std::string player_nickname = "";
    std::string player_name = "";
    bool redraw = true;
    bool score_saved_this_game = false;
    
    // Criar o pássaro
    Bird bird(100, SCREEN_HEIGHT / 2);

    // Criar o cenário com gravidade e velocidade dos obstáculos
    Scenario scenario(0.5, 3.0);

    // Lista de jogadores
    ListaDeJogadores cadastro_jogadores("../assets/BaseJogadores.json");

    // Inicia o timer
    al_start_timer(timer);

    //  loop do jogo
    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //  eventos de timer
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (current_state == PLAYING) {
                //  atualiza ave
                bird.update();

                //  atualizando cenario e verificacao das colisoes 
                bool collision_detected = scenario.update(bird);

                // Verificar se houve colisão com os canos ou limites da tela
                if (collision_detected || bird.getY() < 0 || bird.getY() + 20 > SCREEN_HEIGHT) {
                    current_state = GAME_OVER;
                }
            }
            redraw = true;
        }

        //  eventos do teclado
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (current_state == NICKNAME_INPUT) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER && !player_nickname.empty()) {
                    // Usar o apelido como nome também se não especificado
                    if (player_name.empty()) {
                        player_name = player_nickname;
                    }
                    current_state = PLAYING;
                    score_saved_this_game = false;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && !player_nickname.empty()) {
                    player_nickname.pop_back();
                }
                else if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z) {
                    if (player_nickname.length() < 15) { // Limite de 15 caracteres
                        char c = 'A' + (ev.keyboard.keycode - ALLEGRO_KEY_A);
                        if (!(ev.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT)) {
                            c = c - 'A' + 'a'; // Converter para minúscula
                        }
                        player_nickname += c;
                    }
                }
                else if (ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9) {
                    if (player_nickname.length() < 15) {
                        char c = '0' + (ev.keyboard.keycode - ALLEGRO_KEY_0);
                        player_nickname += c;
                    }
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    if (player_nickname.length() < 15 && !player_nickname.empty()) {
                        player_nickname += ' ';
                    }
                }
            }
            else if (current_state == PLAYING) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    bird.jump(); // Faz o pássaro pular
                }
            }
            else if (current_state == GAME_OVER) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
                    //  restarte 
                    bird = Bird(100, SCREEN_HEIGHT / 2); // Recriar o pássaro
                    scenario.reset(); // Resetar o cenário
                    current_state = PLAYING;
                    score_saved_this_game = false;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_N) {
                    // Novo jogador
                    player_nickname = "";
                    player_name = "";
                    bird = Bird(100, SCREEN_HEIGHT / 2);
                    scenario.reset();
                    current_state = NICKNAME_INPUT;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_T) {
                    // Ver ranking completo
                    current_state = RANKING;
                }
            }
            else if (current_state == RANKING) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    current_state = GAME_OVER;
                }
            }
        }

        //  fechou janela
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        //  renderizacao 
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            //  limpa a tela
            al_clear_to_color(al_map_rgb(135, 206, 235));

            if (current_state == NICKNAME_INPUT) {
                // Tela de entrada de apelido
                int box_width = 400;
                int box_height = 200;
                int box_x = (SCREEN_WIDTH - box_width) / 2;
                int box_y = (SCREEN_HEIGHT - box_height) / 2;

                // Fundo da caixa
                al_draw_filled_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgba(0, 0, 0, 200));
                al_draw_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgb(255, 255, 255), 2);

                // Título
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, box_y + 20, ALLEGRO_ALIGN_CENTER, "FLAPPY BIRD");
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, box_y + 50, ALLEGRO_ALIGN_CENTER, "Digite seu apelido:");

                // Campo de entrada
                int input_x = box_x + 20;
                int input_y = box_y + 80;
                int input_width = box_width - 40;
                int input_height = 30;

                al_draw_filled_rectangle(input_x, input_y, input_x + input_width, input_y + input_height, al_map_rgb(255, 255, 255));
                al_draw_rectangle(input_x, input_y, input_x + input_width, input_y + input_height, al_map_rgb(0, 0, 0), 1);

                // Texto digitado
                std::string display_text = player_nickname;
                if (display_text.length() > 20) {
                    display_text = display_text.substr(display_text.length() - 20);
                }
                al_draw_text(font, al_map_rgb(0, 0, 0), input_x + 5, input_y + 8, 0, display_text.c_str());

                // Cursor piscando
                static int cursor_timer = 0;
                cursor_timer++;
                if ((cursor_timer / 30) % 2 == 0) { // Pisca a cada meio segundo
                    int text_width = al_get_text_width(font, display_text.c_str());
                    al_draw_line(input_x + 5 + text_width, input_y + 5, input_x + 5 + text_width, input_y + 25, al_map_rgb(0, 0, 0), 1);
                }

                // Instruções
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, box_y + 130, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para começar");
                al_draw_text(font, al_map_rgb(150, 150, 150), SCREEN_WIDTH / 2, box_y + 150, ALLEGRO_ALIGN_CENTER, "Máximo 15 caracteres");
            }
            else if (current_state == PLAYING) {
                //  renderiza obstaculos 
                scenario.draw();

                //  renderiza o passaro
                bird.draw();

                //  renderiza pontuacao
                std::string score_text = "Pontuação: " + std::to_string(scenario.getScore());
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, score_text.c_str());

                // Mostra apelido do jogador
                std::string player_text = "Jogador: " + player_nickname;
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 30, 0, player_text.c_str());

                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT - 30, ALLEGRO_ALIGN_CENTER, "Espaço para pular!");
            }
            else if (current_state == GAME_OVER) {
                //  renderiza obstaculos 
                scenario.draw();

                //  renderiza o passaro
                bird.draw();

                // Salvar pontuação do jogador
                if (!score_saved_this_game) {
                    int final_score = scenario.getScore();
                    
                    // Criar ou atualizar jogador
                    Jogador jogador_atual(player_name, player_nickname, final_score, 1);
                    cadastro_jogadores.cadastrarJogador(jogador_atual);
                    cadastro_jogadores.salvarJogadores();
                    score_saved_this_game = true;
                }

                //Ranking Game Over
                int retangulo_Larg = 480;
                int retangulo_Alt = 400;
                int retangulo_X = (SCREEN_WIDTH - retangulo_Larg) / 2;
                int retangulo_Y = (SCREEN_HEIGHT - retangulo_Alt) / 2;

                //Background
                al_draw_filled_rectangle(retangulo_X, retangulo_Y, retangulo_X + retangulo_Larg, retangulo_Y + retangulo_Alt, al_map_rgba(0, 0, 0, 240));
                al_draw_rectangle(retangulo_X, retangulo_Y, retangulo_X + retangulo_Larg, retangulo_Y + retangulo_Alt, al_map_rgb(255, 255, 255), 2);

                // Mensagens e  Ranking
                al_draw_text(font, al_map_rgb(255, 0, 0), retangulo_X + retangulo_Larg / 2, retangulo_Y + 10, ALLEGRO_ALIGN_CENTER, "VOCÊ PERDEU!");
                
                std::string score_final = "Sua pontuação: " + std::to_string(scenario.getScore());
                al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + retangulo_Larg / 2, retangulo_Y + 30, ALLEGRO_ALIGN_CENTER, score_final.c_str());
                
                al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + retangulo_Larg / 2, retangulo_Y + 60, ALLEGRO_ALIGN_CENTER, "Top 5 Jogadores");

                std::vector<Jogador> jog = cadastro_jogadores.getJogadores();
                std::sort(jog.begin(), jog.end(), [](const Jogador& a, const Jogador& b) {
                    return a.getPontuacaoMaxima() > b.getPontuacaoMaxima();
                });

                if (!jog.empty()) {
                    int max_display = std::min(5, (int)jog.size());
                    
                    for (int i = 0; i < max_display; i++) {
                        std::string texto = std::to_string(i + 1) + ". " + jog[i].getApelido() + " - " + std::to_string(jog[i].getPontuacaoMaxima()) + " pts";
                        ALLEGRO_COLOR cor = (i == 0) ? al_map_rgb(255, 215, 0) : al_map_rgb(255, 255, 255); // Dourado para o primeiro
                        al_draw_text(font, cor, retangulo_X + 20, retangulo_Y + 90 + i * 20, ALLEGRO_ALIGN_LEFT, texto.c_str());
                    }
                }
                else {
                    al_draw_text(font, al_map_rgb(255, 255, 255), retangulo_X + retangulo_Larg / 2, retangulo_Y + 120, ALLEGRO_ALIGN_CENTER, "Nenhum jogador cadastrado");
                }
                
                // Opções
                al_draw_text(font, al_map_rgb(200, 200, 200), retangulo_X + retangulo_Larg / 2, retangulo_Y + retangulo_Alt - 80, ALLEGRO_ALIGN_CENTER, "R - Jogar novamente");
                al_draw_text(font, al_map_rgb(200, 200, 200), retangulo_X + retangulo_Larg / 2, retangulo_Y + retangulo_Alt - 60, ALLEGRO_ALIGN_CENTER, "N - Novo jogador");
                al_draw_text(font, al_map_rgb(200, 200, 200), retangulo_X + retangulo_Larg / 2, retangulo_Y + retangulo_Alt - 40, ALLEGRO_ALIGN_CENTER, "T - Ver ranking completo");
                
            }
            else if (current_state == RANKING) {
                // Tela de ranking completo
                al_clear_to_color(al_map_rgb(50, 50, 50));
                
                int box_width = 600;
                int box_height = 500;
                int box_x = (SCREEN_WIDTH - box_width) / 2;
                int box_y = (SCREEN_HEIGHT - box_height) / 2;

                al_draw_filled_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgba(0, 0, 0, 240));
                al_draw_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgb(255, 255, 255), 2);

                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, box_y + 20, ALLEGRO_ALIGN_CENTER, "RANKING COMPLETO - TOP 10");

                std::vector<Jogador> jog = cadastro_jogadores.getJogadores();
                std::sort(jog.begin(), jog.end(), [](const Jogador& a, const Jogador& b) {
                    return a.getPontuacaoMaxima() > b.getPontuacaoMaxima();
                });

                if (!jog.empty()) {
                    int max_display = std::min(10, (int)jog.size());
                    
                    for (int i = 0; i < max_display; i++) {
                        std::string texto = std::to_string(i + 1) + ". " + jog[i].getApelido() + " - " + std::to_string(jog[i].getPontuacaoMaxima()) + " pts (" + std::to_string(jog[i].getNumeroDeJogos()) + " jogos)";
                        ALLEGRO_COLOR cor;
                        if (i == 0) cor = al_map_rgb(255, 215, 0); // Ouro
                        else if (i == 1) cor = al_map_rgb(192, 192, 192); // Prata
                        else if (i == 2) cor = al_map_rgb(205, 127, 50); // Bronze
                        else cor = al_map_rgb(255, 255, 255); // Branco
                        
                        al_draw_text(font, cor, box_x + 20, box_y + 60 + i * 25, ALLEGRO_ALIGN_LEFT, texto.c_str());
                    }
                }
                else {
                    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, box_y + 120, ALLEGRO_ALIGN_CENTER, "Nenhum jogador cadastrado");
                }

                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, box_y + box_height - 30, ALLEGRO_ALIGN_CENTER, "Pressione ENTER ou ESC para voltar");
            }

            al_flip_display();
        }
    }

    //  limpa
    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    cadastro_jogadores.salvarJogadores();


    return 0;
}

