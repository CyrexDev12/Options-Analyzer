#include "ValidationEngine.h"

#include <cmath>
#include <limits>

ValidationResult ValidationEngine::validatePrediction(
    double predictedPrice,
    double actualPrice,
    double tolerancePercent
) {
    ValidationResult result{};

    result.predictedPrice = predictedPrice;
    result.actualPrice = actualPrice;
    result.absoluteError = std::abs(predictedPrice - actualPrice);

    if (actualPrice > 0.0) {
        result.percentError = result.absoluteError / actualPrice;
    } else {
        result.percentError = std::numeric_limits<double>::infinity();
    }

    result.passed = result.percentError <= tolerancePercent;

    return result;
}

bool ValidationEngine::lastNPredictionsPassed(
    const std::vector<ValidationResult>& history,
    int requiredPasses
) {
    if (history.size() < static_cast<size_t>(requiredPasses)) {
        return false;
    }

    for (int i = 0; i < requiredPasses; ++i) {
        int index = static_cast<int>(history.size()) - 1 - i;

        if (!history[index].passed) {
            return false;
        }
    }

    return true;
}

bool ValidationEngine::shouldAlert(
    const std::vector<ValidationResult>& history,
    int requiredPasses,
    const PredictionResult& prediction,
    double minimumScoreForAlert
) {
    bool passedValidation = lastNPredictionsPassed(history, requiredPasses);

    bool strongEdge =
        prediction.score >= minimumScoreForAlert &&
        prediction.expectedMove > 0.0;

    return passedValidation && strongEdge;
}