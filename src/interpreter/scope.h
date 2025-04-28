#pragma once

#include "value.h"

#include <unordered_map>
#include <string>
#include <memory>

class Scope {
   std::shared_ptr<Scope> parent;
   std::unordered_map<std::string, std::shared_ptr<Value>> variables;
public:
   Scope()
      : parent(nullptr)
      {}
   Scope(std::shared_ptr<Scope> parent)
      : parent(parent)
      {}

   std::shared_ptr<Value> get(const std::string& var_name);
   std::shared_ptr<Value> get_in_scope(const std::string& var_name);
   void assign_or_insert(const std::string& var_name, std::shared_ptr<Value> value);
   bool assign(const std::string& var_name, std::shared_ptr<Value> value);
   void force_insert(const std::string& var_name, std::shared_ptr<Value> value);

   std::shared_ptr<Scope> get_parent() { return parent; }
   bool is_global();

   std::string to_str();
};
