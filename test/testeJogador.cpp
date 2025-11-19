#include "doctest.h"
#include "jogador.hpp"
#include <sstream>

class jogadorFixture {
	public:
		Jogador j;
		jogadorFixture() : j("player") {}

		void adicionarCartas() {
			Carta* c1 = new CartaNormal(7,Cor::AZUL);
			Carta* c2 = new CartaEspecial(11,Cor::AMARELO);
			Carta* c3 = new CartaEspecial(14,Cor::BRANCO);
			j.adicionarCarta(c1);
			j.adicionarCarta(c2);
			j.adicionarCarta(c3);
		}

		~jogadorFixture() {
			j.limparCartas();
		}
};

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "Pode jogar a carta escolhida"){
	adicionarCartas();

	Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

	CHECK(j.temCartaValida(cartaDaMesa,0));
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "Dados iniciais"){
	adicionarCartas();

	CHECK(j.getNome() == "player");
	CHECK(j.getNumeroDeCartas() == 3);	
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "Jogar carta com sucesso") {
	adicionarCartas();

	Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

	stringstream ss("0\n");
	streambuf* cinBuffer = cin.rdbuf();
	cin.rdbuf(ss.rdbuf());

	CHECK(*(j.jogarCarta(cartaDaMesa, 0)) == CartaNormal(7,Cor::AZUL));

	cin.rdbuf(cinBuffer);
	CHECK(j.getNumeroDeCartas() == 2);
}
