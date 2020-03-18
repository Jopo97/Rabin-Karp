//Rabin Karp implementation created by Jonah McElfatrick for CMP201 Data structures and Algorithms 1
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <chrono>
#include <thread>
using namespace std;

#define alph 256

//Clock benchmarking
using chrono::duration_cast;
using chrono::milliseconds;
using this_thread::sleep_for;
typedef chrono::steady_clock the_clock;

list<int> FindingOcurrencesHash(string txt, string pattern) {
	//
	int txtHash = 0, patternHash = 0;

	//Stores the number of times that the search loop is run
	int operations = 0;

	//prime number for the reduction of false hits when comparring hashes
	const int prime = 101;

	// h = multiplier for MSB
	int h = 1;
	
	//List of positions for each found position the pattern is at
	list<int> found_positions;

	//stringSize variable for comparring the size of the pattern to the string
	int stringSize = 0;

	//Length of the text being processed and the length of the pattern being searched for
	int txtLength = txt.length();
	int patternLength = pattern.length();

	// Calculate h, the value of h would be eqv to pow(alph, patternLength-1)
	for (int i = 0; i < patternLength - 1; i++) {
		h = (h * alph) % prime;
	}

	// Calculate the starting hash for the text at the start of the whole text string and then the pattern hash
	// The text hash will change frequently later on when comparring the two
	for (int i = 0; i < patternLength; i++) {
		txtHash = ((alph * txtHash) + txt[i]) % prime;
		patternHash = ((alph * patternHash) + pattern[i]) % prime;
	}

	for (int i = 0; i <= txtLength - patternLength; i++) {
		operations++;
		//show_context(txt, i);
		// Compare the hash of the text and the pattern
		if (txtHash == patternHash) {
			//Hashes match, therefore now check if characters match up
			for (int j = 0; j < patternLength; j++) {
				//Variable for the size of the string being comparred
				stringSize = j + 1;
				if (txt[i + j] != pattern[j]) {
					break;
				}
			}
			//Checks to see if the length of the pattern is equal to the length of the string being compared
			if (stringSize == patternLength) {
				found_positions.push_back(i);
			}
		}
		// calculating the next hash for the next part of txt
		txtHash = ((alph * (txtHash - (h * txt[i]))) + txt[i + patternLength]) % prime;
		if (txtHash < 0) {
			txtHash = txtHash + prime;
		}
	}
	return found_positions;
}

string inputFile() {
	//Variables for the contents of the file
	string contents;

	//Opens the text file for reading
	ifstream file("100000names.txt", ios_base::binary);

	//Check to see if the TextFile.txt file was sucessfully opened
	if (!file.good()) {
		cout << "Unable to open Text File";
		system("pause");
		exit(1);
	}

	//Finds end of text file
	file.seekg(0, ios::end);

	contents.reserve(file.tellg());

	//Finds beginning of text file
	file.seekg(0, ios::beg);

	//Assigns the contents of the text file to the string variable contents
	contents.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

	//Closes the input textfile
	file.close();

	return contents;
}

int main() {
	//Variables for holding the text contents of a file, and the pattern that is being searched for
	string txt;
	string pattern = "logan5@gmail.co.uk";;

	//Fills the variable 'txt' with the values from the text file
	txt = inputFile();

	// Start timing
	the_clock::time_point start = the_clock::now();

	list<int> Found_Positions = FindingOcurrencesHash(txt, pattern);

	// Stop timing
	the_clock::time_point end = the_clock::now();

	if (Found_Positions.size() == 0) {
		//If there was no found occurrences of the email then a message stating no emails were found is displayed
		cout << "No occurrences of " << pattern << " were found in the file." << endl;
	}
	else {
		//The number of found emails and there positions will be displayed
		for (auto p : Found_Positions) {
			cout << p << "\n";
		}
		cout << "Found " << Found_Positions.size() << " occurrences of '" << pattern << "' at positions:" << endl;
	}

	//Calculate the time taken for the search function to finish
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "It took " << time_taken << " ms to find all occurrences of " << pattern << "." << endl;

	//Pauses the interface when the results have been outputted to the screen
	system("pause");
	return 0;
}