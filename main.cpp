#include "OptionTypes.h"
#include "BlackScholes.h"
#include "ImpliedVolatility.h"
#include "PredictionEngine.h"
#include "ValidationEngine.h"
#include "ConsoleUI.h"
#include "MarketDataClient.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

int main() {
    std::cout << std::fixed << std::setprecision(4);

    const char* apiKeyFromEnvironment = std::getenv("ALPHA_VANTAGE_API_KEY");

    if (apiKeyFromEnvironment == nullptr) {
        std::cerr << "Missing ALPHA_VANTAGE_API_KEY environment variable.\n";
        std::cerr << "Create a free Alpha Vantage API key, then set it before running.\n";
        return 1;
    }

    MarketDataClient marketDataClient(apiKeyFromEnvironment);

    std::vector<ValidationResult> validationHistory;

    const int requiredPasses = 3;
    const double tolerancePercent = 0.05;
    const double minimumScoreForAlert = 2.0;
    const double slippageEstimate = 0.02;

    bool running = true;

    while (running) {
        try {
            std::cout << "\n========================================\n";
            std::cout << "BLACK-SCHOLES OPTIONS ALERT PROTOTYPE\n";
            std::cout << "========================================\n";

            std::string ticker = ConsoleUI::readTicker();

            std::cout << "\nFetching stock price, risk-free rate, and dividend yield...\n";

            FetchedMarketData fetchedData =
                marketDataClient.fetchForTicker(ticker);

            ConsoleUI::printFetchedMarketData(fetchedData);

            OptionInput option =
                ConsoleUI::readOptionInputFromFetchedData(fetchedData);

            MarketSnapshot market =
                ConsoleUI::readMarketSnapshot(option);

            double predictionWindowMinutes =
                ConsoleUI::readPredictionWindowMinutes();

            double predictedStockPrice =
                ConsoleUI::readPredictedStockPrice(predictionWindowMinutes);

            double predictedIV =
                ConsoleUI::readPredictedIV(predictionWindowMinutes);

            ConsoleUI::printOptionSummary(option);

            double currentBSPrice = BlackScholes::price(option);
            OptionGreeks greeks = BlackScholes::greeks(option);

            std::cout << "\nCurrent Black-Scholes Price: $" << currentBSPrice << "\n";
            std::cout << "Current Market Mark:         $" << market.optionMark << "\n";
            std::cout << "Contract Estimate BS:        $" << currentBSPrice * 100.0 << "\n";
            std::cout << "Contract Estimate Market:    $" << market.optionMark * 100.0 << "\n";

            ConsoleUI::printGreeks(greeks);

            double marketImpliedVol =
                ImpliedVolatility::solveBisection(market.optionMark, option);

            std::cout << "\nMarket-Implied IV From Mark Price: "
                      << marketImpliedVol * 100.0
                      << "%\n";

            PredictionResult prediction =
                PredictionEngine::makePrediction(
                    option,
                    market,
                    predictedStockPrice,
                    predictedIV,
                    predictionWindowMinutes,
                    slippageEstimate
                );

            ConsoleUI::printPrediction(prediction);

            double actualFuturePrice = ConsoleUI::readActualFuturePrice();

            if (actualFuturePrice >= 0.0) {
                ValidationResult validation =
                    ValidationEngine::validatePrediction(
                        prediction.predictedOptionPrice,
                        actualFuturePrice,
                        tolerancePercent
                    );

                validationHistory.push_back(validation);
                ConsoleUI::printValidation(validation);
            }

            bool validationPassed =
                ValidationEngine::lastNPredictionsPassed(
                    validationHistory,
                    requiredPasses
                );

            bool alert =
                ValidationEngine::shouldAlert(
                    validationHistory,
                    requiredPasses,
                    prediction,
                    minimumScoreForAlert
                );

            std::cout << "\n--- ALERT CHECK ---\n";
            std::cout << "Last " << requiredPasses << " predictions passed: "
                      << (validationPassed ? "YES" : "NO") << "\n";

            std::cout << "Signal score >= " << minimumScoreForAlert << ": "
                      << (prediction.score >= minimumScoreForAlert ? "YES" : "NO")
                      << "\n";

            if (alert) {
                std::cout << "\nTRADE ALERT: Bullish option setup detected.\n";
                std::cout << "Predicted option price: $"
                          << prediction.predictedOptionPrice << "\n";
                std::cout << "Current market mark:    $"
                          << prediction.currentMarketMark << "\n";
                std::cout << "Expected move:          $"
                          << prediction.expectedMove << "\n";
                std::cout << "Estimated contract move: $"
                          << prediction.expectedMove * 100.0 << "\n";
            } else {
                std::cout << "\nNo trade alert generated.\n";
            }

            std::cout << "\nContinue? Y/N: ";
            char choice;
            std::cin >> choice;

            if (choice == 'N' || choice == 'n') {
                running = false;
            }
        } catch (const std::exception& ex) {
            std::cerr << "\nERROR: " << ex.what() << "\n";
            std::cerr << "The API may be rate-limited, unavailable, or the ticker may be invalid.\n";

            std::cout << "\nContinue? Y/N: ";
            char choice;
            std::cin >> choice;

            if (choice == 'N' || choice == 'n') {
                running = false;
            }
        }
    }

    std::cout << "\nProgram ended.\n";
    return 0;
}