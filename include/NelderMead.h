#pragma once
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
  std::vector<double> operator+(const X& other) const;
  std::vector<double> operator-(const X& other) const;
};

class NELDERMEAD_API NelderMead {
  std::vector<X> symplex;
  Parser function;
  int dims;

 public:
  NelderMead(std::string expression);
  X Solver();

 private:
  void startPoint();
  void Sort();
  X computeCentroid();
  X reflection(X centroid, X worst_point);
  X expansion(const X& centroid, const X& reflected);
  X contraction(const X& centroid, const X& worst);
  void reduction();
  double calcFunc(X x);
  std::unordered_map<std::string, double> vectorToMap(
  std::vector<double> coords);
  double tolerance = 0.000001;
};

// C-��������� ��� ������ � DLL
#ifdef __cplusplus
extern "C" {
#endif

// ������������ ��������� �� ������ NelderMead
typedef struct NelderMeadHandle NelderMeadHandle;

NELDERMEAD_API NelderMeadHandle* CreateNelderMead(const char* expr);
NELDERMEAD_API void Solve(NelderMeadHandle* handle, double* output);
NELDERMEAD_API void DestroyNelderMead(NelderMeadHandle* handle);

#ifdef __cplusplus
}
#endif
