#pragma once

#include "function.h"

#include <variant>
#include <string>
#include <memory>

struct FunctionDecl;

typedef std::variant<std::monostate, int, float, std::string, FunctionDecl*, std::shared_ptr<BuiltinFunction>> Data;

class Value {
public:
   enum class Type {
      NIL, INT, FLOAT, STRING, FUNCTION, BUILTINFUNCTION
   };

   static std::shared_ptr<Value> make(Type type, Data data) {
      return std::make_shared<Value>(type, data);
   }

   static std::shared_ptr<Value> make(Type type) {
      return std::make_shared<Value>(type);
   }

   static std::string type_to_str(Type type) {
      switch (type) {
         default:
            return "null";
         case Type::INT:
            return "integer";
         case Type::FLOAT:
            return "float";
         case Type::STRING:
            return "string";
         case Type::FUNCTION:
            return "function";
         case Type::BUILTINFUNCTION:
            return "builtinfunction";
      }
   }

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
   bool is_truthy();

   int get_as_int();
   float get_as_float();
   std::string get_as_str();
   FunctionDecl* get_as_function();
   std::shared_ptr<BuiltinFunction> get_as_builtin_function();
};
