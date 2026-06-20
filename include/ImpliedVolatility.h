#ifndef IMPLIED_VOLATILITY_H
#define IMPLIED_VOLATILITY_H

#include "OptionTypes.h"

class ImpliedVolatility {
public:
    static double solveBisection(
        double marketPrice,
        OptionInput input,
        double low = 0.0001,
        double high = 5.0,
        int maxIterations = 100,
        double tolerance = 1e-6
    );
};

#endif