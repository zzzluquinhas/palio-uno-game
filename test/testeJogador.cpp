#include "doctest.h"
#include "jogador.hpp"
#include "excecoes.hpp"

#include <sstream>

class jogadorFixture {
	public:
		Jogador j;
		jogadorFixture() : j("player") {
            cinBufferOriginal = std::cin.rdbuf();
            coutBufferOriginal = std::cout.rdbuf();
        }

		void adicionarCartas() {
			Carta* c1 = new CartaNormal(7,Cor::AZUL);
			Carta* c2 = new CartaEspecial(11,Cor::AMARELO); // +2 Amarelo
			Carta* c3 = new CartaEspecial(14,Cor::BRANCO); // Curinga
			j.adicionarCarta(c1);
			j.adicionarCarta(c2);
			j.adicionarCarta(c3);
		}

        void adicionarCartasInvalidas() {
            Carta* c1 = new CartaNormal(1,Cor::AZUL);
            Carta* c2 = new CartaNormal(2,Cor::VERDE);
            j.adicionarCarta(c1);
            j.adicionarCarta(c2);
        }

        void simularEntrada(const std::string& input) {
            ssEntrada.str(input);
            ssEntrada.clear();
            std::cin.rdbuf(ssEntrada.rdbuf());
        }

        std::string capturarSaida() {
            ssSaida.str("");
            ssSaida.clear();
            std::cout.rdbuf(ssSaida.rdbuf());
            return ssSaida.str();
        }

		~jogadorFixture() {
            std::cin.rdbuf(cinBufferOriginal);
            std::cout.rdbuf(coutBufferOriginal);
		}

    private:
        std::stringstream ssEntrada;
        std::stringstream ssSaida;
        std::streambuf* cinBufferOriginal;
        std::streambuf* coutBufferOriginal;
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

	simularEntrada("0\n");
	capturarSaida();

	CHECK(*(j.jogarCarta(cartaDaMesa, 0)) == CartaNormal(7,Cor::AZUL));

	CHECK(j.getNumeroDeCartas() == 2);
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "temCartaValida - Sem carta valida") {
    adicionarCartasInvalidas();
	Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO); // Nenhuma carta é compatível
	CHECK_FALSE(j.temCartaValida(cartaDaMesa, 0));
	delete cartaDaMesa;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "temCartaValida - Com carta valida coringa") {
    adicionarCartasInvalidas();
	Carta* c3 = new CartaEspecial(14,Cor::BRANCO); // Curinga (ID 14)
	j.adicionarCarta(c3);
	Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO); // Curinga deve ser compatível
	CHECK(j.temCartaValida(cartaDaMesa, 0));
	delete cartaDaMesa;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "jogarCarta - Nao tem cartas") {
    CHECK_THROWS_AS(j.jogarCarta(new CartaNormal(0, Cor::AZUL), 0), NumeroNegativoCartas);
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "jogarCarta - Entrada de indice invalido e depois valido") {
    adicionarCartas();
    Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

    // Tentar índice fora do range (3) e depois um índice que não é número (x), e por fim o índice 0 válido.
    simularEntrada("3\nx\n0\n");
    
    capturarSaida(); // Limpa e prepara a captura de saída

    // Esperado: Exceção ValorCartaInvalido (3), mensagem de erro (x), e sucesso (0)
    Carta* cartaJogada = j.jogarCarta(cartaDaMesa, 0);

    capturarSaida();

    CHECK(j.getNumeroDeCartas() == 2);
    CHECK(*(static_cast<CartaNormal*>(cartaJogada)) == CartaNormal(7,Cor::AZUL));

    delete cartaDaMesa;
    delete cartaJogada;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "jogarCarta - Ajuda 'h'") {
    adicionarCartas();
    Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

    simularEntrada("h\n0\n");
    capturarSaida();

    j.jogarCarta(cartaDaMesa, 0);

    capturarSaida();

    CHECK(j.getNumeroDeCartas() == 2);

    delete cartaDaMesa;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "jogarCarta - Regras 'r'") {
    adicionarCartas();
    Carta* cartaDaMesa = new CartaNormal(7,Cor::AMARELO);

    simularEntrada("r\n0\n");
    capturarSaida();

    j.jogarCarta(cartaDaMesa, 0);

	capturarSaida();

    CHECK(j.getNumeroDeCartas() == 2);

    delete cartaDaMesa;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "jogarCarta - Tentar jogar carta invalida e depois valida") {
    adicionarCartas();
    Carta* cartaDoLixo = new CartaNormal(8, Cor::VERMELHO); // Compatível com NADA

    // Tenta jogar a carta 0 (7 Azul - INVÁLIDA) e depois a carta 2 (Curinga - VÁLIDA)
    simularEntrada("0\n2\n");
    capturarSaida();

    Carta* cartaJogada = j.jogarCarta(cartaDoLixo, 0);

	capturarSaida();

    CHECK(j.getNumeroDeCartas() == 2);
    CHECK(*(static_cast<CartaEspecial*>(cartaJogada)) == CartaEspecial(14,Cor::BRANCO));

    delete cartaDoLixo;
    delete cartaJogada;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "querComprar - Resposta S") {
    adicionarCartas();
    Carta* cartaDoLixo = new CartaNormal(8, Cor::VERMELHO);
    simularEntrada("s\n");
    capturarSaida(); // Limpa e prepara a captura de saída
    CHECK(j.querComprar(cartaDoLixo) == true);
    delete cartaDoLixo;
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "temCarta - Tratamento de ValorCartaInvalido (Indice Negativo)") {
    simularEntrada("-1\n");
    capturarSaida();

    bool erroCapturado = false;
    try {
        j.temCarta("-1");
    } catch (const ValorCartaInvalido& vci) {
        erroCapturado = true;
    }
    CHECK(erroCapturado);
}

DOCTEST_TEST_CASE_FIXTURE(jogadorFixture, "limparCartas e Destrutor") {
    adicionarCartas();
    j.limparCartas();
    CHECK(j.getNumeroDeCartas() == 0);
}