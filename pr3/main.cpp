#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#include <boost/algorithm/string/replace.hpp>

using namespace std;

static char OPEN_BRACKET = '(';
static char CLOSING_BRACKET = ')';
static char EXISTANCE_QUANTOR = 'E';
static char UNIVERSIALITY_QUANTOR = 'A';

class Quantor {
public:

    Quantor(char quantorChar, char variable) {
        this->quantorChar = quantorChar;
        this->variable = variable;
    }

    char quantorChar;
    char variable;
};

string requestExpression() {
    cout << "Enter expression" << endl;
    string expression;
    cin >> expression;

    return expression;
}

vector<Quantor> parseQuantors(string expression, string &matrix) {
    vector<Quantor> quantors;
    int i;
    for (i = 0; i < expression.size(); i++) {
        char currentChar = expression[i];

        if (currentChar == OPEN_BRACKET) { break; }

        if (currentChar == EXISTANCE_QUANTOR
                || currentChar == UNIVERSIALITY_QUANTOR) {

            Quantor q = Quantor(currentChar, expression[i + 1]);
            quantors.push_back(q);
        }
    }

    matrix = "";
    while (++i != expression.size() - 1) {
        matrix += expression[i];
    }

    return quantors;
}

void printSSF(vector<Quantor> quantors, string matrix) {
    for (Quantor q : quantors) {
        cout << q.quantorChar << q.variable;
    }
    cout << OPEN_BRACKET;
    cout << matrix << CLOSING_BRACKET << endl;
}

string makeFunctionalSymbol(vector<char> vars) {
    string functionalSymbol = "f(";

    for (int i = 0; i < vars.size() - 1; i++) {
        functionalSymbol += vars.at(i);
        functionalSymbol += ',';
    }
    functionalSymbol += vars.at(vars.size() - 1);
    functionalSymbol +=  + ')';

    return functionalSymbol;
}

void go(vector<Quantor> &quantors, string &matrix) {
    char varForReplace = 'a';
    vector<char> varsToReplace = vector<char>();
    int size = quantors.size();

    for (int i = 0; i < size; i++) {
        if (quantors.at(i).quantorChar == UNIVERSIALITY_QUANTOR) {
            varsToReplace.push_back(quantors.at(i).variable);

            continue;
        }

        if (varsToReplace.size() == 0) {
            size_t index = matrix.find(quantors.at(i).variable);

            while (index != std::string::npos) {
                string str = "";
                str += varForReplace;
                matrix.replace(index, 1, str);
                index = matrix.find(quantors.at(i).variable, index + 1);
            }

            varForReplace++;
            quantors.erase(quantors.begin() + i);
            size--;
            i--;

        } else {
            string functionalSymbol = makeFunctionalSymbol(varsToReplace);

            size_t index = matrix.find(quantors.at(i).variable);
            while (index != std::string::npos) {
                 matrix.replace(index, 1, functionalSymbol);
                 index = matrix.find(quantors.at(i).variable, index + 1);
            }
        }
    }
}

int main() {

    string matrix;
    string expression = requestExpression();
    vector<Quantor> quantors = parseQuantors(expression, matrix);

    go(quantors, matrix);
    printSSF(quantors, matrix);
    return 0;
}










