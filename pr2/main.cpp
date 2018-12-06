#include <iostream>
#include <stack>
#include <vector>
#include <set>
#include "bits/stdc++.h"

using namespace std;

static char NEGATIVITY = '!';
static char OR = '+';
static char AND = '*';
static char OPEN_BRACKET = '(';
static char CLOSING_BRACKET = ')';

class Operand {
private:
    char operandChar;
    bool isNegative;
    Operand() {}

public:
    Operand(char operandChar, bool isNegative = false) {
        this->operandChar = operandChar;
        this->isNegative = isNegative;
    }

    char getOperandChar() {
        return operandChar;
    }

    bool getIsNegative() {
        return isNegative;
    }

    void setIsNegative(bool isNegative) {
        this->isNegative = isNegative;
    }
};

class Operation {
private:
    vector<Operand> operands;

public:
    Operation(vector<Operand> operands = vector<Operand>()) {
        this->operands = operands;
    }

    vector<Operand> getOperands() {
        return operands;
    }

    void add(Operand operand) {
        operands.push_back(operand);
    }

    void clear() {
        operands.clear();
    }

    bool contains(Operand operandForCheck) {
        for (Operand &oper : operands) {
            if (operandForCheck.getOperandChar() == oper.getOperandChar()
                    && operandForCheck.getIsNegative() == oper.getIsNegative()) {

                return true;
            }
        }

        return false;
    }

    void erase(char toErase) {
        for (vector<Operand> :: iterator it = operands.begin(); it != operands.end(); it++) {
            if ((*it).getOperandChar() == toErase) {
                it = operands.erase(it);
            }
        }
    }
};

class Expression {
private:
    vector<Operation> operations;

public:
    Expression(vector<Operation> operations = vector<Operation>()) {
        this->operations = operations;
    }

    void add(Operation operation) {
        operations.push_back(operation);
    }

    vector<Operation> getOperations() {
        return operations;
    }

    void printExpression() {
        for (int i = 0; i < operations.size(); i++) {
            int operationSize = operations.at(i).getOperands().size();

            for (int j = 0; j < operationSize - 1; j++) {
                Operand operand = operations.at(i).getOperands().at(j);

                if (operand.getIsNegative()) {
                    cout << '!';
                }

                cout << operand.getOperandChar() << '+';
            }

            if (operations.at(i).getOperands().at(operationSize - 1).getIsNegative()) {
                cout << '!';
            }
            cout << operations.at(i).getOperands().at(operationSize - 1).getOperandChar() << ' ';
        }
        cout << endl;
    }
};

string requestExpression() {
    cout << "Enter expression" << endl;
    string expression;
    cin >> expression;

    return expression;
}

Expression parseExpression(string expression) {

    Expression parsedExpression = Expression();
    vector<Operand> operands = vector<Operand>();
    bool nextIsNegative = false;

    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == OR
            || expression[i] == AND
            || expression[i] == OPEN_BRACKET) {

            continue;
        }

        if (expression[i] == CLOSING_BRACKET) {
            parsedExpression.add(Operation(operands));
            operands = vector<Operand>();
            continue;
        }

        if (expression[i] == NEGATIVITY) {
            nextIsNegative = true;
            continue;
        }

        if (nextIsNegative) {
            operands.push_back(Operand(expression[i], true));
            nextIsNegative = false;

        } else {
            operands.push_back(Operand(expression[i]));
        }
    }

    return parsedExpression;
}

bool checkForContrary(Operation oper1, Operation oper2, Operation &result) {
    bool contraryFound = false;
    result.clear();
    char contraryChar = ' ';

    for (int i = 0; i < oper1.getOperands().size(); i++) {
        for (int j = 0; j < oper2.getOperands().size(); j++) {

            Operand operand1 = oper1.getOperands().at(i);
            Operand operand2 = oper2.getOperands().at(j);

            if (operand1.getOperandChar() == operand2.getOperandChar()
                    && operand1.getIsNegative() != operand2.getIsNegative()) {

                if (!contraryFound) {
                    contraryFound = true;
                    contraryChar = operand1.getOperandChar();

                    if (result.contains(contraryChar)) {
                        result.erase(contraryChar);
                    }
                } else {
                    return false;
                }

            } else {
                if (!result.contains(operand1) &&
                        operand1.getOperandChar() != contraryChar) {
                    result.add(operand1);
                }

                if (!result.contains(operand2) &&
                        operand2.getOperandChar() != contraryChar) {
                    result.add(operand2);
                }
            }
        }
    }

    return contraryFound;
}

void start() {
    string expression = requestExpression();
    Expression parsedExpression = parseExpression(expression);

    Operation result;
    for (int i = 0; i < parsedExpression.getOperations().size(); i++) {
        for (int j = 0; j < parsedExpression.getOperations().size(), j != i; j++) {
            if (checkForContrary(parsedExpression.getOperations().at(i),
                                 parsedExpression.getOperations().at(j), result)) {

                if (result.getOperands().size() == 0) {
                    cout << "Formula vivodima" << endl;
                    exit(0);
                } else {
                    parsedExpression.add(result);
                    parsedExpression.printExpression();
                }
            }
        }
    }

    cout << "Formula ne vivodima" << endl;
}

int main()
{
    start();
    cout << "Finished" << endl;
    return 0;
}
