#include "../include/NelderMead.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "pch.h"

using namespace std;

// Структура для хранения переменной

bool X::operator<(const X& other) const { return value < other.value; }

X X::operator+(const X& other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] += other.coordinates[i];
  }

  return valuable;
}

X X::operator-(const X& other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] -= other.coordinates[i];
  }
  return valuable;
}
X X::operator*(int num) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] *= num;
  }
  return valuable;
}
X X::operator*(double num) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] *= num;
  }
  return valuable;
}
X X::operator/(double num) const {
  if (num == 0) throw runtime_error("Invalid dims");
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] /= num;
  }
  return valuable;
}

// Класс решателя Нелдера-Мида

NelderMead::NelderMead(string expression) {
  symplex = {};
  function = Parser(expression);
  dims = function.num_of_variables;
}

X NelderMead::Solver() {
  startPoint();
  Sort();
  int i = 0;
  while (i < 100000) {
    chooseBest();
    Sort();
    if (abs(symplex.back().value - symplex[0].value) <= tolerance) break;
    i++;
  }
  return symplex[dims];
}
void NelderMead::chooseBest() {
  X centroid = computeCentroid();
  X reflected = reflection(centroid, symplex[0]);
  X best = symplex.back();

  if (reflected < best) {
    X expanded = expansion(centroid, reflected);
    if (expanded < reflected) {
      removeBad(expanded);
    } else {
      removeBad(reflected);
    }
  } else if (reflected < symplex[symplex.size() - 2]) {
    removeBad(reflected);
  } else {
    X contracted = contraction(centroid, symplex[0]);
    if (contracted < symplex[0]) {
      removeBad(contracted);
    } else {
      reduction();
    }
  }
}
void NelderMead::removeBad(X point) {
  symplex.erase(symplex.begin());
  symplex.push_back(point);
}
void NelderMead::startPoint() {
  vector<double> init_point(dims, 1.0);
  symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  for (size_t i = 0; i < dims; ++i) {
    vector<double> point = init_point;
    point[i] += 0.05;
    symplex.push_back({point, function.calc(vectorToMap(point))});
  }
}

void NelderMead::startPoint(vector<double> init_point) {
  symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  for (size_t i = 0; i < dims; ++i) {
    vector<double> point = init_point;
    point[i] += 0.05 * point[i];
    symplex.push_back({point, function.calc(vectorToMap(point))});
  }
}

double NelderMead::calcFunc(X x) {
  return function.calc(vectorToMap(x.coordinates));
}

unordered_map<string, double> NelderMead::vectorToMap(vector<double> coords) {
  unordered_map<string, double> variables;
  for (int i = 0; i < coords.size(); i++) {
    variables["x" + to_string(i + 1)] = coords[i];
  }
  return variables;
}

void NelderMead::Sort() { sort(symplex.rbegin(), symplex.rend()); }
X NelderMead::computeCentroid() {
  X centroid = {vector<double>(dims, 0), 0};
  for (int i = 1; i < symplex.size(); i++) {
    centroid = centroid + symplex[i];
  }

  // for (auto& val : centroid.coordinates) {
  //   val /= dims;
  // }
  centroid = centroid / dims;
  centroid.value = calcFunc(centroid);
  return centroid;
}

X NelderMead::reflection(X centroid, X worst_point) {
  X result;
  result = centroid + (centroid - worst_point);
  result.value = calcFunc(result);
  return result;
}
X NelderMead::expansion(const X& centroid, const X& reflected) {
  // expanded.coordinates.resize(dims);
  // for (size_t i = 0; i < dims; ++i) {
  //   expanded.coordinates[i] =
  //       centroid.coordinates[i] +
  //       2 * (reflected.coordinates[i] - centroid.coordinates[i]);
  // }
  X expanded = centroid + (reflected - centroid) * 2;
  expanded.value = calcFunc(expanded);
  return expanded;
}
X NelderMead::contraction(const X& centroid, const X& worst) {
  X contracted = centroid + (worst - centroid) * 0.5;
  // contracted.coordinates.resize(dims);
  // for (size_t i = 0; i < dims; ++i) {
  //     contracted.coordinates[i] = centroid.coordinates[i] +
  //         0.5 * (worst.coordinates[i] - centroid.coordinates[i]);
  // }
  contracted.value = calcFunc(contracted);
  return contracted;
}
void NelderMead::reduction() {
  // const X& best = symplex[0];
  X best = symplex[0];
  for (size_t i = 1; i < symplex.size(); ++i) {
    // for (size_t j = 0; j < dims; ++j) {
    //     symplex[i].coordinates[j] = best.coordinates[j] +
    //         0.5 * (symplex[i].coordinates[j] - best.coordinates[j]);
    // }
    symplex[i] = best + (symplex[i] - best);
    symplex[i].value = calcFunc(symplex[i]);
  }
}

// C-интерфейс для NelderMead
#ifdef __cplusplus
extern "C" {
#endif
struct NelderMeadHandle {
  NelderMead* solver;
  NelderMeadHandle(const char* expr) : solver(new NelderMead(expr)) {}
  ~NelderMeadHandle() { delete solver; }
};

NelderMeadHandle* CreateNelderMead(const char* expr) {
  return new NelderMeadHandle(expr);
}

void Solve(NelderMeadHandle* handle, double* output) {
  if (!handle || !output) return;
  X result = handle->solver->Solver();
  std::copy(result.coordinates.begin(), result.coordinates.end(), output);
}

void DestroyNelderMead(NelderMeadHandle* handle) { delete handle; }

#ifdef __cplusplus
}
#endif
