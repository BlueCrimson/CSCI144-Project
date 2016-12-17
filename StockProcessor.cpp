
#include "StockProcessor.h"
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Passed into pthread to allow multiple arguments in routine
struct ThreadParam {
	float *balance;
    float *funds;
	Stock *stock;
};

// Set to 1 for debug log, 0 otherwise
int DEBUG = 0;

using namespace std;
// Namespace for Util header, used to split text file strings
using namespace Util;

// Constructor
StockProcessor::StockProcessor()
{
	// Default values for user-defined stock values
	deltaPercentage = 5;
	sellThreshold = 5;
	buyThreshold = 5;
    yieldSellThreshold = 3;
	balance = 0;
    funds = 10000;
	// Initialize randomization for stock prices
	srand(static_cast<unsigned> (time(0)));
}

// Destructor
StockProcessor::~StockProcessor()
{
}

// Function that calculates sell transaction
void *doSellStock(void *data)
{
	ThreadParam *param = static_cast<ThreadParam *>(data);
	Stock *stock = param->stock;

	stock->shares++;
	cout << "Sell " << stock->symbol << " at $" << stock->price
		 << " a share (delta: $" << stock->delta << ")" << endl;
    // If sold, increase balance and funds by price of sold stock
	*param->balance += stock->price;
    *param->funds += stock->price;
	return 0;
}

// Function that calculates buy transaction
void *doBuyStock(void *data)
{
	ThreadParam *param = static_cast<ThreadParam *>(data);
	Stock *stock = param->stock;
	stock->shares--;
	cout << "Buy " << stock->symbol << " at $" << stock->price
		 << " a share (delta: $" << stock->delta << ")" << endl;
    // If bought, decrease balance and funds by price of bought stock
	*param->balance -= stock->price;
    *param->funds -= stock->price;
	return 0;
}

// Parses stock txt file using tokens and vectors
void StockProcessor::getStockPrices()
{
	ifstream stocks;
	stocks.open("stocks.txt");
	if (stocks.fail())
		throw runtime_error("Error opening stock data file.");

	if (stocks.is_open())
	{
		// Parses text file using tokens, and pushes tokens into vector for calculations
		string line;
		vector<string> tokens;
		while (getline(stocks, line))
		{
			// Splits text file string using utility function and whitespace
			tokens = SplitString(line, ' ');

			// Skip those without 3 items (symbol, shares, price)
			if (tokens.size() != 3)
				continue;

			Stock stock;
			// Parses stock text file into three tokens
			// Stock name, amount of shares, and stock price
			stock.symbol = tokens[0];
			stock.shares = atoi(tokens[1].c_str());
			stock.price = atof(tokens[2].c_str());
			// Initializes stock delta
			stock.delta = 0;

			stockList.push_back(stock);
		}
	}
	// Closes file once finished
	stocks.close();
}

// Prints out stock name, shares, and prices
void StockProcessor::printStocks()
{
	cout << endl;
	cout << "---------------------" << endl;
	cout << "     S T O C K S     " << endl;
	cout << "---------------------" << endl;

	for (int i = 0; i < stockList.size(); i++)
	{
		const Stock &stock = stockList[i];
		// Set precision simplifies significant digits
		cout << fixed << setprecision(2);
		cout << stock.symbol << ": ";
		cout << stock.shares << " shares, at $";
		cout << stock.price << " per share" << endl;
	}
	cout << "Balance: " << balance << endl;
    cout << "Funds: " << funds << endl;
}

// Randomizes stock prices at the start of each transaction
void StockProcessor::shuffleStocks()
{
	for (int i = 0; i < stockList.size(); i++)
	{
		Stock &stock = stockList[i];
		// Delta is the percentage value that stocks are randomized each day,
		// higher values == more variance in randomness
		float delta = (rand() % (deltaPercentage*2+1) - deltaPercentage) * stock.price;
		delta /= 100.0;

		stock.delta = delta;

		// Debugging
		if (DEBUG) 
		{
			cout << fixed << setprecision(2);
			cout << "delta " << stock.delta << endl;
		}

		stock.price += stock.delta;
	}
}

// Calculates whether or not stock is bought/sold
void StockProcessor::processStocks()
{
	for (int i = 0; i < stockList.size(); i++)
	{
		Stock &stock = stockList[i];
		// Will sell or buy stock based on user-defined sell/buy values

        // First if statement checks if user's funds are sufficient
        if (funds < stock.price)
        {
            // If not sufficient, skip all buyStock() functions
            if (balance < -1000) {
                if (stock.delta > yieldSellThreshold)
                    sellStock(stock);
            }
            else if (stock.delta > sellThreshold)
                sellStock(stock);
        }
        // If funds are sufficient to buy, then check if balance < 0
        else if (balance < -1000) {
            // If balance is negative, change sell threshold to emergency yield threshold
            if (stock.delta > yieldSellThreshold)
                sellStock(stock);
            else if (stock.delta < -buyThreshold)
                buyStock(stock);
        }
        // Else if funds and balance are sufficient, check sell and buy threshold normally
		else if (stock.delta > sellThreshold)
			sellStock(stock);
		else if (stock.delta < -buyThreshold)
			buyStock(stock);
	}
}

// Initializes thread for handling selling stock
void StockProcessor::sellStock(Stock &stock)
{
	// Instantiate ThreadParam object to use in stock calculations
	ThreadParam param;
	param.balance = &balance;
    param.funds = &funds;
	param.stock = &stock;
	// Creates a posix thread for selling stocks
	pthread_t thread1;
	pthread_create(&thread1, NULL, doSellStock, &param);
	pthread_join(thread1, NULL);

}

// Initializes thread for handling buying stock
void StockProcessor::buyStock(Stock &stock)
{
	// Instantiate ThreadParam object to use in stock calculations
	ThreadParam param;
	param.balance = &balance;
    param.funds = &funds;
	param.stock = &stock;
	// Creates a posix thread for buying stocks
	pthread_t thread1;
	pthread_create(&thread1, NULL, doBuyStock, &param);
	pthread_join(thread1, NULL);
}
