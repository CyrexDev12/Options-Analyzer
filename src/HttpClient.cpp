#include "HttpClient.h"

#include <curl/curl.h>
#include <stdexcept>
#include <string>

static size_t writeCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
) {
    size_t totalSize = size * nmemb;

    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);

    return totalSize;
}

std::string HttpClient::get(const std::string& url) {
    CURL* curl = curl_easy_init();

    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL.");
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Timeout settings.
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    // Some endpoints behave better with a user agent.
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "OptionsAnalyzer/1.0");

    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK) {
        std::string error = curl_easy_strerror(result);
        curl_easy_cleanup(curl);

        throw std::runtime_error("HTTP request failed: " + error);
    }

    curl_easy_cleanup(curl);

    return response;
}