#include "value.h"
#include "ast.h"

#include <iostream>

std::string Value::to_str() {
   switch(type) {
      case Type::NIL:
         return "NULL";
      case Type::INT:
         return std::to_string(get_as_int());
      case Type::BOOL:
         return get_as_bool() ? "true" : "false";
      case Type::FLOAT:
         return std::to_string(get_as_float());
      case Type::FUNCTION:
         return get_as_function()->to_str();
      case Type::STRING:
         return get_as_str(); 
      case Type::BUILTINFUNCTION:
         return get_as_builtin_function()->to_str();
      case Type::ARRAY:
         return "array";
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
         return get_as_int() != 0;
      case Type::FLOAT:
         return get_as_float() != 0.0;
      case Type::BOOL:
         return get_as_bool();
      default:
         return false;
   }
}

void error_comparison(Value::Type a, Value::Type b) {
   std::cout << "No valid comparison for types " << Value::type_to_str(a) << " and " << Value::type_to_str(b) << std::endl;
}

bool Value::equals(std::shared_ptr<Value> other) {
   switch (type) {
      case Type::NIL:
         return other->get_type() == Type::NIL;
      case Type::INT:
         {
            int asint = get_as_int();

            // the other could be a float or int, we compare with both
            if (other->get_type() == Type::INT) {
               return asint == other->get_as_int();
            } else if (other->get_type() == Type::FLOAT) {
               return asint == other->get_as_float();
            } else {
               error_comparison(type, other->get_type());
            }

            return false;
         }
      case Type::BOOL:
         {
            if (other->get_type() == Type::BOOL) {
               return get_as_bool() == other->get_as_bool();
            } else {
               error_comparison(type, other->get_type());
            }
            return false;
         }
      case Type::FLOAT:
         {
            float asfloat = get_as_float();

            if (other->get_type() == Type::INT) {
               return asfloat == other->get_as_int();
            } else if (other->get_type() == Type::FLOAT) {
               return asfloat == other->get_as_float();
            } else {
               error_comparison(type, other->get_type());
            }

            return false;
         }
      case Type::FUNCTION:
         {
            if (other->get_type() == Type::FUNCTION) {
               return other->get_as_function()->name == get_as_function()->name;
            }
            return false;
         }
      case Type::STRING:
         {
            if (other->get_type() == Type::STRING) {
               return get_as_str() == other->get_as_str();
            }
            return false;
         }
      case Type::BUILTINFUNCTION:
         {
            if (other->get_type() == Type::BUILTINFUNCTION) {
               return other->get_as_builtin_function()->name == get_as_builtin_function()->name;
            }
         }
   }
}

int Value::get_as_int() {
   int* i = std::get_if<int>(&data);
   return *i;
}

bool Value::get_as_bool() {
   bool* i = std::get_if<bool>(&data);
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

Array* Value::get_as_array() {
   Array** a = std::get_if<Array*>(&data);
   return *a;
}

std::shared_ptr<BuiltinFunction> Value::get_as_builtin_function() {
   auto fn = std::get_if<std::shared_ptr<BuiltinFunction>>(&data);
   return *fn;
}
