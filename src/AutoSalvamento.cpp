#include "AutoSalvamento.hpp"
#include <chrono>
#include <exception>

constexpr std::chrono::seconds INTERVALO_AUTO_SAVE{30};

// O construtor inicializa a flag e dispara a thread,
// passando para ela o método privado 'loop'
AutoSalvamento::AutoSalvamento(Estudio& estudio)
    : estudio_(estudio), parar_(false), thread_(&AutoSalvamento::loop, this) {}

AutoSalvamento::~AutoSalvamento() {
    {
        // Trava o mutex próprio para alterar a flag de parada com segurança
        std::lock_guard<std::mutex> lock(mtx_);
        parar_ = true;
    }

    // Acorda a thread imediatamente se ela estiver dormindo no wait_for
    cv_.notify_one();

    // Aguarda a thread terminar sua execução de forma limpa (RAII)
    if (thread_.joinable()) {
        thread_.join();
    }
}

void AutoSalvamento::loop() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx_);

        // Espera o intervalo, mas acorda na hora se o predicado (parar_ == true) for satisfeito
        if (cv_.wait_for(lock, INTERVALO_AUTO_SAVE, [this]() { return parar_; })) {
            // O destrutor pediu para parar, encerra o loop
            break;
        }

        // Libera o lock do AutoSalvamento antes de realizar o I/O
        lock.unlock();

        try {
            // Chama a gravação protegida (o método salvar() já possui o lock_guard do Estudio)
            estudio_.salvar();
        } catch (const std::exception&) {
            // Tratamento silencioso obrigatório: captura TUDO (std::exception)
            // para impedir que uma falha não mapeada de I/O escape, chame
            // std::terminate e derrube o programa (entrada 27 de docs/decisoes.md)
        }
    }
}
