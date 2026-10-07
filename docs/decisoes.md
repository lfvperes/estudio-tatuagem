# Registro de decisões de design

Cada entrada segue o formato: contexto, alternativas, decisão e motivo.
O diagrama de classes no [README](../README.md) reflete o estado atual.

## 1. Quem guarda o `Servico` no `Agendamento`

**Contexto.** `Servico` é uma classe abstrata (`calcularPreco()` é virtual
puro) e o `Agendamento` precisa guardar um serviço de qualquer tipo
concreto (`Tatuagem`, `Piercing` ou `Retoque`).

**Alternativas.**
- *Por valor* (`Servico servico_`): não compila, pois não se cria objeto de
  classe abstrata. Mesmo numa classe concreta haveria *slicing*: ao copiar
  uma `Tatuagem` para um `Servico`, os atributos e o comportamento da filha
  se perdem.
- *Por referência*: não é dona do objeto e não controla o ciclo de vida.
- *Ponteiro inteligente*: permite polimorfismo. Resta escolher entre
  `shared_ptr` (vários donos) e `unique_ptr` (um único dono).

**Decisão.** O `Agendamento` é o único dono do `Servico`, via
`std::unique_ptr<Servico>`.

**Motivo.** Um serviço pertence a um agendamento específico (descrição,
duração e tamanho são daquela sessão) e nenhuma outra classe o guarda:
`Estudio` chega ao serviço pelos agendamentos, e o histórico do cliente não
guarda objetos `Servico`. Cancelar muda o `status_`, não apaga o
agendamento, então o serviço continua válido. O professor não exige
`shared_ptr`, e `unique_ptr` expressa a posse única no próprio tipo.

**Consequência.** `Agendamento` não é copiável, apenas movível. Funciona
normalmente em `vector<Agendamento>` e como retorno de função.

**Revisitar se** outra classe passar a precisar do mesmo `Servico` (por
exemplo, um `Retoque` que referencie a `Tatuagem` original).

## 2. Histórico de sessões do cliente

**Contexto.** O diagrama inicial tinha `historicoSessoes_` (`vector<string>`)
em `Cliente`.

**Alternativas.** Manter o histórico em `Cliente` como texto, ou derivá-lo
dos agendamentos que o `Estudio` já guarda.

**Decisão.** Remover `historicoSessoes_` do `Cliente`. O `Estudio` oferece
consultas `agendamentosDoCliente(...)` e `agendamentosDoFuncionario(...)`.

**Motivo.** Uma string perde a estrutura (data, serviço, preço) e duplica
informação que já está nos `Agendamento`s. A mesma consulta serve para o
funcionário, o que também viabiliza o cálculo de comissão.

## 3. Tatuador e piercer: `Funcionario` com uma especialidade

**Contexto.** O estúdio tem tatuadores e também faz piercing. Surgiu a
dúvida se haveria subclasses por função.

**Alternativas.**
- Subclasses `Tatuador` e `Piercer` de `Funcionario`.
- Uma única classe `Funcionario` com a especialidade como atributo.
- Atributo com várias especialidades (`vector` ou `set`).

**Decisão.** Uma classe `Funcionario` (herda de `Pessoa`) com um único
atributo `Especialidade especialidade_`, um enum com `Tatuagem` e
`Piercing`.

**Motivo.** As duas funções não diferem em comportamento, só no que o
funcionário faz, então subclasses seriam exagero. Aceitar várias
especialidades cobriria um caso que o estúdio não tem hoje; mantemos simples
e revisitamos se for preciso. A hierarquia de `Pessoa` continua útil para a
disciplina: `Cliente` e `Funcionario` implementam `exibirInfo()` de forma
própria.

## 4. Especialidade exigida pelo serviço

**Contexto.** Para lançar `EspecialidadeIncompativelException`, o `Estudio`
compara a especialidade do funcionário com a que o serviço exige.

**Alternativas.**
- O `Estudio` descobrir o tipo concreto do serviço com `if`s: descartada,
  pois obriga a alterar o `Estudio` a cada novo tipo de serviço.
- Método virtual puro `especialidadeExigida()` em `Servico`.
- Atributo `especialidade_` em `Servico` com getter.

**Decisão.** Atributo `Especialidade especialidade_` com
`getEspecialidade() const` em `Servico`. Cada filha passa o valor fixo ao
construtor da base (`Tatuagem` e `Retoque` passam `Tatuagem`; `Piercing`
passa `Piercing`).

**Motivo.** A especialidade exigida é fixa por tipo e não depende do estado
do objeto, então um dado basta, e é mais simples do que um método sobrescrito
em cada filha. Como cada filha fixa o seu valor, quem usa a classe não
escolhe errado. O polimorfismo continua presente em `calcularPreco()`.

**Revisitar se** a especialidade passar a depender do estado do serviço
(por exemplo, tatuagem complexa exigir funcionário mais experiente). Aí o
método virtual se justifica.

**Premissa.** `Retoque` exige `Especialidade::Tatuagem`, pois só existe
retoque de tatuagem.

## 5. Pagamento

**Contexto.** O diagrama inicial não tinha pagamento.

**Alternativas.** Classe `Pagamento` própria (forma, valor, data), ou um
estado no `Agendamento`.

**Decisão.** `Agendamento` ganha `pago_` e `pagar()`. O valor vem de
`servico_->calcularPreco()`.

**Motivo.** Mais simples, e uma classe `Pagamento` não traria conceitos de
POO que as outras partes já não pratiquem (composição, associação).

