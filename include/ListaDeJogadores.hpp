#ifndef LISTA_DE_JOGADORES_HPP
#define LISTA_DE_JOGADORES_HPP

#include <iostream>
#include <string>
#include <vector>
#include "jogador.hpp"

class ListaDeJogadores {

private:
    std::vector<Jogador> jogadores;
    std::string nome_arquivo;

public:
    ListaDeJogadores(std::string nome_arquivo);

    void cadastrarJogador(Jogador& jogador);
    void removerJogador(const std::string& apelido);
    void listarJogadores();
    std::vector<Jogador> getJogadores() const;
    std::string maiorPontuador();
    void carregarJogadores();
    void salvarJogadores();

    ~ListaDeJogadores();

};

#endif