#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "OptionTypes.h"
#include "MarketDataClient.h"

#include <string>

class ConsoleUI {
public:
    static std::string readTicker();

    static OptionInput readOptionInputFromFetchedData(
        const FetchedMarketData& fetchedData
    );

    static MarketSnapshot readMarketSnapshot(const OptionInput& option);

    static double readPredictionWindowMinutes();
    static double readPredictedStockPrice(double predictionWindowMinutes);
    static double readPredictedIV(double predictionWindowMinutes);
    static double readActualFuturePrice();

    static void printFetchedMarketData(const FetchedMarketData& data);
    static void printOptionSummary(const OptionInput& input);
    static void printGreeks(const OptionGreeks& greeks);
    static void printPrediction(const PredictionResult& result);
    static void printValidation(const ValidationResult& result);

private:
    static OptionType readOptionType();
};

#endif