#include "ImpliedVolatility.h"
#include "BlackScholes.h"

#include <cmath>

double ImpliedVolatility::solveBisection(
    double marketPrice,
    OptionInput input,
    double low,
    double high,
    int maxIterations,
    double tolerance
) {
    double mid = 0.0;

    for (int i = 0; i < maxIterations; ++i) {
        mid = (low + high) / 2.0;
        input.sigma = mid;

        double modelPrice = BlackScholes::price(input);

        if (std::abs(modelPrice - marketPrice) < tolerance) {
            return mid;
        }

        if (modelPrice < marketPrice) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return mid;
}