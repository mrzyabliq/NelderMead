#include <string>
#include <unordered_map>
#include <vector>

#include "Parser.h"
#include "Point.h"

struct Koefs {
  double alpha;
  double beta;
  double gamma;
  double sigma;
};
// Класс симплекса
class Symplex {
 public:
  Symplex() {};
  Symplex(std::string expression, Koefs koef);
  Symplex(std::vector<double> init_point, std::string expression, Koefs koef);
  void sort();
  std::vector<X> getSymplex();
  X getTop(int ind = 0);
  X getBad();
  int getDims();
  X computeCentroid();
  void removeBad(X point);
  X reflection(X centroid, X worst_point);
  X expansion(const X& centroid, const X& reflected);
  X contraction(const X& centroid, const X& worst);
  void reduction();

 private:
  Parser function;
  int dims;
  Koefs koefs;
  std::vector<double> start_point;
  std::vector<X> symplex;

  std::vector<X> initSymplex();
  std::unordered_map<std::string, double> vectorToMap(
      std::vector<double> coords);
  double calcFunc(X x);
};