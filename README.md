Project Build: 

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build

Run: 
.\Debug\OptionsAlertModel.exe


Call API 

Invoke-RestMethod "https://www.alphavantage.co/query?function=OVERVIEW&symbol=AAPL&apikey=$env:ALPHA_VANTAGE_API_KEY"



NEEDED DATA FOR BLACK-SCHOLES OPTIONS ALERT MODEL

==================================================
1. REQUIRED BLACK-SCHOLES INPUTS
==================================================

These are required to calculate the theoretical option price.

1. Underlying stock price
   Symbol: S
   Example: AAPL = 298.01
   Notes: Current live stock price.

2. Strike price
   Symbol: K
   Example: 300
   Notes: Comes from the option contract.

3. Time to expiration
   Symbol: T
   Example: 7 / 365
   Notes: Use calendar time. Later, use exact minutes/seconds until expiration.

4. Risk-free interest rate
   Symbol: r
   Example: 0.037
   Notes: Use short-term Treasury yield, such as the 1-month Treasury rate.

5. Dividend yield
   Symbol: q
   Example: 0.0036 for Apple
   Notes: Use 0 if the stock does not pay dividends.

6. Implied volatility
   Symbol: sigma
   Example: 0.6982
   Notes: Use contract-specific implied volatility.

7. Option type
   Example: Call or Put
   Notes: Required because call and put formulas are different.


==================================================
2. REQUIRED OPTION QUOTE DATA
==================================================

This is needed to compare the model price against the real market price.

1. Option bid price
   Example: 2.25
   Notes: Highest price buyers are offering.

2. Option ask price
   Example: 2.41
   Notes: Lowest price sellers are asking.

3. Option mark / midpoint
   Example: 2.33
   Formula: (Bid + Ask) / 2
   Notes: Usually the best price to compare against Black-Scholes.

4. Last traded price
   Example: 2.30
   Notes: Useful, but can be stale.

5. Bid/ask spread
   Example: 0.16
   Formula: Ask - Bid
   Notes: Needed to avoid bad trades with wide spreads.

6. Option volume
   Example: 1,250
   Notes: Shows how actively the contract traded today.

7. Open interest
   Example: 8,700
   Notes: Shows how many contracts are currently open.


==================================================
3. USEFUL GREEKS
==================================================

These are not strictly required if you calculate them yourself, but they are useful.

1. Delta
   Notes: Estimated option price change for a $1 move in the stock.

2. Gamma
   Notes: Shows how quickly delta changes.

3. Theta
   Notes: Time decay.

4. Vega
   Notes: Option sensitivity to implied volatility changes.

5. Rho
   Notes: Sensitivity to interest rates. Usually less important for short-term options.


==================================================
4. DATA NEEDED FOR 5-10 MINUTE PREDICTION MODEL
==================================================

Your prediction model needs to estimate future stock price and future implied volatility.

1. Recent stock price history
   Example: Last 1 min, 3 min, 5 min, 10 min
   Notes: Used to detect momentum.

2. Recent option mark price history
   Example: Option mark every 10 seconds
   Notes: Used to track short-term option movement.

3. Recent implied volatility history
   Example: IV over last 5 minutes
   Notes: Used to detect IV expansion or contraction.

4. Recent bid/ask history
   Notes: Used to detect spread widening or tightening.

5. Recent stock volume
   Notes: Helps detect activity spikes.

6. Recent option volume
   Notes: Helps detect unusual contract activity.

7. Time of day
   Example: Market open, midday, near close
   Notes: Options behave differently depending on time of day.

8. Previous prediction results
   Notes: Needed for your validation sequence.


==================================================
5. VALIDATION DATA
==================================================

Store this after every prediction.

1. Prediction timestamp
   Example: 10:05:00

2. Prediction window
   Example: 5 minutes

3. Predicted stock price
   Example: 52.10

4. Actual stock price after prediction window
   Example: 52.03

5. Predicted implied volatility
   Example: 71.2%

6. Actual implied volatility after prediction window
   Example: 70.9%

7. Predicted option price
   Example: 2.65

8. Actual option mark after prediction window
   Example: 2.58

9. Prediction error percent
   Example: 2.71%

10. Passed tolerance
    Example: Yes or No


==================================================
6. ALERT RULE DATA
==================================================

These values control when the program sends a trade alert.

1. Required successful predictions
   Example: 3

2. Tolerance percent
   Example: 5%

3. Minimum signal score
   Example: 2.0

4. Minimum expected move
   Example: 0.20 option premium

5. Spread threshold
   Example: Spread must be below 0.10

6. Minimum option volume
   Example: 500

7. Minimum open interest
   Example: 1,000

8. Maximum acceptable spread percent
   Example: Spread must be less than 5% of mark price


==================================================
7. RECOMMENDED CSV COLUMNS
==================================================

timestamp
symbol
option_type
stock_price
strike
expiration
days_to_expiration
minutes_to_expiration
risk_free_rate
dividend_yield
implied_volatility
bid
ask
mark
last
spread
volume
open_interest
delta
gamma
theta
vega
black_scholes_price
predicted_stock_price
predicted_iv
predicted_option_price
actual_future_stock_price
actual_future_iv
actual_future_option_price
prediction_error_percent
signal_score
alert_triggered


==================================================
8. MINIMUM DATA TO START MANUALLY
==================================================

For the current prototype, you only need:

1. Stock ticker
2. Current stock price
3. Strike price
4. Expiration date
5. Days or minutes to expiration
6. Risk-free rate
7. Dividend yield
8. Implied volatility
9. Call or put
10. Option bid
11. Option ask
12. Predicted future stock price
13. Predicted future implied volatility
14. Actual future option price for validation


==================================================
9. CORE REQUIRED INPUTS
==================================================

The most important fields are:

S = current stock price
K = strike price
T = time to expiration
r = risk-free rate
q = dividend yield
sigma = implied volatility
bid = option bid
ask = option ask

These are the core inputs your model needs.


Data we can fetch for free
1. Current Stock Price 
2. Risk Free IR 
3. Dividend Yield 

Change so we enter the stock TICKER and it fetches that info from an API and rest of the information is inputted


WE NEED Predicted stock price after x minutes and we need predicted IV% after 5 min

