#include "Agendamento.hpp"
#include "DadoInvalidoException.hpp"
#include "CancelamentoForaDoPrazoException.hpp"
#include <utility>
#include <ctime>
#include <iomanip>  // get_time
#include <sstream>  // istringstream

constexpr int PRAZO_CANCELAMENTO = 24*60*60;

// converte e valida data 
static std::time_t converterDataHora(const std::string& dataHora) {
    // inicializa struct com campos zerados
    std::tm tm{};
    // le da string
    std::istringstream ss(dataHora);
    // le o que consegue e para
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    // detectar caracteres depois da data e hora
    char extra;
    std::string mensagem = "Data fornecida inválida: " + dataHora + ". Use AAAA-MM-DD HH:MM.";
    if (dataHora.size() != 16 || ss.fail() || ss >> extra)
        throw DadoInvalidoException(mensagem);

    // armazena antes da normalizacao
    int dia = tm.tm_mday;
    int mes = tm.tm_mon;
    std::time_t dataConvertida = std::mktime(&tm);
    // erro de conversao
    if (dataConvertida == -1)
        throw DadoInvalidoException(mensagem);
    // mktime normaliza datas impossíveis (31/02 vira 03/03) sem avisar
    // se dia ou mes mudaram depois da conversao, a data original nao existia
    if (dia != tm.tm_mday || mes != tm.tm_mon)
        throw DadoInvalidoException("Data inexistente no calendário: " + dataHora);
    
    return dataConvertida;
}

// std::move: o unique_ptr não pode ser copiado, então a posse do serviço
// é transferida do parâmetro para o atributo
Agendamento::Agendamento(const std::string& cpfCliente, const std::string& cpfFuncionario, const std::string& dataHora, std::unique_ptr<Servico> servico) : status_(Status::Agendado), pago_(false), cpfCliente_(cpfCliente), cpfFuncionario_(cpfFuncionario), dataHora_(dataHora), servico_(std::move(servico)) {
    // apenas validar, sem usar o retorno
    (void)converterDataHora(dataHora);
}

// confirma se agendado e nao confirmado nem cancelado
void Agendamento::confirmar() {
    if (status_ == Status::Agendado)
        status_ = Status::Confirmado;
    else if (status_ == Status::Confirmado)
        throw EstudioException("Agendamento já confirmado.");
    else
        throw EstudioException("Agendamento cancelado.");
}

// cancela se dentro do prazo e se já nao cancelado
void Agendamento::cancelar() {
    if (status_ == Status::Cancelado)
        throw EstudioException("Agendamento já cancelado.");
    
    // checar prazo de cancelamento: compara com agora
    if (std::time(nullptr) <= converterDataHora(dataHora_) - PRAZO_CANCELAMENTO)
        status_ = Status::Cancelado;
    else
        throw CancelamentoForaDoPrazoException("Cancelamento fora do prazo de 24h para o agendamento de " + dataHora_ + ".");
}

// paga se confirmado e nao pago
void Agendamento::pagar() {
    if (pago_)
        throw EstudioException("Agendamento já pago.");
    if (status_ == Status::Confirmado)
        pago_ = true;
    else
        throw EstudioException("Agendamento não confirmado.");
}

std::string Agendamento::getCpfCliente() const {
    return cpfCliente_;
}

std::string Agendamento::getCpfFuncionario() const {
    return cpfFuncionario_;
}

std::string Agendamento::getDataHora() const {
    return dataHora_;
}

Status Agendamento::getStatus() const {
    return status_;
}

bool Agendamento::isPago() const {
    return pago_;
}

const Servico& Agendamento::getServico() const {
    return *servico_;
}