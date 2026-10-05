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

## Fora do escopo por ora

- Forma de pagamento (dinheiro, Pix, cartão) e pagamento parcial.
