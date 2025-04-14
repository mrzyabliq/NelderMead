#include "NelderMead.h"

#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "pch.h"

using namespace std;


//Структура для хранения переменной

bool X::operator<(const X &other) const { return value < other.value; }

vector<double> X::operator+(const X &other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] += other.coordinates[i];
  }

  return valuable.coordinates;
}

vector<double> X::operator-(const X &other) const {
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] -= other.coordinates[i];
  }

  return valuable.coordinates;
}

//Класс решателя Нелдера-Мида


  NelderMead::NelderMead(string expression) {
    symplex = {};
    function = Parser(expression);
    dims = function.num_of_variables;
  }

  X NelderMead::Solver() {
    startPoint();
    Sort();
    int i = 0;
    while (i < 10000) {
      X reflected = reflection(symplex[0], computeCentroid());
      symplex.erase(symplex.begin());
      symplex.push_back(reflected);
      Sort();
      i++;
    }
    return symplex[dims];
  }

  void NelderMead::startPoint() {
    vector<double> init_point;
    for (int i = 0; i < dims; i++) {
      init_point.push_back(0);
    }
    symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
    for (int i = 0; i < dims; i++) {
      vector<double> cur_var = init_point;
      cur_var[i] = 1;
      symplex.push_back({cur_var, function.calc(vectorToMap(cur_var))});
    }
  }

  double NelderMead::calcFunc(X x) { return function.calc(vectorToMap(x.coordinates)); }

  unordered_map<string, double> NelderMead::vectorToMap(vector<double> coords) {
    unordered_map<string, double> variables;
    for (int i = 0; i < coords.size(); i++) {
      variables["x" + to_string(i)] = coords[i];
    }
    return variables;
  }

  void NelderMead::Sort() { sort(symplex.rbegin(), symplex.rend()); }
  X NelderMead::computeCentroid() {
    X centroid = {vector<double>(dims, 0), 0};
    for (int i = 1; i < symplex.size(); i++) {
      centroid.coordinates = centroid + symplex[i];
    }

    for (auto val : centroid.coordinates) {
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


// C-интерфейс для NelderMead
extern "C" {
  struct NelderMeadHandle {
      NelderMead* solver;
      NelderMeadHandle(const char* expr) : 
          solver(new NelderMead(expr)) {}
      ~NelderMeadHandle() { delete solver; }
  };

  NelderMeadHandle* CreateNelderMead(const char* expr) {
      return new NelderMeadHandle(expr);
  }

  void Solve(NelderMeadHandle* handle, double* output, int size) {
      if (!handle || !output) return;
      X result = handle->solver->Solver();
      if (result.coordinates.size() != size) return;
      std::copy(result.coordinates.begin(), result.coordinates.end(), output);
  }

  void DestroyNelderMead(NelderMeadHandle* handle) {
      delete handle;
  }
}