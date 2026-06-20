#ifndef PREDICTION_ENGINE_H
#define PREDICTION_ENGINE_H

#include "OptionTypes.h"

class PredictionEngine {
public:
    static PredictionResult makePrediction(
        const OptionInput& currentOption,
        const MarketSnapshot& market,
        double predictedStockPrice,
        double predictedIV,
        double predictionWindowMinutes,
        double slippageEstimate
    );

private:
    static double minutesToYears(double minutes);
};

#endif