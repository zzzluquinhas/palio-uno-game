#ifndef CARTA_H
#define CARTA_H

#include "Cor.hpp"

class Carta{
	public:
		string simbolo;
		Cor::Modificador cor;
		Carta(string simbolo, int cod) : simbolo(simbolo), cor(cod){}
};

ostream& operator << (ostream& os, const Carta& c){
	return os << c.cor << "|  " << c.simbolo << "  |" << "\033[0m";
}

#endif