#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::vector;

namespace Cor {
    enum Code {
        VERMELHO = 31,
        VERDE = 32,
        AMARELO = 33,
        AZUL = 34,
        BRANCO  = 39,
        // VERMELHO = 'r',
        // VERDE = 'g',
        // AMARELO = 'y',
        // AZUL = 'b',
        // PADRAO  = 'w',
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[1;" << mod.code << "m";
        }
    };
}

class Carta{
	public:
		char numero;
		Cor::Modifier cor;
		Carta(char numero, Cor::Code cod) : numero(numero), cor(cod){}
		friend ostream& operator << (ostream& os, const Carta& c){
			return os << c.cor << "| +" << c.numero << "  |" << "\033[0m";
		}
};


int main(int argc, char const *argv[]){
	char numero, cor;
	vector<Carta> deck;
	cout << "Número [0,9] cor (r g b y w) [s s para parar]" << endl;
	do{
		cin >> numero >> cor;
		switch(cor){
			case 'r':
				deck.push_back(Carta(numero, Cor::VERMELHO));
				break;
	
			case 'g':
				deck.push_back(Carta(numero, Cor::VERDE));
				break;
	
			case 'b':
				deck.push_back(Carta(numero, Cor::AZUL));
				break;
	
			case 'y':
				deck.push_back(Carta(numero, Cor::AMARELO));
				break;
	
			case 'w':
				deck.push_back(Carta(numero, Cor::BRANCO));
				break;

			default:
				break;
		}
	}while(numero != 's' || cor != 's');

	for(Carta carta : deck)
		cout << carta << "\t";

	cout << endl;
	return 0;
}