#include "value.h"

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
