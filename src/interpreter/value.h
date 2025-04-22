#pragma once

#include <variant>

typedef std::variant<std::monostate, int, float> Data;

class Value {
public:
   enum class Type {
      NIL, INT, FLOAT
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

   Type get_type();
   Data get_data();

   int get_int();
   bool is_truthy();
};
