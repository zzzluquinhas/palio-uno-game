#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "cartaEspecial.hpp"
#include "cartaNormal.hpp"
#include "excecoes.hpp"
#include <regra.hpp>

TEST_CASE("Comparador == entre cartas especiais cor igual"){
	CartaEspecial c1 = CartaEspecial(6, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::VERMELHO);
	CHECK(c1 == c2);
}

TEST_CASE("Comparador == entre cartas especiais número igual"){
	CartaEspecial c1 = CartaEspecial(12, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::AMARELO);
	CHECK(c1 == c2);
}

TEST_CASE("Comparador == entre cartas especiais diferentes"){
	CartaEspecial c1 = CartaEspecial(6, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::AMARELO);
	CHECK_FALSE(c1 == c2);
}

TEST_CASE("Comparador == entre cartas normais cor igual"){
	CartaNormal c1 = CartaNormal(6, Cor::AZUL);
	CartaNormal c2 = CartaNormal(7, Cor::AZUL);
	CHECK(c1 == c2);
}

TEST_CASE("Comparador == entre cartas normais número igual"){
	CartaNormal c1 = CartaNormal(8, Cor::AZUL);
	CartaNormal c2 = CartaNormal(8, Cor::VERDE);
	CHECK(c1 == c2);
}

TEST_CASE("Comparador == entre cartas normais diferentes"){
	CartaNormal c1 = CartaNormal(6, Cor::AZUL);
	CartaNormal c2 = CartaNormal(7, Cor::VERDE);
	CHECK_FALSE(c1 == c2);
}

TEST_CASE("Compatibilidade de carta, regra padrão, cor igual"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(10, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra padrão, numero igual"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaNormal(1, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra padrão, carta branca"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(10, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra padrão"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(10, Cor::AZUL);

	CHECK_FALSE(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +2 em +2, cor diferente"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +2 em +2, cor igual"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +4 em +4"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +4 em +4, cor diferente"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +4 em +2"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK_FALSE(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 1, +2 em +4"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK_FALSE(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +2 em +2, cor diferente"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +2 em +2, cor igual"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +4 em +4"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +4 em +4, cor diferente"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +4 em +2"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("Compatibilidade de carta, regra 2, +2 em +4"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK_FALSE(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("GetID, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::VERMELHO);

	CHECK(c1->getID() == 2);
	delete c1;
}

TEST_CASE("GetID, carta normal"){
	Carta* c1 = new CartaNormal(3, Cor::VERMELHO);

	CHECK_FALSE(c1->getID() == 2);
	delete c1;
}

TEST_CASE("GetID, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->getID() == 12);
	delete c1;
}

TEST_CASE("GetID, carta especial"){
	Carta* c1 = new CartaEspecial(11, Cor::VERMELHO);

	CHECK_FALSE(c1->getID() == 12);
	delete c1;
}

TEST_CASE("GetCor, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::VERMELHO);

	CHECK(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("GetCor, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::AZUL);

	CHECK_FALSE(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("GetCor, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("GetCor, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::AZUL);

	CHECK_FALSE(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("GetNumero, carta normal"){
	CartaNormal c1 = CartaNormal(7, Cor::VERMELHO);

	CHECK(c1.getNumero() == 7);
}

TEST_CASE("Construtor carta genérica"){
	Carta c1 = Carta();

	CHECK(c1.getID() == -1);
	CHECK(c1.getCor() == Cor::BRANCO);
}

TEST_CASE("setDescricaoCarta e getDescricaoCarta"){
	Carta c1 = Carta();
	c1.setDescricaoCarta("Carta de Teste");

	CHECK(c1.getDescricaoCarta() == "Carta de Teste");
}

TEST_CASE("Regra - getDescricao"){
	Regra r1 = Regra("TesteRegra", "Descricao de Teste");

	CHECK(r1.getDescricao() == "Descricao de Teste");
}

TEST_CASE("Carta Especial +4 cor inválida"){
	CHECK_THROWS_AS(CartaEspecial(14, Cor::VERMELHO), CartaNaoBranca);
}