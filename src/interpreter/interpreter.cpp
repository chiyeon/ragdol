#include "interpreter.h"
#include "token.h"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

void Interpreter::log(std::string message, int loglv, char endline) {
   if (loglv > log_level) return;

   for (int i = 0; i < log_scope; ++i) std::cout << "\t";
   std::cout << message << endline;
}

void Interpreter::enter_new_scope() {
   current_scope = std::make_shared<Scope>(current_scope);
}

void Interpreter::exit_scope() {
   if (!current_scope->is_global()) {
      current_scope = current_scope->get_parent(); 
   }
}

void Interpreter::assign_variable(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * assigns variable backwards to closest scope, throwing error
    * if it cannot find it
    */
   if (!current_scope->assign(var_name, value)) {
      std::cout << "ERROR: Could not find variable " << var_name << std::endl;
   }
}

void Interpreter::assign_or_insert_variable(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * assigns variable backwards to closest scope, if it cannot find it
    * it will add it to the current scope
    */
   current_scope->assign_or_insert(var_name, value);
}

std::shared_ptr<Value> Interpreter::find_variable(const std::string& var_name) {
   return current_scope->get(var_name);
}

Interpreter::Interpreter(std::string src)
   : lexer(src)
{
   global_scope = std::make_shared<Scope>();
   current_scope = global_scope;

   /* initialize builtin functions */

   // PRINT
   fndef print_fndef = [](std::vector<std::shared_ptr<Value>> args) -> std::shared_ptr<Value> {
            for (auto& arg : args) {
               std::cout << arg->to_str() + " ";
            }
            std::cout << std::endl;
            return nullptr;
         };
   std::shared_ptr<BuiltinFunction> print_builtin = std::make_shared<BuiltinFunction>(print_fndef);
   global_scope->assign_or_insert("print", Value::make(Value::Type::BUILTINFUNCTION, print_builtin));

   tokens = lexer.tokenize();

   parser.set_tokens(tokens);
}

Interpreter::~Interpreter() {
   delete ast;
}

ASTNode* Interpreter::parse() {
   ast = parser.parse();
   return ast;
}

void Interpreter::interpret() {
   ast->accept(*this); 
}

std::vector<Token>& Interpreter::get_tokens() {
   if (tokens.empty()) {
      tokens = lexer.tokenize();
   }

   parser.set_tokens(tokens);
   //parser.parse();
   /*
   ASTNode* ast = parser.expr();

   std::cout << ast->to_str() << std::endl;

   std::shared_ptr<Value> v = visit_binary_op((BinaryOp*)ast);
   std::cout << v->get_as_int() << std::endl;
   */
   return tokens;
}

std::string Interpreter::token_to_str(Token t) {
   return Token::type_to_str.at(t.type);
}

std::shared_ptr<Value> Interpreter::visit_literal_int(LiteralInt* node) {
   log("Visited literal int: " + node->to_str(), LOG_VERBOSE);

   auto v = Value::make(Value::Type::INT, node->value);
   return v;
}

std::shared_ptr<Value> Interpreter::visit_literal(Literal* node) {
   log("Visited literal " + node->to_str(), LOG_VERBOSE);

   // todo 
   // either copy or reference based on type
   return node->value;
}

std::shared_ptr<Value> Interpreter::visit_binary_op(BinaryOp* node) {
   log("Visited binary operation: " +node->to_str(), LOG_VERBOSE);

   std::shared_ptr<Value> left = node->left->accept(*this);
   std::shared_ptr<Value> right = node->right->accept(*this);

   // for now assume these are ints
   switch(node->op.type) {
      case TokenType::PLUS:
         return Value::make(Value::Type::INT, left->get_as_int() + right->get_as_int());
      case TokenType::MINUS:
         return Value::make(Value::Type::INT, left->get_as_int() - right->get_as_int());
      case TokenType::MULT:
         return Value::make(Value::Type::INT, left->get_as_int() * right->get_as_int());
      case TokenType::DIV:
         return Value::make(Value::Type::INT, left->get_as_int() / right->get_as_int());
      default:
         return Value::make(Value::Type::NIL);
   }
}

