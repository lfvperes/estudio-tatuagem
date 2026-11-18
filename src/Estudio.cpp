#include "Estudio.hpp"
#include "DadoInvalidoException.hpp"
#include "EspecialidadeIncompativelException.hpp"
#include "HorarioIndisponivelException.hpp"
#include "EstudioException.hpp"
#include "PersistenciaException.hpp"
#include "ServicoFactory.hpp"
#include "UtilsCsv.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>

const std::string DIR_DADOS = "dados/";
const std::string ARQ_CLIENTES = "clientes.csv";
const std::string ARQ_FUNCIONARIOS = "funcionarios.csv";
const std::string ARQ_AGENDAMENTOS = "agendamentos.csv";


Estudio::Estudio() {}

Estudio::~Estudio() {}

void Estudio::cadastrarCliente(const Cliente& cliente) {
    // Lock para proteger o acesso ao map de clientes
    std::lock_guard<std::mutex> lock(mtx_);
    // Tenta encontrar o cliente; se achar, significa que já existe
    if (clientes_.find(cliente.getCpf()) != clientes_.end()) {
        throw EstudioException("Já existe um cliente cadastrado com este CPF.");
    }
    // Adiciona ao map usando o CPF como chave
    clientes_.insert({cliente.getCpf(), cliente});
}

void Estudio::cadastrarFuncionario(const Funcionario& funcionario) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (funcionarios_.find(funcionario.getCpf()) != funcionarios_.end()) {
        throw EstudioException("Já existe um funcionário cadastrado com este CPF.");
    }
    funcionarios_.insert({funcionario.getCpf(), funcionario});
}

// --- VALIDAÇÃO EXTRAÍDA (entrada 22 de docs/decisoes.md) ---
// usada pelo agendar() e pela carga, para o arquivo passar pelas mesmas regras do menu
void Estudio::validarAgendamento(const std::string& cpfCliente, const std::string& cpfFuncionario,
                                 const std::string& dataHora, const Servico& servico, Status status) const {
    // 1. Checar se o cliente existe
    auto itCliente = clientes_.find(cpfCliente);
    if (itCliente == clientes_.end()) {
        throw DadoInvalidoException("Cliente não encontrado com o CPF informado: " + cpfCliente);
    }

    // 2. Checar se o funcionário existe
    auto itFuncionario = funcionarios_.find(cpfFuncionario);
    if (itFuncionario == funcionarios_.end()) {
        throw DadoInvalidoException("Funcionário não encontrado com o CPF informado: " + cpfFuncionario);
    }

    // 3. Checar a especialidade exigida (entrada 4 de docs/decisoes.md)
    if (itFuncionario->second.getEspecialidade() != servico.getEspecialidade()) {
        throw EspecialidadeIncompativelException("O funcionário escolhido não possui a especialidade necessária para este serviço.");
    }

    // Se o agendamento em validação (carga) já estiver cancelado, ele não conflita com horários
    if (status == Status::Cancelado) return;

    // 4. Checar conflito de horários (entrada 16 de docs/decisoes.md)
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
}

void Estudio::agendar(const std::string& cpfCliente, const std::string& cpfFuncionario,
                      const std::string& dataHora, std::unique_ptr<Servico> servico) {

    std::lock_guard<std::mutex> lock(mtx_);
    validarAgendamento(cpfCliente, cpfFuncionario, dataHora, *servico);

    // Criar o agendamento se todas as validações passarem.
    // Usamos emplace_back para construir o objeto diretamente no final do vector,
    // e std::move() para transferir a posse do ponteiro unico do serviço.
    agendamentos_.emplace_back(cpfCliente, cpfFuncionario, dataHora, std::move(servico));
}

std::vector<const Agendamento*> Estudio::agendamentosDoCliente(const std::string& cpfCliente) const {
    std::lock_guard<std::mutex> lock(mtx_);
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
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<const Agendamento*> resultado;
    for (const auto& ag : agendamentos_) {
        if (ag.getCpfFuncionario() == cpfFuncionario) {
            resultado.push_back(&ag);
        }
    }
    return resultado;
}


// --- PERSISTÊNCIA: SALVAR (entrada 18 de docs/decisoes.md) ---
void Estudio::salvar() const {
    std::lock_guard<std::mutex> lock(mtx_);
    namespace fs = std::filesystem;

    try {
        fs::create_directories(DIR_DADOS);
    } catch (const fs::filesystem_error& e) {
        throw PersistenciaException(DIR_DADOS, "Não foi possível criar o diretório de dados.");
    }

    std::ofstream outClientes(DIR_DADOS + ARQ_CLIENTES);
    if (!outClientes) throw PersistenciaException(ARQ_CLIENTES, "Falha ao abrir arquivo para escrita.");
    outClientes << "cpf;nome;alergiasObservacoes\n";
    for (const auto& [cpf, c] : clientes_) {
        outClientes << c.getCpf() << UtilsCsv::SEPARADOR
                    << c.getNome() << UtilsCsv::SEPARADOR
                    << c.getAlergiasObservacoes() << "\n";
    }

    std::ofstream outFunc(DIR_DADOS + ARQ_FUNCIONARIOS);
    if (!outFunc) throw PersistenciaException(ARQ_FUNCIONARIOS, "Falha ao abrir arquivo para escrita.");
    outFunc << "cpf;nome;especialidade;comissao\n";
    for (const auto& [cpf, f] : funcionarios_) {
        // Reuso limpo da função do header
        outFunc << f.getCpf() << UtilsCsv::SEPARADOR
                << f.getNome() << UtilsCsv::SEPARADOR
                << especialidadeParaString(f.getEspecialidade()) << UtilsCsv::SEPARADOR
                << f.getComissao() << "\n";
    }

    std::ofstream outAg(DIR_DADOS + ARQ_AGENDAMENTOS);
    if (!outAg) throw PersistenciaException(ARQ_AGENDAMENTOS, "Falha ao abrir arquivo para escrita.");
    outAg << "cpfCliente;cpfFuncionario;dataHora;status;pago;tipoServico;camposServico...\n";
    for (const auto& ag : agendamentos_) {
        std::string statusStr = (ag.getStatus() == Status::Agendado) ? "Agendado" :
                                (ag.getStatus() == Status::Confirmado) ? "Confirmado" : "Cancelado";
        std::string pagoStr = ag.isPago() ? "sim" : "nao";

        outAg << ag.getCpfCliente() << UtilsCsv::SEPARADOR
              << ag.getCpfFuncionario() << UtilsCsv::SEPARADOR
              << ag.getDataHora() << UtilsCsv::SEPARADOR
              << statusStr << UtilsCsv::SEPARADOR
              << pagoStr << UtilsCsv::SEPARADOR
              << ag.getServico().gerarCSV() << "\n";
    }
}


