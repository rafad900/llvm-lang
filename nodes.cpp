#include "nodes.hpp"
#include <iostream>

IntegerNode::IntegerNode(int line, int value) : value(value), ExpNode(line) {}
FloatNode::FloatNode(int line, float value) : value(value), ExpNode(line) {}
PlusNode::PlusNode(int line, ExpNode * left, ExpNode * right) : left(left), right(right), ExpNode(line) {}
MinusNode::MinusNode(int line, ExpNode * left, ExpNode * right) : left(left), right(right), ExpNode(line) {}
MultNode::MultNode(int line, ExpNode * left, ExpNode * right) : left(left), right(right), ExpNode(line) {}
DivNode::DivNode(int line, ExpNode * left, ExpNode * right) : left(left), right(right), ExpNode(line) {}
StatementNode::StatementNode(int line, ExpNode * exp) : ExpNode(line), exp(exp) {}
StringNode::StringNode(int line, char* value): ExpNode(line), value(value) {}
ProgramNode::ProgramNode(int line) : ExpNode(line) {}

void IntegerNode::accept(Visitor &v) {  v.visit(this);}
void FloatNode::accept(Visitor & v) {   v.visit(this);}
void PlusNode::accept(Visitor &v) {     v.visit(this);}
void MinusNode::accept(Visitor &v) {    v.visit(this);}
void MultNode::accept(Visitor &v) {     v.visit(this);}
void DivNode::accept(Visitor &v) {      v.visit(this);}
void StatementNode::accept(Visitor &v) {v.visit(this);}
void ProgramNode::accept(Visitor &v)   {v.visit(this);} 
void StringNode::accept(Visitor &v)    {v.visit(this);}

void ProgramNode::addStatement(StatementNode * statement) {
    statements.push_back(statement);
}

std::vector<StatementNode*> ProgramNode::getStatements() {
    return statements;
}

ExpNode* StatementNode::getExp() { return exp; }


Node::Node(int line) : line(line) {}
int Node::getLine() { return line; }

ExpNode::ExpNode(int line) : Node(line) {}