std::shared_ptr<Value> Interpreter::visit_unary_op(UnaryOp* node) {
   log("Visited unary op: " + node->to_str(), LOG_VERBOSE);

   std::shared_ptr<Value> expr = node->expr->accept(*this);

   switch (node->token.type) {
      case TokenType::PLUS:
         return Value::make(Value::Type::INT, +expr->get_as_int());
      case TokenType::MINUS:
         return Value::make(Value::Type::INT, -expr->get_as_int());
      default:
         std::cout << "Error" << std::endl;
   }
}

std::shared_ptr<Value> Interpreter::visit_statement_list(StatementList* node) {
   log("Visited statement list:" + node->to_str(), LOG_VERBOSE);

   for (auto n : node->statements) {
      n->accept(*this);
   }

   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_block(Block* node) {
   log("Visited block: " + node->to_str(), LOG_VERBOSE);

   enter_new_scope();
   node->statements->accept(*this);
   exit_scope();

   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_no_op(NoOp* node) {
   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_assignment(Assignment* node) {
   log("Visited assignment: " + node->to_str(), LOG_VERBOSE);

   // assign variable to value
   auto nv = node->target->accept(*this);
   if (node->make_new_var) {
      assign_or_insert_variable(node->destination->get_var_name(), nv);
   } else {
      assign_variable(node->destination->get_var_name(), nv);
   }
   
   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_variable(Variable* node) {
   log("Visited variable: " + node->to_str(), LOG_VERBOSE);
   
   // get and return value
   auto var = find_variable(node->get_var_name());

   if (var != nullptr) {
      return var;
   } else {
      std::cout << "error tried to get variable that doesnt exist" << std::endl;
   }
}

std::shared_ptr<Value> Interpreter::visit_function_decl(FunctionDecl* node) {
   log("Visited Function Declaration: " + node->to_str(), LOG_VERBOSE);

   // make sure function of name doesn't exist (in scope?)
   auto var = find_variable(node->name);

   if (var != nullptr) {
      std::cout << "ERROR: cant name function " << node->name << " becasue identifier is taken" << std::endl;
   } else {
      // if doesnt exist, establish var as a variable in our current scope
      auto nv = Value::make(Value::Type::FUNCTION, node);
      assign_or_insert_variable(node->name, nv);

      return nv;
   }

   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_function_call(FunctionCall* node) {
   log("Visited Function Call: " + node->to_str(), LOG_VERBOSE);

   // make sure function exists
   auto var = find_variable(node->name);

   if (var == nullptr) {
      std::cout << "ERROR: function named  " << node->name << " doesn't exist" << std::endl;
   } else {
      // run function body by accepting body
      // TODO worry about scope

      std::vector<std::shared_ptr<Value>> args;

      for (auto& arg : node->arguments) {
         args.push_back(arg->accept(*this));
      }

      // if built in, run built in code:
      if (var->get_type() == Value::Type::BUILTINFUNCTION) {
         var->get_as_builtin_function()->function(args); 
      } else if (var->get_type() == Value::Type::FUNCTION) {
         // todo add args to scope
         var->get_as_function()->body->accept(*this);
      }

      if (node->destination != nullptr) {
         // we have a location to return to
         
         // first make sure we actually return something
         // then return it from this fn
         // assignment would call this
      }
   }

   return nullptr;
}

void Interpreter::print_variables() {
   log("VARIABLES:");
   auto p = current_scope;
   int num = 0;

   int prev_scope = log_scope;

   while (p != nullptr) {
      log_scope++;
      num++;
      log("SCOPE #" + num);
      log(p->to_str());
      p = p->get_parent();
   }

   log_scope = prev_scope;
}
