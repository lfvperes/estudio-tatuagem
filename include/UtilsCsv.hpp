#ifndef UTILS_CSV_HPP
#define UTILS_CSV_HPP

#include <string>
#include <sstream>
#include <type_traits>
#include "DadoInvalidoException.hpp"

namespace UtilsCsv {
    const char SEPARADOR = ';';

    // Valida se a string contém o separador ou quebras de linha (\n)
    // Lança DadoInvalidoException se contiver.
    void validarTextoLivre(const std::string& texto, const std::string& nomeDoCampo);

    // Remove o '\r' do final de uma linha lida de um arquivo: um CSV editado no
    // Windows termina as linhas com "\r\n", e o getline só remove o '\n'
    void limparRetornoDeCarro(std::string& linha);

    // Template genérico para extrair números e recusar lixo (entrada 29 de docs/decisoes.md)
    template <typename T>
    T extrair(const std::string& texto, const std::string& nomeCampo) {
        // Blinda a função em tempo de compilação: só aceita tipos numéricos
        static_assert(std::is_arithmetic_v<T>, "O tipo fornecido para extrair deve ser numérico (int, double, etc).");

        T valor{};
        std::istringstream ss(texto);

        // Tenta ler o valor numérico
        ss >> valor;

        // Se a leitura falhar (ex: texto no lugar de número) ou se
        // sobrar qualquer coisa no buffer (ex: "60min" ou espaço no fim)
        if (ss.fail() || !ss.eof()) {
            throw DadoInvalidoException("O campo " + nomeCampo + " não contém um número válido.");
        }

        return valor;
    }
}

#endif // UTILS_CSV_HPP
