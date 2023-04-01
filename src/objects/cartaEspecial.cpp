#include "cartaEspecial.hpp"
#include "excecoes.hpp"

CartaEspecial::CartaEspecial(int id, int cor) : Carta(id, cor){
	// EXCEÇÃO: cartas de id 13 e 14 devem ser criadas obrigatoriamente na cor BRANCA
	// (se receber outra cor, pode só mudar pra branco e seguir a vida)
	// OBS: elas podem ter a cor alterada no setCor() (pq o jogador escolhe a cor dps)
	switch(id){
		case 10:
			this->funcao = "@"; //Bloqueio
			break;

		case 11:
			this->funcao = "S"; //Inverter
			break;

		case 12:
			this->funcao = "+2"; //Come 2 cartas do baralho
			break;

		case 13:
			if(this->getCor() != Cor::BRANCO)
				throw CartaNaoBranca();
			this->funcao = "C"; //Troca de cor
			break;

		case 14:
			if(this->getCor() != Cor::BRANCO)
				throw CartaNaoBranca();
			this->funcao = "+4"; //Troca de cor e come 4 cartas
			break;
	}
}

//retorna a funcao da carta especial
string CartaEspecial::getFuncao() const{
	return this->funcao;
}