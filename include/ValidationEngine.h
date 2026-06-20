#ifndef VALIDATION_ENGINE_H
#define VALIDATION_ENGINE_H

#include "OptionTypes.h"
#include <vector>

class ValidationEngine {
public:
    static ValidationResult validatePrediction(
        double predictedPrice,
        double actualPrice,
        double tolerancePercent
    );

    static bool lastNPredictionsPassed(
        const std::vector<ValidationResult>& history,
        int requiredPasses
    );

    static bool shouldAlert(
        const std::vector<ValidationResult>& history,
        int requiredPasses,
        const PredictionResult& prediction,
        double minimumScoreForAlert
    );
};

#endif