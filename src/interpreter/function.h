#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Value;

typedef std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> fndef;

/*
class Function {
   std::string name;
   std::vector<std::string> params;

public:
   Function(const std::string& name, std::vector<std::string> params)
      : name(name), params(std::move(params))
      {}
};
*/

struct BuiltinFunction
{
   std::string name;
   fndef function;

   BuiltinFunction(fndef function) : function(function) {}

   std::string to_str() { return "BuiltinFunction<" + name + ">"; }
};
