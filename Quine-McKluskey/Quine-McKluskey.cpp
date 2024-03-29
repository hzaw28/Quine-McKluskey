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
string removeUnderscores(string data);
int mergeDuplicates(int termCount, string data0[][col]);
int compareTerms(int termCount, string data0[][col], string data1[][col]);
int getResults(string original[], string reduced[][terms+1], string result[], int orig, int redu);
int getUnmarged(string data0[][col], string data1[][col], string data2[][col], string data3[][col],
				string newData[][terms+1]);

int main() {
			//set up input and output files
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
		string data[5][terms][col];
				
				//set up for original data and reduced data for later matrix
		string originalData[terms];
		string reducedData[terms][terms+1];

				//set up an array for the results
		string result[terms];

				//get the very first input -- initiate
		temp = sentinel;

				//keep track of the number of terms and variables
		int termCount[5] = { 0 };

				//scan for every term till 'X' appears
		while (temp[0] != 'X') {

					//count the negative variables in each term
			int negatives = 0;
			int varCount = 0;

					//update the variable count
			varCount = temp.length();

					//break down each term into variables
			for (int i = 0; i < varCount; i++) {

						//check number of lower case letters
				if (islower(temp[i])) negatives++;
			}
					//store the term in the array
			data[0][termCount[0]][0] = temp;
			originalData[termCount[0]] = temp;

					//keep negative count and merged status in the array
			data[0][termCount[0]][1] = to_string(negatives);
			data[0][termCount[0]][2] = "F";

					//get the next term
			infile >> temp;

					//increment the term counter
			termCount[0]++;
		}

				//compare elements in the original table and get new table
		for (int i = 0; i < 4; i++) {
			termCount[i+1] = compareTerms(termCount[i], data[i], data[i+1]);
		}

				//get unmarged data from raw data lists - 4 lists is enough in this case
		int unmarged = getUnmarged(data[0], data[1], data[2], data[3], reducedData);

				//get the final result from original data and reduced data lists
		int resultCount = getResults(originalData, reducedData, result, termCount[0], unmarged);

				//print label for original expression
		outfile << "The Original Boolean Expression: " << endl;

				//print out the original expression with plus signs in between
		for (int i = 0; i < termCount[0]; i++) {
			if (i < termCount[0] - 1) outfile << originalData[i] << " + ";
			else outfile << originalData[i] << endl;
		}

				//print label for minimized expression
		outfile << endl << "The Minimized Boolean Expression: " << endl;

				//print out the final results with plus signs in between
		for (int i = 0; i < resultCount; i++) {
			if (i < resultCount-1) outfile << result[i] << " + ";
			else outfile << result[i] << endl;
		}
		
				//print out line to separate out the new expressions
		for (int i = 0; i < 85; i++) outfile << "-";
		outfile << endl;

				//update the sentinel check
		infile >> sentinel;
	}

			//print footer after everything
	footer(outfile);

			//return 0 and quit the app when everything is finished
	return 0;
}

//*****************************************************************************************************
int compareTerms(int termCount, string data0[][col], string data1[][col]) {

		// Receives � original data table and an empty data table and number of terms in original table
		// Task - compare terms in original table and put the results in new table
		// Returns - new data table and its term counts

	if (termCount == 0) return 0;

			//get the number of variables in the term
	int varCount = data0[0][0].length();
	int newCount = 0;

			//compare the elements in the list
	for (int i = 0; i < termCount-1; i++) {
		for (int j = i + 1; j < termCount; j++) {
				
					//set up new string to put in the new table
			string newString; 
				
					//record the differences
			int diff = 0; 

					//store the status to merge or not
			bool marged = false;

					//compare the characters one by one
			for (int k = 0; k < varCount; k++) {
				
						//replace the reducable characters with underscore
				if (data0[i][0][k] != data0[j][0][k]) {
					newString += "_";

							//change mergable status
					marged = true;

							//increment the number of different characters
					diff++;
				}

						//put unmergable characters in a new string
				else {
					newString += data0[j][0][k];
				}
			}

					//put the string in the new list if there is only one difference		
			if (diff == 1) {

						//put the new string
				data1[newCount][0] = newString;

						//put merged status
				data1[newCount][2] = "F";

						//increment the number of 
				newCount++;

						//change the merged status
				if (marged == true) {
					data0[i][2] = data0[j][2] = "T";
				}
			}
		}
	}
			//eliminate duplicates in new array before returning
	newCount = mergeDuplicates(newCount, data1);

	return newCount;
}

