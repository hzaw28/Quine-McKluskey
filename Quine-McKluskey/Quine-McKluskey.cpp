//************************************  PROGRAM IDENTIFICATION  ***************************************
//*                                                                                                   *
//*   PROGRAM FILE NAME: Quine-McKluskey.cpp             ASSIGNMENT #1             Grade: _________   *
//*                                                                                                   *
//*   PROGRAM AUTHOR:     _________________________________________                                   *
//*                                    Hein Htet Zaw                                                  *
//*                                                                                                   *
//*   COURSE #:  CSC 40300 11                                           DUE DATE:  October 17, 2016   *
//*                                                                                                   *
//*****************************************************************************************************
//*****************************************************************************************************
//                                                                                                    *
// Program description: This program is to give an automated tool for producing minimized Boolean     *
//						expressions using the Quine-McKluskey method. Input to this program is given  *
//						in the form of a right-hand side of a Boolean function. The variables in each *
//						term always appears in the same order. Each term always has the same number of*
//						variables but the number of variables in each term varies from expression to  *
//						expression. The maximum number of variables per term is six and the maximum   *
//						number of terms is twelve. Terms are separated by a blank. A TERM sentinel of *
//						'X' indicates the end of the expression. 'S' indicates the end of input file. *
//                                                                                                    *
//*****************************************************************************************************

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

//define the maximum variables and terms in the input file
#define variables 6
#define terms 12
#define col 3

using namespace std;

void header(ofstream &outfile);
void footer(ofstream &outfile);
int compareTerms(int termCount, string data0[][col], string data1[][col]);

int main() {
	//set up input and output files
//	ifstream infile("Test_data.txt", ios::in);		//test case
	ifstream infile("data1.txt", ios::in);
	ofstream outfile("Output.txt", ios::out);

	//print header section before anything
	header(outfile);

	//set up sentinel watcher
	string sentinel;

	//get the very first term
	infile >> sentinel;

	//repeat processing till 'S' appears
	while (sentinel[0] != 'S') {

		//declare a temporary storage variable for inputs
		string temp;

		//create arrays to store the terms / using 3D array in this case
		string data[10][terms][col];

		//get the very first input -- initiate
		temp = sentinel;

		//keep track of the number of terms and variables
		int termCount = 0, termCount1 = 0, termCount2 = 0, termCount3 = 0, termCount4 = 0;
		int varCount = 0;

		//scan for every term till 'X' appears
		while (temp[0] != 'X') {

			//count the negative variables in each term
			int negatives = 0;

			//update the variable count
			varCount = temp.length();

			//break down each term into variables
			for (int i = 0; i < varCount; i++) {

				//check number of lower case letters
				if (islower(temp[i])) negatives++;
			}
			//store the term in the array
			data[0][termCount][0] = temp;

			//keep negative count and merged status in the array
			data[0][termCount][1] = to_string(negatives);
			data[0][termCount][2] = "F";

			//get the next term
			infile >> temp;

			//increment the term counter
			termCount++;
		}

		outfile << endl << termCount << " terms | " << varCount << " variables" << endl << endl;

		termCount1 = compareTerms(termCount, data[0], data[1]);

		//comparing original and new tables
		for (int i = 0; i < termCount; i++) {
			outfile << data[0][i][0] << " | " << data[0][i][1] << " | " << data[0][i][2] << endl;
		}
		outfile << termCount1 << endl;
		for (int i = 0; i < termCount1; i++) {
			outfile << data[1][i][0] << " | " << data[1][i][1] << " | " << data[1][i][2] << endl;
		}

		termCount2 = compareTerms(termCount1, data[1], data[2]);

		outfile << termCount2 << endl;
		for (int i = 0; i < termCount2; i++) {
			outfile << data[1][i][0] << " | " << data[1][i][1] << " | " << data[1][i][2] << endl;
		}


		//update the sentinel check
		infile >> sentinel;
	}

	//print footer after everything
	footer(outfile);

//	cin.get();

	//return 0 and quit the app when everything is finished
	return 0;
}

//*****************************************************************************************************
int compareTerms(int termCount, string data0[][col], string data1[][col]) {

	// Receives � original data table and an empty data table
	// Task - compare terms in original table and put the results in new table
	// Returns - new data table

	if (termCount == 0) return 0;

	int varCount = data0[0][0].length();
	int newCount = 0;

	for (int i = 0; i < termCount-1; i++) {
		for (int j = i + 1; j < termCount; j++) {
			string newString; int diff = 0; 

			for (int k = 0; k < varCount; k++) {
				
				if (data0[i][0][k] != data0[j][0][k]) {
					newString += "_";
					diff++;
				}
				else newString += data0[j][0][k];
			}

			if (diff < 2) {
				data1[newCount][0] = newString;
				data1[newCount][1] = "0";
				data1[newCount][2] = "F";
				newCount++;
			}
		}
	}
	return newCount;
}

//*****************************************************************************************************
void header(ofstream &outfile) {

	// Receives � the output file
	// Task - Prints the output preamble
	// Returns - Nothing

	outfile << setw(33) << "Hein Htet Zaw";
	outfile << setw(15) << "CSC 40300";
	outfile << setw(15) << "Section 11" << endl;
	outfile << setw(37) << "Fall 2016";
	outfile << setw(20) << "Assignment #1" << endl;
	outfile << setw(38) << "-------------------------------------------";
	outfile << setw(38) << "------------------------------------------\n\n";

	return;
}

//*****************************************************************************************************
void footer(ofstream &outfile) {

	// Receives � the output file
	// Task - Prints the output salutation
	// Returns - Nothing

	outfile << endl;
	outfile << setw(35) << "----------------------------------" << endl;
	outfile << setw(35) << "|      END OF PROGRAM OUTPUT     |" << endl;
	outfile << setw(35) << "----------------------------------" << endl;

	return;
}
//*****************************************************************************************************