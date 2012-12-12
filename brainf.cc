
#include <unistd.h>
#include <stdio.h>
#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>
#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
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

  llvm::Function *main = static_cast<llvm::Function*>(module.getOrInsertFunction("main", llvm::Type::getInt32Ty(ctx), NULL));
  llvm::IRBuilder<> builder(llvm::BasicBlock::Create(ctx, "entry", main));
  llvm::AllocaInst *p = builder.CreateAlloca(llvm::Type::getInt32Ty(ctx), 0, "p");
  builder.CreateStore(builder.getInt32(0), p);

  ast->Codegen(builder, module);
  builder.CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(ctx), 0));

  llvm::raw_fd_ostream fd(fileno(stdout), false);
  module.print(fd, NULL);
  return 0;
}
