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
    auto it = std::find_if(jogadores.begin(), jogadores.end(),
                           [&](const Jogador& j) { return j.getApelido() == jogador.getApelido(); });

    if (it != jogadores.end()) {
        // Jogador existente, atualizar
        if (jogador.getPontuacaoMaxima() > it->getPontuacaoMaxima()) {
            it->setPontuacaoMaxima(jogador.getPontuacaoMaxima());
        }
        it->increaseNumeroDeJogos();
    } else {
        // Novo jogador, adicionar
        jogadores.push_back(jogador);
    }
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
    std::ifstream arquivo_leitura(nome_arquivo);
    json j;

    if (arquivo_leitura.is_open()) {
        // Verificar se o arquivo não está vazio antes de tentar parsear
        arquivo_leitura.seekg(0, std::ios::end);
        if (arquivo_leitura.tellg() > 0) {
            arquivo_leitura.seekg(0, std::ios::beg);
            try {
                arquivo_leitura >> j;
            } catch (const std::exception& e) {
                std::cerr << "Erro ao carregar JSON: " << e.what() << std::endl;
                j = json::array(); // Se houver erro, inicializa como array vazio
            }
        } else {
            j = json::array(); // Arquivo vazio, inicializa como array vazio
        }
        arquivo_leitura.close();
    } else {
        // Arquivo não existe, criar um novo arquivo JSON vazio
        std::ofstream arquivo_escrita(nome_arquivo);
        if (arquivo_escrita.is_open()) {
            arquivo_escrita << json::array().dump(4); // Escreve um array JSON vazio
            arquivo_escrita.close();
        }
        j = json::array(); // Inicializa como array vazio
    }

    jogadores.clear();
    for (auto& iterador : j) {
        Jogador auxiliar(iterador["nome"], iterador["apelido"], iterador["pontuacao_maxima"], iterador["numero_de_jogos"]);
        jogadores.push_back(auxiliar);
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
    std::ofstream arquivo(nome_arquivo);
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

std::vector<Jogador> ListaDeJogadores::getJogadores() const {
    return jogadores;
}
