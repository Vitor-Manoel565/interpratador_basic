#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>

class BasicInterpreter {
public:
    void run(const std::string& filename) {
        loadProgram(filename);
        execute();
    }

private:
    std::map<int, std::string> program;
    std::map<std::string, int> variables;
    int currentLine = 10; // Começa na linha 10

    void loadProgram(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo!" << std::endl;
            return;
        }
        std::string line;
        int lineNumber;
        while (std::getline(file, line)) {
			std::cout << line << std::endl;
            std::istringstream iss(line);
            if (!(iss >> lineNumber)) {
                std::cerr << "Erro de sintaxe na linha: " << line << std::endl;
                continue;
            }
            size_t spacePos = line.find(' ');
            if (spacePos == std::string::npos) {
                std::cerr << "Erro de sintaxe na linha: " << line << std::endl;
                continue;
            }
            program[lineNumber] = line.substr(spacePos + 1);
        }
    }

    void execute() {
        while (program.find(currentLine) != program.end()) {
            std::string command = program[currentLine];
            processCommand(command);
            if (currentLine != -1) {
                currentLine += 10; // Incrementa 10 para pular para a próxima linha
            }
        }
    }

    void processCommand(const std::string& command) {
        std::string cmd = command;
        size_t pos;

        // Enquanto houver ; na linha, dividimos a string
        while ((pos = cmd.find(';')) != std::string::npos) {
            std::string singleCommand = cmd.substr(0, pos);
            singleCommand = trim(singleCommand);  // Remove espaços ao redor do comando
            executeSingleCommand(singleCommand);
            cmd.erase(0, pos + 1);
        }

        // Remover espaços ao redor e processar o último (ou único) comando
        cmd = trim(cmd);
        executeSingleCommand(cmd);
    }

    void executeSingleCommand(const std::string& command) {
        if (command.substr(0, 3) == "REM") return; // Comentário

        if (command.substr(0, 3) == "LET") {
            handleLet(command);
        }
        else if (command.substr(0, 4) == "GOTO") {
            handleGoto(command);
        }
        else if (command.substr(0, 5) == "INPUT") {
            handleInput(command);
        }
        else if (command.substr(0, 5) == "PRINT") {
            handlePrint(command);
        }
        else if (command.substr(0, 4) == "HALT") {
            std::cout << "Execução encerrada." << std::endl;
            exit(0); // Finaliza o programa corretamente
        }
        else if (command.substr(0, 2) == "IF") {
            handleIf(command);
        }
        else {
            std::cerr << "Comando desconhecido: " << command << std::endl;
        }
    }


    void handleLet(const std::string& command) {
        // Remove espaços em branco ao redor do nome da variável
        size_t equalPos = command.find('=');
        if (equalPos == std::string::npos) {
            std::cerr << "Erro de sintaxe no comando LET: " << command << std::endl;
            exit(1);
        }
        std::string varName = command.substr(4, equalPos - 4);
        varName.erase(std::remove_if(varName.begin(), varName.end(), ::isspace), varName.end());

        std::string expression = command.substr(equalPos + 1);
        expression = trim(expression);
        variables[varName] = evaluateExpression(expression);
    }


    // Tentei fazer o Goto da seguinte forma:
	// 1 - Verificar se a linha de destino existe
	// 2 - Mudar para a linha de destino
	// 3 - Se a linha de destino não existir, exibir mensagem de erro e encerrar o programa
    void handleGoto(const std::string& command) {
        std::string targetStr = command.substr(5);
        targetStr = trim(targetStr);
        try {
            int targetLine = std::stoi(targetStr);
            if (program.find(targetLine) != program.end()) {
                currentLine = targetLine; // Muda para a linha de destino
            }
            else {
                std::cerr << "Linha de destino não encontrada: " << targetLine << std::endl;
                currentLine = -1; // Para evitar loop infinito
            }
        }
        catch (std::invalid_argument&) {
            std::cerr << "Erro de sintaxe no comando GOTO: " << command << std::endl;
            exit(1);
        }
    }

    void handleInput(const std::string& command) {
        std::string varName = command.substr(6);
        varName = trim(varName);
        varName.erase(std::remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
        if (varName.empty()) {
            std::cerr << "Erro de sintaxe no comando INPUT: " << command << std::endl;
            exit(1);
        }
        int value;
        std::cout << "Digite um valor para " << varName << ": ";
        std::cin >> value;
        variables[varName] = value;
    }

    void handlePrint(const std::string& command) {
        std::string toPrint = command.substr(5);
        toPrint = trim(toPrint);

        // Verifica se o comando contém aspas para strings
        size_t quotePos = toPrint.find('"');
        if (quotePos != std::string::npos) {
            // Extrair e imprimir a string entre aspas
            size_t endQuotePos = toPrint.find('"', quotePos + 1);
            if (endQuotePos != std::string::npos) {
                std::string strToPrint = toPrint.substr(quotePos + 1, endQuotePos - quotePos - 1);
                std::cout << strToPrint << std::endl;
            }
            else {
                std::cerr << "Erro de sintaxe: string não terminada corretamente." << std::endl;
                exit(1);
            }
        }
        else {
            // Caso contrário, imprime o valor da variável
            std::string varName = toPrint;
            varName.erase(std::remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
            if (variables.find(varName) != variables.end()) {
                std::cout << variables[varName] << std::endl;
            }
            else {
				// coloquei esse else para caso a variável não exista
                std::cerr << "Variável não encontrada: " << varName << std::endl;
                exit(1);
            }
        }
    }

    void handleIf(const std::string& command) {
        size_t colonPos = command.find(':'); // Usando ':' como separador
        if (colonPos == std::string::npos) {
            std::cerr << "Erro de sintaxe: Falta ':' no comando IF." << std::endl;
            exit(1);
        }
        std::string condition = command.substr(2, colonPos - 2); // Condição entre "IF" e ":"
        std::string commandsAfterIf = command.substr(colonPos + 1); // Comando após ":"

        // Remover espaços em branco
        condition = trim(condition);
        commandsAfterIf = trim(commandsAfterIf);

        // Avaliar a condição
        if (evaluateCondition(condition)) {
            processCommand(commandsAfterIf); // Executa o comando após o ":"
        }
    }

    bool evaluateCondition(const std::string& condition) {
        std::istringstream iss(condition);
        int left = getValue(iss);
        std::string op;
        iss >> op;
        int right = getValue(iss);

        if (op == "=") return left == right;
        else if (op == "<") return left < right;
        else if (op == ">") return left > right;
        else if (op == "<=") return left <= right;
        else if (op == ">=") return left >= right;
        else if (op == "<>") return left != right;

        std::cerr << "Operador desconhecido na condição: " << op << std::endl;
        exit(1);
    }

    int evaluateExpression(const std::string& expression) {
        std::istringstream iss(expression);
        int result = getValue(iss); // Obter o primeiro valor

        char op;
        while (iss >> op) {
            if (iss.peek() == ' ') iss.ignore(); // Ignorar espaços
            int nextValue = getValue(iss); // Obter o próximo valor
            if (op == '+') result += nextValue;
            else if (op == '-') result -= nextValue;
            else if (op == '*') result *= nextValue;
            else if (op == '/') {
                if (nextValue != 0) result /= nextValue;
                else {
                    std::cerr << "Erro: Divisão por zero." << std::endl;
                    exit(1);
                }
            }
            else {
                std::cerr << "Operador desconhecido na expressão: " << op << std::endl;
                exit(1);
            }
        }
        return result;
    }

    int getValue(std::istringstream& iss) {
        std::string token;
        iss >> token;

        // Verifica se o token é uma variável (letra) ou um número
        if (isalpha(token[0])) {
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (variables.find(token) != variables.end()) {
                return variables[token]; // Retorna o valor da variável
            }
            else {
                std::cerr << "Variável não encontrada: " << token << std::endl;
                exit(1);
            }
        }
        else {
            try {
                return std::stoi(token); // Converte o token em número
            }
            catch (std::invalid_argument&) {
                std::cerr << "Erro de sintaxe na expressão: " << token << std::endl;
                exit(1);
            }
        }
    }

    // Função para remover espaços em branco no início e no fim
    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        if (start == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }
};

int main() {
    BasicInterpreter interpreter;
    interpreter.run("program.bas");
    return 0;
}
