#include "mesa.hpp"
#include "doctest.h"
#include "jogador.hpp"
#include "regra.hpp"
#include "cartaNormal.hpp"
#include "cartaEspecial.hpp"
#include "cor.hpp"
#include "excecoes.hpp" 
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

// Use custom cores array for deterministic card creation
static std::vector<int> CORES_TESTE = {Cor::Codigo::VERMELHO,Cor::Codigo::VERDE,Cor::Codigo::AMARELO,Cor::Codigo::AZUL,Cor::Codigo::BRANCO};

// ====================================
// Mocking/Helper Classes
// ====================================

// Helper class to redirect std::cin and std::cout
struct CIN_Redirect {
    std::streambuf* old_cin;
    std::streambuf* old_cout;
    std::stringstream ss_in;
    std::stringstream ss_out;

    CIN_Redirect(const std::string& input) {
        old_cin = std::cin.rdbuf();
        old_cout = std::cout.rdbuf();
        ss_in.str(input);
        std::cin.rdbuf(ss_in.rdbuf());
        std::cout.rdbuf(ss_out.rdbuf());
    }

    ~CIN_Redirect() {
        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);
    }
};

// Subclass Mesa to expose protected/private methods for testing and gain control over card creation/state
class MesaControlada : public Mesa {
public:
    MesaControlada(std::vector<Jogador*> jogadores, std::vector<Regra*> regras) : Mesa(jogadores, regras) {
        // Clear initial state to allow deterministic tests
        for(Carta*& c : this->baralho) delete c;
        this->baralho.clear();
        this->numCartasBaralho = 0;
        for(Carta*& c : this->lixo) delete c;
        this->lixo.clear();

        // Ensure players start with no cards (will be added by distributing later)
        for(Jogador* j : this->jogadores) j->limparCartas();

        // Reset inverted state and current player position
        this->invertido = false;
        this->posJogadorAtual = 0;
    }

    // Public wrappers for testing
    void moverProLixoPublic(Carta* cartaJogada) { moverProLixo(cartaJogada); }
    int proxJogadorPublic() { return proxJogador(); }
    void colocarLixoNoBaralhoPublic() { colocarLixoNoBaralho(); }
    void efeitoBloquearPublic() { efeitoBloquear(); }
    void efeitoReverterPublic() { efeitoReverter(); }
    void efeitoMudarCorPublic() { efeitoMudarCor(); }
    void efeitoComprarCartaPublic(int numCartas, Jogador* jogadorCompra) { efeitoComprarCarta(numCartas, jogadorCompra); }
    void regraEmpilharCompraCartaPublic(int numCartas, Jogador* jogadorCompra) { regraEmpilharCompraCarta(numCartas, jogadorCompra); }

    void setInvertido(bool state) { this->invertido = state; }
    bool getInvertido() const { return this->invertido; }
    void setPosJogadorAtual(unsigned int pos) { this->posJogadorAtual = pos; }
    unsigned int getPosJogadorAtual() const { return this->posJogadorAtual; }
    std::vector<Carta*>& getBaralho() { return this->baralho; }
    std::vector<Carta*>& getLixo() { return this->lixo; }
    
    // Helper to manually set the deck for deterministic tests
    void preencherBaralho(const std::vector<Carta*>& novasCartas) {
        for(Carta* c : novasCartas) {
            this->baralho.push_back(c);
        }
        this->numCartasBaralho = this->baralho.size();
    }
};

// Subclass Jogador to mock responses for card stacking tests
class JogadorStacker : public Jogador {
public:
    JogadorStacker(std::string nome) : Jogador(nome) {}

    bool podeJogarMais2 = false;
    bool podeJogarMais4 = false;

