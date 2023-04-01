#include "CartaExemplo.hpp"

int main(int argc, char const *argv[]){
	string simbolo;
	char cor;
	vector<Carta> deck;
	cout << "Número [0,9] cor (r g b y w) [s s para parar]" << endl;
	do{
		cin >> simbolo >> cor;
		switch(cor){
			case 'r':
				deck.push_back(Carta(simbolo, Cor::VERMELHO));
				break;

			case 'g':
				deck.push_back(Carta(simbolo, Cor::cores[1]));
				break;
	
			case 'b':
				deck.push_back(Carta(simbolo, 34));
				break;
	
			case 'y':
				deck.push_back(Carta(simbolo, Cor::cores[2]));
				break;
	
			case 'w':
				deck.push_back(Carta(simbolo, Cor::cores[4]));
				break;

			default:
				break;
		}
	}while(simbolo != "s" || cor != 's');

	for(Carta carta : deck)
		cout << carta << "\t";

	cout << endl;
	return 0;
}