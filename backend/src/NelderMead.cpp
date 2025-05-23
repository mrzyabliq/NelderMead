#include "../include/NelderMead.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

X NelderMead::Solver(double tolerance = 0.000001, double alpha = 1,
                     double beta = 0.5, double gamma = 2, double sigma = 1) {
  symplex = {};
  startPoint();
  return solverProcessing(tolerance, alpha, beta, gamma, sigma);
}
X NelderMead::Solver(vector<double> init_point, double tolerance = 0.000001,
                     double alpha = 1, double beta = 0.5, double gamma = 2,
                     double sigma = 1) {
  symplex = {};
  startPoint(init_point);
  return solverProcessing(tolerance, alpha, beta, gamma, sigma);
}
X NelderMead::solverProcessing(double tolerance = 0.000001, double alpha = 1,
                               double beta = 0.5, double gamma = 2,
                               double sigma = 1) {
  initKoefs(alpha, beta, gamma, sigma);
  clearHistory();
  Sort();
  startLogFile();
  int i = 0;
  savePointsToLog(i);
  saveHistory();
  while (i < 100000) {
    chooseBest();
    Sort();
    saveHistory();
    if (abs(symplex.back().value - symplex[0].value) <= tolerance) break;
    i++;
    savePointsToLog(i);
  }
  finishLog();
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
  if (init_point.size() != dims) throw runtime_error("Incorrect init point");
  symplex.push_back({init_point, function.calc(vectorToMap(init_point))});
  for (size_t i = 0; i < dims; ++i) {
    vector<double> point = init_point;
    point[i] += 0.05 * (point[i] == 0 ? 1 : point[i]);
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
int NelderMead::getDims() { return dims; }

void NelderMead::Sort() { sort(symplex.rbegin(), symplex.rend()); }
X NelderMead::computeCentroid() {
  X centroid = {vector<double>(dims, 0), 0};
  for (int i = 1; i < symplex.size(); i++) {
    centroid = centroid + symplex[i];
  }
  centroid = centroid / dims;
  centroid.value = calcFunc(centroid);
  return centroid;
}

X NelderMead::reflection(X centroid, X worst_point) {
  X result;
  result = centroid + (centroid - worst_point) * alpha;
  result.value = calcFunc(result);
  return result;
}
X NelderMead::expansion(const X& centroid, const X& reflected) {
  X expanded = centroid + (reflected - centroid) * gamma;
  expanded.value = calcFunc(expanded);
  return expanded;
}
X NelderMead::contraction(const X& centroid, const X& worst) {
  X contracted = centroid + (worst - centroid) * beta;
  contracted.value = calcFunc(contracted);
  return contracted;
}
void NelderMead::reduction() {
  X best = symplex[0];
  for (size_t i = 1; i < symplex.size(); ++i) {
    symplex[i] = best + (symplex[i] - best) * sigma;
    symplex[i].value = calcFunc(symplex[i]);
  }
}
void NelderMead::startLogFile() {
  outfile.open("logfile.txt", ios::out | ios::trunc);
  outfile << "-------Started method solving-------" << endl;
  outfile << "Calculated dims: " << dims << endl;
}
void NelderMead::finishLog() {
  outfile << "-------Finished solving-------" << endl;
  outfile << "Calculated solution:" << endl << "(";
  for (int j = 0; j < symplex[dims].coordinates.size(); j++) {
    outfile << symplex[dims].coordinates[j] << "; ";
  }
  outfile << ") " << endl;
  outfile << "Value: " << symplex[dims].value;
  outfile.close();
}
void NelderMead::savePointsToLog(int iteration_num) {
  outfile << iteration_num << " iteration" << endl;
  for (auto i : symplex) {
    outfile << "    (";
    for (int j = 0; j < i.coordinates.size(); j++) {
      outfile << i.coordinates[j] << "; ";
    }
    outfile << ") ";
    outfile << "Calculated value: " << i.value << endl;
  }
}
void NelderMead::saveHistory() { history.push_back(symplex[dims].value); }
vector<double> NelderMead::getHistory() {
  if (history.size() == 0)
    throw runtime_error("History is empty");
  else
    return history;
}
void NelderMead::clearHistory() { history.clear(); }
vector<X> NelderMead::pointsForGraph() {
  X extremum = symplex[dims];
  vector<X> points_for_graph;
  if (dims != 1) throw runtime_error("Can't build graphic (too much dims)");
  for (int i = -5; i <= 5; i++) {
    X cur_point = extremum;
    cur_point.coordinates[0] += i;
    cur_point.value = calcFunc(cur_point);
    points_for_graph.push_back(cur_point);
  }
  return points_for_graph;
}
void NelderMead::initKoefs(double alpha, double beta, double gamma,
                           double sigma) {
  this->alpha = alpha;
  this->beta = beta;
  this->gamma = gamma;
  this->sigma = sigma;
}
int NelderMead::getIterations() { return history.size(); }

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
int getDims(NelderMeadHandle* handle) { return handle->solver->getDims(); }
void SolveBasic(NelderMeadHandle* handle, double* output) {
  if (!handle || !output) return;
  X result = handle->solver->Solver();
  copy(result.coordinates.begin(), result.coordinates.end(), output);
}
void SolveWithValue(NelderMeadHandle* handle, double* output, double* value) {
  if (!handle || !output) return;
  X result = handle->solver->Solver();
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveInit(NelderMeadHandle* handle, double* coordinates, double* output) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result = handle->solver->Solver(input);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
}
void SolveFull(NelderMeadHandle* handle, double* coordinates, double* output,
               double* value) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result = handle->solver->Solver(input);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveWithKoefs(NelderMeadHandle* handle, double tolerance, double alpha,
                    double beta, double gamma, double sigma, double* output,
                    double* value) {
  if (!handle || !output) return;
  X result = handle->solver->Solver(tolerance, alpha, beta, gamma, sigma);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void SolveFullKoefs(NelderMeadHandle* handle, double* coordinates,
                    double tolerance, double alpha, double beta, double gamma,
                    double sigma, double* output, double* value) {
  if (!handle || !output) return;
  vector<double> input;
  for (int i = 0; i < handle->solver->getDims(); i++) {
    input.push_back(coordinates[i]);
  }
  X result =
      handle->solver->Solver(input, tolerance, alpha, beta, gamma, sigma);
  copy(result.coordinates.begin(), result.coordinates.end(), output);
  *value = result.value;
}
void GetPointsForGraph(NelderMeadHandle* handle, double* output, int maxSize) {
  if (!handle || !output) return;

  vector<double> history = handle->solver->getHistory();
  copy(history.begin(), history.end(), output);
}
int getIterations(NelderMeadHandle* handle) {
  return handle->solver->getIterations();
}

void DestroyNelderMead(NelderMeadHandle* handle) { delete handle; }

#ifdef __cplusplus
}
#endif
