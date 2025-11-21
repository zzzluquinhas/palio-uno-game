#include"jogador.hpp"
#include "excecoes.hpp"
#include <limits>

//Contrutor do jogador recebe apenas o nome do jogador
Jogador::Jogador(string nome){
	this->nome = nome;
}

//Deletar o jogador deleta suas cartas;
Jogador::~Jogador(){
	for(Carta* &c : cartas)
		delete c;
}

/*
Verifica se a carta jogada pelo jogador pode ser aceita na mesa, ou seja:
Se as duas cartas possuem a mesma cor
Se as duas cartas possuem o mesmo simbolo
Se a carta do jogador é uma carta coringa (aceita de qualquer forma)
*/

//Verifica se o jogador tem em seu baralho alguma carta que possa ser jogada na mesa
bool Jogador::temCartaValida(Carta* cartaDoLixo, int regraEmpilharCompraCarta){
	for(Carta* &cj : cartas){
		if(cj->ehCompativel(cartaDoLixo, regraEmpilharCompraCarta)){
			return true;
		}
	}
	return false;
}

//O jogador pega uma carta da mesa
void Jogador::adicionarCarta(Carta* cartaDaMesa){
	cartas.push_back(cartaDaMesa);
}

//O jogador joga uma carta na mesa
Carta* Jogador::jogarCarta(Carta* cartaDoLixo, int regraEmpilharCompraCarta){
	// RegraEmpilharCartaCompra:
	// 0: Não é possivel jogar uma carta de compra em resposta à outra
	// 1: Jogar uma carta de compra em resposta à outra, +4 não pode ser jogado em resposta à um +2
	// 2: Jogar uma carta de compra em resposta à outra, +4 pode ser jogado em resposta à um +2


	string indice; //Entrada do jogador
	// string palio("p"), PALIO("P"); //p ou P mostra que o jogador ficará com 1 carta
	// string chars = "p P"; //variavel utilizada pare remover "p","P"," "

	// bool falouPalio = false; //marca se o jogador falou palio
	bool verificadorDeEntrada; // Marca se a entrada do jogador é válida
	bool cartaValida; // Marca se a carta escolhida pelo jogador é válida

	//Se o jogador tiver cartas segue o jogo
	if(cartas.size() > 0){
		mostrarCartas();
		
		vector<Carta*>::iterator ith;
		

		do{
				ith = cartas.begin();

				//Pega qual carta o jogador deseja jogar (h = help)
				do{
					cout << "\n--------------------------------------------------------------------------------------------\n";
					cout << "\nQual carta deseja jogar? Digite o indice da carta, ou:" << endl;
					cout << "\th - ver as funcoes das cartas" << endl;
					cout << "\tr - ver as regras" << endl;

					cin >> indice;

					//verifica se o jogador gritou palio (está para finaliza o jogo)
					// transform(resposta.begin(), resposta.end(), resposta.begin(), ::tolower);
					// if(resposta.find(palio) != string::npos){
					// 	falouPalio = true;
					// 	for (char c: chars) {
					// 		resposta.erase(std::remove(resposta.begin(), resposta.end(), c), resposta.end());
					// 	}
					// }

					//verifica se o jogador entrou com uma variavel permitida
					if(indice == "h"){
						ajuda();
						verificadorDeEntrada = false;
					}else if(indice == "r"){
						regras();
						verificadorDeEntrada = false;
					}else{
						try{
							verificadorDeEntrada = temCarta(indice);
						} catch (const ValorCartaInvalido& vci){
							cout << vci.what() << endl;
							verificadorDeEntrada = false;
						} catch (const exception& e){
							cout << "Por favor insira um número!" << endl;
							verificadorDeEntrada = false;
						}
					}
					
				}while(!verificadorDeEntrada);
		
				for(int i=0; i<stoi(indice); i++){
					ith++;
				}

			try{
				if(!(*ith)->ehCompativel(cartaDoLixo, regraEmpilharCompraCarta)){
					throw CartaInvalida();
				}
				else{
					cartaValida = true;
				}

			} catch(const CartaInvalida& ci){
				cout << ci.what() << endl;
				cartaValida = false;
			}

		}while(!cartaValida);

		Carta* aux = *ith;
		cartas.erase(ith);

		//Se o jogador estiver com 2 cartas e jogar essa carta, ela precisa falar UNO
		// if(cartas.size() == 1 && falouPalio == false){
		// 	cout << "Voce esqueceu de gritar Palio, coma 2 cartas" << endl;
		// }

		return aux;
	}
	//EXCECAO numero de cartas não pode ser negativo
	throw NumeroNegativoCartas();

}

//Pega o numero de cartas do jogador
int Jogador::getNumeroDeCartas(){
	return cartas.size();
}

//Pega o nome do jogador
string Jogador::getNome(){
	return this->nome;
}

//Ao final do jogo, pega todas as cartas dos jogadores 
void Jogador::limparCartas(){
	for(Carta* &c : cartas){
		delete c;
	}
	cartas.clear();
}

void Jogador::mostrarCartas(){
	//Printa as cartas do jogador
	int indice = 0;
	int quebra = 0;
	for(Carta* &cj : cartas){
		cout << "  (" << indice << ")";
		if(indice < 10){
			cout << "  ";
		} else if(indice < 100){
			cout << " ";
		}
		if(cj->getID() < 10){
			CartaNormal* cn = (CartaNormal*) cj;
			cout << *cn;
		}
		else{
			CartaEspecial* ce;
			try{
				ce = (CartaEspecial*) cj;
			} catch (CartaNaoBranca cnb){
				ce->setCor(Cor::BRANCO);
			}
			cout << *ce;
		}
		cout << " ";
		indice++;
		quebra++;

		if(quebra > 8){
			cout << endl;
			quebra = 0;
		}
	}
	cout << endl;
}

