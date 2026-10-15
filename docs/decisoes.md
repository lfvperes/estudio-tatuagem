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
normalmente em `vector<Agendamento>` e como retorno de função. O serviço é
criado pelo menu e entregue pronto (entrada 11).

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

**Resolvido.** Pagar duas vezes, ou pagar agendamento cancelado ou não
confirmado, lança `EstudioException` (entradas 14 e 15).

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

**Exceções.**
- Em `Servico`, duração `<= 0` lança `DadoInvalidoException`.
- Em `Tatuagem`, enum desconhecido lança `DadoInvalidoException`. Tratado como dado inválido por simplicidade; um `std::logic_error` seria mais preciso (erro de programação) e evitaria que o menu o trate como erro do usuário, mas exigiria outra família de exceção. Este caminho é praticamente inalcançável.

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

## 9. Hierarquia de exceções e mensagens

**Contexto.** O diagrama prevê `EstudioException` (derivada de
`std::runtime_error`) e quatro filhas. Faltava definir quem escolhe o texto
de cada erro. O diagrama previa também `ClienteMenorDeIdadeException`, mas
ela foi removida: usá-la exigiria alterar o diagrama e os atributos já
implementados.

**Alternativas.**
- Todas as filhas recebem a mensagem de quem lança.
- Todas têm mensagem fixa dentro da classe.

**Decisão.** `HorarioIndisponivelException`,
`EspecialidadeIncompativelException`, `DadoInvalidoException` e
`CancelamentoForaDoPrazoException` recebem `const std::string&` de quem
lança. As exceções não guardam a mensagem: ela fica no `std::runtime_error`
e é lida com `what()`. Cada exceção tem seu `.hpp` e seu `.cpp`, como no
resto do projeto.

**Motivo.** Em todas há dado concreto (qual horário, qual dado inválido,
qual era o prazo, qual especialidade faltou), e a mensagem de quem lança é
mais útil ao usuário. Os tipos separados deixam o `catch` reagir de forma
diferente a cada erro sem ler o texto.

**Revisitar se** quiser padronizar o texto:
`EspecialidadeIncompativelException` poderia receber a especialidade
exigida e a fornecida (`Especialidade.hpp` já está na `main`) e montar a
mensagem sozinha. Foi criada recebendo texto porque o header ainda não
estava na `main`.

## 10. Cliente e funcionário no `Agendamento`: referência por CPF

**Contexto.** O `Agendamento` precisa saber quem é o cliente e quem é o
funcionário. O diagrama inicial os ligava por associação direta.

**Alternativas.**
- Guardar cópias de `Cliente` e `Funcionario`: duplica dados, que ficariam
  desatualizados.
- `shared_ptr` nos dois lados: dá acesso direto, mas traz contagem de
  referências, risco de ciclo e semântica de remoção menos óbvia.
- Guardar só o CPF (`cpfCliente_`, `cpfFuncionario_`) e buscar a pessoa no
  `Estudio`.

**Decisão.** `Agendamento` guarda os CPFs como `std::string`. O `Estudio`
guarda `std::map` por CPF para clientes e funcionários, no lugar de
`vector`.

**Motivo.** É a opção mais simples e deixa claro quem é dono de cada objeto
(o `Estudio`). A busca por CPF no `map` é direta. Checar se os CPFs existem
é responsabilidade do `Estudio`, não do `Agendamento`.

**Revisitar se** o `Agendamento` precisar chamar métodos de `Cliente` ou
`Funcionario` com frequência.

## 11. O menu cria o `Servico` e o `Estudio` o entrega ao `Agendamento`

**Contexto.** Como `Servico` é abstrato, alguém precisa decidir o tipo
concreto e coletar seus dados (tamanho, joia, etc.).

**Decisão.** O menu cria o serviço (`std::make_unique<Tatuagem>(...)`, etc.).
O `Estudio` recebe o `unique_ptr<Servico>` pronto e o repassa ao construtor
do `Agendamento` com `std::move`.

**Motivo.** Só o menu conhece a escolha do usuário, e o `Estudio` não
precisa saber os tipos concretos (consistente com a entrada 4).

**Consequência.** Se o construtor lançar `DadoInvalidoException` (por
exemplo, data inválida), o serviço é destruído junto, sem vazamento, graças
ao `unique_ptr`. O menu deve recriar o serviço se o usuário digitar a data
de novo.

## 12. `Status` com três valores e `pago_` separado

**Contexto.** O agendamento passa por etapas, e o pagamento é um fato à
parte (entrada 5).

**Alternativas.** Um estado único com `Pago` incluso, ou estados adicionais
como `Concluido` e `NaoCompareceu`.

**Decisão.** `enum class Status { Agendado, Confirmado, Cancelado }`, com
`Agendado` como estado inicial, e `pago_` como `bool` independente. Não há
cancelamento automático por falta de confirmação.

