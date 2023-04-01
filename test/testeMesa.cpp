#define DOCTEST_CONFIG_IMPEMENT_WITH_MAIN

#include "mesa.hpp"
#include "doctest.h"
#include "vector"

#include "mesa.hpp"

//Testes Mesa::Mesa

//Nome dos jogadores
//Distribuiçao de 7 cartas para cada
TEST_CASE("01 - Distribuir cartas iniciais"){
    vector<Jogador*> jogadores;
    vector<Regra*> regras;

    Jogador* temp1 = new Jogador("Jogador 1");
    Jogador* temp2 = new Jogador("Jogador 2");

	jogadores.push_back(temp1);
    jogadores.push_back(temp2);

    Mesa mesa(jogadores, regras);

    CHECK(temp1->getNumeroDeCartas() == 7);
    CHECK(temp2->getNumeroDeCartas() == 7);

    CHECK(temp1->getNome() == "Jogador 1");
    CHECK(temp2->getNome() == "Jogador 2");

    CHECK(mesa.getNumCartasBaralho() == 93);

}

//Regra ta ativa?
TEST_CASE("02 - "){
    vector<Jogador*> jogadores;
    vector<Regra*> regras;

    Jogador* temp1 = new Jogador("Jogador 1");
    Jogador* temp2 = new Jogador("Jogador 2");

	jogadores.push_back(temp1);
    jogadores.push_back(temp2);

    Mesa mesa(jogadores, regras);

    //CHECK_FALSE(mesa.regraTaAtivada());

}

TEST_CASE("03 - "){
    vector<Jogador*> jogadores;
    vector<Regra*> regras;

    Jogador* temp1 = new Jogador("Jogador 1");
    Jogador* temp2 = new Jogador("Jogador 2");

	jogadores.push_back(temp1);
    jogadores.push_back(temp2);

    Mesa mesa(jogadores, regras);

}

TEST_CASE("04 - "){


}

TEST_CASE("05 - "){


}

TEST_CASE("06 - "){


}

TEST_CASE("07 - "){


}

TEST_CASE("08 - "){


}

TEST_CASE("09 - "){


}

TEST_CASE("10 - "){


}

TEST_CASE("11 - "){


}

TEST_CASE("12 - "){


}

TEST_CASE("13 - "){


}

TEST_CASE("14 - "){


}

TEST_CASE("15 - "){


}

TEST_CASE("16 - "){


}

TEST_CASE("17 - "){


}

TEST_CASE("18 - "){


}

TEST_CASE("19 - "){


}

TEST_CASE("20 - "){


}