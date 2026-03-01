#pragma once

#include <vector>
#include <fstream>
#include <string>

class Node {
public:
    Node(int line);
    int getLine();
    virtual void accept(class Visitor &v) = 0;
private:
    int line;
};

class ExpNode: public Node {
public:
    ExpNode(int line);
};

class StringNode: public ExpNode {
public:
    StringNode(int line, char * value);
    void accept(Visitor &v);
    char* value;
};

class IntegerNode: public ExpNode {
public:
    IntegerNode(int line, int value);
    void accept(Visitor &v);
    int value;
};

class FloatNode: public ExpNode {
public:
    FloatNode(int line, float value);
    void accept(Visitor &v);
    float value;
};

class PlusNode: public ExpNode {
public:
    PlusNode(int line, ExpNode * left, ExpNode * right);
    void accept(Visitor &v);
    ExpNode *left, *right;
};

class MinusNode: public ExpNode {
public:
    MinusNode(int line, ExpNode * left, ExpNode * right);
    void accept(Visitor &v);
    ExpNode *left, *right;
};

class MultNode: public ExpNode {
public:
    MultNode(int line, ExpNode * left, ExpNode * right);
    void accept(Visitor &v);
    ExpNode *left, *right;
};

class DivNode: public ExpNode {
public:
    DivNode(int line, ExpNode * left, ExpNode * right);
    void accept(Visitor &v);
    ExpNode *left, *right;
};

class StatementNode: public ExpNode {
public:
    StatementNode(int line, ExpNode *exp);
    ExpNode* getExp();
    void accept(Visitor &v);
private:
    ExpNode* exp;
};

class ProgramNode: public ExpNode {
public:
    ProgramNode(int line);
    void accept(Visitor &v);
    void addStatement(StatementNode* statement);
    std::vector<StatementNode*> getStatements();
private:
    std::vector<StatementNode*> statements;
};

class Visitor {
public:
    virtual void visit(IntegerNode * node);
    virtual void visit(FloatNode * node);
    virtual void visit(PlusNode * node);
    virtual void visit(MinusNode * node);
    virtual void visit(MultNode * node);
    virtual void visit(DivNode * node);
    virtual void visit(ProgramNode * node);
    virtual void visit(StatementNode * node);
    virtual void visit(StringNode * node);
};