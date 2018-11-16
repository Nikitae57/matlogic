#include <iostream>
#include <stack>

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
    stack<char> varsStack = stack<char>();
    stack<Operation> operationStack = stack<Operation>();

    string expression;
    cout << "Enter expression" << endl;
    cin >> expression;

    string postfixExpr = "";
    for (int i = 0; i < expression.length(); i++) {
        if (isVariable(expression[i])) {
            postfixExpr += expression[i];
            continue;
        }

        if (expression[i] == '!' || expression[i] == '(') {
            operationStack.push(Operation::create(expression[i]));
            continue;
        }

        if (expression[i] == ')') {
            checkStack(operationStack);

            char topOperatorChar;
            while ((topOperatorChar = operationStack.top().operatorChar) != '(') {
                checkStack(operationStack);

                postfixExpr += topOperatorChar;
                operationStack.pop();
            }
            operationStack.pop();
            continue;
        }

        // binary operation
        Operation operation = Operation::create(expression[i]);
        if (operationStack.size() == 0) {
            operationStack.push(operation);
            continue;
        }

        Operation topOperation = operationStack.top();
        while (topOperation.operatorChar == '!'
               || topOperation.priority >= operation.priority) {

            postfixExpr += topOperation.operatorChar;
            operationStack.pop();
        }
        operationStack.push(operation);
    }

    while (operationStack.size() > 0) {
        postfixExpr += operationStack.top().operatorChar;
        operationStack.pop();
    }
    cout << postfixExpr;

    return 0;
}
