#ifndef UTILS_CSV_HPP
#define UTILS_CSV_HPP

#include <string>

namespace UtilsCsv {
    const char SEPARADOR = ';';

    // Valida se a string contém o separador ou quebras de linha (\n)
    // Lança DadoInvalidoException se contiver.
    void validarTextoLivre(const std::string& texto, const std::string& nomeDoCampo);

    // Remove o '\r' do final de uma linha lida de um arquivo: um CSV editado no
    // Windows termina as linhas com "\r\n", e o getline só remove o '\n'
    void limparRetornoDeCarro(std::string& linha);

    // Extrações numéricas seguras
    int extrairInt(const std::string& texto, const std::string& nomeCampo);
    double extrairDouble(const std::string& texto, const std::string& nomeCampo);
}

#endif // UTILS_CSV_HPP