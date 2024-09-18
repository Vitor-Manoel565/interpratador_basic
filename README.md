### Estrutura Geral do Código

O código tem duas partes principais:
1. **Classe `BasicInterpreter`**: Define toda a lógica do interpretador.
2. **Função `main()`**: Cria uma instância da classe `BasicInterpreter` e chama o método `run()` para carregar e executar um arquivo `.bas`.

### 1. Classe `BasicInterpreter`

#### Variáveis Membro
- **`std::map<int, std::string> program`**: Armazena o código do programa, onde a chave é o número da linha e o valor é o comando associado a essa linha.
- **`std::map<std::string, int> variables`**: Armazena os valores das variáveis utilizadas no programa.
- **`int currentLine = 10`**: Representa a linha atual do programa (inicia na linha 10, como em muitos dialetos BASIC).

#### Métodos

1. **`void run(const std::string& filename)`**
   - Função Main que é chamada para carregar e executar o programa.
   - Usa dois métodos:
     - **`loadProgram(filename)`**: Carrega o programa a partir de um arquivo.
     - **`execute()`**: Executa os comandos carregados.

2. **`void loadProgram(const std::string& filename)`**
   - Lê o arquivo `.bas` linha por linha.
   - Cada linha do arquivo contém um número de linha (Label) seguido por um comando.
   - As linhas do arquivo são armazenadas no map `program`, onde a chave é o número da linha e o valor é o comando associado.
   
3. **`void execute()`**
   - Executa o programa linha por linha.
   - A cada iteração, chama o método `processCommand(command)` para processar o comando daquela linha.
   - A linha atual é incrementada em 10 a cada iteração, simulando a estrutura típica dos programas BASIC.
   
4. **`void processCommand(const std::string& command)`**
   - Divide a linha em comandos separados por `;` e processa cada um individualmente com o método `executeSingleCommand(singleCommand)` ou se não houver mais comando so espaço ele quebra.

5. **`void executeSingleCommand(const std::string& command)`**
   - Processa o comando individual.
   - Verifica o tipo de comando (ex.: `REM`, `LET`, `GOTO`, `INPUT`, `PRINT`, `HALT`, `IF`) e chama o método apropriado para executar esse comando.

6. **`void handleLet(const std::string& command)`**
   - Interpreta o comando `LET`, que atribui um valor a uma variável.
   - Exemplo: `LET X = 10` atribui o valor 10 à variável `X`.

7. **`void handleGoto(const std::string& command)`**
   - Não está funcionando corretamente pq não consegui implementar essa parte
   - Interpreta o comando `GOTO`, que muda o fluxo de execução para uma linha específica do programa.
   - Exemplo: `GOTO 20` faz com que o interpretador pule para a linha 20 do programa.

8. **`void handleInput(const std::string& command)`**
   - Interpreta o comando `INPUT`, que solicita ao usuário que insira um valor para uma variável.
   - Exemplo: `INPUT X` solicita ao usuário um valor para `X`.

9. **`void handlePrint(const std::string& command)`**
   - Interpreta o comando `PRINT`, que imprime uma string ou o valor de uma variável na saída.
   - Exemplo: `PRINT "Olá"` imprime `"Olá"`; `PRINT X` imprime o valor da variável `X`.

10. **`void handleIf(const std::string& command)`**
    - Interpreta o comando condicional `IF`.
    - Exemplo: `IF X = 10 : PRINT "X é 10"` verifica se `X` é igual a 10 e, se for, executa o comando após `:`.

11. **`bool evaluateCondition(const std::string& condition)`**
    - Avalia a condição de um comando `IF`. Suporta operadores como `=`, `<`, `>`, `<=`, `>=`, e `<>` (diferente).
    - Exemplo: `IF X = 10` avalia se `X` é igual a 10.

12. **`int evaluateExpression(const std::string& expression)`**
    - Avalia uma expressão matemática. Suporta operadores como `+`, `-`, `*`, `/`.
    - Exemplo: `LET X = 5 + 3` avalia a expressão `5 + 3` e armazena o valor 8 em `X`.

13. **`int getValue(std::istringstream& iss)`**
    - Retorna o valor de uma variável ou um número diretamente.
    - Se o token for um nome de variável, retorna o valor armazenado nessa variável; se for um número, converte-o para `int`.

14. **`std::string trim(const std::string& s)`**
    - Remove espaços em branco no início e no final de uma string.

### 2. Função `main()`

A função `main()` apenas cria uma instância da classe `BasicInterpreter` e chama o método `run()`, passando o nome do arquivo `.bas` que será carregado e executado.

```cpp
int main() {
    BasicInterpreter interpreter;
    interpreter.run("fimDeLinha.bas");
    return 0;
}
```

### Exemplos de Uso

Um arquivo `.bas` pode conter comandos como:

```bas
10 LET X = 10
20 PRINT "O valor de X é: "; PRINT X
30 IF X = 10 : PRINT "X é igual a 10."
40 HALT
```

### Pontos Importantes

1. **GOTO**: Não está funcionando corretamente, não consegui implementar essa parte professor :/
2. **ARQUIVOS BASIC**: Os arquivos Basic estão  na pasta teste, só editar ou criar um novo arquivo .bas e colocar o nome do arquivo na função main para rodar o programa.

Pprocessa e executa comandos a partir de um arquivo `.bas`.
