#ifndef AUTO_SALVAMENTO_HPP
#define AUTO_SALVAMENTO_HPP

#include "Estudio.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

class AutoSalvamento {
    private:
        Estudio& estudio_;

        // Atributos de sincronização próprios do AutoSalvamento (entrada 25 de docs/decisoes.md)
        std::mutex mtx_;
        std::condition_variable cv_;
        bool parar_;

        // AVISO: A thread DEVE ser estritamente o ÚLTIMO atributo declarado.
        // Em C++, a inicialização segue a ordem de declaração no header.
        // Assim garantimos que o mutex e a flag já existem quando a thread iniciar.
        std::thread thread_;

        void loop();

    public:
        // O construtor inicia a thread e o destrutor a encerra (Padrão RAII)
        AutoSalvamento(Estudio& estudio);
        ~AutoSalvamento();

        // Proibir cópia e atribuição: uma thread em execução não pode ser copiada
        AutoSalvamento(const AutoSalvamento&) = delete;
        AutoSalvamento& operator=(const AutoSalvamento&) = delete;
};

#endif // AUTO_SALVAMENTO_HPP
