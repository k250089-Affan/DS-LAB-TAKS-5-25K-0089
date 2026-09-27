#include <iostream>
#include <string>
using namespace std;

const int MAXTOK = 500;

struct TokenStack {
    string data[MAXTOK];
    int top = -1;
    void push(string s) { data[++top] = s; }
    string pop() { return data[top--]; }
    string peek() { return data[top]; }
    bool isEmpty() { return top == -1; }
};

int precedence(const string &op) {
    if (op == "+") return 1;
    return 0;
}

bool isOperator(const string &t) {
    return t == "+";
}

int infixToPostfix(string tokens[], int n, string result[]) {
    TokenStack st;
    int rcount = 0;

    for (int i = 0; i < n; i++) {
        string tok = tokens[i];
        if (!isOperator(tok)) {
            result[rcount++] = tok;
        } else {
            while (!st.isEmpty() && precedence(st.peek()) >= precedence(tok)) {
                result[rcount++] = st.pop();
            }
            st.push(tok);
        }
    }
    while (!st.isEmpty()) result[rcount++] = st.pop();
    return rcount;
}

int infixToPrefix(string tokens[], int n, string result[]) {
    string revTokens[MAXTOK];
    for (int i = 0; i < n; i++) {
        revTokens[i] = tokens[n - 1 - i];
    }

    TokenStack st;
    string temp[MAXTOK];
    int tcount = 0;

    for (int i = 0; i < n; i++) {
        string tok = revTokens[i];
        if (!isOperator(tok)) {
            temp[tcount++] = tok;
        } else {
            while (!st.isEmpty() && precedence(st.peek()) > precedence(tok)) {
                temp[tcount++] = st.pop();
            }
            st.push(tok);
        }
    }
    while (!st.isEmpty()) temp[tcount++] = st.pop();

    for (int i = 0; i < tcount; i++) {
        result[i] = temp[tcount - 1 - i];
    }
    return tcount;
}

int main() {
    int n;
    cout << "Enter number of terms (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of terms." << endl;
        return 0;
    }

    string terms[MAXTOK];
    for (int i = 1; i <= n; i++) {
        int value = 3 * i - 2;
        terms[i - 1] = to_string(value);
    }

    string tokens[MAXTOK];
    int tcount = 0;
    for (int i = 0; i < n; i++) {
        tokens[tcount++] = terms[i];
        if (i != n - 1) tokens[tcount++] = "+";
    }

    string postfix[MAXTOK], prefix[MAXTOK];
    int postCount = infixToPostfix(tokens, tcount, postfix);
    int preCount  = infixToPrefix(tokens, tcount, prefix);

    cout << "\nGeneral Form : 1 + 4 + 7 + ... + (3n - 2)" << endl;
    cout << "For n = " << n << ":\n" << endl;

    cout << "Series  : ";
    for (int i = 0; i < n; i++) {
        cout << terms[i];
        if (i != n - 1) cout << " + ";
    }
    cout << endl;

    cout << "Postfix : ";
    for (int i = 0; i < postCount; i++) {
        cout << postfix[i];
        if (i != postCount - 1) cout << " ";
    }
    cout << endl;

    cout << "Prefix  : ";
    for (int i = 0; i < preCount; i++) {
        cout << prefix[i];
        if (i != preCount - 1) cout << " ";
    }
    cout << endl;

    return 0;
}