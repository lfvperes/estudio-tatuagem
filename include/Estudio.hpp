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
        // Mapas para busca rápida por CPF (entrada 10 de docs/decisoes.md)
        std::map<std::string, Cliente> clientes_;
        std::map<std::string, Funcionario> funcionarios_;

        // Vetor que guarda a posse de todos os agendamentos do estúdio
        std::vector<Agendamento> agendamentos_;

        // Método privado para centralizar a validação (entrada 22 de docs/decisoes.md)
        void validarAgendamento(const std::string& cpfCliente, const std::string& cpfFuncionario,
                                const std::string& dataHora, const Servico& servico, Status status = Status::Agendado) const;

    public:
        Estudio();
        ~Estudio();

        void cadastrarCliente(const Cliente& cliente);
        void cadastrarFuncionario(const Funcionario& funcionario);

        // Recebe os CPFs, a data e o ponteiro único do serviço.
        // Passamos o unique_ptr por valor para podermos usar std::move() ao chamar.
        void agendar(const std::string& cpfCliente, const std::string& cpfFuncionario,
                     const std::string& dataHora, std::unique_ptr<Servico> servico);

        // AVISO: Os ponteiros retornados apontam diretamente para os elementos do
        // vector interno. Como um novo agendamento pode causar a realocação do vector,
        // os ponteiros desta lista só são válidos até a próxima chamada de agendar().
        std::vector<const Agendamento*> agendamentosDoCliente(const std::string& cpfCliente) const;
        std::vector<const Agendamento*> agendamentosDoFuncionario(const std::string& cpfFuncionario) const;

        // Persistência em CSV na pasta dados/ (entrada 18 de docs/decisoes.md)
        void salvar() const;
        void carregar();
};

#endif // ESTUDIO_HPP