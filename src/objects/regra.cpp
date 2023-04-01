#include "regra.hpp"

// Inicializa a regra
Regra::Regra(string nome, string descricao){
	this->nome = nome;
	this->descricao = descricao;
}

// Retorna o nome da regra
string Regra::getNome(){
	return this->nome;
}

// Retorna a descrição da regra
string Regra::getDescricao(){
	return this->descricao;
}