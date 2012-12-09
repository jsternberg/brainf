#pragma once

#include <string>
#include <memory>
#include "ast.h"
using namespace std;

unique_ptr<AST> Parse(const string& inp);
