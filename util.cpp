#include "util.h"
// Used to parse text file into tokens for calculations
vector<string> Util::SplitString(const string &s, char delim)
{
	// Streams strings from text file
	stringstream ss(s);
	string item;
	vector<string> tokens;
	// Loops through text file until all items are pushed into vector
	while (getline(ss, item, delim)) 
	{
		tokens.push_back(item);
	}
	return tokens;
}
