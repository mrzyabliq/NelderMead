#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Parser.h"

#ifdef _WIN32
#ifdef NELDERMEAD_EXPORTS
#define NELDERMEAD_API __declspec(dllexport)
#else
#define NELDERMEAD_API __declspec(dllimport)
#endif
#else
#define NELDERMEAD_API __attribute__((visibility("default")))
#endif

struct X {
  std::vector<double> coordinates;
  double value;

  bool operator<(const X& other) const;
  X operator+(const X& other) const;
  X operator-(const X& other) const;
  X operator*(int num) const;
  X operator*(double num) const;
  X operator/(double num) const;
};

class NELDERMEAD_API NelderMead {
  std::vector<X> symplex;
  Parser function;
  int dims;

 public:
  NelderMead(std::string expression);
  X Solver(double tolerance, double alpha, double beta, double gamma, double sigma);
  X Solver(std::vector<double> init_point, double tolerance, double alpha, double beta, double gamma, double sigma);
  int getDims();
  std::vector<X> pointsForGraph();
  std::vector<double> getHistory();

 private:
  void startPoint();
  void startPoint(std::vector<double> init_point);
  void Sort();
  void reduction();
  void removeBad(X point);
  void chooseBest();
  void savePointsToLog(int iteration_num);
  void startLogFile();
  void finishLog();
  void saveHistory();
  void clearHistory();
  void initKoefs(double alpha, double beta, double gamma, double sigma);
  X computeCentroid();
  X reflection(X centroid, X worst_point);
  X expansion(const X& centroid, const X& reflected);
  X contraction(const X& centroid, const X& worst);
  double calcFunc(X x);
  std::unordered_map<std::string, double> vectorToMap(
      std::vector<double> coords);
  std::ofstream outfile;
  double tolerance;
  std::vector<double> history;
  double alpha;
  double beta;
  double gamma;
  double sigma;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NelderMeadHandle NelderMeadHandle;

NELDERMEAD_API NelderMeadHandle* CreateNelderMead(const char* expr);
NELDERMEAD_API void SolveBasic(NelderMeadHandle* handle, double* output);
NELDERMEAD_API void SolveWithValue(NelderMeadHandle* handle, double* output,
                                   double* value);
NELDERMEAD_API void SolveInit(NelderMeadHandle* handle, double* coordinates,
                              double* output);
NELDERMEAD_API void SolveWithKoefs(NelderMeadHandle* handle, double alpha, double beta,
                    double gamma, double sigma, double* output, double* value);
NELDERMEAD_API void SolveFull(NelderMeadHandle* handle, double* coordinates,
                              double* output, double* value);
NELDERMEAD_API void SolveFullKoefs(NelderMeadHandle* handle, double* coordinates, double alpha,
                    double beta, double gamma, double sigma, double* output,
                    double* value);
NELDERMEAD_API void DestroyNelderMead(NelderMeadHandle* handle);
NELDERMEAD_API void GetPointsForGraph(NelderMeadHandle* handle, double* output,
                                      int maxSize);
NELDERMEAD_API int getDims(NelderMeadHandle* handle);
#ifdef __cplusplus
}
#endif
