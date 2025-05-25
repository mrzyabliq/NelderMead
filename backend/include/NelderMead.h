#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Parser.h"
#include "Symplex.h"
#ifdef _WIN32
#ifdef NELDERMEAD_EXPORTS
#define NELDERMEAD_API __declspec(dllexport)
#else
#define NELDERMEAD_API __declspec(dllimport)
#endif
#else
#define NELDERMEAD_API __attribute__((visibility("default")))
#endif

class NELDERMEAD_API NelderMead {
  Symplex symplex;
  std::string expression;

 public:
  NelderMead(std::string expression);
  X Solver(double init_tolerance, double alpha, double beta, double gamma,
           double sigma);
  X Solver(std::vector<double> init_point, double init_tolerance, double alpha,
           double beta, double gamma, double sigma);

  int getDims();
  int getIterations();
  std::vector<double> getHistory();

 private:
  X solverProcessing(double tolerance);
  bool condition(double tolerance);
  void chooseBest();
  void savePointsToLog(int iteration_num);

  void startLogFile();
  void finishLog();
  void saveHistory();
  void clearHistory();
  std::ofstream outfile;
  std::vector<double> history;
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
NELDERMEAD_API void SolveWithKoefs(NelderMeadHandle* handle, double tolerance,
                                   double alpha, double beta, double gamma,
                                   double sigma, double* output, double* value);
NELDERMEAD_API void SolveFull(NelderMeadHandle* handle, double* coordinates,
                              double* output, double* value);
NELDERMEAD_API void SolveFullKoefs(NelderMeadHandle* handle,
                                   double* coordinates, double tolerance,
                                   double alpha, double beta, double gamma,
                                   double sigma, double* output, double* value);
NELDERMEAD_API void DestroyNelderMead(NelderMeadHandle* handle);
NELDERMEAD_API void GetPointsForGraph(NelderMeadHandle* handle, double* output,
                                      int maxSize);
NELDERMEAD_API int getDims(NelderMeadHandle* handle);
NELDERMEAD_API int getIterations(NelderMeadHandle* handle);
#ifdef __cplusplus
}
#endif
