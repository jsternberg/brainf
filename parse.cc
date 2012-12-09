
#include <vector>
#include "parse.h"

unique_ptr<AST> Parse(const string& inp) {
  vector<BodyAST*> stack;
  auto *ast = new BodyAST;
  for (char ch : inp) {
    switch (ch) {
      case '+':
        ast->append(new OperationAST(true));
        break;
      case '-':
        ast->append(new OperationAST(false));
        break;
      case '<':
        ast->append(new ShiftAST(false));
        break;
      case '>':
        ast->append(new ShiftAST(true));
        break;
      case '[':
        // todo: this should be a WhileAST which should inherit from
        // BodyAST
        stack.push_back(ast);
        ast = new BodyAST;
        break;
      case ']':
        if (stack.empty()) // error, for now just discard the input
          break;
        stack.back()->append(ast);
        ast = stack.back();
        stack.pop_back();
        break;
      case '.': break;
      case ',': break;
      default: break; // ignore invalid characters
    }
  }

  // automatically close all open blocks
  while (!stack.empty()) {
    stack.back()->append(ast);
    ast = stack.back();
    stack.pop_back();
  }
  return unique_ptr<AST>(ast);
}
