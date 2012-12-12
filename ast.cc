
#include <sstream>
#include <llvm/Constants.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/ValueSymbolTable.h>
#include <llvm/Support/IRBuilder.h>
#include "ast.h"

void BodyAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module& module) {
  using namespace llvm;
  for (unique_ptr<ExprAST>& expr : exprs_) {
    expr->Codegen(builder, module);
  }
}

void OperationAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module&) {
  using namespace llvm;
  ValueSymbolTable *symTable = builder.GetInsertBlock()->getValueSymbolTable();
  AllocaInst *p = static_cast<AllocaInst*>(symTable->lookup("p"));

  Value *ptr = builder.CreateLoad(p);
  Value *var = builder.CreateLoad(ptr);
  Value *next = builder.CreateAdd(var, builder.getInt32(increment_ ? 1 : -1));
  builder.CreateStore(next, ptr);
}

void ShiftAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module&) {
  using namespace llvm;
  ValueSymbolTable *symTable = builder.GetInsertBlock()->getValueSymbolTable();
  AllocaInst *p = static_cast<AllocaInst*>(symTable->lookup("p"));

  Value *ptr = builder.CreateLoad(p);
  Value *off = builder.CreateGEP(ptr, builder.getInt32(right_ ? 1 : -1));
  builder.CreateStore(off, p);
}

void WhileAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module& module) {
  using namespace llvm;
  BasicBlock *block = builder.GetInsertBlock();
  ValueSymbolTable *symTable = block->getValueSymbolTable();
  AllocaInst *p = static_cast<AllocaInst*>(symTable->lookup("p"));
  Function *f = block->getParent();

  LLVMContext& ctx = block->getContext();
  BasicBlock *cmp = BasicBlock::Create(ctx, "");
  BasicBlock *start = BasicBlock::Create(ctx, "");
  BasicBlock *end = BasicBlock::Create(ctx, "");

  builder.CreateBr(cmp);
  f->getBasicBlockList().push_back(cmp);

  builder.SetInsertPoint(cmp);
  Value *var = builder.CreateLoad(builder.CreateLoad(p));
  Value *cond = builder.CreateICmpNE(var, builder.getInt32(0));
  builder.CreateCondBr(cond, start, end);

  f->getBasicBlockList().push_back(start);
  builder.SetInsertPoint(start);

  for (unique_ptr<ExprAST>& expr : exprs_) {
    expr->Codegen(builder, module);
  }
  builder.CreateBr(cmp);

  f->getBasicBlockList().push_back(end);
  builder.SetInsertPoint(end);
}

void PrintAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module& module) {
  using namespace llvm;
  BasicBlock *block = builder.GetInsertBlock();
  ValueSymbolTable *symTable = block->getValueSymbolTable();
  AllocaInst *p = static_cast<AllocaInst*>(symTable->lookup("p"));

  LLVMContext& ctx = block->getContext();
  Function *f = static_cast<Function*>(module.getOrInsertFunction("putchar", Type::getInt32Ty(ctx), Type::getInt32Ty(ctx), NULL));

  Value *var = builder.CreateLoad(builder.CreateLoad(p));
  builder.CreateCall(f, var);
}

void GetAST::Codegen(llvm::IRBuilder<>& builder, llvm::Module& module) {
  using namespace llvm;
  BasicBlock *block = builder.GetInsertBlock();
  ValueSymbolTable *symTable = block->getValueSymbolTable();
  AllocaInst *p = static_cast<AllocaInst*>(symTable->lookup("p"));

  LLVMContext& ctx = block->getContext();
  Function *f = static_cast<Function*>(module.getOrInsertFunction("getchar", Type::getInt32Ty(ctx), NULL));

  Value *var = builder.CreateCall(f);
  builder.CreateStore(var, builder.CreateLoad(p));
}
