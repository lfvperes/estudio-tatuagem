#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include <vector>

#include "Estudio.hpp"
#include "Cliente.hpp"
#include "Funcionario.hpp"
#include "Tatuagem.hpp"
#include "Piercing.hpp"
#include "Retoque.hpp"
#include "EstudioException.hpp"
#include "DadoInvalidoException.hpp" // <-- Header adicionado

// Função auxiliar para limpar o buffer do teclado
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

int main() {
    Estudio estudio;
    int opcao = -1;

    while (opcao != 0) {
        exibirMenu();
        
        // Tenta ler a opção do menu
        if (!(std::cin >> opcao)) {
            if (std::cin.eof()) {
                std::cout << "\nFim de entrada detetado. A sair...\n";
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
                std::cout << "CPF: ";
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
                std::cout << "CPF: ";
                std::getline(std::cin, cpf);
                
                std::cout << "Especialidade (1 - Tatuagem, 2 - Piercing): ";
                if (!(std::cin >> espOpcao)) {
                    limparBuffer();
                    throw DadoInvalidoException("Entrada inválida. Digite um número para a especialidade.");
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
                int tipoServico, duracao;

                std::cout << "CPF do Cliente: ";
                std::getline(std::cin, cpfC);
                std::cout << "CPF do Funcionário: ";
                std::getline(std::cin, cpfF);
                std::cout << "Data e Hora (AAAA-MM-DD HH:MM): ";
                std::getline(std::cin, dataHora);

                std::cout << "Tipo de Serviço (1 - Tatuagem, 2 - Piercing, 3 - Retoque): ";
                if (!(std::cin >> tipoServico)) {
                    limparBuffer();
                    throw DadoInvalidoException("Entrada inválida. Digite um número para o tipo de serviço.");
                }
                
                std::cout << "Duração (minutos): ";
                if (!(std::cin >> duracao)) {
                    limparBuffer();
                    throw DadoInvalidoException("Entrada inválida. Digite um número para a duração.");
                }
                limparBuffer();

                std::unique_ptr<Servico> servico;

                if (tipoServico == 1) {
                    int tamOpcao, compOpcao;
                    std::string descricao;
                    
                    std::cout << "Tamanho (0 - Pequena, 1 - Média, 2 - Grande): ";
                    if (!(std::cin >> tamOpcao)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para o tamanho.");
                    }
                    
                    std::cout << "Complexidade (0 - Baixa, 1 - Média, 2 - Alta): ";
                    if (!(std::cin >> compOpcao)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para a complexidade.");
                    }
                    limparBuffer();
                    
                    std::cout << "Descrição: ";
                    std::getline(std::cin, descricao);

                    Tamanho tam = static_cast<Tamanho>(tamOpcao);
                    Complexidade comp = static_cast<Complexidade>(compOpcao);
                    servico = std::make_unique<Tatuagem>(duracao, tam, comp, descricao);

                } else if (tipoServico == 2) {
                    std::string tipoJoia;
                    double precoJoia;
                    
                    std::cout << "Tipo da Joia: ";
                    std::getline(std::cin, tipoJoia);
                    
                    std::cout << "Preço da Joia: ";
                    if (!(std::cin >> precoJoia)) {
                        limparBuffer();
                        throw DadoInvalidoException("Entrada inválida. Digite um número para o preço.");
                    }
                    limparBuffer();

                    servico = std::make_unique<Piercing>(duracao, tipoJoia, precoJoia);

                } else if (tipoServico == 3) {
                    char feitaAqui;
                    std::cout << "Tatuagem original foi feita neste estúdio? (s/n): ";
                    std::cin >> feitaAqui;
                    limparBuffer();

                    bool feitaNoEstudio = (feitaAqui == 's' || feitaAqui == 'S');
                    servico = std::make_unique<Retoque>(duracao, feitaNoEstudio);
                } else {
                    std::cout << "Opção de serviço inválida.\n";
                    continue;
                }

                estudio.agendar(cpfC, cpfF, dataHora, std::move(servico));
                std::cout << "Agendamento realizado com sucesso!\n";

            } else if (opcao == 4 || opcao == 5) {
                std::string cpf;
                std::cout << "Informe o CPF: ";
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
                                  << " | Status: " << (int)ag->getStatus()
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
    
    std::cout << "Encerrando o sistema...\n";
    return 0;
}