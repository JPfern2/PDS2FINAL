//  bibliotecas 
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
// nossas classes
#include "../include/bird.hpp"
#include "../include/scenario.hpp"

//  constantes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

    // Criar o pássaro
    Bird bird(100, SCREEN_HEIGHT / 2);

    // Criar o cenário com gravidade e velocidade dos obstáculos
    Scenario scenario(0.5, 3.0);

    bool game_over = false;
    bool redraw = true;

    // Inicia o timer
    al_start_timer(timer);

    //  loop do jogo
    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //  eventos de timer
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (!game_over) {
                //  atualiza ave
                bird.update();

                //  atualizando cenario e verificacao das colisoes 
                bool collision_detected = scenario.update(bird);

                // Verificar se houve colisão com os canos ou limites da tela
                if (collision_detected || bird.getY() < 0 || bird.getY() + 20 > SCREEN_HEIGHT) {
                    game_over = true;
                }
            }
            redraw = true;
        }

        //  eventos do teclado
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && !game_over) {
                bird.jump(); // Faz o pássaro pular
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_R && game_over) {
                //  restarte 
                bird = Bird(100, SCREEN_HEIGHT / 2); // Recriar o pássaro
                scenario.reset(); // Resetar o cenário
                game_over = false;
            }
        }

        //  fechou janela
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        //  renderizacao 
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            //  llimpa a tela
            al_clear_to_color(al_map_rgb(135, 206, 235));

            //  renderiza obstaculos 
            scenario.draw();

            //  renderiza o passaro
            bird.draw();

            //  renderiza pontuacao
            std::string score_text = "Pontuação: " + std::to_string(scenario.getScore());
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, score_text.c_str());

            //  Mostra mensagens
            if (game_over) {
                al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTER, "It's over baby!");
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "R para Reiniciar");
            }
            else {
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2,
                    SCREEN_HEIGHT - 30, ALLEGRO_ALIGN_CENTER, "Espaço para pular!");
            }

            al_flip_display();
        }
    }

    //  limpa
    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);

    return 0;
}