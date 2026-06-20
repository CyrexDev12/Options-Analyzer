#include "ConsoleUI.h"

#include <iostream>
#include <iomanip>
#include <string>

std::string ConsoleUI::readTicker() {
    std::string ticker;

    std::cout << "\nEnter stock ticker, example AAPL, SPY, RBLX: ";
    std::cin >> ticker;

    return ticker;
}

OptionType ConsoleUI::readOptionType() {
    char typeChar;

    std::cout << "Option type, C for call or P for put: ";
    std::cin >> typeChar;

    if (typeChar == 'P' || typeChar == 'p') {
        return OptionType::Put;
    }

    return OptionType::Call;
}

OptionInput ConsoleUI::readOptionInputFromFetchedData(
    const FetchedMarketData& fetchedData
) {
    OptionInput option{};

    option.S = fetchedData.stockPrice;
    option.r = fetchedData.riskFreeRate;
    option.q = fetchedData.dividendYield;

    std::cout << "\nEnter strike price: ";
    std::cin >> option.K;

    double daysToExpiration;
    std::cout << "Enter calendar days to expiration: ";
    std::cin >> daysToExpiration;
    option.T = daysToExpiration / 365.0;

    double ivPercent;
    std::cout << "Enter implied volatility %, example 69.82: ";
    std::cin >> ivPercent;
    option.sigma = ivPercent / 100.0;

    option.type = readOptionType();

    return option;
}

MarketSnapshot ConsoleUI::readMarketSnapshot(const OptionInput& option) {
    MarketSnapshot market{};

    market.stockPrice = option.S;
    market.impliedVolatility = option.sigma;

    std::cout << "\nEnter option bid price: ";
    std::cin >> market.optionBid;

    std::cout << "Enter option ask price: ";
    std::cin >> market.optionAsk;

    market.optionMark = (market.optionBid + market.optionAsk) / 2.0;

    return market;
}

double ConsoleUI::readPredictionWindowMinutes() {
    double minutes;

    std::cout << "\nPrediction window in minutes, example 5 or 10: ";
    std::cin >> minutes;

    return minutes;
}

double ConsoleUI::readPredictedStockPrice(double predictionWindowMinutes) {
    double predictedStockPrice;

    std::cout << "Enter predicted stock price after "
              << predictionWindowMinutes
              << " minutes: ";

    std::cin >> predictedStockPrice;

    return predictedStockPrice;
}

double ConsoleUI::readPredictedIV(double predictionWindowMinutes) {
    double predictedIVPercent;

    std::cout << "Enter predicted IV % after "
              << predictionWindowMinutes
              << " minutes: ";

    std::cin >> predictedIVPercent;

    return predictedIVPercent / 100.0;
}

double ConsoleUI::readActualFuturePrice() {
    double actualFuturePrice;

    std::cout << "\nAfter the prediction window, enter actual option mark price.\n";
    std::cout << "Enter -1 if you do not have it yet: ";
    std::cin >> actualFuturePrice;

    return actualFuturePrice;
}

void ConsoleUI::printFetchedMarketData(const FetchedMarketData& data) {
    std::cout << "\n--- FETCHED MARKET DATA ---\n";
    std::cout << "Ticker:              " << data.ticker << "\n";
    std::cout << "Stock Price:         $" << data.stockPrice << "\n";
    std::cout << "Risk-Free Rate:      " << data.riskFreeRate * 100.0 << "%\n";
    std::cout << "Dividend Yield:      " << data.dividendYield * 100.0 << "%\n";
}

void ConsoleUI::printOptionSummary(const OptionInput& input) {
    std::cout << "\n--- OPTION INPUTS ---\n";
    std::cout << "Stock Price:        $" << input.S << "\n";
    std::cout << "Strike Price:       $" << input.K << "\n";
    std::cout << "Time to Expiry:     " << input.T << " years\n";
    std::cout << "Risk-Free Rate:     " << input.r * 100.0 << "%\n";
    std::cout << "Dividend Yield:     " << input.q * 100.0 << "%\n";
    std::cout << "Implied Volatility: " << input.sigma * 100.0 << "%\n";
    std::cout << "Type:               "
              << (input.type == OptionType::Call ? "Call" : "Put")
              << "\n";
}

void ConsoleUI::printGreeks(const OptionGreeks& greeks) {
    std::cout << "\n--- GREEKS ---\n";
    std::cout << "Delta: " << greeks.delta << "\n";
    std::cout << "Gamma: " << greeks.gamma << "\n";
    std::cout << "Theta: " << greeks.theta << " per day\n";
    std::cout << "Vega:  " << greeks.vega << " per 1% IV move\n";
}

void ConsoleUI::printPrediction(const PredictionResult& result) {
    std::cout << "\n--- PREDICTION RESULT ---\n";
    std::cout << "Current BS Price:       $" << result.currentBSPrice << "\n";
    std::cout << "Current Market Mark:    $" << result.currentMarketMark << "\n";
    std::cout << "Predicted Stock Price:  $" << result.predictedStockPrice << "\n";
    std::cout << "Predicted IV:           " << result.predictedIV * 100.0 << "%\n";
    std::cout << "Predicted Option Price: $" << result.predictedOptionPrice << "\n";
    std::cout << "Expected Move:          $" << result.expectedMove << "\n";
    std::cout << "Expected Move Percent:  " << result.expectedMovePercent * 100.0 << "%\n";
    std::cout << "Bid/Ask Spread:         $" << result.spread << "\n";
    std::cout << "Signal Score:           " << result.score << "\n";
}

void ConsoleUI::printValidation(const ValidationResult& result) {
    std::cout << "\n--- VALIDATION RESULT ---\n";
    std::cout << "Predicted Price: $" << result.predictedPrice << "\n";
    std::cout << "Actual Price:    $" << result.actualPrice << "\n";
    std::cout << "Absolute Error:  $" << result.absoluteError << "\n";
    std::cout << "Percent Error:   " << result.percentError * 100.0 << "%\n";
    std::cout << "Passed:          " << (result.passed ? "YES" : "NO") << "\n";
}