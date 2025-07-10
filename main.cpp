#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cmath>
using namespace std;

unordered_map<string, int> precedence = {
    {"*", 3},
    {"/", 3},
    {"+", 2},
    {"-", 2}
};
unordered_set<string> functions = {"pow", "abs", "max", "min"};

vector<string> tokenize(const string& infix) {
    vector<string> result;
    string num_buffer;
    string func_buffer;
    for (char c : infix) {
        if (isdigit(c) || c == '.') {
            num_buffer += c;
        }
        else {
            if (!num_buffer.empty()) {
                result.emplace_back(num_buffer);
                num_buffer.clear();
            }
            if (precedence.contains(string(1, c))) {
                result.emplace_back(1, c);
            }
            else if (isspace(c)) {
                if (!func_buffer.empty()) {
                    result.emplace_back(func_buffer);
                    func_buffer.clear();
                }
            }
            else if (c == '(' || c == ')'|| c == ',') {
                if (!func_buffer.empty()) {
                    result.emplace_back(func_buffer);
                    func_buffer.clear();
                }
                result.emplace_back(1,c);
            }
            else {
                func_buffer += c;
            }
        }
    }
    if (!num_buffer.empty()) {
        result.emplace_back(num_buffer);
    }
    return result;
}
bool is_number (string str) {
    for (char c :str) {
        if (!isdigit(c) && c != '.') return false;
    }
    return true;
}
vector<string> shunting_yard(vector<string> tokens) {
    stack<string> operators_stack;
    vector<string> output;
    for (string token : tokens) {
        if (is_number(token)) output.emplace_back(token);
        else if (functions.contains(token)) operators_stack.push(token);
        else if (precedence.contains(token)) {
            while (!operators_stack.empty() && operators_stack.top() != "(" && precedence[operators_stack.top()] >= precedence[token]) {
                output.emplace_back(operators_stack.top());
                operators_stack.pop();
            }
            operators_stack.push(token);
        }
        else if (token == ",") {
            while (!operators_stack.empty() && operators_stack.top() != "(") {
                output.emplace_back(operators_stack.top());
                operators_stack.pop();
            }
        }
        else if (token == "(") {
            operators_stack.push(token);
        }
        else if (token == ")") {
            while (!operators_stack.empty() && operators_stack.top() != "(") {
                output.emplace_back(operators_stack.top());
                operators_stack.pop();
            }
            operators_stack.pop();
            if (!operators_stack.empty() && functions.contains(operators_stack.top())) {
                output.emplace_back(operators_stack.top());
                operators_stack.pop();
            }
        }
    }
    while (!operators_stack.empty()) {
        output.emplace_back(operators_stack.top());
        operators_stack.pop();
    }
    return output;
}

float calculate(vector<string> tokens) {
    stack<float> S;
    for ( string token : tokens) {
        if (is_number(token)) {
            S.push(stof(token));
        }
        else {
            float b = S.top();
            S.pop();
            if (token == "abs") S.push(abs(b));
            else {
                if (!S.empty()) {
                    float a = S.top();
                    S.pop();

                    if (token == "+") S.push(a + b);
                    else if (token == "-") S.push(a - b);
                    else if (token == "*") S.push(a * b);
                    else if (token == "/") S.push(a / b);
                    else if (token == "pow") S.push(pow(a, b));
                    else if (token == "max") S.push(max(a, b));
                    else if (token == "min") S.push(min(a, b));
                }
                else return numeric_limits<float>::infinity();
            }

        }
    }
    return S.top();
}

int main() {
    auto input = []() {
        string input;
        getline(cin, input);

        auto tokens = tokenize(input);
        auto rpn = shunting_yard(tokens);
        auto res = calculate(rpn);

        if (res == numeric_limits<float>::infinity()) cout << "Invalid expression" << endl;
        else cout << res << endl;
    };
    while (true) {
        input();
    }
}