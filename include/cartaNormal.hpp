#ifndef CARTANORMAL_HPP
#define CARTANORMAL_HPP

#include "carta.hpp"

class CartaNormal : public Carta{
	protected:
		int numero;

	public:
		CartaNormal(int numero, int cor) : Carta(numero, cor), numero(numero) {};
		int getNumero() const;

	friend ostream& operator << (ostream& os, CartaNormal& c){
		return os << Cor::Modificador(c.getCor()) << "|  " << c.getNumero() << "  |" << "\033[0m";
	}
};


#endif