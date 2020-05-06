
#include "pch.h"
#include <iostream>
#include "mpi.h"
#include <fstream>
#include <string>
#include <ctime>
#include <vector> 
#include "Helpers.h"
#include <chrono>
using namespace std;


void ActionParentProcess(int p, int nrProc) {

	vector<Monom> polyResult;
	auto startTimer = chrono::high_resolution_clock::now();
	cout << nrProc << endl;
	int start = 1;
	int end = p / (nrProc-1);
	int rest = p % (nrProc-1);
	int count = end;
	
	for (int i = 0; i < (nrProc-1); i++)
	{
		if (rest) {
			end = end + 1;
			rest--;
		}

		MPI_Send(&start, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
		MPI_Send(&end, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);

		MPI_Status status;
		int done;
		MPI_Recv(&done, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, &status);
		for (int j = 0; j < done; j++)
		{
			int gradt, coeft;
			MPI_Recv(&gradt, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&coeft, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, &status);

			Monom curent;
			curent.grad = gradt;
			curent.coef = coeft;
			bool gasitGrad = false;
			for (int i = 0; i < polyResult.size() && gasitGrad == false; i++)
			{
				if (polyResult[i].grad == curent.grad) {
					gasitGrad = true;
					polyResult[i].coef = polyResult[i].coef + curent.coef;
				}
			}
			if (gasitGrad == false) {
				polyResult.push_back(curent);
			}
		}
		start = end + 1;
		end = end + count;

	}
	auto endTimer = chrono::high_resolution_clock::now();
	cout << "\n" << chrono::duration_cast<std::chrono::nanoseconds>(endTimer - startTimer).count();
	/*for (int i = 0; i < polyResult.size(); i++)
	{
		cout << polyResult[i].grad << " " << polyResult[i].coef << endl;
	}*/
	/*WriteResultToFile("polyy-sum.txt", polyResult);*/
}

void ActionChildProcess() {

	MPI_Status status;
	int start, end, done;
	vector<Monom> polyResult;

	MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

	for (int i = start; i <= end; i++)
	{
		ifstream file("polyp8" + to_string(i)+ ".txt");
		int grad, coef;
		while (file >> grad)
		{
			if (file >> coef)
			{
				Monom curent;
				curent.grad = grad;
				curent.coef = coef;
				bool gasitGrad = false;
				for (int j = 0; j < polyResult.size() && gasitGrad==false; j++)
				{
					if (polyResult[j].grad == curent.grad) {
							gasitGrad = true;
							polyResult[j].coef = polyResult[j].coef + curent.coef;
					}
				}
				if (gasitGrad == false) {
					polyResult.push_back(curent);
				}
									
			}
		}
	}

	done = polyResult.size();
	/*cout << done<< " start" << start<< "end"<< end<<endl;*/
	MPI_Send(&done, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Send(&polyResult[0].grad, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	for (int i = 0; i < polyResult.size(); i++) {
		int gradt = polyResult[i].grad;
		int coeft = polyResult[i].coef;
		MPI_Send(&gradt, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		MPI_Send(&coeft, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	//cout << endl;
	
}

int main()
{
	int p = 1000;
	/*for (int i = 1; i <= p; i++)
	{
		CreateRandomNumbersFile("polyp8"+to_string(i)+".txt", 10000, 1, 50);
	}*/
	int rank, nrProc;
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		std::cout << "Err MPI init\n";
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &nrProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		ActionParentProcess(p,nrProc);
	}
	else {
		ActionChildProcess();
	}

	MPI_Finalize();

	
}