// --- PERSISTÊNCIA: CARREGAR (entradas 18, 21 e 22 de docs/decisoes.md) ---
// Função auxiliar para quebrar a linha do CSV
static std::vector<std::string> quebrarLinhaCsv(const std::string& linha) {
    std::vector<std::string> campos;
    std::stringstream ss(linha);
    std::string campo;
    while (std::getline(ss, campo, UtilsCsv::SEPARADOR)) {
        campos.push_back(campo);
    }
    // Tratar colunas vazias no final
    if (!linha.empty() && linha.back() == UtilsCsv::SEPARADOR) {
        campos.push_back("");
    }
    return campos;
}

// não trava o mutex: chama os cadastros, que já travam, e roda antes
// de o auto-save começar (entrada 26 de docs/decisoes.md)
void Estudio::carregar() {
    clientes_.clear();
    funcionarios_.clear();
    agendamentos_.clear();

    std::string linha;

    // Carregar Clientes
    std::ifstream inClientes(DIR_DADOS + ARQ_CLIENTES);
    if (inClientes) {
        int numLinha = 1;
        std::getline(inClientes, linha); // Pula cabeçalho
        while (std::getline(inClientes, linha)) {
            numLinha++;
            UtilsCsv::limparRetornoDeCarro(linha);
            if (linha.empty()) continue;
            try {
                auto campos = quebrarLinhaCsv(linha);
                if (campos.size() != 3) throw DadoInvalidoException("Número incorreto de colunas.");
                cadastrarCliente(Cliente(campos[1], campos[0], campos[2]));
            } catch (const EstudioException& e) {
                throw PersistenciaException(ARQ_CLIENTES, numLinha, e.what());
            }
        }
    }

    // Carregar Funcionários
    std::ifstream inFunc(DIR_DADOS + ARQ_FUNCIONARIOS);
    if (inFunc) {
        int numLinha = 1;
        std::getline(inFunc, linha);
        while (std::getline(inFunc, linha)) {
            numLinha++;
            UtilsCsv::limparRetornoDeCarro(linha);
            if (linha.empty()) continue;
            try {
                auto campos = quebrarLinhaCsv(linha);
                if (campos.size() != 4) throw DadoInvalidoException("Número incorreto de colunas.");

                Especialidade esp;
                if (campos[2] == "Tatuagem") esp = Especialidade::Tatuagem;
                else if (campos[2] == "Piercing") esp = Especialidade::Piercing;
                else throw DadoInvalidoException("Especialidade desconhecida no CSV.");

                // Usa o extrator seguro e elimina o catch(invalid_argument) que falhava no out_of_range
                double comissao = UtilsCsv::extrairDouble(campos[3], "Comissão");
                cadastrarFuncionario(Funcionario(campos[1], campos[0], esp, comissao));
            } catch (const EstudioException& e) {
                throw PersistenciaException(ARQ_FUNCIONARIOS, numLinha, e.what());
            }
        }
    }

    // Carregar Agendamentos
    std::ifstream inAg(DIR_DADOS + ARQ_AGENDAMENTOS);
    if (inAg) {
        int numLinha = 1;
        std::getline(inAg, linha);
        while (std::getline(inAg, linha)) {
            numLinha++;
            UtilsCsv::limparRetornoDeCarro(linha);
            if (linha.empty()) continue;
            try {
                auto campos = quebrarLinhaCsv(linha);
                if (campos.size() < 7) throw DadoInvalidoException("Número incorreto de colunas.");

                Status status;
                if (campos[3] == "Agendado") status = Status::Agendado;
                else if (campos[3] == "Confirmado") status = Status::Confirmado;
                else if (campos[3] == "Cancelado") status = Status::Cancelado;
                else throw DadoInvalidoException("Status desconhecido: " + campos[3]);

                bool pago;
                if (campos[4] == "sim") pago = true;
                else if (campos[4] == "nao") pago = false;
                else throw DadoInvalidoException("Campo pago desconhecido: " + campos[4]);

                // Isola a tag e os campos do serviço polimórfico
                std::string tagServico = campos[5];
                std::vector<std::string> camposServico(campos.begin() + 6, campos.end());

                std::unique_ptr<Servico> servico = ServicoFactory::criarDoCSV(tagServico, camposServico);

                // Passa o status para ignorar conflitos de horários em arquivos restaurados (Cancelados)
                validarAgendamento(campos[0], campos[1], campos[2], *servico, status);
                agendamentos_.emplace_back(campos[0], campos[1], campos[2], std::move(servico), status, pago);

            } catch (const EstudioException& e) {
                throw PersistenciaException(ARQ_AGENDAMENTOS, numLinha, e.what());
            }
        }
    }
}