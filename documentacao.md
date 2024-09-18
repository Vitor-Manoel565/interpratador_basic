### Estrutura Geral do C�digo

O c�digo tem duas partes principais:
1. **Classe `BasicInterpreter`**: Define toda a l�gica do interpretador.
2. **Fun��o `main()`**: Cria uma inst�ncia da classe `BasicInterpreter` e chama o m�todo `run()` para carregar e executar um arquivo `.bas`.

### 1. Classe `BasicInterpreter`

#### Vari�veis
- **`std::map<int, std::string> program`**: Armazena o c�digo do programa, onde a chave � o n�mero da linha e o valor � o comando associado a essa linha.
- **`std::map<std::string, int> variables`**: Armazena os valores das vari�veis utilizadas no programa.

#### M�todos

1. **`void run(const std::string& filename)`**
   - Fun��o Main que � chamada para carregar e executar o programa.
   - Usa dois m�todos:
     - **`loadProgram(filename)`**: Carrega o programa a partir de um arquivo.
     - **`execute()`**: Executa os comandos carregados.

2. **`void loadProgram(const std::string& filename)`**
   - L� o arquivo `.bas` linha por linha.
   - Cada linha do arquivo cont�m um n�mero de linha (Label) seguido por um comando.
   - As linhas do arquivo s�o armazenadas no map `program`, onde a chave � o n�mero da linha e o valor � o comando associado.
   
3. **`void execute()`**
   - Executa o programa linha por linha.
   - A cada itera��o, chama o m�todo `processCommand(command)` para processar o comando daquela linha.
   - A linha atual � incrementada em 10 a cada itera��o, simulando a estrutura t�pica dos programas BASIC.
   
4. **`void processCommand(const std::string& command)`**
   - Divide a linha em comandos separados por `;` e processa cada um individualmente com o m�todo `executeSingleCommand(singleCommand)` ou se n�o houver mais comando so espa�o ele quebra.

5. **`void executeSingleCommand(const std::string& command)`**
   - Processa o comando individual.
   - Verifica o tipo de comando (ex.: `REM`, `LET`, `GOTO`, `INPUT`, `PRINT`, `HALT`, `IF`) e chama o m�todo para executar esse comando.

6. **`void handleLet(const std::string& command)`**
   - Interpreta o comando `LET`, que atribui um valor a uma vari�vel.
   - Exemplo: `LET X = 10` atribui o valor 10 � vari�vel `X`.

7. **`void handleGoto(const std::string& command)`**
   - N�o est� funcionando corretamente pq n�o consegui implementar essa parte
   - No entanto o comando `GOTO`, muda o fluxo de execu��o para uma linha espec�fica do programa.

8. **`void handleInput(const std::string& command)`**
   - Interpreta o comando `INPUT`, que solicita ao usu�rio que insira um valor para uma vari�vel.
   - Exemplo: `INPUT X` solicita ao usu�rio um valor para `X`.

9. **`void handlePrint(const std::string& command)`**
   - Interpreta o comando `PRINT`, que imprime uma string ou o valor de uma vari�vel na sa�da.
   - Exemplo: `PRINT "Ol�"` imprime `"Ol�"`; `PRINT X` imprime o valor da vari�vel `X`.

10. **`void handleIf(const std::string& command)`**
    - Interpreta o comando condicional `IF`.
    - Exemplo: `IF X = 10 : PRINT "X � 10"` verifica se `X` � igual a 10 e, se for, executa o comando ap�s `:`.

11. **`bool evaluateCondition(const std::string& condition)`**
    - Avalia a condi��o de um comando `IF`. Suporta operadores como `=`, `<`, `>`, `<=`, `>=`, e `<>` (diferente).
    - Exemplo: `IF X = 10` avalia se `X` � igual a 10.

12. **`int evaluateExpression(const std::string& expression)`**
    - Avalia uma express�o matem�tica. Suporta operadores como `+`, `-`, `*`, `/`.
    - Exemplo: `LET X = 5 + 3` avalia a express�o `5 + 3` e armazena o valor 8 em `X`.

13. **`int getValue(std::istringstream& iss)`**
    - Retorna o valor de uma vari�vel ou um n�mero diretamente.
    - Se o token for um nome de vari�vel, retorna o valor armazenado nessa vari�vel; se for um n�mero, converte-o para `int`.

14. **`std::string trim(const std::string& s)`**
    - Remove espa�os em branco no in�cio e no final de uma string.

### 2. Fun��o `main()`

A fun��o `main()` apenas cria uma inst�ncia da classe `BasicInterpreter` e chama o m�todo `run()`, passando o nome do arquivo `.bas` que ser� carregado e executado.

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
20 PRINT "O valor de X �: "; PRINT X
30 IF X = 10 : PRINT "X � igual a 10."
40 HALT
```

### Pontos Importantes

1. **GOTO**: N�o est� funcionando corretamente, n�o consegui implementar essa parte professor :/
2. **ARQUIVOS BASIC**: Os arquivos Basic est�o  na pasta teste, s� editar ou criar um novo arquivo .bas e colocar o nome do arquivo na fun��o main para rodar o programa.
3. **ARQUIVOS VCX**: São arquivos do Visual Code.

Pprocessa e executa comandos a partir de um arquivo `.bas`.