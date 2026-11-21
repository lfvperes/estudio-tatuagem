#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

namespace UtilsCsv {

    void validarTextoLivre(const std::string& texto, const std::string& nomeDoCampo) {
        // Procurar o separador ';' ou a quebra de linha normal '\n'
        std::string caracteresInvalidos = std::string(1, SEPARADOR) + "\n";
        
        if (texto.find_first_of(caracteresInvalidos) != std::string::npos) {
            throw DadoInvalidoException("O campo " + nomeDoCampo + " não pode conter o caractere '" 
                                        + SEPARADOR + "' ou quebra de linha.");
        }
    }

    void limparRetornoDeCarro(std::string& linha) {
        if (!linha.empty() && linha.back() == '\r') {
            linha.pop_back();
        }
    }
}