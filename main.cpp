#include <iostream>

#include "nodes.hpp"
#include "llvmvisitor.hpp"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Pass.h"

extern ProgramNode* program;
extern int yyparse();


int main(int argc, char **argv) {
    std::cout << "Starting the program\n";
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> mod;
    mod = std::make_unique<llvm::Module>("RafLang module", context);

    yyparse();
    std::cout << "Called the parser\n";
    llvm::StringRef filename = "test.ll";
    std::error_code ec;
    llvm::raw_fd_ostream out(filename, ec);
    std::cout << "Created the file\n";
    LLVMVisitor v(out, context, mod);
    program->accept(v);
    std::cout << "Created the visitor and started the accept\n";
    return 0;
}