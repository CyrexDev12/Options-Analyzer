#include "MarketDataClient.h"
#include "HttpClient.h"

#include <nlohmann/json.hpp>

#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

MarketDataClient::MarketDataClient(std::string alphaVantageApiKey)
    : apiKey(std::move(alphaVantageApiKey)) {}

void MarketDataClient::waitForAlphaVantageRateLimit() {
    // Alpha Vantage free API asks for 1 request per second.
    // Use 1500ms for a safety buffer.
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

void MarketDataClient::throwIfAlphaVantageMessage(const json& data) {
    if (data.contains("Information")) {
        throw std::runtime_error(
            "Alpha Vantage information message: " +
            data["Information"].get<std::string>()
        );
    }

    if (data.contains("Note")) {
        throw std::runtime_error(
            "Alpha Vantage note/rate-limit message: " +
            data["Note"].get<std::string>()
        );
    }

    if (data.contains("Error Message")) {
        throw std::runtime_error(
            "Alpha Vantage error message: " +
            data["Error Message"].get<std::string>()
        );
    }
}

FetchedMarketData MarketDataClient::fetchForTicker(const std::string& ticker) {
    FetchedMarketData data{};

    data.ticker = ticker;

    std::cout << "Fetching stock price from Alpha Vantage...\n";
    data.stockPrice = fetchStockPrice(ticker);

    waitForAlphaVantageRateLimit();

    std::cout << "Fetching dividend yield from Alpha Vantage...\n";
    data.dividendYield = fetchDividendYield(ticker);

    // Fixed risk-free rate for now.
    // 0.037 = 3.70%
    data.riskFreeRate = 0.037;

    return data;
}

double MarketDataClient::fetchStockPrice(const std::string& ticker) {
    std::string url =
        "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol="
        + ticker
        + "&apikey="
        + apiKey;

    std::string response = HttpClient::get(url);

    json data = json::parse(response);

    throwIfAlphaVantageMessage(data);

    if (!data.contains("Global Quote")) {
        std::cerr << "\nERROR: GLOBAL_QUOTE missing.\n";
        std::cerr << "Raw Alpha Vantage response:\n";
        std::cerr << response << "\n";

        throw std::runtime_error(
            "Alpha Vantage GLOBAL_QUOTE response missing 'Global Quote'."
        );
    }

    json quote = data["Global Quote"];

    if (!quote.contains("05. price")) {
        std::cerr << "\nERROR: price missing from GLOBAL_QUOTE.\n";
        std::cerr << "Raw Alpha Vantage response:\n";
        std::cerr << response << "\n";

        throw std::runtime_error(
            "Alpha Vantage quote response missing '05. price'."
        );
    }

    std::string priceString = quote["05. price"];

    if (priceString.empty()) {
        throw std::runtime_error("Alpha Vantage returned an empty stock price.");
    }

    return std::stod(priceString);
}

double MarketDataClient::fetchDividendYield(const std::string& ticker) {
    std::string url =
        "https://www.alphavantage.co/query?function=OVERVIEW&symbol="
        + ticker
        + "&apikey="
        + apiKey;

    std::string response = HttpClient::get(url);

    json data = json::parse(response);

    throwIfAlphaVantageMessage(data);

    if (!data.contains("DividendYield")) {
        std::cerr << "\nERROR: DividendYield missing.\n";
        std::cerr << "Raw Alpha Vantage response:\n";
        std::cerr << response << "\n";

        throw std::runtime_error(
            "Alpha Vantage OVERVIEW response missing DividendYield."
        );
    }

    std::string dividendYieldString = data["DividendYield"];

    if (
        dividendYieldString.empty() ||
        dividendYieldString == "None" ||
        dividendYieldString == "none" ||
        dividendYieldString == "null" ||
        dividendYieldString == "NULL" ||
        dividendYieldString == "-"
    ) {
        return 0.0;
    }

    return std::stod(dividendYieldString);
}

double MarketDataClient::fetchRiskFreeRate() {
    // FRED 1-month Treasury yield CSV.
    // Values are in percent, example 3.71.
    // We convert to decimal, example 0.0371.
    std::string url =
        "https://fred.stlouisfed.org/graph/fredgraph.csv?id=DGS1MO";

    std::string csv = HttpClient::get(url);

    double latestPercent = parseLatestFredCsvValue(csv);

    return latestPercent / 100.0;
}

double MarketDataClient::parseLatestFredCsvValue(const std::string& csv) {
    std::stringstream stream(csv);
    std::string line;

    double latestValue = 0.0;
    bool foundValue = false;

    // Skip CSV header.
    std::getline(stream, line);

    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream lineStream(line);

        std::string date;
        std::string value;

        std::getline(lineStream, date, ',');
        std::getline(lineStream, value, ',');

        if (value.empty() || value == ".") {
            continue;
        }

        latestValue = std::stod(value);
        foundValue = true;
    }

    if (!foundValue) {
        throw std::runtime_error(
            "No valid risk-free rate found in FRED CSV."
        );
    }

    return latestValue;
}