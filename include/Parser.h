#pragma once
#include <string>
#include <unordered_map>
#include <vector>


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
  std::string expression;
};

class Parser {
  std::string expression;
  std::vector<Token> expression_parsed;

 public:
  Parser(std::string expression);
  Parser() = default;

  double calc(std::unordered_map<std::string, double> variables);
  int num_of_variables;

 private:
  void Parse();
  int parse_num(int cur_pos);
  int parse_expression(int cur_pos);
  int parse_arg(int cur_pos);
  void next_pos();

  double calc_exp_plus();
  double calc_exp_mult();
  double calc_exp_raise();
  double calc_end();

  std::unordered_map<std::string, double> var_nums;
  int pos = 0;
};
