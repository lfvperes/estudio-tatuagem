#include "ServicoFactory.hpp"
#include "Tatuagem.hpp"
#include "Piercing.hpp"
#include "Retoque.hpp"
#include "PersistenciaException.hpp"

namespace ServicoFactory {

    std::unique_ptr<Servico> criarDoCSV(const std::string& tag, const std::vector<std::string>& campos) {
        try {
            if (tag == "Tatuagem") {
                int duracao = std::stoi(campos.at(0));
                
                Tamanho tam = (campos.at(1) == "Pequena") ? Tamanho::Pequena :
                              (campos.at(1) == "Media") ? Tamanho::Media : Tamanho::Grande;
                              
                Complexidade comp = (campos.at(2) == "Baixa") ? Complexidade::Baixa :
                                    (campos.at(2) == "Media") ? Complexidade::Media : Complexidade::Alta;
                                    
                return std::make_unique<Tatuagem>(duracao, tam, comp, campos.at(3));
                
            } else if (tag == "Piercing") {
                int duracao = std::stoi(campos.at(0));
                return std::make_unique<Piercing>(duracao, campos.at(1), std::stod(campos.at(2)));
                
            } else if (tag == "Retoque") {
                int duracao = std::stoi(campos.at(0));
                bool feitaAqui = (campos.at(1) == "sim");
                return std::make_unique<Retoque>(duracao, feitaAqui);
            }
            
            throw PersistenciaException("agendamentos.csv", "Tag de serviço polimórfico desconhecida: " + tag);
            
        } catch (const std::out_of_range&) {
            throw PersistenciaException("agendamentos.csv", "Colunas insuficientes para recriar o serviço da tag '" + tag + "'.");
        } catch (const std::invalid_argument&) {
            throw PersistenciaException("agendamentos.csv", "Erro numérico nas colunas do serviço da tag '" + tag + "'.");
        }
    }
}