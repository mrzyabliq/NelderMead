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

vector<double> X::operator+(const X& other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] += other.coordinates[i];
  }

  return valuable.coordinates;
}

vector<double> X::operator-(const X& other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] -= other.coordinates[i];
  }

  return valuable.coordinates;
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
    X centroid = computeCentroid();
    X reflected = reflection(centroid, symplex[0]);
    // symplex.erase(symplex.begin());
    // symplex.push_back(reflected);
    X best = symplex.back();

    if (reflected.value < best.value) {
      // Расширение
      X expanded = expansion(centroid, reflected);
      if (expanded.value < reflected.value) {
        symplex.erase(symplex.begin());
        symplex.push_back(expanded);
      } else {
        symplex.erase(symplex.begin());
        symplex.push_back(reflected);
      }
    } else if (reflected.value < symplex[symplex.size() - 2].value) {
        symplex.erase(symplex.begin());
        symplex.push_back(reflected);
    } else {
      // Сжатие
      X contracted = contraction(centroid, symplex[0]);
      if (contracted.value < symplex[0].value) {
        symplex.erase(symplex.begin());
        symplex.push_back(contracted);
      } else {
        reduction();
      }
    }
    Sort();
    if (abs(symplex.back().value - symplex[0].value) <= tolerance) break;
    i++;
  }
  return symplex[dims];
}

void NelderMead::startPoint() {
  // vector<double> init_point;
  // for (int i = 0; i < dims; i++) {
  //   init_point.push_back(0);
  // }
  // symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  // for (int i = 0; i < dims; i++) {
  //   vector<double> cur_var = init_point;
  //   cur_var[i] = 1;
  //   symplex.push_back({cur_var, function.calc(vectorToMap(cur_var))});
  // }
  vector<double> init_point(dims, 1.0);
  symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  for (size_t i = 0; i < dims; ++i) {
    vector<double> point = init_point;
    point[i] += 0.05;  // Добавляем 5% к i-й координате
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
    centroid.coordinates = centroid + symplex[i];
  }

  for (auto& val : centroid.coordinates) {
    val /= dims;
  }
  centroid.value = calcFunc(centroid);
  return centroid;
}

X NelderMead::reflection(X centroid, X worst_point) {
  X result;
  result.coordinates = (centroid - worst_point);
  result.coordinates = result + centroid;
  result.value = calcFunc(result);
  return result;
}
X NelderMead::expansion(const X& centroid, const X& reflected) {
  X expanded;
  expanded.coordinates.resize(dims);
  for (size_t i = 0; i < dims; ++i) {
    expanded.coordinates[i] =
        centroid.coordinates[i] +
        2 * (reflected.coordinates[i] - centroid.coordinates[i]);
  }
  expanded.value = calcFunc(expanded);
  return expanded;
}
X NelderMead::contraction(const X& centroid, const X& worst) {
  X contracted;
  contracted.coordinates.resize(dims);
  for (size_t i = 0; i < dims; ++i) {
      contracted.coordinates[i] = centroid.coordinates[i] + 
          0.5 * (worst.coordinates[i] - centroid.coordinates[i]);
  }
  contracted.value = calcFunc(contracted);
  return contracted;
}
void NelderMead::reduction() {
  const X& best = symplex[0];
  for (size_t i = 1; i < symplex.size(); ++i) {
      for (size_t j = 0; j < dims; ++j) {
          symplex[i].coordinates[j] = best.coordinates[j] + 
              0.5 * (symplex[i].coordinates[j] - best.coordinates[j]);
      }
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

void* CreateParser(const char* expr) {
        try {
            return new Parser(std::string(expr));
        } catch(...) {
            return nullptr;
        }
    }

double ParserCalc(void* parser, const char* variable_names[], double variable_values[], int count) {
        if (!parser) return NAN;
        
        Parser* p = static_cast<Parser*>(parser);
        std::unordered_map<std::string, double> vars;
        
        for (int i = 0; i < count; ++i) {
            vars[variable_names[i]] = variable_values[i];
        }
        
        return p->calc(vars);
    }

void DestroyParser(void* parser) {
        delete static_cast<Parser*>(parser);
    }

#ifdef __cplusplus
}
#endif
