#include "../includes_usr/fileIO.h"
#include <iostream>
#include <fstream>
#include <string>
//#include <algorithm>
using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	ifstream bookistream;
	bookistream.open(filename, ios::in);  // create input stream of books from file
										  //with name filename

	if(!bookistream.is_open())
		return COULD_NOT_OPEN_FILE; // return could not open const if couldnt open file

	if(books.size() == 0){
		return NO_BOOKS_IN_LIBRARY;	// returns no books in library const if books vector is empty
	}

	string book1;	//string to hold a a book that is read in
	book abook;				// book struct to be loaded with string book1 and pushed back onto vector books
	while(getline(bookistream, book1)){
		std::cout << abook;

		books.push_back(abook);


	}

	bookistream.close();
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	ofstream bookostream;
	bookostream.open(filename, ios::in);

	if(!bookostream.is_open())
			return COULD_NOT_OPEN_FILE;

	if(books.size() == 0)
			return NO_BOOKS_IN_LIBRARY;


	vector<book>::iterator myBookVectorIterator;
	for(myBookVectorIterator = books.begin();
	        myBookVectorIterator != books.end();
	        myBookVectorIterator++){

		bookostream << *myBookVectorIterator;

	}
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	return SUCCESS;
}
