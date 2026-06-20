#include "BlackScholes.h"
#include "NormalDistribution.h"

#include <cmath>
#include <algorithm>

double BlackScholes::d1(const OptionInput& input) {
    return (
        std::log(input.S / input.K)
        + (input.r - input.q + 0.5 * input.sigma * input.sigma) * input.T
    ) / (input.sigma * std::sqrt(input.T));
}

double BlackScholes::d2(const OptionInput& input) {
    return d1(input) - input.sigma * std::sqrt(input.T);
}

double BlackScholes::price(const OptionInput& input) {
    if (input.T <= 0.0) {
        if (input.type == OptionType::Call) {
            return std::max(input.S - input.K, 0.0);
        }

        return std::max(input.K - input.S, 0.0);
    }

    if (input.sigma <= 0.0) {
        if (input.type == OptionType::Call) {
            return std::max(input.S - input.K, 0.0);
        }

        return std::max(input.K - input.S, 0.0);
    }

    double D1 = d1(input);
    double D2 = d2(input);

    double dividendDiscount = std::exp(-input.q * input.T);
    double rateDiscount = std::exp(-input.r * input.T);

    if (input.type == OptionType::Call) {
        return input.S * dividendDiscount * normalCDF(D1)
             - input.K * rateDiscount * normalCDF(D2);
    }

    return input.K * rateDiscount * normalCDF(-D2)
         - input.S * dividendDiscount * normalCDF(-D1);
}

OptionGreeks BlackScholes::greeks(const OptionInput& input) {
    OptionGreeks output{};

    double D1 = d1(input);
    double D2 = d2(input);

    double dividendDiscount = std::exp(-input.q * input.T);
    double rateDiscount = std::exp(-input.r * input.T);

    if (input.type == OptionType::Call) {
        output.delta = dividendDiscount * normalCDF(D1);
    } else {
        output.delta = dividendDiscount * (normalCDF(D1) - 1.0);
    }

    output.gamma =
        dividendDiscount * normalPDF(D1)
        / (input.S * input.sigma * std::sqrt(input.T));

    output.vega =
        input.S * dividendDiscount * normalPDF(D1) * std::sqrt(input.T);

    double firstTerm =
        -(input.S * dividendDiscount * normalPDF(D1) * input.sigma)
        / (2.0 * std::sqrt(input.T));

    if (input.type == OptionType::Call) {
        output.theta =
            firstTerm
            - input.r * input.K * rateDiscount * normalCDF(D2)
            + input.q * input.S * dividendDiscount * normalCDF(D1);
    } else {
        output.theta =
            firstTerm
            + input.r * input.K * rateDiscount * normalCDF(-D2)
            - input.q * input.S * dividendDiscount * normalCDF(-D1);
    }

    // Convert theta from annual decay to daily decay.
    output.theta /= 365.0;

    // Convert vega to price change per 1% IV move.
    output.vega /= 100.0;

    return output;
}