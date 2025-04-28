#pragma once

#include <variant>
#include <string>

struct FunctionDecl;

typedef std::variant<std::monostate, int, float, FunctionDecl*> Data;

class Value {
public:
   enum class Type {
      NIL, INT, FLOAT, FUNCTION
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
   bool is_truthy();
};
