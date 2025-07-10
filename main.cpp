#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cmath>
#include <algorithm>

using namespace std;

unordered_map<string, int> precedence = {
    {"*", 3},
    {"/", 3},
    {"+", 2},
    {"-", 2}
};
unordered_set<string> functions = {"pow", "abs", "max", "min"};

unordered_map<string, float> global_variables;

std::vector<std::string> tokenize(const std::string& infix) {
    std::vector<std::string> tokens;
    std::string tk;
    for (char ch : infix) {
        if (!isspace(ch)) {
            if (ch == '(' || ch == ')' || ch == ',' || precedence.contains(std::string(1, ch))) {
                if (!tk.empty()) {
                    tokens.emplace_back(tk);
                    tk.clear();
                }
                tokens.emplace_back(1, ch);
            }
            else tk += ch;
        }
    }
    if (!tk.empty()) tokens.push_back(tk);
    return tokens;
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
        if (is_number(token) || global_variables.contains(token)) output.emplace_back(token);
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
        else if (global_variables.contains(token)) {
            S.push(global_variables.at(token));
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

void variable_definition(const std::string& input) {
    if (size_t eq_pos = input.find('='); eq_pos != std::string::npos) {
        std::string var_name = input.substr(4, eq_pos - 4);
        var_name.erase(std::ranges::remove_if(var_name, ::isspace).begin(), var_name.end());
        std::string expr = input.substr(eq_pos + 1);
        expr.erase(std::ranges::remove_if(expr, ::isspace).begin(), expr.end());

        auto tokens = shunting_yard(tokenize(expr));
        const double value = calculate(tokens);
        if (value == std::numeric_limits<double>::infinity()) std::cout << "Incorrect expression" << std::endl;
        else global_variables[var_name] = value;
    }
    else std::cout << "Missing '=' in variable assignment" << std::endl;
}

int main() {
    auto input = []() {
        string input;
        getline(cin, input);
        if (input.starts_with("var ")) {
            variable_definition(input);
            return;
        }

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