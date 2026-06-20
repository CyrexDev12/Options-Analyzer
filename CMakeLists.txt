cmake_minimum_required(VERSION 3.16)

project(OptionsAlertModel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(CURL REQUIRED)

add_executable(OptionsAlertModel
    main.cpp
    src/NormalDistribution.cpp
    src/BlackScholes.cpp
    src/ImpliedVolatility.cpp
    src/PredictionEngine.cpp
    src/ValidationEngine.cpp
    src/ConsoleUI.cpp
    src/HttpClient.cpp
    src/MarketDataClient.cpp
)

target_link_libraries(OptionsAlertModel PRIVATE CURL::libcurl)