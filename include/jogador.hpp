#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include "cartaEspecial.hpp"
#include "cartaNormal.hpp"
#include <vector>
#include <algorithm>

class Jogador{
	private:
		string nome;
		vector<Carta*>cartas;

	public:
		//Contrutor e Destrutor
		Jogador(string nome);
		~Jogador();

		
		bool temCartaValida(Carta* cartaDoLixo, int regraEmpilharCompraCarta);
		void adicionarCarta(Carta* cartaDaMesa);
		Carta* jogarCarta(Carta* cartaDoLixo, int regraEmpilharCompraCarta);
		int getNumeroDeCartas();
		string getNome();
		void limparCartas();
		void mostrarCartas();

		//acoes do jogador
		bool querComprar(Carta* cartaDoLixo);
		bool querPularTurno(Carta* cartaDoLixo);
		bool querFazerIsso(string fazerIsso);

		//verifica se o jogador tem alguma carta no baralho que possa ser jogada
		bool temCarta(string indice);

		void ajuda(); //explica as cartas do jogo
		void regras(); //explica as regras do jogo
};

#endif