#include "mesa.hpp"

static vector<int> cores = {Cor::Codigo::VERMELHO,Cor::Codigo::VERDE,Cor::Codigo::AMARELO,Cor::Codigo::AZUL,Cor::Codigo::BRANCO};

//Limpa a memória da mesa
Mesa::~Mesa(){
	for(Carta* &c : this->baralho){
		delete c;
	}
	for(Carta* &c : this->lixo){
		delete c;
	}
	for(Jogador* &j : this->jogadores){
		j->limparCartas();
	}
}

// Inicializa a mesa
// @param jogadores Vetor de jogadores a ser inicializado na mesa
// @param regras Vetor de regras do jogo
Mesa::Mesa(vector<Jogador*> jogadores, vector<Regra*> regras){
	// Inicializar os jogadores
	this->jogadores = jogadores;

	// Inicializar as regras
	this->regras = regras;

	// Inicializar as cartas
	Carta* carta;
	for(int cartasCriadas = 0; cartasCriadas < 4; cartasCriadas++){
		int id = 0;
		// Inicializar as cartas numeradas 0
		if(!regraTaAtivada("TirarCarta" + to_string(id))){
			carta = new CartaNormal(id, cores[cartasCriadas]);
			this->baralho.push_back(carta);
			this->numCartasBaralho++;
		}

		// Inicializar o restante das cartas coloridas 2 vezes
		for(int numVezesCriada = 0; numVezesCriada < 2; numVezesCriada++){
			id = 1;
			// Cartas numeradas de 1 a 9
			for(; id < 10; id++){
				if(!regraTaAtivada("TirarCarta" + to_string(id))){
					carta = new CartaNormal(id, cores[cartasCriadas]);
					this->baralho.push_back(carta);
					this->numCartasBaralho++;
				}
			}

			// Especiais coloridas
			for(; id < 13; id++){
				if(!regraTaAtivada("TirarCarta" + to_string(id))){
					carta = new CartaEspecial(id, cores[cartasCriadas]);
					this->baralho.push_back(carta);
					this->numCartasBaralho++;
				}
			}
		}

		// Inicializar as cartas especiais sem cor
		for(; id < 15; id++){
			if(!regraTaAtivada("TirarCarta" + to_string(id))){
				carta = new CartaEspecial(id, Cor::BRANCO);
				this->baralho.push_back(carta);
				this->numCartasBaralho++;
			}
		}
	}

	// Embaralhar
	embaralhar();

	// Distribuir as cartas para todos os jogadores
	for(int i = 0; i < 7; i++){
		for(Jogador* j : this->jogadores){
			distribuirCarta(j, this->baralho.back());
		}
	}

	// Distribuir uma carta para o lixo
	bool continuar = false;
	vector<Carta*> aux;
	do{
		moverProLixo(this->baralho.back());
		this->baralho.pop_back();
		if(lixo.back()->getID() > 9){
			aux.push_back(lixo.back());
			lixo.pop_back();
		} else{
			continuar = true;
		}
	} while (!continuar);
	while(aux.size() > 0){
		baralho.push_back(aux.back());
		aux.pop_back();
	}
	this->numCartasBaralho--;

	// Escolher um jogador para começar o jogo
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(0, (this->jogadores.size() - 1));
	this->posJogadorAtual = distr(gen);
}

// Retorna o número de cartas do baralho
int Mesa::getNumCartasBaralho(){
	return this->numCartasBaralho;
}

// Verifica se a regra ta ativada
// @param nomeRegra Nome da regra a ser verificada
bool Mesa::regraTaAtivada(string nomeRegra){
	for(Regra* r : this->regras){
		if(r->getNome() == nomeRegra){
			return true;
		}
	}
	return false;
}

// Cuida do estado da partida e retorna o ganhador
Jogador* Mesa::gerenciarPartida(){
	while(true){
		for(Jogador* j : this->jogadores){
			if(j->getNumeroDeCartas() == 0){
				return j;
			}
		}
		cout << this->jogadores[this->posJogadorAtual]->getNome() <<", digite enter para continuar:";
		cin.ignore();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		printarInfo();
		gerenciarTurno(this->jogadores[this->posJogadorAtual]);
	}
}

void Mesa::printarInfo(){
	system("cls||clear");
	cout << "Número de cartas dos jogadores:" << endl;
	for(Jogador* j : this->jogadores){
		cout << j->getNome() << ": " << j->getNumeroDeCartas() << "\t";
	}
	cout << endl;
	cout << "\nPróximo jogador: " << this->jogadores[proxJogador()]->getNome() << endl;
	cout << "Jogador atual: " << this->jogadores[this->posJogadorAtual]->getNome() << endl;
}

