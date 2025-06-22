#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include <iostream>
#include <string>

class Jogador{

private:
    std::string nome;
    std::string apelido;
    int pontuacao_maxima;
    int numero_de_jogos;

public:
    Jogador(const std::string& _nome, const std::string& _apelido, int _pontuacao_maxima, int _numero_de_jogos);

    std::string getNome();
    std::string getApelido();
    int getPontuacaoMaxima();
    int getNumeroDeJogos();

    void setPontuacaoMaxima(int nova_pontuacao);
    void increaseNumeroDeJogos();

};

#endif