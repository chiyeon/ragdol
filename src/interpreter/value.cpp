#include "value.h"
#include "ast.h"

std::string Value::to_str() {
   switch(type) {
      case Type::NIL:
         return "NULL";
      case Type::INT:
         {
            int* i = std::get_if<int>(&data);
            return std::to_string(*i);
         }
      case Type::FLOAT:
         {
            float* f = std::get_if<float>(&data);
            return std::to_string(*f);
         }
      case Type::FUNCTION:
         {
            FunctionDecl** fn = std::get_if<FunctionDecl*>(&data);
            return "Function<" + (*fn)->name + ">";
         }
   }
}

Value::Type Value::get_type() {
   return type;
}

Data Value::get_data() {
   return data;
}

bool Value::is_truthy() {
   switch(type) {
      case Type::NIL:
         return false;
      case Type::INT:
         {
            int* i = std::get_if<int>(&data);
            return *i != 0;
         }
      case Type::FLOAT:
         {
            float* i = std::get_if<float>(&data);
            return *i != 0.0;
         }
   }
}

int Value::get_int() {
   int* i = std::get_if<int>(&data);
   return *i;
}
