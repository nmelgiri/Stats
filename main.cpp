#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <chrono>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;

// Basic Statistics Calculations
double mean(vector <double> values)
{
	double result = 0;
	double sum = 0;

	for (int index = 0; index < values.size(); index++)
	{
		sum += values[index];
	}
	result = sum / values.size();

	return result;
}
double stdDev(vector <double> values)
{
	double avg = mean(values);
	double totalDiff = 0;
	double result = 0;

	for (int index = 0; index < values.size(); index++)
	{
		totalDiff += pow(avg - values[index], 2);
	}
	result = sqrt(totalDiff / values.size());

	return result;
}
double mean_of_means(vector<vector<double>> MeanSet)
{
	vector <double> meanOfMeans;
	vector <double> varianceOfMeans;
	for (int index = 0; index < MeanSet.size(); index++)
	{
		meanOfMeans.push_back(mean(MeanSet[index]));
		varianceOfMeans.push_back(stdDev(MeanSet[index]) * stdDev(MeanSet[index]));
	}
	return 0;
}

// Core Data Gathering Operation
vector <vector <vector <double>>> getData()
{
	// Seed the random number generator
	srand(time(NULL));

	// Set storage vectors and variables
	vector <double> TimeDiffPerIteration;
	vector <vector <double>> IterationsPerTrial;
	vector < vector <vector <double>>> AllTrials;
	vector < vector< vector < vector <double>>>> allRawValues;
	// This "3d Array" is also called a tensor; Just like vectors and matricies but in 3d

	// Set perameters
	const double maxMilliseconds = 1; 
	const double repetitions = 50;
	const double numTrials = 50;
	const double millisecondStep = 0.01;


	// Heart of the program


	// First for loop for the number of overall trials we will have
	for (int trial = 0; trial < numTrials; trial++)
	{
		// Second loop for testing every millisecond step [eg. 0ms, 2ms, 4ms, 6ms, ... n ms]
		for (double milliseconds = 0; milliseconds < maxMilliseconds; milliseconds += millisecondStep)
		{
			// Third loop to test the millisecond interval N times
			for (int j = 0; j < repetitions; j++)
			{
				double randomNum1 = double(rand()) / double(RAND_MAX);
				Sleep(milliseconds);
				double randomNum2 = double(rand()) / double(RAND_MAX);
				TimeDiffPerIteration.push_back(fabs(randomNum1 - randomNum2));
			}
			IterationsPerTrial.push_back(TimeDiffPerIteration);
			TimeDiffPerIteration.erase(TimeDiffPerIteration.begin(), TimeDiffPerIteration.begin() + TimeDiffPerIteration.size());
		}
		AllTrials.push_back(IterationsPerTrial);
		IterationsPerTrial.erase(IterationsPerTrial.begin(), IterationsPerTrial.begin() + IterationsPerTrial.size());
	}
	return AllTrials;
}

// For Displaying Tensors
void displayVector(vector<double> display, ofstream &fout)
{
	for (int index = 0; index < display.size(); index++)
	{
		fout << fixed << setprecision(10) << setw(4) << display[index] << ", ";
	}
}
void displayVector(vector<vector<double>> display, ofstream &fout)
{
	for (int i = 0; i < display.size(); i++)
	{
		for (int j = 0; j < display[i].size(); j++)
		{
			fout << fixed << setprecision(10) << setw(4) << display[i][j] << ", ";
		}
		fout << endl;
	}
}
void displayVector(vector<vector<vector<double>>> display, ofstream &fout)
{
	for (int i = 0; i < display.size(); i++)
	{
		for (int j = 0; j < display[i].size(); j++)
		{
			for (int k = 0; k < display[i][j].size(); k++)
			{
				fout << fixed << setprecision(10) << setw(4) << display[i][j][k] << ", ";
			}
			fout << endl;
		}
		fout << endl;
	}
}

int main()
{	
	vector<vector<vector<double>>> data = getData();	
	/* 
	Structure of vector 'data' ----> Let's refer to it as a 3d tensor now (a 3d matrix)

	
		TimeDiff: A 1d Vector
			[a, b, c, d]

		IterationsPerTrial: a 2d Matrix
			[ a, b, c, d ]			[TimeDiff1]
			[ e, f, g, h ]	----->	[TimeDiff2]
			[ i, j, k, l ]			[TimeDiff3]

		data: a 3d tensor (3d matrix if you prefer). Like a bunch of squares on top of one
		another to make a cube
			data[0]			data[1]
			[ a, b, c, d ]	[ a', b', c', d' ]			
			[ e, f, g, h ]	[ e', f', g', h' ]
			[ i, j, k, l ],	[ i', j', k', l' ]...
	*/

	//--------------------------------------------------------------------------------------------
	// Parse Data and perform calculations and/or file i/o


	// 1.) calculate the mean and stdDev of each 1d vector of the differences (TimeDiff)
	vector<vector<double>> timeDiffMean;
	vector<vector<double>> timeDiffStdDev;
	for (int i = 0; i < data.size(); i++)
	{
		vector<double> x; ///push_back an empty vector
		timeDiffMean.push_back(x);
		timeDiffStdDev.push_back(x);

		/// Calculate means and stdDev
		for (int j = 0; j < data[i].size(); j++)
		{
			/// The mean and stdDev for every trial with same time difference (ie. 0ms, 1ms, 2ms, etc.)
			timeDiffMean[i].push_back(mean(data[i][j]));
			timeDiffStdDev[i].push_back(stdDev(data[i][j]));
		}
	}

	// 2.) Send to txt file to then run linear regression on python
	ofstream foutData("datatable.csv");
	displayVector(data, foutData);
	foutData.close();

	ofstream foutMean("datatableMeans.csv");
	displayVector(timeDiffMean, foutMean);
	foutMean.close();

	ofstream foutStdDev("datatableStdDev.csv");
	displayVector(timeDiffStdDev, foutStdDev);
	foutStdDev.close();

	system("pause");
	return EXIT_SUCCESS;
}