bool Jogador::querComprar(Carta* cartaDoLixo){
	if(cartaDoLixo->getID() < 10){
		cout << "Carta do lixo: " << *((CartaNormal*) cartaDoLixo) << endl;
	}
	else{
		cout << "Carta do lixo: " << *((CartaEspecial*) cartaDoLixo) << endl;
	}
	mostrarCartas();
	return querFazerIsso("comprar uma carta");
}

bool Jogador::querPularTurno(Carta* cartaDoLixo){
	if(cartaDoLixo->getID() < 10){
		cout << "Carta do lixo: " << *((CartaNormal*) cartaDoLixo) << endl;
	}
	else{
		cout << "Carta do lixo: " << *((CartaEspecial*) cartaDoLixo) << endl;
	}
	mostrarCartas();
	return querFazerIsso("pular o seu turno");
}

bool Jogador::querFazerIsso(string fazerIsso){
	char resposta;
	bool respostaEhValida;
	cout << "Gostaria de " << fazerIsso << "? s/n" << endl;
	do{
		cin >> resposta;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		try{
			if(!(tolower(resposta) == 's') && !(tolower(resposta) == 'n')){
				throw RespostaInvalida();
			}
			else{
				respostaEhValida = true;
			}
		} catch(const RespostaInvalida& ri){
			cout << ri.what() << endl;
			respostaEhValida = false;
		}
	} while(!respostaEhValida);
	system("clear||cls");
	if(tolower(resposta) == 's'){
		return true;
	}
	return false;
}

bool Jogador::temCarta(string indice){
	if(!((stoi(indice) >= 0) && ((unsigned) stoi(indice)) < cartas.size())){
		throw ValorCartaInvalido();
	}
	return true;
}

void Jogador::ajuda(){
	cout << "\n--------------------------------------------------------------------------------------------\n";

	cout << "\n_________________" << endl;
	cout << "| CARTAS BASICAS |" << endl;
	cout << "__________________ \n" << endl;
	cout << "- Comprar Duas Cartas ( +2 ) - Quando esta carta for jogada, o proximo jogador deve " << endl;
	cout << "comprar 2 cartas e perde a vez. Ela apenas pode ser jogada sobre uma cor que combine." << endl;
	cout << "\n- Inverter ( S ) - Ao descartar esta carta, o sentido do jogo e invertido (se estiver " <<endl;
	cout << "indo para a esquera, muda para direita e vice-versa). Ela apenas pode ser jogada sobre " <<endl;
	cout << "uma cor que combine ou sobre outra carta Inverter." << endl;
	cout << "\n- Bloquear ( @ ) - Quando voce joga esta carta, o proximo jogador e 'pulado' (perde a vez)." << endl;
	cout << "Ela apenas pode ser jogada sobre uma cor que combine ou sobre outra carta Pular." <<endl;
	cout << "\n- Curinga - Ao jogar esta carta, voce escolhe a cor que continuara o jogo (pode ser " <<endl;
	cout << "qualquer cor, inclusive a que estava sendo jogada antes do Curinga)." << endl;
	cout << "\n- Comprar Quatro Cartas ( +4 ) - Ao jogar esta carta, voce pode escolher a cor a ser " << endl;
	cout << "jogada, alem de fazer com que o proximo jogador tenha que comprar 4 cartas da pilha de Compras, " << endl;
	cout << "perdendo tambem a vez.\n" << endl;

}

void Jogador::regras(){

	cout << "\n--------------------------------------------------------------------------------------------\n";

	cout << "\n______________________" << endl;
	cout << "| REGRAS DO JOGO PALIO |" << endl;
	cout << "________________________ \n" << endl;

	cout << "O conteudo do jogo consiste em 108 cartas de jogo, sendo:" << endl;
	cout << "\n- 19 Cartas Azuis - 0 a 9.\n- 19 Cartas Verdes - 0 a 9.\n- 19 Cartas Amarelas - 0 a 9." << endl;
	cout << "- 19 Cartas Vermelhas - 0 a 9.\n- 8 Cartas Comprar Duas Cartas - 2 de cada cor." << endl;
	cout <<  "- 8 Cartas Inverter - 2 de cada cor.\n- 8 Cartas Pular - 2 de cada cor." <<endl;
	cout << "- 4 Cartas Curinga. \n- 4 Cartas Curinga Comprar Quatro Cartas." << endl;
	cout << "\nO objetivo do PALIO e ser o primeiro jogador a ficar sem cartas na mao. " << endl;
	cout << "Para o efeito tera de jogar uma carta de cada vez, que corresponda ao " << endl;
	cout << "numero ou cor da carta jogada anteriormente. O jogo termina quando os jogadores " << endl;
	cout << "ficarem sem cartas na mao. " << endl;
	cout << "\nO jogo inicia-se tirando uma carta de cima da pilha. De seguida o jogador " << endl;
	cout << "a esquerda pode dar inicio ao jogo.\n" << endl;
	cout << "No modo de jogo personalizado voce pode optar por ativar ou desativar algumas" << endl;
	cout << "regras do jogo padrao." << endl;
	cout << "\nE possivel ativar ou desativar regras como:" << endl;
	cout << "Empilhar cartas de comprar (+2 em cima de +2, +4 em cima de +4 etc.)." << endl;
	cout << "Jogar carta depois de comprar no seu turno." << endl;
	cout << "Comprar carta sem precisar no seu turno." << endl;
	cout << "Pular ou nao o seu turno." << endl;
	cout << "Jogar com cartas padrão ou remover algumas cartas.\n" << endl;

}
