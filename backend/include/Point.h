#include <vector>
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