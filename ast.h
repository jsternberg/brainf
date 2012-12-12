#pragma once

#include <memory>
#include <vector>
#include <llvm/Support/IRBuilder.h>

using namespace std;

struct ExprAST {
  virtual void Codegen(llvm::IRBuilder<>&, llvm::Module&) = 0;
};

struct BodyAST : ExprAST {
  vector<unique_ptr<ExprAST>> exprs_;
  void append(ExprAST *expr) { exprs_.push_back(unique_ptr<ExprAST>(expr)); }
  virtual void Codegen(llvm::IRBuilder<>& builder, llvm::Module&);
};

struct OperationAST : ExprAST {
  bool increment_;
  OperationAST(bool increment) : increment_(increment) {}
  virtual void Codegen(llvm::IRBuilder<>& builder, llvm::Module&);
};

struct WhileAST : BodyAST {
  virtual void Codegen(llvm::IRBuilder<>& builder, llvm::Module&);
};

struct PrintAST : ExprAST {
  virtual void Codegen(llvm::IRBuilder<>& builder, llvm::Module&);
};

struct GetAST : ExprAST {
  virtual void Codegen(llvm::IRBuilder<>& builder, llvm::Module&);
};
