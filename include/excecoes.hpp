#ifndef EXCECOES_HPP
#define EXCECOES_HPP
#include <string> 
#include "cartaEspecial.hpp"
using namespace std;

class NumeroNegativoCartas : public exception{
	public:
		virtual const char* what() const throw() {
			return "Número negativo de cartas. Impossível jogar cartas. Jogo encerrado!";
		}
};

class CartaNaoBranca {
};

class CartaInvalida : public exception{
	public:
		virtual const char* what() const throw() {
			return "Você não pode jogar essa carta!";
		}
};

class ValorCartaInvalido : public exception{
	public:
		virtual const char* what() const throw() {
			return "O número digitado da carta não está dentro dos valores válidos!";
		}
};

class RespostaInvalida: public exception{
	public:

		virtual const char* what() const throw() {
			return "Resposta inválida! Sua resposta deve ser 's' ou 'n'\n";
		}
};

class NumeroInvalidoJogadores: public exception{
	public:
		virtual const char* what() const throw() {
			return "Número inválido de jogadores! Digite um valor entre 2 a 6";
		}
		void mensagem(){
			std::cout << "Digite enter para continuar:";
			cin.ignore();
			cin.ignore();
		}
};

class ValorCorInvalido : public exception{
	public:
		virtual const char* what() const throw() {
			return "O número digitado da cor não está dentro dos valores válidos!";
		}
};

class OpcaoInvalida: public exception{
	public:

		virtual const char* what() const throw() {
			return "Opcao inválida!\n";
		}
};

#endif