#include "doctest.h"
#include "jogador.hpp"

TEST_CASE("Teste 01 - Pode jogar a carta escolhida"){
	Jogador j = Jogador("lucas");
	Carta* c1 = new CartaNormal(7,Cor::AZUL);
	Carta* c2 = new CartaEspecial(11,Cor::AMARELO);
	Carta* c3 = new CartaEspecial(14,Cor::BRANCO);
	j.adicionarCarta(c1);
	j.adicionarCarta(c2);
	j.adicionarCarta(c3);

	Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

	CHECK(j.temCartaValida(cartaDaMesa,0));
}

TEST_CASE("Teste 02"){

	// CHECK_THROWS_AS(FUNCAO(), EXCECAO)
}

TEST_CASE("Teste 03 - Dados iniciais"){
	Jogador j = Jogador("lucas");
	Carta* c1 = new CartaNormal(7,Cor::AZUL);
	Carta* c2 = new CartaEspecial(11,Cor::AMARELO);
	Carta* c3 = new CartaEspecial(14,Cor::BRANCO);
	j.adicionarCarta(c1);
	j.adicionarCarta(c2);
	j.adicionarCarta(c3);

	CHECK(j.getNome() == "lucas");
	CHECK(j.getNumeroDeCartas() == 3);	
}

TEST_CASE("Teste 04 - Limpar Cartas"){
	Jogador j = Jogador("lucas");
	Carta* c1 = new CartaNormal(7,Cor::AZUL);
	Carta* c2 = new CartaEspecial(11,Cor::AMARELO);
	Carta* c3 = new CartaEspecial(14,Cor::BRANCO);
	j.adicionarCarta(c1);
	j.adicionarCarta(c2);
	j.adicionarCarta(c3);

	j.limparCartas();

	CHECK(j.getNumeroDeCartas() == 0);	
}
