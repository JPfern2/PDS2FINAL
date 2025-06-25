#include "../include/jogador.hpp"

Jogador::Jogador(const std::string& _nome, const std::string& _apelido, int _pontuacao_maxima, int _numero_de_jogos)
    : nome(_nome), apelido(_apelido), pontuacao_maxima(_pontuacao_maxima), numero_de_jogos(_numero_de_jogos) {
}

std::string Jogador::getNome() const { return nome; }
std::string Jogador::getApelido() const { return apelido; }
int Jogador::getPontuacaoMaxima() const { return pontuacao_maxima; }
int Jogador::getNumeroDeJogos() const { return numero_de_jogos; }

void Jogador::setPontuacaoMaxima(int nova_pontuacao) {
    if (nova_pontuacao > pontuacao_maxima) {
        pontuacao_maxima = nova_pontuacao;
    }
}

void Jogador::increaseNumeroDeJogos() { numero_de_jogos++; }