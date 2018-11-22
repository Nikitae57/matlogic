#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

/*
 * + - 'или'
 * * - 'и'
 * ! - 'не'
 * > - 'импликация'
 * = - 'эквиваленция'
 */

class Operation {
public:
    Operation() {}

    Operation(int priority, char operatorChar) {
        this->priority = priority;
        this->operatorChar = operatorChar;
    }

    int priority;
    char operatorChar;

    static Operation create(char operatorChar) {
        Operation operation;
        switch(operatorChar) {
            case '=':
                operation = Operation(1, operatorChar);
            break;

            case '>':
                operation = Operation(2, operatorChar);
            break;

            case '+':
                operation = Operation(3, operatorChar);
            break;

            case '*':
                operation = Operation(4, operatorChar);
            break;

            case '!':
                operation = Operation(5, operatorChar);
            break;

            case '(':
                operation = Operation(0, operatorChar);
            break;

            case ')':
                operation = Operation(0, operatorChar);
            break;

        }

        return operation;
    }
};

bool stackIsEmpty(stack<Operation> st) {
    if (st.size() == 0) {
        return true;
    }

    return false;
}

template<typename T> bool contains(vector<T> vec, T charToSearchFor) {
    return find(vec.begin(), vec.end(), charToSearchFor) < vec.end();
}

string intToBinaryString(int i) {
    vector<bool> vec = vector<bool>();
    while (i > 0) {
        vec.push_back(i % 2);
        i = i >> 1;
    }

    string result = "";
    for (int i = vec.size() - 1; i >= 0; i--) {
        if (vec.at(i)) {
            result += '1';
        } else {
            result += '0';
        }
    }

    return result;
}

string intToBinaryString(int i, int capacity) {
    string str = intToBinaryString(i);
    string leadingZeros = "";

    for (int i = 0; i < capacity - str.length(); i++) {
        leadingZeros += '0';
    }

    return leadingZeros + str;
}

void checkStack(stack<Operation> st) {
    if (stackIsEmpty(st)) {
        cout << "Incorrect expression" << endl;
        exit(-1);
    }
}

bool isVariable(char character) {
    return character != '+'
        && character != '*'
        && character != '!'
        && character != '>'
        && character != '='
        && character != '('
        && character != ')';
}

int main() {
    stack<Operation> operationStack = stack<Operation>();

    string expression;
    cout << "Enter expression" << endl;
    cin >> expression;

    string postfixExpr = "";
    for (int i = 0; i < expression.length(); i++) {
        if (isVariable(expression[i])) {
            postfixExpr += expression[i];

        } else if (expression[i] == '!' || expression[i] == '(') {
            operationStack.push(Operation::create(expression[i]));
            continue;

        } else if (expression[i] == ')') {
            checkStack(operationStack);

            char topOperatorChar;
            while ((topOperatorChar = operationStack.top().operatorChar) != '(') {
                checkStack(operationStack);

                postfixExpr += topOperatorChar;
                operationStack.pop();
            }
            operationStack.pop();
            continue;

        } else {
            // binary operation
            Operation operation = Operation::create(expression[i]);
            Operation topOperation;
            while (operationStack.size() > 0) {
                topOperation = operationStack.top();

                if (topOperation.operatorChar == '!'
                        || topOperation.priority >= operation.priority) {

                    postfixExpr += topOperation.operatorChar;
                    operationStack.pop();
                } else {
                    break;
                }
            }
            operationStack.push(operation);
        }
    }

    while (operationStack.size() > 0) {
        postfixExpr += operationStack.top().operatorChar;
        operationStack.pop();
    }

    cout << "Postfix representation:" << endl;
    cout << postfixExpr << endl << endl;

    vector<char> variableSet = vector<char>();
    for (int i = 0; i < expression.length(); i++) {
        if (!isVariable(expression[i])) { continue; }

        if (!contains(variableSet, expression[i])) {
            variableSet.push_back(expression[i]);
        }
    }

    int numberOfVariables = variableSet.size();
    int table[numberOfVariables][1 << numberOfVariables];
    string binaryStr;
    for (int j = 0; j < (1 << numberOfVariables); j++) {
        binaryStr = intToBinaryString(j, numberOfVariables);

        for (int i = 0; i < numberOfVariables; i++) {
            table[i][j] = binaryStr[i] - '0';
        }
    }

    string str = postfixExpr;
    for (int j = 0; j < (1 << numberOfVariables); j++) {
        for (int i = 0; i < postfixExpr.size(); i++) {
            if (!isVariable(postfixExpr[i])) { continue; }

            int pos = find(variableSet.begin(), variableSet.end(), postfixExpr[i]) - variableSet.begin();
            str[i] = table[pos][j] + '0';
        }

        // ТУТ
        cout << str << endl;
        str = postfixExpr;
    }


    return 0;
}
