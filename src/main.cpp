#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <vector>

#include "Estudio.hpp"
#include "Cliente.hpp"
#include "Funcionario.hpp"
#include "ServicoFactory.hpp"
#include "EstudioException.hpp"
#include "DadoInvalidoException.hpp"
#include "PersistenciaException.hpp"
#include "AutoSalvamento.hpp"

// Função auxiliar para limpar o buffer do teclado (evita saltos no std::getline)
void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void exibirMenu() {
    std::cout << "\n=== SISTEMA DO ESTÚDIO DE TATUAGEM ===" << std::endl;
    std::cout << "1. Cadastrar Cliente" << std::endl;
    std::cout << "2. Cadastrar Funcionário" << std::endl;
    std::cout << "3. Agendar Serviço" << std::endl;
    std::cout << "4. Listar Agendamentos de um Cliente" << std::endl;
    std::cout << "5. Listar Agendamentos de um Funcionário" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}

// Função auxiliar para converter o enum Status em um texto legível para o usuário
static std::string statusParaString(Status status) {
    switch (status) {
        case Status::Agendado: return "Agendado";
        case Status::Confirmado: return "Confirmado";
        case Status::Cancelado: return "Cancelado";
    }
    return "?";
}

int main() {
    Estudio estudio;

    // 1. CARREGAR (entrada 23 de docs/decisoes.md)
    try {
        estudio.carregar();
    } catch (const PersistenciaException& e) {
        std::cout << "\n[ERRO CRÍTICO NA INICIALIZAÇÃO DOS DADOS]\n" << e.what() << "\n";
        std::cout << "O sistema será encerrado para evitar corrupção ou sobrescrita de arquivos.\n";
        return 1;
    }

    int opcao = -1;

    // Bloco de escopo: o autoSave é destruído ao fechar o bloco, antes do
    // salvar() final (entrada 25 de docs/decisoes.md)
    {
        // Inicia a thread de salvamento periódico
        AutoSalvamento autoSave(estudio);

        while (opcao != 0) {
            exibirMenu();

            // Tenta ler a opção do menu
            if (!(std::cin >> opcao)) {
                if (std::cin.eof()) {
                    std::cout << "\nFim de entrada detectado. Saindo...\n";
                    break;
                }
                limparBuffer();
                std::cout << "Entrada inválida. Tente novamente.\n";
                continue;
            }
            limparBuffer();

            try {
                if (opcao == 1) {
                    std::string nome, cpf, observacoes;
                    std::cout << "Nome do Cliente: ";
                    std::getline(std::cin, nome);
                    std::cout << "CPF (11 dígitos, apenas números): ";
                    std::getline(std::cin, cpf);
                    std::cout << "Alergias/Observações (opcional): ";
                    std::getline(std::cin, observacoes);

                    Cliente c(nome, cpf, observacoes);
                    estudio.cadastrarCliente(c);
                    std::cout << "Cliente cadastrado com sucesso!\n";

                } else if (opcao == 2) {
                    std::string nome, cpf;
                    int espOpcao;
                    double comissao;

                    std::cout << "Nome do Funcionário: ";
                    std::getline(std::cin, nome);
                    std::cout << "CPF (11 dígitos, apenas números): ";
                    std::getline(std::cin, cpf);

                    std::cout << "Especialidade (1 - Tatuagem, 2 - Piercing): ";
                    if (!(std::cin >> espOpcao)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para a especialidade.");
                    }
                    // Nova validação de intervalo
                    if (espOpcao != 1 && espOpcao != 2) {
                        throw DadoInvalidoException("Opção de especialidade inválida. Escolha 1 ou 2.");
                    }

                    std::cout << "Comissão (%): ";
                    if (!(std::cin >> comissao)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para a comissão.");
                    }
                    limparBuffer();

                    Especialidade esp = (espOpcao == 2) ? Especialidade::Piercing : Especialidade::Tatuagem;
                    Funcionario f(nome, cpf, esp, comissao);
                    estudio.cadastrarFuncionario(f);
                    std::cout << "Funcionário cadastrado com sucesso!\n";

                } else if (opcao == 3) {
                    std::string cpfC, cpfF, dataHora;
                    int tipoServico;

                    std::cout << "CPF do Cliente (11 dígitos, apenas números): ";
                    std::getline(std::cin, cpfC);
                    std::cout << "CPF do Funcionário (11 dígitos, apenas números): ";
                    std::getline(std::cin, cpfF);
                    std::cout << "Data e Hora (AAAA-MM-DD HH:MM): ";
                    std::getline(std::cin, dataHora);

                    std::cout << "Tipo de Serviço (1 - Tatuagem, 2 - Piercing, 3 - Retoque): ";
                    if (!(std::cin >> tipoServico)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para o tipo de serviço.");
                    }
                    limparBuffer();

                    std::unique_ptr<FabricaServico> fabrica = ServicoFactory::criarFabrica(tipoServico);
                    std::vector<std::string> campos;

                    for (const Pergunta& pergunta : fabrica->perguntas()) {
                        if (pergunta.opcoes.empty()) {
                            std::string resposta;
                            std::cout << pergunta.texto << ": ";
                            std::getline(std::cin, resposta);
                            campos.push_back(resposta);
                        } else {
                            std::cout << pergunta.texto << " (";
                            for (std::size_t i = 0; i < pergunta.opcoes.size(); ++i) {
                                if (i > 0) std::cout << ", ";
                                std::cout << i + 1 << " - " << pergunta.opcoes[i];
                            }
                            std::cout << "): ";

                            int escolha;
                            if (!(std::cin >> escolha)) {
                                limparBuffer();
                                throw DadoInvalidoException("Entrada inválida. Digite o número de uma das opções.");
                            }
                            limparBuffer();

                            // as opções aparecem a partir de 1; o vector começa em 0
                            escolha -= 1;

                            if (escolha < 0 || static_cast<std::size_t>(escolha) >= pergunta.opcoes.size()) {
                                throw DadoInvalidoException("Opção inválida para " + pergunta.texto + ".");
                            }

                            campos.push_back(pergunta.opcoes[escolha]);
                        }
                    }

                    std::unique_ptr<Servico> servico = fabrica->criar(campos);
                    estudio.agendar(cpfC, cpfF, dataHora, std::move(servico));
                    std::cout << "Agendamento realizado com sucesso!\n";

                } else if (opcao == 4 || opcao == 5) {
                    std::string cpf;
                    std::cout << "Informe o CPF (11 dígitos, apenas números): ";
                    std::getline(std::cin, cpf);

                    std::vector<const Agendamento*> lista = (opcao == 4)
                        ? estudio.agendamentosDoCliente(cpf)
                        : estudio.agendamentosDoFuncionario(cpf);

                    if (lista.empty()) {
                        std::cout << "Nenhum agendamento encontrado para este CPF.\n";
                    } else {
                        std::cout << "\n--- Agendamentos Encontrados ---\n";
                        for (const Agendamento* ag : lista) {
                            std::cout << "Data: " << ag->getDataHora()
                                      << " | Status: " << statusParaString(ag->getStatus())
                                      << " | Detalhes: " << ag->getServico().detalhes()
                                      << " | Preço: " << ag->getServico().calcularPreco() << "\n";
                        }
                    }
                } else if (opcao != 0) {
                    std::cout << "Opção inválida.\n";
                }

            } catch (const EstudioException& e) {
                std::cout << "\n[ERRO DE SISTEMA] " << e.what() << "\n";
            } catch (const std::exception& e) {
                std::cout << "\n[ERRO DESCONHECIDO] " << e.what() << "\n";
            }
        }
    } // aqui o autoSave sai de escopo: o destrutor para a thread e faz o join()

    // 2. SALVAR NA SAÍDA (entrada 23 de docs/decisoes.md)
    std::cout << "Encerrando o sistema...\n";
    try {
        estudio.salvar();
        std::cout << "Dados salvos com sucesso na pasta 'dados/'.\n";
    } catch (const PersistenciaException& e) {
        std::cout << "\n[FALHA AO SALVAR DADOS]\n" << e.what() << "\n";
        std::cout << "Aviso: Suas últimas alterações podem não ter sido gravadas.\n";
        return 1;
    }

    return 0;
}