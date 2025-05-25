#include "Point.h"

#include <stdexcept>

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
  if (num == 0) throw std::runtime_error("Invalid dims");
  X valuable = *this;
  for (int i = 0; i < coordinates.size(); i++) {
    valuable.coordinates[i] /= num;
  }
  return valuable;
}