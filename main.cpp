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
    int currentLine = 10; // Come�a na linha 10

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
                currentLine += 10; // Incrementa 10 para pular para a pr�xima linha
            }
        }
    }

    void processCommand(const std::string& command) {
        std::string cmd = command;
        size_t pos;

        // Enquanto houver ; na linha, dividimos a string
        while ((pos = cmd.find(';')) != std::string::npos) {
            std::string singleCommand = cmd.substr(0, pos);
            singleCommand = trim(singleCommand);  // Remove espa�os ao redor do comando
            executeSingleCommand(singleCommand);
            cmd.erase(0, pos + 1);
        }

        // Remover espa�os ao redor e processar o �ltimo (ou �nico) comando
        cmd = trim(cmd);
        executeSingleCommand(cmd);
    }

    void executeSingleCommand(const std::string& command) {
        if (command.substr(0, 3) == "REM") return; // Coment�rio

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
            std::cout << "Execu��o encerrada." << std::endl;
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
        // Remove espa�os em branco ao redor do nome da vari�vel
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
	// 3 - Se a linha de destino n�o existir, exibir mensagem de erro e encerrar o programa
    void handleGoto(const std::string& command) {
        std::string targetStr = command.substr(5);
        targetStr = trim(targetStr);
        try {
            int targetLine = std::stoi(targetStr);
            if (program.find(targetLine) != program.end()) {
                currentLine = targetLine; // Muda para a linha de destino
            }
            else {
                std::cerr << "Linha de destino n�o encontrada: " << targetLine << std::endl;
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

        // Verifica se o comando cont�m aspas para strings
        size_t quotePos = toPrint.find('"');
        if (quotePos != std::string::npos) {
            // Extrair e imprimir a string entre aspas
            size_t endQuotePos = toPrint.find('"', quotePos + 1);
            if (endQuotePos != std::string::npos) {
                std::string strToPrint = toPrint.substr(quotePos + 1, endQuotePos - quotePos - 1);
                std::cout << strToPrint << std::endl;
            }
            else {
                std::cerr << "Erro de sintaxe: string n�o terminada corretamente." << std::endl;
                exit(1);
            }
        }
        else {
            // Caso contr�rio, imprime o valor da vari�vel
            std::string varName = toPrint;
            varName.erase(std::remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
            if (variables.find(varName) != variables.end()) {
                std::cout << variables[varName] << std::endl;
            }
            else {
				// coloquei esse else para caso a vari�vel n�o exista
                std::cerr << "Vari�vel n�o encontrada: " << varName << std::endl;
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
        std::string condition = command.substr(2, colonPos - 2); // Condi��o entre "IF" e ":"
        std::string commandsAfterIf = command.substr(colonPos + 1); // Comando ap�s ":"

        // Remover espa�os em branco
        condition = trim(condition);
        commandsAfterIf = trim(commandsAfterIf);

        // Avaliar a condi��o
        if (evaluateCondition(condition)) {
            processCommand(commandsAfterIf); // Executa o comando ap�s o ":"
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

        std::cerr << "Operador desconhecido na condi��o: " << op << std::endl;
        exit(1);
    }

    int evaluateExpression(const std::string& expression) {
        std::istringstream iss(expression);
        int result = getValue(iss); // Obter o primeiro valor

        char op;
        while (iss >> op) {
            if (iss.peek() == ' ') iss.ignore(); // Ignorar espa�os
            int nextValue = getValue(iss); // Obter o pr�ximo valor
            if (op == '+') result += nextValue;
            else if (op == '-') result -= nextValue;
            else if (op == '*') result *= nextValue;
            else if (op == '/') {
                if (nextValue != 0) result /= nextValue;
                else {
                    std::cerr << "Erro: Divis�o por zero." << std::endl;
                    exit(1);
                }
            }
            else {
                std::cerr << "Operador desconhecido na express�o: " << op << std::endl;
                exit(1);
            }
        }
        return result;
    }

    int getValue(std::istringstream& iss) {
        std::string token;
        iss >> token;

        // Verifica se o token � uma vari�vel (letra) ou um n�mero
        if (isalpha(token[0])) {
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (variables.find(token) != variables.end()) {
                return variables[token]; // Retorna o valor da vari�vel
            }
            else {
                std::cerr << "Vari�vel n�o encontrada: " << token << std::endl;
                exit(1);
            }
        }
        else {
            try {
                return std::stoi(token); // Converte o token em n�mero
            }
            catch (std::invalid_argument&) {
                std::cerr << "Erro de sintaxe na express�o: " << token << std::endl;
                exit(1);
            }
        }
    }

    // Fun��o para remover espa�os em branco no in�cio e no fim
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
