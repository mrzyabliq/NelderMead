#include "Symplex.h"

#include <algorithm>

using namespace std;

Symplex::Symplex(string expression, Koefs koef)
    : function(expression),
      dims(function.num_of_variables),
      koefs(koef),
      symplex(initSymplex()) {};
Symplex::Symplex(vector<double> init_point, string expression,
                 Koefs koef)
    : start_point(init_point),
      function(expression),
      dims(function.num_of_variables),
      symplex(initSymplex()) {};
void Symplex::sort() { std::sort(symplex.rbegin(), symplex.rend()); }
vector<X> Symplex::getSymplex() {return symplex;}
X Symplex::getTop(int ind) {return symplex[dims - ind];}
X Symplex::getBad() {return symplex[0];}
int Symplex::getDims() {return dims;}
vector<X> Symplex::initSymplex() {
  vector<X> init_symplex = {};
  vector<double> init_point(dims, 1.0);
  if (start_point.size() > 0)
    init_point = start_point;
  else
    init_point.assign(dims, 1.0);

  init_symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  for (size_t i = 0; i < dims; ++i) {
    vector<double> point = init_point;
    point[i] += 0.05 * (point[i] == 0 ? 1 : point[i]);
    init_symplex.push_back({point, function.calc(vectorToMap(point))});
  }
  return init_symplex;
}
unordered_map<string, double> Symplex::vectorToMap(vector<double> coords) {
  unordered_map<string, double> variables;
  for (int i = 0; i < coords.size(); i++) {
    variables["x" + to_string(i + 1)] = coords[i];
  }
  return variables;
}
X Symplex::computeCentroid() {
  X centroid = {vector<double>(dims, 0), 0};
  for (int i = 1; i < symplex.size(); i++) {
    centroid = centroid + symplex[i];
  }
  centroid = centroid / dims;
  centroid.value = calcFunc(centroid);
  return centroid;
}
double Symplex::calcFunc(X x) {
  return function.calc(vectorToMap(x.coordinates));
}
void Symplex::removeBad(X point) {
  symplex.erase(symplex.begin());
  symplex.push_back(point);
}
X Symplex::reflection(X centroid, X worst_point) {
  X result;
  result = centroid + (centroid - worst_point) * (koefs.alpha);
  result.value = calcFunc(result);
  return result;
}
X Symplex::expansion(const X& centroid, const X& reflected) {
  X expanded = centroid + (reflected - centroid) * koefs.gamma;
  expanded.value = calcFunc(expanded);
  return expanded;
}
X Symplex::contraction(const X& centroid, const X& worst) {
  X contracted = centroid + (worst - centroid) * koefs.beta;
  contracted.value = calcFunc(contracted);
  return contracted;
}
void Symplex::reduction() {
  X best = symplex[0];
  for (size_t i = 1; i < symplex.size(); ++i) {
    symplex[i] = best + (symplex[i] - best) * koefs.sigma;
    symplex[i].value = calcFunc(symplex[i]);
  }
}