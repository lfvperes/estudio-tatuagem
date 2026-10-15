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

int main() {
    Estudio estudio;
    int opcao = -1;

    while (opcao != 0) {
        exibirMenu();
        
        // Tenta ler a opção
        if (!(std::cin >> opcao)) {
            // Deteta se o utilizador forçou a saída (Ctrl+D / EOF)
            if (std::cin.eof()) {
                std::cout << "\nFim de entrada detetado. A sair...\n";
                break; // Sai do loop imediatamente
            }
            
            // Se não foi EOF, foi alguém que digitou uma letra no lugar de um número
            limparBuffer();
            std::cout << "Entrada inválida. Tente novamente.\n";
            continue;
        }
        limparBuffer(); // Limpa o '\n' que sobrou do cin >> opcao
        
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
                std::cin >> espOpcao;
                std::cout << "Comissão (%): ";
                std::cin >> comissao;
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
                std::cin >> tipoServico;
                std::cout << "Duração (minutos): ";
                std::cin >> duracao;
                limparBuffer();

                std::unique_ptr<Servico> servico;

                if (tipoServico == 1) {
                    int tamOpcao, compOpcao;
                    std::string descricao;
                    std::cout << "Tamanho (0 - Pequena, 1 - Média, 2 - Grande): ";
                    std::cin >> tamOpcao;
                    std::cout << "Complexidade (0 - Baixa, 1 - Média, 2 - Alta): ";
                    std::cin >> compOpcao;
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
                    std::cin >> precoJoia;
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

                // O std::move transfere a posse do ponteiro único para o Estudio e, subsequentemente, para o Agendamento
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
            // Captura qualquer exceção do sistema (HorarioIndisponivel, DadoInvalido, etc.)
            std::cout << "\n[ERRO DE SISTEMA] " << e.what() << "\n";
        } catch (const std::exception& e) {
            // Captura erros padrão do C++
            std::cout << "\n[ERRO DESCONHECIDO] " << e.what() << "\n";
        }
    }

    std::cout << "Encerrando o sistema...\n";
    return 0;
}