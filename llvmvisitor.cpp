#include <fstream>
#include <iostream>
#include "llvmvisitor.hpp"
#include "nodes.hpp"


LLVMVisitor::LLVMVisitor(llvm::raw_fd_ostream &out,
                         llvm::LLVMContext &context,
                         std::unique_ptr<llvm::Module> &mod) : out(out), mod(mod), context(context), builder(context) {
    ret = nullptr;
    floatInst = false;
}

void LLVMVisitor::visit(ProgramNode * node) {
    std::cout << "Beginning to create the program\n";
    llvm::FunctionType* functionReturnType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context), llvm::ArrayRef<llvm::Type*>(), false);

    llvm::Function* mainFunction = llvm::Function::Create(
        functionReturnType, llvm::GlobalValue::ExternalLinkage, "main", mod.get());

    llvm::BasicBlock* body = llvm::BasicBlock::Create(context, "body", mainFunction);
    builder.SetInsertPoint(body);

    std::vector<llvm::Type*> params;
    params.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0));
    llvm::FunctionType *printfType = llvm::FunctionType::get(builder.getInt32Ty(), params, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", mod.get());
    
    for (auto statement : node->getStatements()) {
        statement->accept(*this);
    }

    builder.CreateRetVoid();

    mod->print(out, nullptr);
    out.close();
}

void LLVMVisitor::visit(StatementNode *node) {
    node->getExp()->accept(*this);

    std::vector<llvm::Value*> printArgs;
    llvm::Value* formatStr;
    if (floatInst) {
        formatStr = builder.CreateGlobalStringPtr("%f\n");
    } else {
        formatStr = builder.CreateGlobalStringPtr("%d\n");
    }

    printArgs.push_back(formatStr);
    printArgs.push_back(ret);
    builder.CreateCall(mod.get()->getFunction("printf"), printArgs);

    floatInst = false;
}

void LLVMVisitor::visit(IntegerNode * node) {
    ret = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(context), node->value);
}

void LLVMVisitor::visit(FloatNode * node) {
    ret = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), node->value);
    floatInst = true;
}

void LLVMVisitor::visit(PlusNode * node) {
    node->left->accept(*this);
    llvm::Value *lhs = ret;
    node->right->accept(*this);
    llvm::Value *rhs = ret;

    if (floatInst) {
        if (!lhs->getType()->isDoubleTy()) 
            lhs = builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context));
        if (!rhs->getType()->isDoubleTy()) 
            rhs = builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context));
        ret = builder.CreateFAdd(lhs, rhs);
    } else {
        ret = builder.CreateAdd(lhs, rhs);
    }
}

void LLVMVisitor::visit(MinusNode * node) {
    node->left->accept(*this);
    llvm::Value *lhs = ret;
    node->right->accept(*this);
    llvm::Value *rhs = ret;

    if (floatInst) {
        if (!lhs->getType()->isDoubleTy()) 
            lhs = builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context));
        if (!rhs->getType()->isDoubleTy()) 
            rhs = builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context));
        ret = builder.CreateFSub(lhs, rhs);
    } else {
        ret = builder.CreateSub(lhs, rhs);
    }
}

void LLVMVisitor::visit(DivNode * node) {
    node->left->accept(*this);
    llvm::Value *lhs = ret;
    node->right->accept(*this);
    llvm::Value *rhs = ret;

    if (floatInst) {
        if (!lhs->getType()->isDoubleTy()) 
            lhs = builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context));
        if (!rhs->getType()->isDoubleTy()) 
            rhs = builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context));
        ret = builder.CreateFDiv(lhs, rhs);
    } else {
        ret = builder.CreateSDiv(lhs, rhs);
    }
}

void LLVMVisitor::visit(MultNode * node) {
    node->left->accept(*this);
    llvm::Value *lhs = ret;
    node->right->accept(*this);
    llvm::Value *rhs = ret;

    if (floatInst) {
        if (!lhs->getType()->isDoubleTy()) 
            lhs = builder.CreateSIToFP(lhs, llvm::Type::getDoubleTy(context));
        if (!rhs->getType()->isDoubleTy()) 
            rhs = builder.CreateSIToFP(rhs, llvm::Type::getDoubleTy(context));
        ret = builder.CreateFMul(lhs, rhs);
    } else {
        ret = builder.CreateMul(lhs, rhs);
    }
}