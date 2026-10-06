# Estúdio de Tatuagem

Sistema de gerenciamento de um estúdio de tatuagem em C++, com interface em
terminal. Projeto desenvolvido para a disciplina de Programação Orientada a
Objetos (ICMC-USP).

## Requisitos da disciplina

- **Classes**: entidades do domínio (pessoas, serviços, agendamentos, estúdio).
- **Herança**: `Cliente` e `Funcionario` herdando de `Pessoa`; `Tatuagem`,
  `Piercing` e `Retoque` herdando de `Servico`; hierarquia de exceções a
  partir de `EstudioException`.
- **Polimorfismo**: métodos virtuais puros `exibirInfo()` (em `Pessoa`) e
  `calcularPreco()` (em `Servico`), usados por meio de
  `std::unique_ptr<Servico>` no `Agendamento`, que é o único dono do
  serviço.
- **Tratamento de erros com exceções**: hierarquia própria de exceções
  derivada de `std::runtime_error`.

As classes ainda não foram implementadas — o diagrama abaixo é o ponto de
partida de design acordado pela dupla; ele pode e deve ser ajustado conforme
o código for escrito.

## Diagrama de classes

```mermaid
classDiagram
    class Pessoa {
        <<abstract>>
        -string nome_
        -string cpf_
        +exibirInfo() void
    }

    class Cliente {
        -string alergiasObservacoes_
        +exibirInfo() void
    }

    class Funcionario {
        -Especialidade especialidade_
        -double comissao_
        +exibirInfo() void
    }

    class Especialidade {
        <<enumeration>>
        Tatuagem
        Piercing
    }

    Pessoa <|-- Cliente
    Pessoa <|-- Funcionario
    Funcionario --> Especialidade
    Servico --> Especialidade

    class Servico {
        <<abstract>>
        -int duracaoMinutos_
        -Especialidade especialidade_
        -double taxaMaoDeObra_
        -double taxaFixaSessao_
        +getDuracaoMinutos() int
        +getEspecialidade() Especialidade
        +getTaxaMaoDeObra() double
        +getTaxaFixaSessao() double
        +detalhes() string
        +calcularPreco() double
    }

    class Tatuagem {
        -Tamanho tamanho_
        -Complexidade complexidade_
        -string descricao_
        +getDescricao() string
        +detalhes() string
        +calcularPreco() double
    }

    class Tamanho {
        <<enumeration>>
        Pequena
        Media
        Grande
    }

    class Complexidade {
        <<enumeration>>
        Baixa
        Media
        Alta
    }

    Tatuagem --> Tamanho
    Tatuagem --> Complexidade

    class Piercing {
        -string tipoJoia_
        -double precoJoia_
        +detalhes() string
        +calcularPreco() double
    }

    class Retoque {
        -bool feitaNoEstudio_
        +isFeitaNoEstudio() bool
        +detalhes() string
        +calcularPreco() double
    }

    Servico <|-- Tatuagem
    Servico <|-- Piercing
    Servico <|-- Retoque

    class Agendamento {
        -string dataHora_
        -Status status_
        -bool pago_
        -unique_ptr~Servico~ servico_
        +confirmar() void
        +cancelar() void
        +pagar() void
    }

    class Estudio {
        -vector~Cliente~ clientes_
        -vector~Funcionario~ funcionarios_
        -vector~Agendamento~ agendamentos_
        +cadastrarCliente(Cliente) void
        +cadastrarFuncionario(Funcionario) void
        +agendar(...) Agendamento
        +agendamentosDoCliente(...) vector~Agendamento~
        +agendamentosDoFuncionario(...) vector~Agendamento~
    }

    Agendamento --> Cliente
    Agendamento --> Funcionario
    Agendamento *-- Servico : dono único
    Estudio --> Cliente
    Estudio --> Funcionario
    Estudio --> Agendamento

    class EstudioException {
        <<runtime_error>>
    }

    class HorarioIndisponivelException
    class EspecialidadeIncompativelException
    class ClienteMenorDeIdadeException
    class DadoInvalidoException
    class CancelamentoForaDoPrazoException

    EstudioException <|-- HorarioIndisponivelException
    EstudioException <|-- EspecialidadeIncompativelException
    EstudioException <|-- ClienteMenorDeIdadeException
    EstudioException <|-- DadoInvalidoException
    EstudioException <|-- CancelamentoForaDoPrazoException
```

## Como compilar e rodar

Requisitos: `g++` com suporte a C++17 e `make`.

> Ainda não há código em `src/`/`include/` — o `make` só vai gerar um
> executável depois que as classes e o `main.cpp` forem escritos.

```bash
make
```

```bash
make run
```

Para limpar os artefatos de build:

```bash
make clean
```

## Estrutura de pastas

```
estudio-tatuagem/
├── include/     # Headers (.hpp) com as declarações das classes
├── src/         # Implementações (.cpp) e main.cpp
├── build/       # Artefatos de compilação (gerado pelo make, ignorado pelo git)
├── Makefile
├── .gitignore
└── README.md
```

## Fluxo de trabalho com Git

Regras da dupla:

1. A branch `main` deve **sempre compilar** (`make` sem erros).
2. Toda mudança é feita em uma branch própria, criada a partir da `main`
   atualizada, nomeada seguindo [Conventional Commits](https://www.conventionalcommits.org/):
   `<tipo>/<descricao-curta>`, por exemplo `feat/cadastro-cliente`,
   `fix/calculo-preco-tatuagem`, `docs/atualizar-readme`.
   Tipos mais usados:
   - `feat/` — nova funcionalidade
   - `fix/` — correção de bug
   - `refactor/` — refatoração sem mudança de comportamento
   - `docs/` — documentação
   - `test/` — testes
   - `chore/` — tarefas de manutenção (build, configs, etc.)
3. As mensagens de commit também devem seguir Conventional Commits:
   `<tipo>: <descrição>` (ex.: `feat: adiciona cadastro de cliente`).
4. Ao terminar, abra um Pull Request para a `main`. **O outro integrante deve
   revisar antes do merge.**
5. Antes de começar algo novo, sempre atualize sua `main` local com
   `git pull`.

### Comandos básicos (para quem tem pouca experiência com git)

Atualizar a `main` local antes de começar algo novo:

```bash
git checkout main
git pull
```

Criar uma branch a partir da main atualizada, usando o padrão
`<tipo>/<descricao-curta>`:

```bash
git checkout -b feat/nome-da-feature
```

Ver o que foi alterado:

```bash
git status
git diff
```

Adicionar e commitar as mudanças:

```bash
git add .
git commit -m "feat: descrição clara da mudança"
```

Enviar a branch para o GitHub:

```bash
git push -u origin feat/nome-da-feature
```

Depois disso, abra o Pull Request na interface do GitHub (ou com
`gh pr create`), peça revisão do colega e só então faça o merge para a
`main`.

Após o merge, volte para a main e atualize antes de começar a próxima tarefa:

```bash
git checkout main
git pull
```

## Divisão de tarefas

| Tarefa | Responsável |
|--------|-------------|
| `Servico` → `Tatuagem`, `Piercing`, `Retoque` | Luís |
| `Pessoa` → `Cliente`, `Funcionario` | Miguel |
| `Agendamento` | a definir |
| Exceções (`EstudioException` e derivadas) | a definir |
| `Estudio` e menu | a definir (em dupla) |

## Autores

- Luís Filipe Vasconcelos Peres
- Miguel Lima