**Motivo.** Misturar pagamento e confirmação num só estado obrigaria
combinações como "confirmado e pago". Manter `pago_` à parte evita isso.
Três valores bastam para as regras atuais.

**Limitação aceita.** É possível cancelar um agendamento já pago. O estado
resultante (cancelado e pago) não tem noção de reembolso.

## 13. `dataHora_` como texto validado e prazo de 24h

**Contexto.** O agendamento precisa de data e hora, e o cancelamento depende
de comparar com "agora".

**Alternativas.**
- Guardar `std::time_t` ou `std::tm`: facilita contas, mas complica exibir e
  comparar igualdade exata (usada para `HorarioIndisponivel`).
- Guardar texto sem validar: aceitaria qualquer coisa.

**Decisão.** `dataHora_` é `std::string` no formato fixo
`AAAA-MM-DD HH:MM`. O construtor valida e lança `DadoInvalidoException`
quando: o tamanho é diferente de 16, o formato não é aceito por
`std::get_time`, sobra texto depois da data, ou a data não existe (por
exemplo 31/02, detectada comparando dia e mês antes e depois de
`std::mktime`). A conversão para `time_t` fica numa função livre no `.cpp`
e é refeita sempre que necessária.

O prazo de cancelamento é de 24 horas (constante `PRAZO_CANCELAMENTO`),
comparado com o relógio do sistema (`std::time(nullptr)`), sem parâmetro em
`cancelar()`. Cancelar com 24h ou mais de antecedência é permitido.

**Motivo.** O texto valida na entrada e mantém a igualdade exata simples. O
relógio do sistema evita um parâmetro que só serviria para testes: uma data
em 2099 está sempre dentro do prazo, e uma de 2000 sempre fora.

**Limitações aceitas.**
- `mktime` usa o fuso local do sistema; horário de verão não é tratado.
- A duração do serviço não entra em nenhuma checagem de horário.

**Revisitar se** a conversão passar a ser usada em muitos pontos: aí vale
guardar o `time_t` como atributo.

## 14. Transições inválidas lançam `EstudioException`

**Contexto.** `confirmar()`, `cancelar()` e `pagar()` só valem em certos
estados.

**Alternativas.** Uma exceção nova (por exemplo,
`TransicaoInvalidaException`) ou a base `EstudioException`.

**Decisão.** Transição inválida lança a `EstudioException` base, com
mensagem clara. As regras:
- `confirmar()`: só se `Agendado`.
- `cancelar()`: só se `Agendado` ou `Confirmado`; já cancelado lança
  `EstudioException`; menos de 24h lança `CancelamentoForaDoPrazoException`.
- `pagar()`: só se `Confirmado` e ainda não pago.

**Motivo.** Evita mais uma classe de exceção sem ganho para o menu atual,
que apenas mostra a mensagem.

**Revisitar se** o menu precisar tratar esse caso separadamente.

## 15. `pagar()` exige confirmação

**Decisão.** `pagar()` não confirma implicitamente. Pagar um agendamento só
agendado, cancelado ou já pago lança `EstudioException`. O valor vem de
`getServico().calcularPreco()`.

**Motivo.** Pagamento e confirmação são ações distintas do domínio, e um
setter simples (`setPago`) perderia a proteção do estado.

## 16. Conflito de horário (no `Estudio`)

**Contexto.** `HorarioIndisponivelException` é lançada pelo `Estudio`, não
pelo `Agendamento`.

**Decisão.** Igualdade exata da string `dataHora_`, valendo para o mesmo
funcionário e também para o mesmo cliente. O formato fixo e validado
(entrada 13) torna a comparação confiável.

**Limitação aceita.** A duração do serviço não entra: dois agendamentos
sobrepostos, mas com horários de início diferentes, não conflitam.

## Fora do escopo por ora

- Forma de pagamento (dinheiro, Pix, cartão) e pagamento parcial.

## 17. Modificação de estado do agendamento pelo menu

**Contexto.** A classe `Agendamento` possui os métodos `confirmar()`, `cancelar()` e `pagar()`, que inclusive validam prazos e lançam exceções. No entanto, o menu atual não oferece opções para interagir com esses métodos.

**Decisão.** A interação com a mudança de estado dos agendamentos via terminal fica **fora do escopo** da entrega atual. 

**Motivo.** Para o menu conseguir alterar um agendamento, seria necessário introduzir o conceito de identificador único (ID) para cada agendamento (ou realizar uma busca composta por CPF + Data), além de criar métodos intermediários na classe `Estudio` e expandir consideravelmente o `main.cpp`. O objetivo principal da disciplina (aplicar conceitos fundamentais de POO) já foi atingido, e a lógica de transição de estados e exceções já está implementada e protegida a nível de domínio na classe `Agendamento`, mesmo que não seja chamada pela interface atual.
