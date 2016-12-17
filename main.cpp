#include "StockProcessor.h"
#include "stock.h"
#include "unistd.h"
using namespace std;

int main()
{
	// Instantiates StockProcessor object
	StockProcessor *sp = new StockProcessor();
	// Prompts user to enter delta value
	// Delta is the percentage that stock can randomly change each day
	cout << "Enter stock delta percentage: ";
	cin >> sp->deltaPercentage;
	// Error handling if user puts extreme value
	while (sp->deltaPercentage > 30 || sp->deltaPercentage < 1)
	{
		cerr << "Value out of bounds, enter something between 1 and 30." << endl;
		cout << "Enter stock delta percentage: ";
		cin >> sp->deltaPercentage;
	}
	// Prompts user to enter sell percentage
	cout << "Enter sell threshold percentage: ";
	cin >> sp->sellThreshold;
	while (sp->sellThreshold > 20 || sp->sellThreshold < 1)
	{
		cerr << "Sell threshold out of bounds, enter something between 1 and 20" << endl;
		cout << "Enter sell threshold percentage: ";
		cin >> sp->sellThreshold;
	}

    cout << "Enter emergency sell threshold percentage: ";
    cin >> sp->yieldSellThreshold;
    while (sp->yieldSellThreshold > 20 || sp->yieldSellThreshold < 1)
    {
        cerr << "Emergency threshold out of bounds, enter something between 1 and 20" << endl;
        cout << "Enter emergency sell threshold percentage: ";
        cin >> sp->yieldSellThreshold;
    }

	cout << "Enter buy threshold percentage: ";
	cin >> sp->buyThreshold;
	while (sp->buyThreshold > 20 || sp->buyThreshold < 1)
	{
		cerr << "Buy threshold out of bounds, enter something between 1 and 20" << endl;
		cout << "Enter buy threshold percentage: ";
		cin >> sp->buyThreshold;
	}
	// Parses stock information using tokens and vector
	sp->getStockPrices();
	// Prints stock information from text file
	sp->printStocks();
	// Loops through 10000 stock transactions
	for (int i = 0; i <= 10000; i++)
	{
		sp->shuffleStocks();
		sp->processStocks();
		sp->printStocks();
		// Sleeps to simulate calculations and improve readability
		usleep(100000);
	}
	// Deletes StockProcessor object before exiting
	delete sp;
	return 0;
}
