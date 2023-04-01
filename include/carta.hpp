#ifndef CARTA_HPP
#define CARTA_HPP

#include "cor.hpp"

class Carta{
	protected:
		int id;
		string descricaoCarta;
		Cor::Modificador cor;

	public:
		Carta();
		Carta(int id, int cod) : id(id), cor(cod) {};
		bool ehCompativel(Carta* c, int regraEmpilharCompraCarta);
		void setDescricaoCarta(string descricao);
		string getDescricaoCarta();
		int getID() const;
		int getCor() const;
		void setCor(int cor);
		bool operator==(const Carta& c);

};

#endif