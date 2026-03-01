#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include "nodes.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

class LLVMVisitor: public Visitor {
public:
    LLVMVisitor(llvm::raw_fd_ostream &out, llvm::LLVMContext &context, std::unique_ptr<llvm::Module> &mod);
    void visit(ProgramNode *node);
    void visit(StatementNode *node);
    void visit(IntegerNode *node);
    void visit(FloatNode *node);
    void visit(PlusNode *node);
    void visit(MinusNode*node);
    void visit(MultNode *node);
    void visit(DivNode *node);
private:
    llvm::raw_fd_ostream &out;
    llvm::LLVMContext &context;
    std::unique_ptr<llvm::Module> &mod;
    llvm::IRBuilder<> builder;

    llvm::Value *ret;
    bool floatInst;
};