// Gerencia o turno do jogador
// @param jogador Jogador que esta jogando agora
void Mesa::gerenciarTurno(Jogador* jogadorAtual){
	bool cartaComprada = false;
	while(jogadorQuerCarta(jogadorAtual, this->lixo.back())){
		colocarLixoNoBaralho();
		if(this->baralho.size() > 0){
			Carta* ultimaDoBaralho = this->baralho.back();
			distribuirCarta(jogadorAtual, ultimaDoBaralho);
			system("cls||clear");
			cout << jogadorAtual->getNome() << " comprou uma carta!" << endl;
			cartaComprada = true;
		}
		else{
			cout << "O baralho não tem mais cartas para voce comprar" << endl;
		}
	}
	Carta* ultimaDoLixo = this->lixo.back();
	if(cartaComprada == false || regraTaAtivada("JogarDepoisDeComprar")){
		if(jogadorAtual->temCartaValida(ultimaDoLixo, JOGADA_NORMAL)){
			if(!regraTaAtivada("PularTurno")){
				receberCartaDoJogador(jogadorAtual, ultimaDoLixo);
			}
			else if(!(jogadorAtual->querPularTurno(ultimaDoLixo))){
				receberCartaDoJogador(jogadorAtual, ultimaDoLixo);
			}
		}
		else{
			system("cls||clear");
			cout << "Jogador pulou o turno" << endl;
		}
	}
	this->posJogadorAtual = proxJogador();
}

// Recebe a carta jogada pelo jogador e coloca ela no lixo
// @param jogador Jogador que vai jogar a carta
// @param ultimaDoLixo Ultima carta do lixo
void Mesa::receberCartaDoJogador(Jogador* jogador, Carta* ultimaDoLixo){
	// Imprime a ultima carta do lixo
	cout << "\n--------------------------------------------------------------------------------------------\n";
	if(ultimaDoLixo->getID() < 10){
		cout << "\nCarta do lixo: " << *((CartaNormal*) ultimaDoLixo) << endl;
		cout << "\n";
	}
	else{
		cout << "\nCarta do lixo: " << *((CartaEspecial*) ultimaDoLixo) << endl;
		cout << "\n";
	}
	Carta* cartaJogador = jogador->jogarCarta(ultimaDoLixo, JOGADA_NORMAL);

	moverProLixo(cartaJogador);

	// Chama as funções das cartas especiais
	switch (cartaJogador->getID()){
	case 10:
		system("cls||clear");
		efeitoBloquear();
		break;
	case 11:
		system("cls||clear");
		efeitoReverter();
		break;
	case 12:
		system("cls||clear");
		efeitoBloquear();
		if(!regraTaAtivada("EmpilharComprarCarta")){
			efeitoComprarCarta(2, this->jogadores[this->posJogadorAtual]);
		}
		else{
			cout << "EmpilharComprarCarta" << endl;
			regraEmpilharCompraCarta(2, this->jogadores[this->posJogadorAtual]);
		}
		break;
	case 13:
		efeitoMudarCor();
		system("cls||clear");
		break;
	case 14:
		efeitoMudarCor();
		system("cls||clear");
		efeitoBloquear();
		if(!regraTaAtivada("EmpilharComprarCarta")){
			efeitoComprarCarta(4, this->jogadores[this->posJogadorAtual]);
		}
		else{
			cout << "EmpilharComprarCarta" << endl;
			regraEmpilharCompraCarta(4, this->jogadores[this->posJogadorAtual]);
		}
		break;
	default:
		system("cls||clear");
		break;
	}
}

// Coloca a carta no lixo
// @param cartaJogada Carta jogada pelo jogador
void Mesa::moverProLixo(Carta* cartaJogada){
	this->lixo.push_back(cartaJogada);
}

// Muda o jogador atual
int Mesa::proxJogador(){
	if(this->invertido){
		if(this->posJogadorAtual == 0){
			return (this->jogadores.size() - 1);
		}
		else{
			return (this->posJogadorAtual - 1);
		}
	}
	else{
		if(this->posJogadorAtual == (this->jogadores.size() - 1)){
			return 0;
		}
		else{
			return (this->posJogadorAtual + 1);
		}
	}
}

// Embaralha as cartas do baralho
void Mesa::embaralhar(){
	std::random_shuffle(begin(baralho), end(baralho));
}

// Pergunta ao jogador se ele quer comprar uma carta
// @param jogador Jogador a perguntar
// @param ultimaDoLixo Ultima carta do lixo
bool Mesa::jogadorQuerCarta(Jogador* jogador, Carta* ultimaDoLixo){
	if(!jogador->temCartaValida(ultimaDoLixo, JOGADA_NORMAL)){
		if(regraTaAtivada("PularTurno")){
			// Se o jogador não tiver carta válida e não quiser pular o turno, ele quer comprar uma carta
			return !(jogador->querPularTurno(ultimaDoLixo));
		}
		return true;
	}
	else if(regraTaAtivada("CompraOpcional")){
		return jogador->querComprar(ultimaDoLixo);
	}
	return false;
}

// Distribui a ultima carta do baralho
// @param jogador Jogador a distribuir a carta
// @param carta Carta a ser distribuida ao jogador
void Mesa::distribuirCarta(Jogador* jogador, Carta* carta){
	this->baralho.pop_back();
	jogador->adicionarCarta(carta);
	this->numCartasBaralho--;
}

