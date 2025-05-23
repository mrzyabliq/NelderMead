#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#ifdef _WIN32
#ifdef NELDERMEAD_EXPORTS
#define NELDERMEAD_API __declspec(dllexport)
#else
#define NELDERMEAD_API __declspec(dllimport)
#endif
#else
#define NELDERMEAD_API __attribute__((visibility("default")))
#endif

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
  int num_of_variables=0;

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
  std::set<int> variables_set;
  std::unordered_map<std::string, double> var_nums;
  int pos = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ParserHandle ParserHandle;

NELDERMEAD_API ParserHandle* CreateParser(const char* expr);
NELDERMEAD_API double ParserCalc(ParserHandle* handle, const char* variable_names[], double variable_values[], int count);
NELDERMEAD_API void DestroyParser(ParserHandle* handle);


#ifdef __cplusplus
}
#endif
