#pragma once
#include "string"
using namespace std;

struct Monom {
	unsigned int grad;
	unsigned int coef;
};

void CreateRandomNumbersFile(const string file_name, const int size, const int min, const int max);

void WriteResultToFile(const string output_file_name, vector<Monom> polySum);