#include "../include/jogador.hpp"
#include "../include/ListaDeJogadores.hpp"
#include <iostream>
#include <fstream>
#include "../include/json.hpp"

using json = nlohmann::json;

ListaDeJogadores::ListaDeJogadores(std::string nome_arquivo) : nome_arquivo(nome_arquivo) { carregarJogadores(); }

ListaDeJogadores::~ListaDeJogadores() {
    salvarJogadores();
}

void ListaDeJogadores::cadastrarJogador(Jogador& jogador) {
    for (auto iterador : jogadores){
        if (iterador.getApelido() == jogador.getApelido()) {
            std::cout << "Jogador com apelido '" << jogador.getApelido() << "' já existe." << std::endl;
            return;
        }
    }
    jogadores.push_back(jogador);
}

void ListaDeJogadores::removerJogador(const std::string& apelido) {
    for (auto iterador = jogadores.begin(); iterador != jogadores.end();) {
        if (iterador->getApelido() == apelido) {
            iterador = jogadores.erase(iterador);
        }else{
            iterador++;
        }
    }
}

void ListaDeJogadores::listarJogadores() {
    for (auto iterador = jogadores.begin(); iterador != jogadores.end(); iterador++) {
        std::cout << "Nome: " << iterador->getNome() << ", Apelido: " << iterador->getApelido()
                  << ", Pontuacao Maxima: " << iterador->getPontuacaoMaxima()
                  << ", Numero de Jogos: " << iterador->getNumeroDeJogos() << std::endl;
    }
}

void ListaDeJogadores::carregarJogadores() {
    std::ifstream arquivo("../assets/" + nome_arquivo);
    
    if (arquivo.is_open()) {

        //Confeir se o JSON está vazio
        arquivo.seekg(0, std::ios::end);
        size_t tamanho = arquivo.tellg();
        arquivo.seekg(0);

        json j;

        try {
            if (tamanho > 0) {
                arquivo >> j;
            } else {
                j = json::array();
            }

            jogadores.clear();

            for (auto& iterador : j) {
                Jogador auxiliar(iterador["nome"], iterador["apelido"], iterador["pontuacao_maxima"], iterador["numero_de_jogos"]);
                jogadores.push_back(auxiliar);
            }
        } catch (const std::exception& e) {
           //Adcionar exceção
        }
    }
}

void ListaDeJogadores::salvarJogadores() {
    json j;
    for (auto& jogador : jogadores) {
        j.push_back({
            {"nome", jogador.getNome()},
            {"apelido", jogador.getApelido()},
            {"pontuacao_maxima", jogador.getPontuacaoMaxima()},
            {"numero_de_jogos", jogador.getNumeroDeJogos()}
        });
    }
    std::ofstream arquivo("../assets/" + nome_arquivo);
    if (arquivo.is_open()) {
        arquivo << j.dump(4);
    }
}

std::string ListaDeJogadores::maiorPontuador() {
    if (jogadores.empty()) {
        return "Nenhum jogador cadastrado.";
    }

    Jogador* maior = &jogadores[0];
    for (auto& jogador : jogadores) {
        if (jogador.getPontuacaoMaxima() > maior->getPontuacaoMaxima()) {
            maior = &jogador;
        }
    }
    return maior->getNome() + " (" + maior->getApelido() + ") com " + std::to_string(maior->getPontuacaoMaxima()) + " pontos.";
}