**Pendente.** Definir se pagar duas vezes ou pagar agendamento cancelado
lança exceção própria.

## 6. `Especialidade`: `enum class` em header próprio

**Contexto.** `Especialidade` (`Tatuagem`, `Piercing`) é usada por `Servico`
e por `Funcionario`. Os nomes dos enumeradores coincidem com os das classes
`Tatuagem` e `Piercing`.

**Alternativas.**
- Declarar o enum dentro de `Funcionario.hpp`: `Servico` passaria a depender
  de `Funcionario` só por causa do enum, com risco de include circular.
- `enum` comum: os enumeradores caem no escopo global e conflitam com as
  classes de mesmo nome.
- `enum class` em header próprio.

**Decisão.** `enum class Especialidade` em `include/Especialidade.hpp`,
que contém apenas o enum.

**Motivo.** O header próprio mantém `Servico` e `Funcionario` independentes
entre si. O `enum class` mantém os enumeradores dentro do escopo
(`Especialidade::Tatuagem`), o que elimina o conflito de nomes e impede
conversões implícitas para inteiro.

## 7. Preço dos serviços

**Contexto.** Cada serviço calcula o preço com regra própria
(`calcularPreco()`), mas todos têm duas parcelas em comum: uma taxa fixa por
sessão e a mão de obra. O diagrama inicial só previa `precoBase_` e
`percentualDesconto_` em `Retoque`, sem nada equivalente nas demais.

**Alternativas.**
- Constantes em cada `.cpp`, com `Servico` sem saber de preço.
- Taxas como atributos de `Servico`, com cada filha informando seus valores ao
  construtor da base.

**Decisão.** `Servico` guarda `taxaFixaSessao_` e `taxaMaoDeObra_` (com
getters). Cada filha define seus valores como constantes nomeadas no próprio
`.cpp` e os passa ao construtor da base. Só na `Tatuagem` a mão de obra é
multiplicada por fatores; nas demais, apenas soma.

| Serviço | Taxa fixa | Mão de obra | Fórmula |
|---|---|---|---|
| `Tatuagem` | 30 | 50 | `fixa + mão de obra × fatorTamanho × fatorComplexidade` |
| `Piercing` | 30 | 30 | `precoJoia_ + fixa + mão de obra` |
| `Retoque` | 30 | 40 | `fixa + mão de obra`, com 30% de desconto se `feitaNoEstudio_` |

- Fatores de tamanho: pequena 1, média 2, grande 4. Complexidade: baixa 1,
  média 1,5, alta 2.
- `Tamanho` e `Complexidade` são `enum class` em `Tatuagem.hpp` (não há área
  em cm²). Tatuagem muito grande vira várias sessões, uma por `Agendamento`.
- `Retoque` não tem `precoBase_` nem tamanho. O desconto de 30% vale sobre as
  duas taxas.

**Risco aceito.** Os dois `double` do construtor de `Servico` podem ser
trocados sem aviso do compilador. Aceito por haver apenas três chamadas (uma
por filha). Se o projeto crescer: agrupar as taxas num `struct` com campos
nomeados, ou usar um tipo distinto para cada taxa.

**Motivo.**
- Valores (taxas, fatores, desconto): escolhidos pela dupla e conferidos como
  realistas com uma pessoa que entende de tatuagem. São ajustáveis.
- Taxas na base: padroniza que todo serviço tem as duas parcelas, e os
  getters ficam disponíveis a quem tiver um `Servico*`. O custo é um
  construtor da base com quatro parâmetros.
- `Retoque` guarda o fato (`feitaNoEstudio_`) e não o desconto: a regra fica
  só em `calcularPreco()`, e mudá-la não altera os dados guardados.

**Revisitar se** `Retoque` precisar de tamanho (mover os enums para um header
próprio).

**Pendente.** `DadoInvalidoException` não existe: `TODO` em `Servico` (duração
`<= 0`) e em `fatorTamanho`/`fatorComplexidade` (enum desconhecido).

## 8. Texto de exibição dos serviços

**Contexto.** O `Estudio` precisa mostrar um serviço (por exemplo, ao listar
agendamentos) sem saber de que tipo ele é, e o que se mostra depende do tipo:
tamanho e descrição na `Tatuagem`, joia no `Piercing`, origem no `Retoque`.

**Alternativas.**
- Um getter por atributo em cada filha, com o `Estudio` montando o texto.
- Método virtual puro, com cada filha escrevendo o texto inteiro.
- Método virtual com implementação padrão na base, estendido pelas filhas.

**Decisão.** `Servico` tem `virtual std::string detalhes() const` com
implementação padrão (a duração, por exemplo `120 min`). Cada filha a
sobrescreve, chama `Servico::detalhes()` e monta o próprio texto. O preço não
entra no texto: o `Estudio` o obtém por `calcularPreco()`.

- `Tatuagem (grande, complexidade alta, 120 min): <descrição>`
- `Piercing (argola, 30 min)`
- `Retoque (feita no estúdio, 60 min)`

**Motivo.**
- Getters por atributo obrigariam o `Estudio` a descobrir o tipo de cada
  serviço (um `if` por tipo, descartado também na entrada 4), a ser alterado
  a cada serviço novo e a conhecer os atributos internos das filhas.
- A implementação padrão formata a duração em um lugar só; as filhas
  acrescentam apenas o que é delas.

**Revisitar se** a interface precisar de outro formato (colunas, tabela): aí
convém separar a formatação das classes de domínio.

## Fora do escopo por ora

- Forma de pagamento (dinheiro, Pix, cartão) e pagamento parcial.
