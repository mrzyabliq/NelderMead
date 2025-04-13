#include "Parser.h"

#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "pch.h"
using namespace std;
enum class Typess {
    Number,
    Variable,
    Plus,
    Minus,
    Multiply,
    Divide,
    LParenthesis,
    RParenthesis,
    Function,
    Argument,
    Raise
  };
  struct Token {
    Typess type;
    double value;
    int index;
    string expression;
  };
  class Parser {
   public:
    Parser(string expression) {
      this->expression = expression;
      Parse();
    }
    Parser() {}
  
    double calc(unordered_map<string, double> variables) {
      pos = 0;
      if (variables.size() != num_of_variables)
        throw runtime_error("Invalid input");
      var_nums = variables;
      double result = calc_exp_plus();
  
      return result;
    }
    string expression;
    vector<Token> expression_parsed;
    int num_of_variables = expression_parsed.size();
  
   private:
    unordered_map<string, double> var_nums;
    int pos;
    void Parse() {
      int cur_pos = 0;
      while (cur_pos < this->expression.size()) {
        char c = expression[cur_pos];
        if (c == ' ')
          cur_pos++;
        else if (isdigit(c))
          cur_pos = parse_num(cur_pos);
        else if (isalpha(c))
          cur_pos = parse_expression(cur_pos);
        else {
          switch (c) {
            case '+':
              expression_parsed.push_back({Typess::Plus});
              break;
            case '-':
              expression_parsed.push_back({Typess::Minus});
              break;
            case '*':
              expression_parsed.push_back({Typess::Multiply});
              break;
            case '/':
              expression_parsed.push_back({Typess::Divide});
              break;
            case '^':
              expression_parsed.push_back({Typess::Raise});
              break;
            case '(':
              expression_parsed.push_back({Typess::LParenthesis});
              break;
            case ')':
              expression_parsed.push_back({Typess::RParenthesis});
              break;
            default:
              throw runtime_error("Invalid character");
          }
          cur_pos++;
        }
      }
    }
    void next_pos() { pos = pos < expression_parsed.size() - 1 ? pos + 1 : pos; }
    double calc_exp_plus() {
      double res = calc_exp_mult();
      while (expression_parsed[pos].type == Typess::Plus ||
             expression_parsed[pos].type == Typess::Minus) {
        next_pos();
        auto op_type = expression_parsed[pos - 1].type;
        double pod_res = calc_exp_mult();
        if (op_type == Typess::Plus)
          res += pod_res;
        else if (op_type == Typess::Minus)
          res -= pod_res;
        if (pos >= expression_parsed.size()) {
          break;
        }
      }
      return res;
    }
    double calc_exp_mult() {
      double res = calc_exp_raise();
      while (expression_parsed[pos].type == Typess::Multiply ||
             expression_parsed[pos].type == Typess::Divide) {
        next_pos();
        auto op_type = expression_parsed[pos - 1].type;
        double pod_res = calc_exp_raise();
        if (op_type == Typess::Multiply)
          res *= pod_res;
        else if (op_type == Typess::Divide)
          res = pod_res != 0 ? (double)res / pod_res
                             : throw runtime_error("Division by zero");
      }
      return res;
    }
    double calc_exp_raise() {
      double res = calc_end();
      while (expression_parsed[pos].type == Typess::Raise) {
        next_pos();
        double pod_res = calc_end();
        res = std::pow(res, pod_res);
      }
      return res;
    }
    double calc_end() {
      if (expression_parsed[pos].type == Typess::Number) {
        double vlaue = expression_parsed[pos].value;
        next_pos();
        return vlaue;
      }
      if (expression_parsed[pos].type == Typess::Function) {
        string func_name = expression_parsed[pos].expression;
        next_pos();
        double arg = calc_end();
  
        if (func_name == "sin") return std::sin(arg);
        if (func_name == "cos") return std::cos(arg);
        if (func_name == "tan") return std::tan(arg);
        if (func_name == "exp") return std::exp(arg);
        if (func_name == "abs") return std::abs(arg);
      }
      if (expression_parsed[pos].type == Typess::Variable) {
        double value = var_nums["x" + to_string(expression_parsed[pos].index)];
        next_pos();
        return value;
      }
      if (expression_parsed[pos].type == Typess::LParenthesis) {
        next_pos();
        double res = calc_exp_plus();
        if (expression_parsed[pos].type != Typess::RParenthesis) {
          throw runtime_error("Missing closing parenthesis");
        }
        next_pos();
        return res;
      }
      if (expression_parsed[pos].type == Typess::Minus) {
        next_pos();
        return -calc_exp_raise();
      }
      throw runtime_error("Unexpected token");
    }
    int parse_num(int cur_pos) {
      double num = expression[cur_pos] - '0';
      bool dot = false;
      int nums_after_dot = 1;
      cur_pos++;
      while (cur_pos < expression.size() &&
             (isdigit(expression[cur_pos]) || expression[cur_pos] == '.')) {
        if (expression[cur_pos] == '.') {
          if (dot) {
            throw runtime_error("Invalid number");
          }
  
          dot = true;
        } else {
          num *= 10;
          num += expression[cur_pos] - '0';
          if (dot) {
            nums_after_dot *= 10;
          }
        }
        cur_pos++;
      }
      num = (double)num / nums_after_dot;
      expression_parsed.push_back({Typess::Number, num});
      return cur_pos;
    }
    int parse_expression(int cur_pos) {
      int start_pos = cur_pos;
      if (expression[cur_pos] == 'x') {
        int num = 0;
        cur_pos++;
        while (cur_pos < expression.size() && (isdigit(expression[cur_pos]))) {
          num *= 10;
          num += expression[cur_pos] - '0';
          cur_pos++;
        }
        num_of_variables++;
        expression_parsed.push_back({Typess::Variable, 0.0, num});
        return cur_pos;
      }
      while (cur_pos < expression.size() && isalnum(expression[cur_pos]))
        cur_pos++;
      string name = expression.substr(start_pos, cur_pos - start_pos);
      if (name == "sin" || name == "cos" || name == "tan" || name == "cotan" ||
          name == "exp" || name == "abs")
        expression_parsed.push_back({Typess::Function, 0.0, 0, name});
  
      return cur_pos;
    }
    int parse_arg(int cur_pos) {
      while (cur_pos < expression.size() &&
             (expression[cur_pos] == '(' || expression[cur_pos] == ' '))
        cur_pos++;
      if (cur_pos == expression.size())
        throw runtime_error("Ivalid argument of function");
      int start_pos = cur_pos;
      while (cur_pos < expression.size() && expression[cur_pos] != ')') cur_pos++;
      if (cur_pos == expression.size()) throw runtime_error("Invalid argument");
      string sub_expression = expression.substr(start_pos, cur_pos - start_pos);
      expression_parsed.push_back({Typess::Argument, 0.0, 0, sub_expression});
      return cur_pos;
    }
  };
  
  struct ParserHandle {
    std::unordered_map<std::string, double> variables;
    Parser parser;
  
    ParserHandle(const char* expr) : parser(expr) {}
  };
  
  extern "C" {
  ParserHandle* CreateParser(const char* expression) {
    return new ParserHandle(expression);
  }
  
  void SetVariable(ParserHandle* handle, const char* name, double value) {
    if (handle) {
      handle->variables[name] = value;
    }
  }
  
  double Evaluate(ParserHandle* handle) {
    if (!handle) return NAN;
    return handle->parser.calc(handle->variables);
  }
  
  void DestroyParser(ParserHandle* handle) { delete handle; }
  }