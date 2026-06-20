#include "PredictionEngine.h"
#include "BlackScholes.h"

#include <algorithm>

double PredictionEngine::minutesToYears(double minutes) {
    return minutes / (365.0 * 24.0 * 60.0);
}

PredictionResult PredictionEngine::makePrediction(
    const OptionInput& currentOption,
    const MarketSnapshot& market,
    double predictedStockPrice,
    double predictedIV,
    double predictionWindowMinutes,
    double slippageEstimate
) {
    PredictionResult result{};

    result.currentBSPrice = BlackScholes::price(currentOption);
    result.currentMarketMark = market.optionMark;
    result.predictedStockPrice = predictedStockPrice;
    result.predictedIV = predictedIV;
    result.spread = market.optionAsk - market.optionBid;

    OptionInput futureOption = currentOption;
    futureOption.S = predictedStockPrice;
    futureOption.sigma = predictedIV;
    futureOption.T = std::max(
        currentOption.T - minutesToYears(predictionWindowMinutes),
        0.0
    );

    result.predictedOptionPrice = BlackScholes::price(futureOption);
    result.expectedMove = result.predictedOptionPrice - result.currentMarketMark;

    if (result.currentMarketMark > 0.0) {
        result.expectedMovePercent = result.expectedMove / result.currentMarketMark;
    } else {
        result.expectedMovePercent = 0.0;
    }

    double tradingCostEstimate = result.spread + slippageEstimate;

    if (tradingCostEstimate > 0.0) {
        result.score = result.expectedMove / tradingCostEstimate;
    } else {
        result.score = 0.0;
    }

    return result;
}