    // Mock temCartaValida to control flow in regraEmpilharCompraCarta
    bool temCartaValida(Carta* cartaDoLixo, int regraEmpilharCompraCarta) {
        // Check if a stackable card is available in hand based on flags
        if (regraEmpilharCompraCarta == 2 && cartaDoLixo->getID() == 12 && podeJogarMais4) {
             for (Carta* c : cartas) {
                 if (c->getID() == 14) return true;
             }
        }
        
        if (regraEmpilharCompraCarta == 0) {
            for (Carta* c : cartas) {
                if (c->ehCompativel(cartaDoLixo, 0)) return true;
            }
        }
        
        // This is simplified to just check if there is a stackable card of the same type.
        if (cartaDoLixo->getID() == 12 && podeJogarMais2) {
             for (Carta* c : cartas) {
                 if (c->getID() == 12) return true;
             }
        }
        if (cartaDoLixo->getID() == 14 && podeJogarMais4) {
             for (Carta* c : cartas) {
                 if (c->getID() == 14) return true;
             }
        }

        return false;
    }

    // Mock jogarCarta to read the index from cin
    Carta* jogarCarta(Carta* cartaDoLixo, int regraEmpilharCompraCarta) {
        int index_lido = 0;
        if (!(std::cin >> index_lido)) {
             throw std::runtime_error("Failed to read simulated card index in JogadorStacker::jogarCarta.");
        }
        
        if (index_lido >= 0 && index_lido < (int)cartas.size()) {
            Carta* cardToReturn = cartas[index_lido];
            cartas.erase(cartas.begin() + index_lido);
            return cardToReturn;
        }

        throw std::runtime_error("Invalid card index provided in JogadorStacker::jogarCarta.");
    }
    
    // Mock querFazerIsso/querComprar/querPularTurno to read from redirected cin
    bool querFazerIsso(std::string fazerIsso) {
        char resposta = ' ';
        if (!(std::cin >> resposta)) {
            return false;
        }

        return tolower(resposta) == 's';
    }

    bool querComprar(Carta* cartaDoLixo) { return Jogador::querComprar(cartaDoLixo); }
    bool querPularTurno(Carta* cartaDoLixo) { return Jogador::querPularTurno(cartaDoLixo); }

    void mostrarCartas() {} // Silence output
    void ajuda() {} // Silence output
    void regras() {} // Silence output

};

// Fixture base
struct MesaFixture {
    std::vector<Jogador*> jogadores;
    std::vector<Regra*> regras;

    MesaFixture() {
        jogadores.push_back(new Jogador("Player1"));
        jogadores.push_back(new Jogador("Player2"));
    }

    ~MesaFixture() {
        for(Jogador* j : jogadores) delete j;
        for(Regra* r : regras) delete r;
    }
};

// Testes para a classe Mesa
// ====================================

