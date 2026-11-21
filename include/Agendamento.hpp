#ifndef AGENDAMENTO_HPP
#define AGENDAMENTO_HPP

#include <string>
#include <memory>
#include "Servico.hpp"

enum class Status {Agendado, Confirmado, Cancelado};

class Agendamento {
    private:
        Status status_;
        bool pago_;
        std::string cpfCliente_;
        std::string cpfFuncionario_;
        std::string dataHora_;
        // único dono do serviço; por isso Agendamento só é movível
        std::unique_ptr<Servico> servico_;
    public:
        Agendamento(const std::string& cpfCliente, const std::string& cpfFuncionario, const std::string& dataHora, std::unique_ptr<Servico> servico);

        // restaura dados do CSV, ver entrada 21 de docs/decisoes.md
        Agendamento(const std::string& cpfCliente, const std::string& cpfFuncionario,
                    const std::string& dataHora, std::unique_ptr<Servico> servico,
                    Status status, bool pago);

        void confirmar();
        void cancelar();
        void pagar();

        std::string getCpfCliente() const;
        std::string getCpfFuncionario() const;
        std::string getDataHora() const;
        Status getStatus() const;
        bool isPago() const;
        // referência somente leitura; o Estudio consulta especialidade e preço,
        // mas a posse do serviço continua no Agendamento
        const Servico& getServico() const;
};

#endif // AGENDAMENTO_HPP