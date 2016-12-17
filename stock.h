//
//  stock.h
//  CSCI 144 Project
//
//  Created by Kevin on 11/21/16.
//  Copyright © 2016 Kevin. All rights reserved.
//

#pragma once

#include <string>

using namespace std;

struct Stock 
{
	string symbol;
	int shares;
	float price;

	// Price difference since last time in percent
	float delta;
};

