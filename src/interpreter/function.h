#pragma once

#include <string>
#include <vector>

class Function {
   std::string name;
   std::vector<std::string> params;

public:
   Function(const std::string& name, std::vector<std::string> params)
      : name(name), params(std::move(params))
      {}
};
