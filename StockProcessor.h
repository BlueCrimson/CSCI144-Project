#pragma once

#include <iostream>
#include <pthread.h>
#include "stock.h"
#include "util.h"

// Contains prototypes for all StockProcessor functions and variables
class StockProcessor
{
public:
	vector<Stock> stockList;
    int deltaPercentage;
    int buyThreshold;
    int sellThreshold;
    int yieldSellThreshold;
    float balance;
    float funds;

// Constructor and destructor declarations
public:
	StockProcessor();
	~StockProcessor();

// Prototypes for StockProcessor functions
public:
	void getStockPrices();
	void printStocks();
	void shuffleStocks();
	void processStocks();

// Prototypes for private helper functions
private:
	void sellStock(Stock &stock);
	void buyStock(Stock &stock);
};

