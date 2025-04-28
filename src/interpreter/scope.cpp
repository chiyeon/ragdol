#include "scope.h"

std::shared_ptr<Value> Scope::get(const std::string& var_name) {
   /*
    * searches for variable in current and any upper scopes
    */
   auto val = variables.find(var_name);

   // if value not in self, check parent (if parent exists)
   if (val == variables.end()) {
      if (parent != nullptr)
         return parent->get(var_name);
      else // if no parent, return nullptr
         return nullptr;
   }

   // return found value
   return val->second;
}

std::shared_ptr<Value> Scope::get_in_scope(const std::string& var_name) {
   /*
    * searches for variable ONLY in current scope
    */
   auto val = variables.find(var_name);

   if (val == variables.end()) {
      return nullptr;
   } else {
      return val->second;
   }
}

void Scope::assign_or_insert(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * stores pair. looks backwards for matching name, otherwise
    * it adds it to this scope if doesn't exist
    */
   // try to assign down the line first
   if (!assign(var_name, value)) {
      // if it doesn't work, insert into current scope
      variables.insert_or_assign(var_name, value);
   }
}

bool Scope::assign(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * Assigns var_name to value ONLY IF it can find
    * it somewhere in the scope list
    *
    * does NOT create new entries
    *
    * returns T/F if successful or not
    */
   // if exists in scope
   if (get_in_scope(var_name) != nullptr) {
      // shared ptr should handle any dangling pointers
      variables[var_name] = value;
      return true;
   } else {
      // if doesn't exist we can:
      
      // if we're global state (end)
      if (parent == nullptr) {
         // return false, we searched all scopes and 
         // couldn't find it
         return false;
      } else {
         // otherwise recursively try to assign
         return parent->assign(var_name, value);
      }
   }
}

void Scope::force_insert(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * forcibly inserts (or assigns) pair into this current scope
    * doesn't search
    */

   variables.insert_or_assign(var_name, value);
}

bool Scope::is_global() {
   return parent == nullptr;
}

std::string Scope::to_str() {
   std::string out = "SCOPE\n";

   for (const auto& [var_name, value] : variables) {
      out += "\t" + var_name + "\t = " + value->to_str() + "\n";
   }

   return out;
}
