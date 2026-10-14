#ifndef ESTUDIO_HPP
#define ESTUDIO_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Cliente.hpp"
#include "Funcionario.hpp"
#include "Agendamento.hpp"
#include "Servico.hpp"

class Estudio {
    private:
        // Decisão 10: Mapas para busca rápida por CPF
        std::map<std::string, Cliente> clientes_;
        std::map<std::string, Funcionario> funcionarios_;
        
        // Vetor que guarda a posse de todos os agendamentos do estúdio
        std::vector<Agendamento> agendamentos_;
        
    public:
        Estudio();
        ~Estudio();

        void cadastrarCliente(const Cliente& cliente);
        void cadastrarFuncionario(const Funcionario& funcionario);
        
        // Recebe os CPFs, a data e o ponteiro único do serviço. 
        // Passamos o unique_ptr por valor para podermos usar std::move() ao chamar.
        void agendar(const std::string& cpfCliente, const std::string& cpfFuncionario, 
                     const std::string& dataHora, std::unique_ptr<Servico> servico);
        
        // Retornam vetores de ponteiros para evitar cópias indevidas do Agendamento
        std::vector<const Agendamento*> agendamentosDoCliente(const std::string& cpfCliente) const;
        std::vector<const Agendamento*> agendamentosDoFuncionario(const std::string& cpfFuncionario) const;
};

#endif // ESTUDIO_HPP