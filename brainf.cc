
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>
#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Value.h>
#include <llvm/ValueSymbolTable.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include "parse.h"
#include "reader.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <input-file>\n", argv[0]);
    return 1;
  }

  string err;
  auto contents = ReadFile(argv[1], &err);
  if (!err.empty()) {
    fprintf(stderr, "error: %s\n", err.c_str());
    return 1;
  }

  auto ast = Parse(contents);
  llvm::LLVMContext ctx;
  llvm::Module module(argv[1], ctx);

  llvm::Function *memset = static_cast<llvm::Function*>(module.getOrInsertFunction("memset", llvm::Type::getInt8PtrTy(ctx), llvm::Type::getInt8PtrTy(ctx), llvm::Type::getInt32Ty(ctx), llvm::Type::getInt64Ty(ctx), NULL));
  llvm::Function *main = static_cast<llvm::Function*>(module.getOrInsertFunction("main", llvm::Type::getInt32Ty(ctx), NULL));
  llvm::IRBuilder<> builder(llvm::BasicBlock::Create(ctx, "entry", main));
  llvm::AllocaInst *tape = builder.CreateAlloca(llvm::ArrayType::get(llvm::Type::getInt8Ty(ctx), 30000), 0, "tape");
  llvm::AllocaInst *p = builder.CreateAlloca(llvm::Type::getInt8PtrTy(ctx), 0, "p");

  {
    std::vector<llvm::Value*> args;
    args.push_back(builder.getInt32(0));
    args.push_back(builder.getInt32(0));
    llvm::Value *t = builder.CreateInBoundsGEP(tape, args);
    builder.CreateCall3(memset, t, builder.getInt32(0), builder.getInt64(30000));
    builder.CreateStore(t, p);
  }

  ast->Codegen(builder, module);
  builder.CreateRet(builder.getInt32(0));

  llvm::raw_fd_ostream fd(fileno(stdout), false);
  module.print(fd, NULL);
  return 0;
}
