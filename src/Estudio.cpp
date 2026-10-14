#include "Estudio.hpp"
#include "DadoInvalidoException.hpp"
#include "EspecialidadeIncompativelException.hpp"
#include "HorarioIndisponivelException.hpp"
#include "EstudioException.hpp"

Estudio::Estudio() {}

Estudio::~Estudio() {}

void Estudio::cadastrarCliente(const Cliente& cliente) {
    // Tenta encontrar o cliente; se achar, significa que já existe
    if (clientes_.find(cliente.getCpf()) != clientes_.end()) {
        throw EstudioException("Já existe um cliente cadastrado com este CPF.");
    }
    // Adiciona ao map usando o CPF como chave
    clientes_.insert({cliente.getCpf(), cliente});
}

void Estudio::cadastrarFuncionario(const Funcionario& funcionario) {
    if (funcionarios_.find(funcionario.getCpf()) != funcionarios_.end()) {
        throw EstudioException("Já existe um funcionário cadastrado com este CPF.");
    }
    funcionarios_.insert({funcionario.getCpf(), funcionario});
}

void Estudio::agendar(const std::string& cpfCliente, const std::string& cpfFuncionario, 
                      const std::string& dataHora, std::unique_ptr<Servico> servico) {
    
    // 1. Checar se o cliente existe
    auto itCliente = clientes_.find(cpfCliente);
    if (itCliente == clientes_.end()) {
        throw DadoInvalidoException("Cliente não encontrado com o CPF informado.");
    }

    // 2. Checar se o funcionário existe
    auto itFuncionario = funcionarios_.find(cpfFuncionario);
    if (itFuncionario == funcionarios_.end()) {
        throw DadoInvalidoException("Funcionário não encontrado com o CPF informado.");
    }

    // 3. Checar a especialidade exigida (Decisão 4)
    if (itFuncionario->second.getEspecialidade() != servico->getEspecialidade()) {
        throw EspecialidadeIncompativelException(
            "O funcionário escolhido não possui a especialidade necessária para este serviço."
        );
    }

    // 4. Checar conflito de horários (Decisão 16)
    for (const auto& ag : agendamentos_) {
        // Agendamentos cancelados não ocupam horário na agenda
        if (ag.getStatus() != Status::Cancelado && ag.getDataHora() == dataHora) {
            
            // Verifica se o funcionário já está ocupado
            if (ag.getCpfFuncionario() == cpfFuncionario) {
                throw HorarioIndisponivelException("O funcionário já possui um agendamento neste horário.");
            }
            
            // Verifica se o cliente já está ocupado
            if (ag.getCpfCliente() == cpfCliente) {
                throw HorarioIndisponivelException("O cliente já possui um agendamento neste horário.");
            }
        }
    }

    // 5. Criar o agendamento se todas as validações passarem.
    // Usamos emplace_back para construir o objeto diretamente no final do vector,
    // e std::move() para transferir a posse do ponteiro unico do serviço.
    agendamentos_.emplace_back(cpfCliente, cpfFuncionario, dataHora, std::move(servico));
}

std::vector<const Agendamento*> Estudio::agendamentosDoCliente(const std::string& cpfCliente) const {
    std::vector<const Agendamento*> resultado;
    for (const auto& ag : agendamentos_) {
        if (ag.getCpfCliente() == cpfCliente) {
            // Guarda o endereço do agendamento (ponteiro) sem copiar o objeto
            resultado.push_back(&ag);
        }
    }
    return resultado;
}

std::vector<const Agendamento*> Estudio::agendamentosDoFuncionario(const std::string& cpfFuncionario) const {
    std::vector<const Agendamento*> resultado;
    for (const auto& ag : agendamentos_) {
        if (ag.getCpfFuncionario() == cpfFuncionario) {
            resultado.push_back(&ag);
        }
    }
    return resultado;
}