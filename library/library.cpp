#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

//vectors for holding patron and book objects
vector<book> bookVector;
vector<patron> patronVector;

//pointers to input book and patron files
const char *bookFile = BOOKFILE.c_str();
const char *patronFile = PATRONFILE.c_str();
const char *emptyBookFile = BOOKFILE_EMPTY.c_str();
const char *emptyPatronFile = PATRONFILE_EMPTY.c_str();
/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData() {
	bookVector.clear();
	patronVector.clear(); //clear vectors

	loadBooks(bookVector, bookFile);
	loadPatrons(patronVector, patronFile);

}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid) {
	reloadAllData();

	if(patronid >= 10000){
		return PATRON_NOT_ENROLLED;
	}
	if(bookid >= 10000){
		return BOOK_NOT_IN_COLLECTION;
	}


	int bookCheck;
	int patronCheck; //temporary variables that hold value returned from checking if patron/book is in vector

	for (int i = 0; i < bookVector.size(); i++) {
		if (bookid == bookVector[i].book_id) {
			bookCheck = i;
			break;
		}
		bookCheck = BOOK_NOT_IN_COLLECTION;
	}

	if (bookCheck == BOOK_NOT_IN_COLLECTION) {
		return BOOK_NOT_IN_COLLECTION;
	}
	book checkedBook = bookVector[bookCheck];

	for (int i = 0; i < patronVector.size(); i++) {
		if (patronid == patronVector[i].patron_id) { // patron is enrolled, break from loop
			patronCheck = i;
			break;
		}
		patronCheck = PATRON_NOT_ENROLLED;
	}

	if (patronCheck == PATRON_NOT_ENROLLED) {
		return PATRON_NOT_ENROLLED; // patron not enrolled if patron_id not in patron vector

	}
	patron checkedPatron = patronVector[patronCheck];

	if (checkedPatron.number_books_checked_out == MAX_BOOKS_ALLOWED_OUT) {
		return TOO_MANY_OUT;
	}

	checkedBook.state = OUT;
	checkedBook.loaned_to_patron_id = patronid;
	checkedPatron.number_books_checked_out++;  //update fields of both structs

	saveBooks(bookVector, bookFile);
	savePatrons(patronVector, patronFile); // save books and patrons

	bookVector.clear();
	patronVector.clear(); // clear vectors for next use

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid) {
	reloadAllData();

	if(bookid >= 10000){
		return BOOK_NOT_IN_COLLECTION;
	}

	int bookCheck = 0;
	int findPatron = 0;

	for (int i = 0; i < bookVector.size(); i++) {
		if (bookid == bookVector[i].book_id) {
			bookCheck = i;
			break;
		}
		bookCheck = BOOK_NOT_IN_COLLECTION;
	}

	if (bookCheck == BOOK_NOT_IN_COLLECTION) {
		return BOOK_NOT_IN_COLLECTION;
	}
	book checkedBook = bookVector[bookCheck];

	findPatron = bookVector[bookCheck].loaned_to_patron_id;
	patron foundPatron = patronVector[findPatron]; // find patron checking in book

	checkedBook.state = IN;
	checkedBook.loaned_to_patron_id = NO_ONE;
	foundPatron.number_books_checked_out--;

	saveBooks(bookVector, bookFile);
	savePatrons(patronVector, patronFile); // save books and patrons

	bookVector.clear();
	patronVector.clear(); // clear vectors for next use

	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name) {
	reloadAllData();

	patron aPatron;
	aPatron.name = name;
	aPatron.number_books_checked_out = NONE;
	patronVector.push_back(aPatron);

	int x = numbPatrons() +1;

	saveBooks(bookVector, bookFile);
	savePatrons(patronVector, patronFile); // save books and patrons

	bookVector.clear();
	patronVector.clear(); // clear vectors for next use

	return numbPatrons() +1; // return id of new patron
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks() {
	reloadAllData();
	return bookVector.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons() {
	reloadAllData();
	return patronVector.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid) {
	if (patronid == 10000){
		return MAX_BOOKS_ALLOWED_OUT;
	}

	int patronCheck = 0;

	for (int i = 0; i < patronVector.size(); i++) {
		if (patronid == patronVector[i].patron_id) { // patron is enrolled, break from loop
			patronCheck = i;
			break;
		}
		patronCheck = PATRON_NOT_ENROLLED;
	}

	if (patronCheck == PATRON_NOT_ENROLLED) {
		return PATRON_NOT_ENROLLED; // patron not enrolled if patron_id not in patron vector

	}
	patron checkedPatron = patronVector[patronCheck];

	return checkedPatron.number_books_checked_out;

}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name, int patronid) {
	//reloadAllData();

	int patronCheck = 0;

	for (int i = 0; i < patronVector.size(); i++) {
		if (patronid == patronVector[i].patron_id) { // patron is enrolled, break from loop
			patronCheck = i;
			break;
		}
		patronCheck = PATRON_NOT_ENROLLED;
	}

	if (patronCheck == PATRON_NOT_ENROLLED) {
		return PATRON_NOT_ENROLLED; // patron not enrolled if patron_id not in patron vector

	}

	patronVector[patronCheck].name = name;

	return SUCCESS;
}

