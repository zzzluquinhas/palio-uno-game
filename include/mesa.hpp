#ifndef MESA_HPP
#define MESA_HPP

#define JOGADA_NORMAL 0

#include <map>
#include <random>
#include <algorithm>

#include "cartaNormal.hpp"
#include "cartaEspecial.hpp"
#include "jogador.hpp"
#include "regra.hpp"
#include "excecoes.hpp"

/*
	**CARTAS DO JOGO PADRAO**
	76 cartas numericas (19 vermelhas, 19 verdes, 19 azuis, 19 amarelas)
	4 +4
	4 troca de cor
	8 +2 (2 vermelhas, 2 verdes, 2 azuis, 2 amarelas)
	8 Inverter (2 vermelhas, 2 verdes, 2 azuis, 2 amarelas)
	8 Bloqueio (2 vermelhas, 2 verdes, 2 azuis, 2 amarelas)
	(108 no total)
*/
class Mesa{
	private:
		bool invertido;
		int numCartasBaralho;
		unsigned int posJogadorAtual;
		vector<Jogador*> jogadores;
		vector<Carta*> baralho;
		vector<Carta*> lixo;
		vector<Regra*> regras;

	public:
		~Mesa();
		Mesa(vector<Jogador*> jogadores, vector<Regra*> regras);
		
		int getNumCartasBaralho();
		bool regraTaAtivada(string nomeRegra);
		
		Jogador* gerenciarPartida();
		void printarInfo();
		void gerenciarTurno(Jogador* jogadorAtual);
		void receberCartaDoJogador(Jogador* jogador, Carta* ultimaDoLixo);
		void moverProLixo(Carta* cartaJogada);
		int proxJogador();

		void embaralhar();
		bool jogadorQuerCarta(Jogador* jogador, Carta* ultimaDoLixo);
		void distribuirCarta(Jogador* jogador, Carta* carta);
		void colocarLixoNoBaralho();

		void efeitoBloquear();
		void efeitoReverter();
		void regraEmpilharCompraCarta(int numCartas, Jogador* jogadorCompra);
		void efeitoComprarCarta(int numCartas, Jogador* jogadorCompra);
		void efeitoMudarCor();
};

#endif