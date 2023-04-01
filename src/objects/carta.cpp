#include "carta.hpp"

//Contrutor da Carta
Carta::Carta() : cor(Cor::BRANCO){
	this->id = -1;
}

bool Carta::ehCompativel(Carta* c, int regraEmpilharCompraCarta){
	// RegraEmpilharCartaCompra:
	// 0: Não é possivel jogar uma carta de compra em resposta à outra
	// 1: Jogar uma carta de compra em resposta à outra, +4 não pode ser jogado em resposta à um +2
	// 2: Jogar uma carta de compra em resposta à outra, +4 pode ser jogado em resposta à um +2
	switch (regraEmpilharCompraCarta){
	case 0:
		if(*this == *c || (this->cor.getCodigo() == Cor::BRANCO)){
			return true;
		}
		break;
	case 1:
		if(this->getID() == c->getID()){
			return true;
		}
		break;
	case 2:
		if(this->getID() == c->getID() || this->getID() == 14){
			return true;
		}
		break;
	}
	return false;
}

//set da Descricao em caso de alteracao
void Carta::setDescricaoCarta(string descricao){
	this->descricaoCarta = descricao;
}

//get Descricao da carta utilizada no menu
string Carta::getDescricaoCarta(){
	return this->descricaoCarta;
}

//get ID da carta
int Carta::getID() const{
	return this->id;
}

//get cor da carta
int Carta::getCor() const{
	return this->cor.getCodigo();
}

//set cor da carta (pelo ID)
void Carta::setCor(int cor){
	// cout << "cor ant: " << this->cor.getCodigo() << endl;
	this->cor = Cor::Modificador(cor);
	// cout << "cor dep: " << this->cor.getCodigo() << endl;
}

//sobrecarga de operador para testar se o jogador pode jogar a carta
bool Carta::operator==(const Carta& c){
	return ((this->id == c.id) || (this->cor == c.cor));
}