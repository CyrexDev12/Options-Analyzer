#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "OptionTypes.h"

class BlackScholes {
public:
    static double price(const OptionInput& input);
    static OptionGreeks greeks(const OptionInput& input);

private:
    static double d1(const OptionInput& input);
    static double d2(const OptionInput& input);
};

#endif