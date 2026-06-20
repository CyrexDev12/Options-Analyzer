#ifndef OPTION_TYPES_H
#define OPTION_TYPES_H

#include <vector>

enum class OptionType {
    Call,
    Put
};

struct OptionInput {
    double S;       // Underlying stock price
    double K;       // Strike price
    double T;       // Time to expiration in years
    double r;       // Risk-free rate as decimal, example 0.037
    double q;       // Dividend yield as decimal, example 0.01
    double sigma;   // Implied volatility as decimal, example 0.6982
    OptionType type;
};

struct OptionGreeks {
    double delta;
    double gamma;
    double theta;   // Per day
    double vega;    // Per 1% IV move
};

struct MarketSnapshot {
    double stockPrice;
    double optionBid;
    double optionAsk;
    double optionMark;
    double impliedVolatility;
};

struct PredictionResult {
    double currentBSPrice;
    double currentMarketMark;
    double predictedStockPrice;
    double predictedIV;
    double predictedOptionPrice;
    double expectedMove;
    double expectedMovePercent;
    double spread;
    double score;
};

struct ValidationResult {
    double predictedPrice;
    double actualPrice;
    double absoluteError;
    double percentError;
    bool passed;
};

#endif