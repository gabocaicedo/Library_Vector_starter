#include "../includes_usr/fileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <ctype.h>
//#include <algorithm>
const char CHAR_TO_SEARCH_FOR = ',';

using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char *filename) {
	ifstream bookistream;
	bookistream.open(filename, ios::in); // create input stream of books from file
										 //with name filename

	if (!bookistream.is_open())
		return COULD_NOT_OPEN_FILE; // return could not open const if couldnt open file

	if (books.size() == 0)
		return NO_BOOKS_IN_LIBRARY;	// returns no books in library const if books vector is empty


	std::string book1;		//string to hold a a book that is read in
	std::string bookID;
	std::string state;
	std::string ltpatronID;

	book abook;	// book struct to be loaded with string book1 and pushed back onto vector books
	std::stringstream ss;

	while (!bookistream.eof()) {
		getline(bookistream, book1); //get first line (first book) from file
		ss.str(book1);

		books.clear();	//clear vector of books

		std::getline(ss, bookID, CHAR_TO_SEARCH_FOR); //get string in line up to "," and store in string bookID
		abook.book_id = stoi(bookID); // convert bookID string to int and set book_id field of struct to converted string bookID

		std::getline(ss, abook.title, CHAR_TO_SEARCH_FOR); //get title

		std::getline(ss, abook.author, CHAR_TO_SEARCH_FOR); // get author

		std::getline(ss, state, CHAR_TO_SEARCH_FOR); //get checkout state
		if (stoi(state) == 1)
			abook.state = (IN);

		std::getline(ss, ltpatronID, CHAR_TO_SEARCH_FOR); // get patronID that book is loaned to
		abook.loaned_to_patron_id = stoi(ltpatronID);

		books.push_back(abook); //add a book struct to book vector
		ss.clear();				//clear ss stream reading in lines of book file

	}

	bookistream.close();
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char *filename) {
	ofstream bookostream;
	bookostream.open(filename, ios::in);
	if (!bookostream.is_open())
		return COULD_NOT_OPEN_FILE; // check if file open

	if (books.size() == 0)
		return NO_BOOKS_IN_LIBRARY; // check library

	std::string book;
	for (int i = 0; i < books.size(); ++i) {

		book = to_string(books[i].book_id) + "," + books[i].title + ","
				+ books[i].author + "," + to_string(books[i].state) + ","
				+ to_string(books[i].loaned_to_patron_id); //feed all fields of book struct into a string so that we can write to file
														   // must convert book_id, state, and loaned_to_patron_id to strings

		bookostream << book << std::endl; // write back to ofstream bookostream

		/*vector<book>::iterator myBookVectorIterator;
		 for(myBookVectorIterator = books.begin();
		 myBookVectorIterator != books.end();
		 myBookVectorIterator++){

		 bookostream << myBookVectorIterator;*/

	}
	if (bookostream.is_open())
		bookostream.close(); //close output stream if open
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char *filename) {
	ifstream patronistream;
	patronistream.open(filename, ios::in);

	if (!patronistream.is_open())
		return COULD_NOT_OPEN_FILE; // return could not open constant if couldn't open input patron file

	if (patrons.size() == 0) {
		return NO_PATRONS_IN_LIBRARY; // returns no patrons in library constant if patron vector is empty
	}

	std::string patron1;
	std::string patronID;
	std::string numbooks;
	patron apatron;
	std::stringstream ss;

	while (!patronistream.eof()) {
		getline(patronistream, patron1);
		ss.str(patron1);

		patrons.clear();

		std::getline(ss, patronID, CHAR_TO_SEARCH_FOR);
		apatron.patron_id = stoi(patronID);

		std::getline(ss, apatron.name, CHAR_TO_SEARCH_FOR);

		std::getline(ss, numbooks, CHAR_TO_SEARCH_FOR);
		apatron.number_books_checked_out = stoi(numbooks);

		patrons.push_back(apatron);
		ss.clear();
	}

	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char *filename) {
	ofstream patronostream;
	patronostream.open(filename, ios::in);
	if (!patronostream.is_open())
		return COULD_NOT_OPEN_FILE;

	if (patrons.size() == 0)
		return NO_PATRONS_IN_LIBRARY;

	std::string patron;
	for (int i = 0; i < patrons.size(); ++i) {
		patron = to_string(patrons[i].patron_id) + "," + patrons[i].name + ","
				+ to_string(patrons[i].number_books_checked_out);

		patronostream << patron << std::endl;

	}
	if (patronostream.is_open()) {
		patronostream.close();
	}

	return SUCCESS;
}
