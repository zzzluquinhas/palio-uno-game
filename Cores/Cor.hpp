#ifndef COR_HPP
#define COR_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Cor{
	enum Codigo{
		VERMELHO = 31,
		VERDE = 32,
		AMARELO = 33,
		AZUL = 34,
		BRANCO = 39,
	};

	vector<int> cores = {31,32,33,34,39};

	class Modificador{
		int codigo;
		public:
			Modificador(int _codigo) : codigo(_codigo) {}
			friend ostream& operator<<(ostream& os, const Modificador& mod){
				return os << "\033[1;" << mod.codigo << "m";
			}
	};
}

#endif