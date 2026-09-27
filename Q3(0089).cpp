#include <iostream>
#include <string>
using namespace std;

const int MAXTOK = 200;

struct TokenStack {
    string data[MAXTOK];
    int top = -1;

    void push(string s) { data[++top] = s; }
    string pop() { return data[top--]; }
    string peek() { return data[top]; }
    bool isEmpty() { return top == -1; }
};

bool isFunction(const string &t) {
    return t == "sqrt" || t == "sin" || t == "cos" || t == "tan" || t == "pm";
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

int tokenize(const string &expr, string tokens[]) {
    int count = 0;
    string current = "";
    for (size_t i = 0; i <= expr.length(); i++) {
        if (i == expr.length() || expr[i] == ' ') {
            if (current.length() > 0) {
                tokens[count++] = current;
                current = "";
            }
        } else {
            current += expr[i];
        }
    }
    return count;
}

int infixToPostfix(string tokens[], int n, string result[]) {
    TokenStack st;
    int rcount = 0;

    for (int i = 0; i < n; i++) {
        string tok = tokens[i];

        if (tok != "(" && tok != ")" && !isOperator(tok) && !isFunction(tok)) {
            result[rcount++] = tok;
        } else if (isFunction(tok)) {
            st.push(tok);
        } else if (tok == "(") {
            st.push(tok);
        } else if (tok == ")") {
            while (!st.isEmpty() && st.peek() != "(") {
                result[rcount++] = st.pop();
            }
            if (!st.isEmpty()) st.pop();
            while (!st.isEmpty() && isFunction(st.peek())) {
                result[rcount++] = st.pop(); 
            }
        } else if (isOperator(tok)) {
            while (!st.isEmpty() && st.peek() != "(" &&
                   (isFunction(st.peek()) || precedence(st.peek()) >= precedence(tok))) {
                result[rcount++] = st.pop();
            }
            st.push(tok);
        }
    }

    while (!st.isEmpty()) {
        result[rcount++] = st.pop();
    }

    return rcount;
}

int infixToPrefix(string tokens[], int n, string result[]) {
    string revTokens[MAXTOK];
    for (int i = 0; i < n; i++) {
        string tok = tokens[n - 1 - i];
        if (tok == "(") tok = ")";
        else if (tok == ")") tok = "(";
        revTokens[i] = tok;
    }

    TokenStack st;
    string temp[MAXTOK];
    int tcount = 0;

    for (int i = 0; i < n; i++) {
        string tok = revTokens[i];

        if (tok != "(" && tok != ")" && !isOperator(tok) && !isFunction(tok)) {
            temp[tcount++] = tok;
        } else if (isFunction(tok)) {
            st.push(tok);
        } else if (tok == "(") {
            st.push(tok);
        } else if (tok == ")") {
            while (!st.isEmpty() && st.peek() != "(") {
                temp[tcount++] = st.pop();
            }
            if (!st.isEmpty()) st.pop();
            while (!st.isEmpty() && isFunction(st.peek())) {
                temp[tcount++] = st.pop();
            }
        } else if (isOperator(tok)) {
            while (!st.isEmpty() && st.peek() != "(" &&
                   (isFunction(st.peek()) || precedence(st.peek()) > precedence(tok))) {
                temp[tcount++] = st.pop();
            }
            st.push(tok);
        }
    }

    while (!st.isEmpty()) {
        temp[tcount++] = st.pop();
    }

    for (int i = 0; i < tcount; i++) {
        result[i] = temp[tcount - 1 - i];
    }

    return tcount;
}

void printTokens(string tokens[], int n) {
    for (int i = 0; i < n; i++) {
        cout << tokens[i];
        if (i != n - 1) cout << " ";
    }
    cout << endl;
}

void convertAndShow(const string &title, const string &infixExpr) {
    string tokens[MAXTOK], postfix[MAXTOK], prefix[MAXTOK];

    int n = tokenize(infixExpr, tokens);
    int pcount = infixToPostfix(tokens, n, postfix);
    int qcount = infixToPrefix(tokens, n, prefix);

    cout << "\n---- " << title << " ----" << endl;
    cout << "Infix RHS : " << infixExpr << endl;
    cout << "Postfix   : ";
    printTokens(postfix, pcount);
    cout << "Prefix    : ";
    printTokens(prefix, qcount);
}

int main() {
    cout << "Converting RHS of trigonometric identities to Postfix / Prefix" << endl;
    cout << "(theta/alpha/beta are angle variables, pm = +/- , sqrt = square root)" << endl;

    convertAndShow(
        "Double-Angle Formula: tan(theta/2) RHS",
        "pm ( sqrt ( ( 1 - cos theta ) / ( 1 + cos theta ) ) )"
    );

    convertAndShow(
        "Pythagorean Identity: sin^2(theta) RHS",
        "1 - ( cos theta ) ^ 2"
    );

    convertAndShow(
        "Product-to-Sum Identity: sin(alpha)*sin(beta) RHS",
        "( 1 / 2 ) * ( cos ( alpha - beta ) - cos ( alpha + beta ) )"
    );

    return 0;
}