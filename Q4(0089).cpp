#include <iostream>
#include <string>
using namespace std;

const int MAXN = 20;
const int MAXTOK = 100; 

double stringToDouble(const string &s) {
    double result = 0.0;
    double sign = 1.0;
    size_t i = 0;

    if (i < s.length() && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') sign = -1.0;
        i++;
    }

    while (i < s.length() && s[i] >= '0' && s[i] <= '9') {
        result = result * 10.0 + (s[i] - '0');
        i++;
    }

    if (i < s.length() && s[i] == '.') {
        i++;
        double frac = 0.1;
        while (i < s.length() && s[i] >= '0' && s[i] <= '9') {
            result += (s[i] - '0') * frac;
            frac *= 0.1;
            i++;
        }
    }

    return result * sign;
}

string doubleToString(double val) {
    long long intPart = (long long) val;
    double fracPart = val - intPart;
    if (fracPart < 0) fracPart = -fracPart;

    string result = "";
    if (val < 0 && intPart == 0) result += "-";
    result += to_string(intPart);

    if (fracPart > 0.00001) {
        result += ".";
        for (int i = 0; i < 4; i++) {
            fracPart *= 10;
            int digit = (int) fracPart;
            result += (char)('0' + digit);
            fracPart -= digit;
        }
    }
    return result;
}

int tokenizeExpression(const string &expr, string tokens[]) {
    int count = 0;
    size_t i = 0;
    size_t len = expr.length();

    while (i < len) {
        char c = expr[i];

        if (c == ' ') {
            i++;
            continue;
        }

        if ((c >= '0' && c <= '9') || c == '.') {
            string num = "";
            while (i < len && ((expr[i] >= '0' && expr[i] <= '9') || expr[i] == '.')) {
                num += expr[i];
                i++;
            }
            tokens[count++] = num;
        }
        else if (c == 'x' || c == 'X') {
            tokens[count++] = "x";
            i++;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||
                 c == '(' || c == ')') {

            if (c == '-' && (count == 0 || tokens[count - 1] == "(" ||
                              tokens[count - 1] == "+" || tokens[count - 1] == "-" ||
                              tokens[count - 1] == "*" || tokens[count - 1] == "/" ||
                              tokens[count - 1] == "^")) {
                tokens[count++] = "0";
            }
            string op(1, c);
            tokens[count++] = op;
            i++;
        }
        else {
            i++;
        }
    }
    return count;
}

bool isOperator(const string &t) {
    return t == "+" || t == "-" || t == "*" || t == "/" || t == "^";
}

int precedence(const string &op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

struct TokenStack {
    string data[MAXTOK];
    int top = -1;
    void push(string s) { data[++top] = s; }
    string pop() { return data[top--]; }
    string peek() { return data[top]; }
    bool isEmpty() { return top == -1; }
};

int infixToPostfix(string tokens[], int n, string result[]) {
    TokenStack st;
    int rcount = 0;

    for (int i = 0; i < n; i++) {
        string tok = tokens[i];

        if (tok != "(" && tok != ")" && !isOperator(tok)) {
            result[rcount++] = tok;
        } else if (tok == "(") {
            st.push(tok);
        } else if (tok == ")") {
            while (!st.isEmpty() && st.peek() != "(") {
                result[rcount++] = st.pop();
            }
            if (!st.isEmpty()) st.pop(); 
        } else if (isOperator(tok)) {
            while (!st.isEmpty() && st.peek() != "(" &&
                   precedence(st.peek()) >= precedence(tok)) {
                result[rcount++] = st.pop();
            }
            st.push(tok);
        }
    }

    while (!st.isEmpty()) result[rcount++] = st.pop();
    return rcount;
}

double power(double base, double exp) {
    long long e = (long long) exp;
    double result = 1.0;
    for (long long i = 0; i < e; i++) result *= base;
    return result;
}

double evaluatePostfix(string postfix[], int n, double xValue) {
    double stackArr[MAXTOK];
    int top = -1;

    for (int i = 0; i < n; i++) {
        string tok = postfix[i];

        if (tok == "x") {
            stackArr[++top] = xValue;
        } else if (isOperator(tok)) {
            double b = stackArr[top--];
            double a = stackArr[top--];
            double res = 0;
            if (tok == "+") res = a + b;
            else if (tok == "-") res = a - b;
            else if (tok == "*") res = a * b;
            else if (tok == "/") res = a / b;
            else if (tok == "^") res = power(a, b);
            stackArr[++top] = res;
        } else {
            stackArr[++top] = stringToDouble(tok);
        }
    }

    return stackArr[top];
}

bool evaluateCondition(const string &condition, double xValue) {
    string op = "";
    size_t opPos = string::npos;
    size_t opLen = 0;

    for (size_t i = 0; i + 1 < condition.length(); i++) {
        string two = condition.substr(i, 2);
        if (two == ">=" || two == "<=" || two == "==" || two == "!=") {
            op = two;
            opPos = i;
            opLen = 2;
            break;
        }
    }

    if (op == "") {
        for (size_t i = 0; i < condition.length(); i++) {
            if (condition[i] == '>' || condition[i] == '<') {
                op = string(1, condition[i]);
                opPos = i;
                opLen = 1;
                break;
            }
        }
    }

    if (opPos == string::npos) {
        cout << "Invalid condition format: " << condition << endl;
        return false;
    }

    string rhsStr = condition.substr(opPos + opLen);
    size_t start = 0;
    while (start < rhsStr.length() && rhsStr[start] == ' ') start++;
    rhsStr = rhsStr.substr(start);

    double rhsVal = stringToDouble(rhsStr);

    if (op == ">=") return xValue >= rhsVal;
    if (op == "<=") return xValue <= rhsVal;
    if (op == "==") return xValue == rhsVal;
    if (op == "!=") return xValue != rhsVal;
    if (op == ">")  return xValue > rhsVal;
    if (op == "<")  return xValue < rhsVal;

    return false;
}

int main() {
    int n;
    string expressions[MAXN];
    string conditions[MAXN];

    cout << "Enter number of piecewise expressions: ";
    cin >> n;
    cin.ignore();

    if (n <= 0 || n > MAXN) {
        cout << "Invalid number of pieces." << endl;
        return 0;
    }

    for (int i = 0; i < n; i++) {
        cout << "Enter Expression " << (i + 1) << " (in terms of x): ";
        getline(cin, expressions[i]);
        cout << "Enter Condition " << (i + 1) << " (e.g. x >= 0): ";
        getline(cin, conditions[i]);
    }

    double xValue;
    cout << "Enter x: ";
    cin >> xValue;

    int selected = -1;
    for (int i = 0; i < n; i++) {
        if (evaluateCondition(conditions[i], xValue)) {
            selected = i;
            break;
        }
    }

    if (selected == -1) {
        cout << "\nNo condition satisfied for x = " << xValue << endl;
        return 0;
    }

    string tokens[MAXTOK], postfix[MAXTOK];
    int tokCount = tokenizeExpression(expressions[selected], tokens);
    int postCount = infixToPostfix(tokens, tokCount, postfix);

    double answer = evaluatePostfix(postfix, postCount, xValue);

    cout << "\nThe program selects:" << endl;
    cout << "Condition   : " << conditions[selected] << endl;
    cout << "Expression  : " << expressions[selected] << endl;

    cout << "Postfix     : ";
    for (int i = 0; i < postCount; i++) {
        cout << postfix[i];
        if (i != postCount - 1) cout << " ";
    }
    cout << endl;

    cout << "f(" << doubleToString(xValue) << ") = " << answer << endl;

    return 0;
}