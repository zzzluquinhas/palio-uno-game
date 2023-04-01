#ifndef CARTAESPECIAL_HPP
#define CARTAESPECIAL_HPP

#include "carta.hpp"

class CartaEspecial : public Carta{
	protected:
		string funcao;

	public:
		CartaEspecial(int id, int cor);
		string getFuncao() const;
		
	friend ostream& operator << (ostream& os, const CartaEspecial& c){
		if(c.id == 12 || c.id == 14)
			return os << Cor::Modificador(c.getCor()) << "| " << c.getFuncao() << "  |" << "\033[0m";
		return os << Cor::Modificador(c.getCor()) << "|  " << c.getFuncao() << "  |" << "\033[0m";

	}
};


#endif