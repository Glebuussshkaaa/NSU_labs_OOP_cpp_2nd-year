#include <iostream>
#include <string>

class Node {
public:
    Node *arg1;
    Node *arg2;

    Node() : arg1(nullptr), arg2(nullptr) {}

    virtual ~Node() = default;
};


class Op : public Node {
public:
    enum Operator {
        ADD, SUB, MUL, DIV
    };

    explicit Op(Operator op) : op(op) {}

    Operator getOperator() const {
        return op;
    }

private:
    Operator op;
};

class Value : public Node {
public:
    explicit Value(int value) :
            value(value) {}

    int getValue() const {
        return value;
    }

private:
    int value;
};

class Parser {
public:
    explicit Parser(const std::string &text) :
            text(text),
            pos(0) {}

    Node *parse() {
        Node *root = S();
        if (pos < text.length()) {
            throw std::invalid_argument("Лишние символы в конце выражения");
        }
        return root;
    }

    void skipWhitespace() {
        while (pos < text.length() && std::isspace(text[pos])) {
            pos++;
        }
    }

private:
    std::string text;
    size_t pos;

    Node *S() {
        Node *left = M();
        skipWhitespace();
        while (pos < text.length() && (text[pos] == '+' || text[pos] == '-')) {
            char opChar = text[pos];
            pos++;
            Node *right = M();
            Op *opNode = new Op(opChar == '+' ? Op::ADD : Op::SUB);
            opNode->arg1 = left;
            opNode->arg2 = right;
            left = opNode;
        }
        if (pos < text.length() && text[pos] != ')' && !std::isspace(text[pos])) {
            throw std::invalid_argument("Недопустимый символ в выражении: " + std::string(1, text[pos]));
        }
        return left;
    }

    Node *M() {
        Node *left = P();
        skipWhitespace();
        if (text[pos] == '*') {
            pos++;
            Node *right = M();
            Op *opNode = new Op(Op::MUL);
            opNode->arg1 = left;
            opNode->arg2 = right;
            return opNode;
        } else if (text[pos] == '/') {
            pos++;
            Node *right = M();
            Op *opNode = new Op(Op::DIV);
            opNode->arg1 = left;
            opNode->arg2 = right;
            return opNode;
        } else if (pos < text.length() && text[pos] != '+' && text[pos] != '-' && text[pos] != ')' &&
                   !std::isspace(text[pos])) {
            throw std::invalid_argument("Недопустимый символ в выражении: " + std::string(1, text[pos]));
        }
        return left;
    }

    Node *P() {
        skipWhitespace();
        if (isdigit(text[pos])) {
            int value = 0;
            while (isdigit(text[pos])) {
                value = value * 10 + (text[pos] - '0');
                pos++;
            }
            return new Value(value);
        } else if (text[pos] == '(') {
            pos++;
            Node *node = S();
            if (text[pos] != ')') {
                throw std::invalid_argument("нет обратной скобки");
            }
            pos++;
            return node;
        } else {
            throw std::invalid_argument("Недопустимый символ в выражении: " + std::string(1, text[pos]));
        }
    }
};

void printTree(const Node *node, int indent = 0) {
    if (!node) return;

    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }

    const Op *opNode = dynamic_cast<const Op *>(node);
    if (opNode) {
        char opChar;
        switch (opNode->getOperator()) {
            case Op::ADD:
                opChar = '+';
                break;
            case Op::SUB:
                opChar = '-';
                break;
            case Op::MUL:
                opChar = '*';
                break;
            case Op::DIV:
                opChar = '/';
                break;
        }
        std::cout << "Op(" << opChar << ")" << std::endl;
    } else {
        auto valueNode = dynamic_cast<const Value *>(node);
        if (valueNode) {
            std::cout << "Value(" << valueNode->getValue() << ")" << std::endl;
        }
    }
    printTree(node->arg1, indent + 1);
    printTree(node->arg2, indent + 1);
}

void deleteTree(Node *node) {
    if (node) {
        deleteTree(node->arg1);
        deleteTree(node->arg2);
        delete node;
    }
}

int main() {
    std::string exp = "3 - 2 - 1";
    Parser parser(exp);

    try {
        Node *root = parser.parse();
        printTree(root);
        deleteTree(root);
    } catch (const std::invalid_argument &exc) {
        std::cerr << "Invalid argument: " << exc.what() << std::endl;
    }
    return 0;
}

