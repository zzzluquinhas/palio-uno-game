// NOTA: Não definimos DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN aqui
// porque ele já está definido em testeCartas.cpp
#include "doctest.h"
#include "mesa.hpp"
#include "jogador.hpp"
#include "regra.hpp"
#include "cartaNormal.hpp"
#include "cartaEspecial.hpp"
#include "cor.hpp"
#include <vector>
#include <iostream>
#include <sstream>

// Helper para simular entrada do usuário (cin) e silenciar saída (cout) durante os testes
struct CIN_Redirect_Integracao {
    std::streambuf* old_cin;
    std::streambuf* old_cout;
    std::stringstream ss_in;
    std::stringstream ss_out;

    CIN_Redirect_Integracao(const std::string& input) {
        old_cin = std::cin.rdbuf();
        old_cout = std::cout.rdbuf();
        ss_in.str(input);
        std::cin.rdbuf(ss_in.rdbuf());
        std::cout.rdbuf(ss_out.rdbuf());
    }

    ~CIN_Redirect_Integracao() {
        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);
    }
};

DOCTEST_TEST_SUITE("Testes de Integração") {

    // 1. Integração Mesa-Jogador-Baralho: Inicialização correta da partida
    DOCTEST_TEST_CASE("1. Inicialização: Distribuição de Cartas e Setup do Baralho") {
        std::vector<Jogador*> jogadores;
        jogadores.push_back(new Jogador("Jogador1"));
        jogadores.push_back(new Jogador("Jogador2"));
        std::vector<Regra*> regras;

        { // ESCOPO DE VIDA DA MESA
            // Ao instanciar a Mesa, o construtor deve criar o baralho e distribuir 7 cartas para cada
            Mesa mesa(jogadores, regras);

            // Verificações de integração
            CHECK(jogadores[0]->getNumeroDeCartas() == 7);
            CHECK(jogadores[1]->getNumeroDeCartas() == 7);
            
            // O baralho deve ter cartas restantes (108 total - 14 distribuídas - 1 lixo)
            CHECK(mesa.getNumCartasBaralho() > 0);
            
            // Deve haver 1 carta no lixo inicial
            CHECK(mesa.lixo.size() == 1);
        } // Mesa é destruída AQUI, enquanto os jogadores ainda existem.

        // Limpeza segura
        delete jogadores[0];
        delete jogadores[1];
    }

    // 2. Integração Mesa-Jogador: Jogada válida move carta da mão para o lixo
    DOCTEST_TEST_CASE("2. Fluxo de Jogada: Carta sai da Mão e vai para o Lixo") {
        std::vector<Jogador*> jogadores;
        Jogador* p1 = new Jogador("P1");
        jogadores.push_back(p1);
        jogadores.push_back(new Jogador("P2"));
        std::vector<Regra*> regras;
        
        { // ESCOPO
            Mesa mesa(jogadores, regras);
            
            // Setup forçado para garantir determinismo
            p1->limparCartas();
            mesa.lixo.clear();

            // Coloca carta Azul 5 no Lixo
            mesa.lixo.push_back(new CartaNormal(5, Cor::AZUL));
            // Dá carta Azul 7 para o jogador
            p1->adicionarCarta(new CartaNormal(7, Cor::AZUL));

            // Simula input: "0" (escolhe a primeira carta da mão)
            CIN_Redirect_Integracao redirect("0\n");

            // Executa a interação
            mesa.receberCartaDoJogador(p1, mesa.lixo.back());

            // Verifica se a carta saiu da mão do jogador
            CHECK(p1->getNumeroDeCartas() == 0);
            // Verifica se o lixo agora tem 2 cartas e a do topo é a carta jogada (Azul 7)
            CHECK(mesa.lixo.size() == 2);
            CHECK(mesa.lixo.back()->getID() == 7);
            CHECK(mesa.lixo.back()->getCor() == Cor::AZUL);
        }

        delete jogadores[0];
        delete jogadores[1];
    }

    // 3. Integração Mesa-CartaEspecial: Efeito de Compra (+2) afeta o Jogador Alvo
    DOCTEST_TEST_CASE("3. Efeito Carta Especial: Mesa aplica +2 no inventário do Jogador") {
        std::vector<Jogador*> jogadores;
        Jogador* vitima = new Jogador("Vitima");
        jogadores.push_back(new Jogador("Atacante"));
        jogadores.push_back(vitima);
        std::vector<Regra*> regras;
        
        { // ESCOPO
            Mesa mesa(jogadores, regras);
            
            // Limpa cartas para contagem exata
            vitima->limparCartas();
            
            // Executa efeito de comprar carta na mesa direcionado à vítima
            mesa.efeitoComprarCarta(2, vitima);

            // Verifica se a classe Jogador recebeu as cartas através da Mesa
            CHECK(vitima->getNumeroDeCartas() == 2);
            
            // Verifica se as cartas saíram do baralho da Mesa
            CHECK(mesa.getNumCartasBaralho() > 0);
        }

        delete jogadores[0];
        delete jogadores[1];
    }

    // 4. Integração Regra-Mesa: Regra de remover cartas altera a criação do baralho
    DOCTEST_TEST_CASE("4. Regras Personalizadas: Remover cartas '0' do jogo") {
        std::vector<Jogador*> jogadores;
        jogadores.push_back(new Jogador("J1"));
        jogadores.push_back(new Jogador("J2"));
        
        // Adiciona regra para tirar cartas com ID 0
        std::vector<Regra*> regras;
        regras.push_back(new Regra("TirarCarta0", "Remove zeros"));

        { // ESCOPO
            Mesa mesa(jogadores, regras);

            bool achouZero = false;
            
            // Verifica no baralho
            for(Carta* c : mesa.baralho) {
                if(c->getID() == 0) achouZero = true;
            }
            // Verifica nas mãos
            for(Jogador* j : jogadores) {
                for(Carta* c : j->cartas) {
                    if(c->getID() == 0) achouZero = true;
                }
            }
            // Verifica no lixo
            for(Carta* c : mesa.lixo) {
                if(c->getID() == 0) achouZero = true;
            }

            CHECK_FALSE(achouZero);
        }

        delete jogadores[0];
        delete jogadores[1];
        for(Regra* r : regras) delete r; 
    }

    // 5. Integração Mesa-Fluxo: Efeito Inverter altera a ordem dos jogadores
    DOCTEST_TEST_CASE("5. Fluxo de Jogo: Carta Inverter altera ponteiro do próximo jogador") {
        std::vector<Jogador*> jogadores;
        jogadores.push_back(new Jogador("J1")); // Pos 0
        jogadores.push_back(new Jogador("J2")); // Pos 1
        jogadores.push_back(new Jogador("J3")); // Pos 2
        std::vector<Regra*> regras;

        { // ESCOPO
            Mesa mesa(jogadores, regras);
            mesa.invertido = false;
            mesa.posJogadorAtual = 0; // J1

            // Fluxo normal: 0 -> 1
            CHECK(mesa.proxJogador() == 1);

            // Aplica efeito reverter (simulando carta jogada)
            mesa.efeitoReverter(); // Agora invertido = true

            // Fluxo invertido: 0 -> 2 (volta para o último da lista)
            CHECK(mesa.proxJogador() == 2);
        }

        delete jogadores[0];
        delete jogadores[1];
        delete jogadores[2];
    }
}