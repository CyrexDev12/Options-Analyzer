#ifndef MARKET_DATA_CLIENT_H
#define MARKET_DATA_CLIENT_H

#include <string>
#include <nlohmann/json.hpp>

struct FetchedMarketData {
    std::string ticker;
    double stockPrice;
    double riskFreeRate;
    double dividendYield;
};

class MarketDataClient {
public:
    explicit MarketDataClient(std::string alphaVantageApiKey);

    FetchedMarketData fetchForTicker(const std::string& ticker);

private:
    std::string apiKey;

    double fetchStockPrice(const std::string& ticker);
    double fetchDividendYield(const std::string& ticker);
    double fetchRiskFreeRate();

    static double parseLatestFredCsvValue(const std::string& csv);
    static void waitForAlphaVantageRateLimit();

    static void throwIfAlphaVantageMessage(const nlohmann::json& data);
};

#endif