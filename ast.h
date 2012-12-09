#pragma once

#include <memory>
#include <string>
#include <vector>
using namespace std;

struct AST {
  virtual string ToString() = 0;
};

struct BodyAST : AST {
  vector<unique_ptr<AST>> exprs_;
  void append(AST *expr) { exprs_.push_back(unique_ptr<AST>(expr)); }
  string ToString();
};

struct OperationAST : AST {
  bool increment_;
  OperationAST(bool increment) : increment_(increment) {}
  string ToString();
};

struct ShiftAST : AST {
  bool right_;
  ShiftAST(bool right) : right_(right) {}
  string ToString();
};