// Coloca as cartas do lixo no baralho e embaralha o baralho
void Mesa::colocarLixoNoBaralho(){
	if(this->numCartasBaralho == 0 && this->lixo.size() != 0){
		// Guardar a ultima carta do lixo separada do lixo
		Carta* ultimaDoLixo = this->lixo.back();
		this->lixo.pop_back();

		// Colocar o lixo todo no baralho
		while(this->lixo.size() > 0){
			Carta* carta = this->lixo.back();
			if((carta->getID() == 13) ||(carta->getID() == 14)){
				carta->setCor(Cor::BRANCO);
			}
			this->lixo.pop_back();
			this->baralho.push_back(carta);
		}

		// Retornar a ultima carta para o lixo
		this->lixo.push_back(ultimaDoLixo);

		// Embaralhar
		embaralhar();
	}
}

// Efeito de pular o turno do próximo jogador
void Mesa::efeitoBloquear(){
	this->posJogadorAtual = proxJogador();
}

// Efeito de inverter a ordem do jogo
void Mesa::efeitoReverter(){
	switch (this->invertido){
	case true:
		this->invertido = false;
		break;
	case false:
		this->invertido = true;
		break;
	}
}

// Possibilita o empilhamento de cartas de compra pelo jogador
// @param numCartas Número de cartas a comprar acumuladas
// @param jogadorCompra Jogador a comprar o número de cartas
void Mesa::regraEmpilharCompraCarta(int numCartas, Jogador* jogadorCompra){
	system("cls||clear");
	Carta* cartaLixo = this->lixo.back();
	int empilharMais4EmMais2 = regraTaAtivada("+4Em+2");
	if(regraTaAtivada("+4Em+2")){
		empilharMais4EmMais2 = 2;
	}
	else{
		empilharMais4EmMais2 = 1;
	}

	// Se o jogador não tiver cartas de compra válidas para jogar
	if(!jogadorCompra->temCartaValida(cartaLixo, empilharMais4EmMais2)){
		efeitoComprarCarta(numCartas, jogadorCompra);
	}

	// Se o Jogador tiver cartas de compra válidas para jogar
	else{
		// Checar se o jogador gostaria de empilhar a carta
		string resposta;
		cout << jogadorCompra->getNome() << " deseja empilhar cartas de compra? s/n" << endl;
		cout << *((CartaEspecial*) cartaLixo);
		cout << "Cartas a comprar: " << numCartas << endl;
		jogadorCompra->mostrarCartas();
		do{
			cin >> resposta;
			if(!(resposta == "s") && !(resposta == "n")){
				cout << "Resposta inválida" << endl;
			}
		} while(!(resposta == "s") && !(resposta == "n"));

		system("cls||clear");

		// Se o jogador quiser empilhar cartas de compra
		if(resposta == "s"){
			Carta* cartaEmResposta = jogadorCompra->jogarCarta(cartaLixo, empilharMais4EmMais2);
			moverProLixo(cartaEmResposta);
			if(jogadorCompra->getNumeroDeCartas() != 0){
				// Ações diferentes caso o jogador mande um +2 ou um +4
				if(cartaEmResposta->getID() == 12){
					cout << jogadorCompra->getNome() << " jogou um +2" << endl;
					this->posJogadorAtual = proxJogador();
					jogadorCompra = this->jogadores[this->posJogadorAtual];
					regraEmpilharCompraCarta(numCartas + 2, jogadorCompra);
				}
				else{
					cout << jogadorCompra->getNome() << " jogou um +4" << endl;
					efeitoMudarCor();
					this->posJogadorAtual = proxJogador();
					jogadorCompra = this->jogadores[this->posJogadorAtual];
					regraEmpilharCompraCarta(numCartas + 4, jogadorCompra);
				}
			}
		}

		// Se o Jogador não quiser empilhar cartas de compra
		else{
			efeitoComprarCarta(numCartas, jogadorCompra);
		}
	}
}

// Efeito de fazer o jogador comprar uma carta
// @param numCartas Numero de cartas a comprar
// @param jogadorCompra Jogador que vai comprar as cartas
void Mesa::efeitoComprarCarta(int numCartas, Jogador* jogadorCompra){
	cout << jogadorCompra->getNome() << " comprou " << numCartas << " cartas!" << endl;
	for(int cartasCompradas = 0; cartasCompradas < numCartas; cartasCompradas++){
		colocarLixoNoBaralho();
		if(this->baralho.size() > 0){
			Carta* carta = this->baralho.back();
			distribuirCarta(jogadorCompra, carta);
		}
	}
}

// Efeito de mudar a cor da carta
void Mesa::efeitoMudarCor(){
	Carta* ultimaDoLixo = this->lixo.back();
	cout << "Escolha a cor: 0:Vermelho 1:Verde 2:Amarelo 3:Azul" << endl;
	string cor;
	bool corValida;
	do{
		cin >> cor;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		try{
			if(!(stoi(cor) >= 0 && stoi(cor) <=3)){
				throw ValorCorInvalido();
			}
			else{
				corValida = true;
			}
		} catch(const ValorCorInvalido& vci){
			cout << vci.what() << endl;
			corValida = false;
		} catch (const exception& e){
			cout << "Por favor insira um número!" << endl;
		}
	} while(!corValida);
	ultimaDoLixo->setCor(cores[stoi(cor)]);
}