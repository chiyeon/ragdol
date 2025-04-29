#include "value.h"
#include "ast.h"

std::string Value::to_str() {
   switch(type) {
      case Type::NIL:
         return "NULL";
      case Type::INT:
         {
            return std::to_string(get_as_int());
         }
      case Type::FLOAT:
         {
            return std::to_string(get_as_float());
         }
      case Type::FUNCTION:
         {
            return get_as_function()->to_str();
         }
      case Type::STRING:
         {
            return get_as_str(); 
         }
      case Type::BUILTINFUNCTION:
         {
            return get_as_builtin_function()->to_str();
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

int Value::get_as_int() {
   int* i = std::get_if<int>(&data);
   return *i;
}

float Value::get_as_float() {
   float* f = std::get_if<float>(&data);
   return *f;
}

std::string Value::get_as_str() {
   std::string* s = std::get_if<std::string>(&data);
   return *s;
}

FunctionDecl* Value::get_as_function() {
   FunctionDecl** fn = std::get_if<FunctionDecl*>(&data);
   return *fn;
}

std::shared_ptr<BuiltinFunction> Value::get_as_builtin_function() {
   auto fn = std::get_if<std::shared_ptr<BuiltinFunction>>(&data);
   return *fn;
}
