#ifndef MENUINICIAL_HPP
#define MENUINICIAL_HPP
#include "menuAjuda.hpp"
#include "jogador.hpp"
#include "mesa.hpp"
#include <iostream>
#include <vector>
#include "regra.hpp"
#include <limits>

class MenuInicial{
	private:
		menuAjuda ajuda;
		vector<Regra*> regras;

	public:
		MenuInicial();
		void menu();
		void pedirAjuda ();
		void iniciar ();
		void escolherRegras(int modoJogo);
		void ativarRegra(string nomeRegra, string descricao, string complemento);
		void regrasEmUso();
};

#endif 
