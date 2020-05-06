#include "pch.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector> 

using namespace std;

struct Monom {
	unsigned int grad;
	unsigned int coef;
};

void WriteResultToFile(const string output_file_name, vector<Monom> polySum) {
	ofstream outputFile;
	outputFile.open(output_file_name, ios::trunc);
	for (int i = 0; i < polySum.size(); i++) {
		outputFile << polySum[i].grad << " " << polySum[i].coef << " ";
	}

	outputFile.close();
}

void CreateRandomNumbersFile(const string file_name, const int size, const int min, const int max) {
	ofstream outputFile;
	outputFile.open(file_name, ios::trunc);
	int count = 0;
	while (count < size) {
		outputFile << rand() % max + min << " ";
		count++;
	}

	outputFile.close();
}