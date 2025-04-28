#pragma once

#include "function.h"

#include <variant>
#include <string>
#include <memory>

struct FunctionDecl;

typedef std::variant<std::monostate, int, float, FunctionDecl*, std::shared_ptr<BuiltinFunction>> Data;

class Value {
public:
   enum class Type {
      NIL, INT, FLOAT, FUNCTION, BULITINFUNCTION
   };

private:
   Type type;
   Data data;

public:
   Value(Type type, Data data)
      : type(type)
      , data(data)
      {}
   
   Value(Type type)
      : type(type)
      {}

   std::string to_str();

   Type get_type();
   Data get_data();

   int get_int();
   FunctionDecl* get_function();
   std::shared_ptr<BuiltinFunction> get_builtin_function();
   bool is_truthy();
};