DOCTEST_TEST_SUITE("Mesa Class Tests") {

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "03 - regraTaAtivada - Rule is Active") {
        regras.push_back(new Regra("TestRule", "Desc"));
        Mesa mesa(jogadores, regras);
        CHECK(mesa.regraTaAtivada("TestRule") == true);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "04 - regraTaAtivada - Rule is Inactive") {
        regras.push_back(new Regra("OtherRule", "Desc"));
        Mesa mesa(jogadores, regras);
        CHECK(mesa.regraTaAtivada("TestRule") == false);
    }
    
    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "05 - proxJogador - Normal Direction & Wrap Around (Last to First)") {
        MesaControlada mesa(jogadores, regras);
        
        mesa.setPosJogadorAtual(0);
        CHECK(mesa.proxJogadorPublic() == 1);
        
        mesa.setPosJogadorAtual(1);
        CHECK(mesa.proxJogadorPublic() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "06 - proxJogador - Inverted Direction & Wrap Around (First to Last)") {
        MesaControlada mesa(jogadores, regras);
        mesa.setInvertido(true);

        mesa.setPosJogadorAtual(0);
        CHECK(mesa.proxJogadorPublic() == 1);
        
        mesa.setPosJogadorAtual(1);
        CHECK(mesa.proxJogadorPublic() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "07 - efeitoReverter - Toggle Inversion State") {
        MesaControlada mesa(jogadores, regras);
        mesa.setInvertido(false);
        
        mesa.efeitoReverterPublic();
        CHECK(mesa.getInvertido() == true);
        
        mesa.efeitoReverterPublic();
        CHECK(mesa.getInvertido() == false);
    }
    
    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "08 - efeitoBloquear - Skip Next Player") {
        MesaControlada mesa(jogadores, regras); 
        mesa.setPosJogadorAtual(0);
        
        mesa.efeitoBloquearPublic(); 
        
        CHECK(mesa.getPosJogadorAtual() == 1);
        CHECK(mesa.proxJogadorPublic() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "09 - colocarLixoNoBaralho - Baralho is not Empty (Should do nothing)") {
        MesaControlada mesa(jogadores, regras);
        
        mesa.preencherBaralho({new CartaNormal(1, CORES_TESTE[0])}); 
        mesa.moverProLixoPublic(new CartaNormal(2, CORES_TESTE[1])); 
        
        mesa.colocarLixoNoBaralhoPublic();
        
        CHECK(mesa.getBaralho().size() == 1);
        CHECK(mesa.getLixo().size() == 1);
        CHECK(mesa.getNumCartasBaralho() == 1);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "11 - efeitoComprarCarta - Buy 3 Cards, Deck Not Empty") {
        MesaControlada mesa(jogadores, regras);
        
        mesa.preencherBaralho({
            new CartaNormal(1, CORES_TESTE[0]), new CartaNormal(2, CORES_TESTE[1]), new CartaNormal(3, CORES_TESTE[2])
        });
        
        Jogador* j = jogadores[0];
        mesa.efeitoComprarCartaPublic(3, j);
        
        CHECK(j->getNumeroDeCartas() == 3);
        CHECK(mesa.getNumCartasBaralho() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "12 - efeitoComprarCarta - Deck Empty, No Lixo") {
        MesaControlada mesa(jogadores, regras);
        
        Jogador* j = jogadores[0];
        mesa.efeitoComprarCartaPublic(3, j);
        
        CHECK(j->getNumeroDeCartas() == 0);
        CHECK(mesa.getNumCartasBaralho() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "13 - efeitoMudarCor - Valid Color Change (AZUL)") {
        MesaControlada mesa(jogadores, regras);
        mesa.moverProLixoPublic(new CartaEspecial(13, Cor::BRANCO));
        
        CIN_Redirect redirect("3\n"); 
        mesa.efeitoMudarCorPublic();
        
        CHECK(mesa.getLixo().back()->getCor() == Cor::AZUL);
    }
    
    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "14 - efeitoMudarCor - Invalid then Valid Color Change (AMARELO)") {
        MesaControlada mesa(jogadores, regras);
        mesa.moverProLixoPublic(new CartaEspecial(13, Cor::BRANCO));
        
        CIN_Redirect redirect("-1\n5\n2\n"); 
        mesa.efeitoMudarCorPublic();
        
        CHECK(mesa.getLixo().back()->getCor() == Cor::AMARELO);
    }
    
    // ====================================
    // Testes de Fluxo e Regras (Empilhamento)
    // ====================================

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "15 - regraEmpilharCompraCarta - Player Cannot Stack -> Forced Buy (Base Case)") {
        regras.push_back(new Regra("EmpilharComprarCarta", "Empilhar"));
        MesaControlada mesa(jogadores, regras);
        
        mesa.preencherBaralho({
            new CartaNormal(1, CORES_TESTE[0]), new CartaNormal(2, CORES_TESTE[1]), new CartaNormal(3, CORES_TESTE[2])
        });
        mesa.moverProLixoPublic(new CartaEspecial(12, CORES_TESTE[0]));

        JogadorStacker* j = new JogadorStacker("Stacker");
        delete jogadores[0];
        jogadores[0] = j;

        mesa.regraEmpilharCompraCartaPublic(3, j); 
        
        CHECK(j->getNumeroDeCartas() == 3);
        CHECK(mesa.getNumCartasBaralho() == 0);
    }
	
    // ====================================
    // Testes de Efeitos de Cartas (receberCartaDoJogador)
    // ====================================

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "19 - receberCartaDoJogador - ID 10 (Bloqueio)") {
        MesaControlada mesa(jogadores, regras); 
        mesa.setPosJogadorAtual(0);
        mesa.moverProLixoPublic(new CartaNormal(1, CORES_TESTE[0]));

        JogadorStacker* j = new JogadorStacker("Chooser");
        j->adicionarCarta(new CartaEspecial(10, CORES_TESTE[0]));
        
        CIN_Redirect redirect("0\n"); 
        
        delete jogadores[0]; 
        jogadores[0] = j;

        mesa.receberCartaDoJogador(j, mesa.getLixo().front()); 
        
        CHECK(mesa.getPosJogadorAtual() == 1);
    }
    
    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "20 - receberCartaDoJogador - ID 11 (Reverter)") {
        MesaControlada mesa(jogadores, regras); 
        mesa.setPosJogadorAtual(0);
        mesa.moverProLixoPublic(new CartaNormal(1, CORES_TESTE[0]));

        JogadorStacker* j = new JogadorStacker("Chooser");
        j->adicionarCarta(new CartaEspecial(11, CORES_TESTE[0]));
        
        CIN_Redirect redirect("0\n");
        
        delete jogadores[0]; 
        jogadores[0] = j;

        mesa.receberCartaDoJogador(j, mesa.getLixo().front()); 
        
        CHECK(mesa.getInvertido() == true);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "21 - receberCartaDoJogador - ID 13 (Mudar Cor)") {
        MesaControlada mesa(jogadores, regras);
        mesa.setPosJogadorAtual(0);
        mesa.moverProLixoPublic(new CartaNormal(1, CORES_TESTE[0]));

        JogadorStacker* j = new JogadorStacker("Chooser");
        j->adicionarCarta(new CartaEspecial(13, Cor::BRANCO));
        
        CIN_Redirect redirect("0\n3\n"); 
        
        delete jogadores[0]; 
        jogadores[0] = j;

        mesa.receberCartaDoJogador(j, mesa.getLixo().front()); 
        
        CHECK(mesa.getLixo().back()->getID() == 13);
        CHECK(mesa.getLixo().back()->getCor() == Cor::AZUL);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "22 - receberCartaDoJogador - ID 12 (+2) - No EmpilharCompraCarta (Padrão)") {
        MesaControlada mesa(jogadores, regras);
        mesa.setPosJogadorAtual(0);
        mesa.moverProLixoPublic(new CartaNormal(1, CORES_TESTE[0]));

        mesa.preencherBaralho({new CartaNormal(1, 0), new CartaNormal(2, 0)}); 

        JogadorStacker* j1 = new JogadorStacker("Chooser1");
        j1->adicionarCarta(new CartaEspecial(12, CORES_TESTE[0]));
        
        CIN_Redirect redirect("0\n"); 
        
        delete jogadores[0]; 
        jogadores[0] = j1;

        mesa.receberCartaDoJogador(j1, mesa.getLixo().front()); 
        
        CHECK(mesa.getPosJogadorAtual() == 1);
        CHECK(jogadores[1]->getNumeroDeCartas() == 2);
        CHECK(mesa.getNumCartasBaralho() == 0);
    }

    DOCTEST_TEST_CASE_FIXTURE(MesaFixture, "23 - receberCartaDoJogador - ID 14 (+4) - No EmpilharCompraCarta (Padrão)") {
        MesaControlada mesa(jogadores, regras); 
        mesa.setPosJogadorAtual(0);
        mesa.moverProLixoPublic(new CartaNormal(1, CORES_TESTE[0]));

        mesa.preencherBaralho({new CartaNormal(1, 0), new CartaNormal(2, 0), new CartaNormal(3, 0), new CartaNormal(4, 0)}); 

        JogadorStacker* j1 = new JogadorStacker("Chooser1");
        j1->adicionarCarta(new CartaEspecial(14, Cor::BRANCO));
        
        CIN_Redirect redirect("0\n1\n"); 
        
        delete jogadores[0]; 
        jogadores[0] = j1;

        mesa.receberCartaDoJogador(j1, mesa.getLixo().front()); 
        
        CHECK(mesa.getPosJogadorAtual() == 1);
        CHECK(jogadores[1]->getNumeroDeCartas() == 4);
        CHECK(mesa.getNumCartasBaralho() == 0);
        CHECK(mesa.getLixo().back()->getCor() == Cor::VERDE);
    }
}