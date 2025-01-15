#include <iostream>
#include <sstream>
#include <stack>
#include <cctype>
#include <stdexcept>
using namespace std;

int evaluateExpression(const string& expression) {
    stack<int> values;
    stack<char> operators;

    auto applyOperator = [](int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return b != 0 ? a / b : throw runtime_error("Division by zero");
            default: throw runtime_error("Invalid operator");
        }
    };

    try {
        for (size_t i = 0; i < expression.length(); ++i) {
            if (isspace(expression[i]))
                continue;
            if (isdigit(expression[i])) {
                int value = 0;
                while (i < expression.length() && isdigit(expression[i])) {
                    value = value * 10 + (expression[i] - '0');
                    ++i;
                }
                --i;
                values.push(value);
            } else if (expression[i] == '(') {
                operators.push(expression[i]);
            } else if (expression[i] == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    int b = values.top(); values.pop();
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(a, b, op));
                }
                if (operators.empty() || operators.top() != '(') {
                    throw runtime_error("Mismatched parentheses");
                }
                operators.pop();
            } else {
                while (!operators.empty() && (operators.top() == '*' || operators.top() == '/' ||
                      (operators.top() == '+' || operators.top() == '-') && (expression[i] == '+' || expression[i] == '-'))) {
                    int b = values.top(); values.pop();
                    int a = values.top(); values.pop();
                    char op = operators.top(); operators.pop();
                    values.push(applyOperator(a, b, op));
                }
                operators.push(expression[i]);
            }
        }

        while (!operators.empty()) {
            if (values.size() < 2) {
                throw runtime_error("Invalid expression");
            }
            int b = values.top(); values.pop();
            int a = values.top(); values.pop();
            char op = operators.top(); operators.pop();
            values.push(applyOperator(a, b, op));
        }

        if (values.size() != 1) {
            throw runtime_error("Invalid expression");
        }

        return values.top();
    } catch (const exception& e) {
        throw runtime_error(string("Error during evaluation: ") + e.what());
    }
}

int main() {
    string input;
    cout << "Enter a basic arithmetic expression: ";
    getline(cin, input);

    try {
        int result = evaluateExpression(input);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
