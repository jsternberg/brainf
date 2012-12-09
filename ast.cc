
#include <sstream>
#include "ast.h"

string BodyAST::ToString() {
  stringstream ss;
  ss << "[";
  for (size_t i = 0, sz = exprs_.size(); i < sz; ++i) {
    if (i) ss << ", ";
    ss << exprs_[i]->ToString();
  }
  ss << "]";
  return ss.str();
}

string OperationAST::ToString() {
  if (increment_) return "inc";
  else return "dec";
}

string ShiftAST::ToString() {
  if (right_) return "shiftr";
  else return "shiftl";
}
