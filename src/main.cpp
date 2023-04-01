#include "menuInicial.hpp"


int main(int argc, char const *argv[]){
	// char repete;
		MenuInicial menu;
		menu.menu();
	// do{
	// 	cout << "Iniciou-se o jogo!" << endl;
	// 	cout << "Quantos jogadores vão jogar?" << endl;

	// 	cin >> nJog;
	// 	string nome;
	// 	vector<Jogador*> players;

	// 	for(int i=0; i<nJog; i++){
	// 		cout << "Digite o nome do " << i+1 << "º jogador: ";
	// 		cin >> nome;
	// 		Jogador* temp = new Jogador(nome);
	// 		players.push_back(temp);
	// 	}

	// 	system("cls||clear");

	// 	Mesa* jogo = new Mesa(players);
	// 	Jogador* ganhador = jogo->gerenciarPartida();
	// 	delete jogo;

	// 	cout << "Fim! " << ganhador->getNome() << " ganhou!" << endl;
	// 	// cout << "\n\n--------" << endl;
	// 	// cout << "Jogador" << "\t" << "Pontuação" << endl;
	// 	// for(auto it : placar)
	// 	// 	cout << it.first << "\t" << it.second << endl;

	// 	cout << "Jogar Novamente? (S/N)" << endl;
	// 	cin >> repete;
	// }while(tolower(repete)!='n');

	return 0;
}