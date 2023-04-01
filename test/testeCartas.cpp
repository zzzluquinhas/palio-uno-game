#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "cartaEspecial.hpp"
#include "cartaNormal.hpp"

TEST_CASE("01 - Comparador == entre cartas especiais cor igual"){
	CartaEspecial c1 = CartaEspecial(6, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::VERMELHO);
	CHECK(c1 == c2);
}

TEST_CASE("02 - Comparador == entre cartas especiais número igual"){
	CartaEspecial c1 = CartaEspecial(12, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::AMARELO);
	CHECK(c1 == c2);
}

TEST_CASE("03 - Comparador == entre cartas especiais diferentes"){
	CartaEspecial c1 = CartaEspecial(6, Cor::VERMELHO);
	CartaEspecial c2 = CartaEspecial(12, Cor::AMARELO);
	CHECK_FALSE(c1 == c2);
}

TEST_CASE("04 - Comparador == entre cartas normais cor igual"){
	CartaNormal c1 = CartaNormal(6, Cor::AZUL);
	CartaNormal c2 = CartaNormal(7, Cor::AZUL);
	CHECK(c1 == c2);
}

TEST_CASE("05 - Comparador == entre cartas normais número igual"){
	CartaNormal c1 = CartaNormal(8, Cor::AZUL);
	CartaNormal c2 = CartaNormal(8, Cor::VERDE);
	CHECK(c1 == c2);
}

TEST_CASE("06 - Comparador == entre cartas normais diferentes"){
	CartaNormal c1 = CartaNormal(6, Cor::AZUL);
	CartaNormal c2 = CartaNormal(7, Cor::VERDE);
	CHECK_FALSE(c1 == c2);
}

TEST_CASE("07 - Compatibilidade de carta, regra padrão, cor igual"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(10, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("08 - Compatibilidade de carta, regra padrão, numero igual"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaNormal(1, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("09 - Compatibilidade de carta, regra padrão, carta branca"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(10, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("10 - Compatibilidade de carta, regra padrão"){
	Carta* c1 = new CartaNormal(1, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(10, Cor::AZUL);

	CHECK_FALSE(c1->ehCompativel(c2,0));
	delete c1;
	delete c2;
}

TEST_CASE("11 - Compatibilidade de carta, regra 1, +2 em +2, cor diferente"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("12 - Compatibilidade de carta, regra 1, +2 em +2, cor igual"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("13 - Compatibilidade de carta, regra 1, +4 em +4"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("14 - Compatibilidade de carta, regra 1, +4 em +4, cor diferente"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("15 - Compatibilidade de carta, regra 1, +4 em +2"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK_FALSE(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("16 - Compatibilidade de carta, regra 1, +2 em +4"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK_FALSE(c1->ehCompativel(c2,1));
	delete c1;
	delete c2;
}

TEST_CASE("17 - Compatibilidade de carta, regra 2, +2 em +2, cor diferente"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("18 - Compatibilidade de carta, regra 2, +2 em +2, cor igual"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("19 - Compatibilidade de carta, regra 2, +4 em +4"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("20 - Compatibilidade de carta, regra 2, +4 em +4, cor diferente"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("21 - Compatibilidade de carta, regra 2, +4 em +2"){
	Carta* c1 = new CartaEspecial(14, Cor::BRANCO);
	Carta* c2 = new CartaEspecial(12, Cor::AZUL);

	CHECK(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("22 - Compatibilidade de carta, regra 2, +2 em +4"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);
	Carta* c2 = new CartaEspecial(14, Cor::BRANCO);
	c2->setCor(1);

	CHECK_FALSE(c1->ehCompativel(c2,2));
	delete c1;
	delete c2;
}

TEST_CASE("23 - GetID, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::VERMELHO);

	CHECK(c1->getID() == 2);
	delete c1;
}

TEST_CASE("24 - GetID, carta normal"){
	Carta* c1 = new CartaNormal(3, Cor::VERMELHO);

	CHECK_FALSE(c1->getID() == 2);
	delete c1;
}

TEST_CASE("25 - GetID, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->getID() == 12);
	delete c1;
}

TEST_CASE("26 - GetID, carta especial"){
	Carta* c1 = new CartaEspecial(11, Cor::VERMELHO);

	CHECK_FALSE(c1->getID() == 12);
	delete c1;
}

TEST_CASE("27 - GetCor, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::VERMELHO);

	CHECK(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("28 - GetCor, carta normal"){
	Carta* c1 = new CartaNormal(2, Cor::AZUL);

	CHECK_FALSE(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("29 - GetCor, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::VERMELHO);

	CHECK(c1->getCor() == Cor::VERMELHO);
	delete c1;
}

TEST_CASE("30 - GetCor, carta especial"){
	Carta* c1 = new CartaEspecial(12, Cor::AZUL);

	CHECK_FALSE(c1->getCor() == Cor::VERMELHO);
	delete c1;
}
