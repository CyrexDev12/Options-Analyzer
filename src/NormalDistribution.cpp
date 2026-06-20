#include "NormalDistribution.h"
#include <cmath>

constexpr double PI = 3.14159265358979323846;

double normalPDF(double x) {
    return (1.0 / std::sqrt(2.0 * PI)) * std::exp(-0.5 * x * x);
}

double normalCDF(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}