//*****************************************************************************************************
int mergeDuplicates(int termCount, string data0[][col]) {

		// Receives � a table with terms and number of terms
		// Task - eliminate the duplicate data and keep only one
		// Returns - table without duplicate terms and new term count

			//look for duplicate elements
	for (int i = 0; i < termCount; i++) {
		for (int j = i + 1; j < termCount; j++) {
					
					//eliminate the second one if duplicate is found
			if (data0[i][0] == data0[j][0]){

						//reduce the table size
				termCount--;

						//delete the second duplicate element
				data0[j][0] = { NULL };
				data0[j][1] = { NULL };
				data0[j][2] = { NULL };

						//replace the duplicate with the next element and move table forward
				for (int k = j; k < termCount; k++) {
					data0[k][0] = data0[k + 1][0];
					data0[k][1] = data0[k + 1][1];
					data0[k][2] = data0[k + 1][2];
				}
			}
		}
	}

	return termCount;
}

//*****************************************************************************************************
int getUnmarged(string data0[][col], string data1[][col], string data2[][col], string data3[][col],
				 string newData[][terms+1]) {

		// Receives � data tables with elements
		// Task - look for unmarged terms and put in one new list
		// Returns - the new list with unmarged terms
		
			//keep track of new data count
	int newCount = 0;
	for (int i = 0; i < terms; i++) {

				//check in list 0
		if (data0[i][2] == "F") {

					//put it in the new list if found
			newData[newCount][12] = data0[i][0];
			newCount++;
		}

				//check in list 1
		if (data1[i][2] == "F") {

					//put it in the new list if found
			newData[newCount][12] = data1[i][0];
			newCount++;
		}

				//check in list 2
		if (data2[i][2] == "F") {

					//put it in the new list if found
			newData[newCount][12] = data2[i][0];
			newCount++;
		}

				//check in list 3
		if (data3[i][2] == "F") {
					
					//put it in the new list if found
			newData[newCount][12] = data3[i][0];
			newCount++;
		}
	}

	return newCount;
}

//*****************************************************************************************************
int getResults(string original[], string reduced[][terms+1], string result[], int orig, int redu) {
	
		// Receives � original, reduced, and a new data lists; and their counts
		// Task - get resutls from original data and reduced data lists
		// Returns - list of results and number of out results

			//set up result count
	int resultCount = 0;

	for (int i = 0; i < redu; i++) {

				//go through the original terms
		for (int j = 0; j < orig; j++) {
			int	match = 0;

					//look for matches and unmatches
			for (int k = 0; k < reduced[i][12].length(); k++) {
				if (reduced[i][12][k] == original[j][k] ||
					reduced[i][12][k] == '_') {

							//record number of matches
					match++;
				}
				
			}
					//place X where the matches are
			if(match==reduced[i][12].length()) reduced[i][j] = "X";
		}
	}

			//get the matches as a list
	int reduNum = 0;
	for (int i = 0; i < orig; i++) {
		int count = 0; int reduNum = 0;

				//scan through reduced elements
		for (int j = 0; j < redu; j++) {
				
					//get the number of reduced terms with only X in colomn
			if (reduced[j][i] == "X") {
				reduNum = j;

						//keep track of the number
				count++;
			}

					//put the results into the result array
			if (j == redu - 1 && count == 1) {

						//remove the underscores before putting in
				result[resultCount] = removeUnderscores(reduced[reduNum][12]);
				resultCount++;
			}
		}
	}

			//prepare the result list to pass back
	for (int i = 0; i < resultCount; i++) {
		for (int j = i + 1; j < resultCount; j++) {

					//eliminate the second one if duplicate is found
			if (result[i] == result[j]) {

						//reduce the table size
				resultCount--;

						//delete the second duplicate element
				result[j] = { NULL };
	
						//replace the duplicate with the next element and move table forward
				for (int k = j; k < resultCount; k++) {
					result[k] = result[k + 1];
				}
			}
		}
	}

	return resultCount;
}

//*****************************************************************************************************
string removeUnderscores(string data) {

		// Receives � get a string with or without understores
		// Task - removes the underscores
		// Returns - a new string without underscores

			//set up a new string
	string newString = "";

			//check all the characters in the string
	for (int i = 0; i < data.length(); i++) {

				//remove the underscore if there is one
		if (data[i] != '_') {
			newString += data[i];
		}
	}

	return newString;
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
