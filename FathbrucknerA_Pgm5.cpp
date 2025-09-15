// -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -= -
// Program: Using linked list keep track of list of categories and its items from a file, be able to display the categories, the categories list, add a category,and delete categories. 
// Process: Using a linked list of categories and items this programs keeps track of lists of items from a file and input, it can delete a category and items, look up all categories, look up a list of a category, and add a category through functions.
// Results: There are 8 categories the only category with no items in it are Frozen. Candy is at the top of the list and Produce is at the bottom. Produce has apples, bannanas, cabbage, and carrots in it.
// Class: CS2020
// Section: 1002
// Term: Spring 2025
// Author: Austin Fathbruckner
//
// Reflection: I had some issues figureing out how to add items to a linked list from a file and by input. I had to figure out how to change the head of the list when needed and when else to move the point so it can be added. I figure out to find how to add aphabetically by finding where the point is less than the added name. One issue I had was figureing out how todo the substr() and find() but once I did it was very useful. 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

const string DATA = "Pgm5.txt"; //sets the name of the input file

enum MenuOption { PRINT_CATEGORY = 1, PRINT_ITEMS = 2, REMOVE_CATEGORY = 3, ADD_CATEGORY = 4, QUIT = 5 }; //enumeration of the menu options

class Item //class for all items of a category
{
public:
	string desc; //item in a class
	Item* pNextItem; //pointer to the next item
	Item(string desc) { //constructor 
		this->desc = desc;
		pNextItem = nullptr;
	}  
};

class Category //name of categories for the items 
{
public:
	string name; //category name
	Category* pNextCat; //pointer to the next category
	Item* pItemHead; //head of the linked list
	Category(string name) { //constructor
		this->name = name;
		pNextCat = nullptr;
		pItemHead = nullptr;
	} 
};

void createList(Category*&); //declares functions so functions can be after main
void appendItem(Item*&, string);
void insertCategory(Category*&, Category*);
void printItems(Category*, string);
void printCategoryList(Category*);
void parseInput(string&, string&, string&);
void parseItems(string&, Item*&);
void destroyItemList(Item*&);
void destroyCategoryList(Category*&);
int menu();
void removeCategory(Category*&, string);

int main() {
	Category* pHead = nullptr; 
	createList(pHead); //creates list from inputfile
	int option; 
	string input;
	string cateName;
	string items;

	do {
		option = menu(); //displays menu until and selects option untill quit is chosen 
		if (option == PRINT_CATEGORY) {
			printCategoryList(pHead); //prints categories
		}

		else if (option == PRINT_ITEMS) {
			cout << "Item category? "; //ask for category then prints the categories list of items
			cin.ignore();
			getline(cin, input); 
			printItems(pHead, input);
		}

		else if (option == ADD_CATEGORY) {
			cout << "Item category and items: "; //adds a category and its items by user input
			cin.ignore();
			getline(cin, input);
			parseInput(input, cateName, items);
			Category* check = pHead;
			bool exisits = false;

			while (check) { //wont add to list if already in list
				if (check->name == cateName) {
					cout << cateName << " already in list" << endl;
					exisits = true;
					break;
				}
				check = check->pNextCat;
			}

			if (!exisits) { //adds category if not in category list
				Category* newCate = new Category(cateName);
				insertCategory(pHead, newCate);
				parseItems(items, newCate->pItemHead);
			}
		}

		else if (option == REMOVE_CATEGORY) {
			cout << "Enter category: "; //removes a category and its list items by user input
			cin.ignore();
			getline(cin, input);
			removeCategory(pHead, input);
		}

	} while (option != QUIT);
	
	destroyCategoryList(pHead); //deletes all linked lists
	return 0;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: createList
// Description: reads a file to create a aphabetical linked list of all categories and its associated items
// Parameters: passes the head pointer so the function knows where the list starts
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void createList(Category*& pHead) {
	ifstream inFile(DATA);
	string data;
	string cate;
	string items;

	if (!inFile) { //checks that the file can be opened 
		cout << "Error opening file." << endl;
		return;
	}
	
	while (getline(inFile, data)) { //reads the file to find the categories and its list of items then adds to the likned list then closes the file when done reading the file
		parseInput(data, cate, items); 
		Category* newCate = new Category(cate);
		insertCategory(pHead, newCate);
		parseItems(items, newCate->pItemHead);
	}
	inFile.close(); 
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: appendItem
// Description: adds a item to a categories item list
// Parameters: passes the linked list of the item list and the name of the item being addded
// Returns: void
// Author: Austin Fathbrukner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void appendItem(Item*& pItemHead, string desc) {
	Item* newItem = new Item(desc);

	Item* check = pItemHead;

	if (!pItemHead || pItemHead->desc > desc) { //if nothing or should be above the head item add to the listtop
		newItem->pNextItem = pItemHead;
		pItemHead = newItem;
		return;
	}

	Item* pCurr = pItemHead;

	while (pCurr->pNextItem && pCurr->pNextItem->desc < desc) { //goes through the list to find where item should be added
		pCurr = pCurr->pNextItem;
	}

	newItem->pNextItem = pCurr->pNextItem; //puts in order
	pCurr->pNextItem = newItem;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: insertCategory
// Description: adds a category to the linked list in alpha order
// Parameters: passes the linked list category list and the category being added 
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void insertCategory(Category*& pHead, Category* newCate) {
	if (!pHead || newCate->name < pHead->name) { //adds to head of list if nothing in the list or is greater than the head
		newCate->pNextCat = pHead;
		pHead = newCate;
		return;
	}

	Category* pCurr = pHead; 
	while (pCurr->pNextCat && pCurr->pNextCat->name < newCate->name) { //finds where the category should be added
		pCurr = pCurr->pNextCat;
	}

	newCate->pNextCat = pCurr->pNextCat; //puts in order
	pCurr->pNextCat = newCate;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: printItems
// Description: prints the items of a category list
// Parameters: passes the linked list category and the name of catgeogry that needs to be found and display item list
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void printItems(Category* pHead, string cate) {
	while (pHead && pHead->name != cate) { //finds the category name in the linked list
		pHead = pHead->pNextCat;
	}

	if (!pHead) { //displays error if it cant find the category 
		cout << "Category not found." << endl;
		return;
	}

	if (!pHead->pItemHead) { //displays that item list is empty if there is nothing in the item list
		cout << cate << "'s item list is empty" << endl;
		return;
	}

	Item* pIlist = pHead->pItemHead;
	string result;

	while (pIlist) { //makes the item list comma seperated and then displays it
		result += pIlist->desc;
		if (pIlist->pNextItem) {
			result += ", ";
		}
		pIlist = pIlist->pNextItem;
	}
	cout << result << endl;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: printCategoryList
// Description: prints the list of categories in the linked list
// Parameters: passes the linked list of the category list
// Returns: What value is returned from this function
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void printCategoryList(Category* pHead) { //prints the list of categories 
	int i = 1;
	cout << setfill('=') << setw(13) << "" << endl;
	cout << "Categories: " << endl;
	while (pHead) {
		cout << setfill(' ') << setw(2) << right << i << ": " << setw(15) << left << pHead->name << endl;
		pHead = pHead->pNextCat;
		i++;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: parseInput
// Description: seperates the category name from the itmes of the category in the file or from user input
// Parameters: passes the input vale the name of the category and the name(s) of the items of a category
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void parseInput(string& input, string& cate, string& items) {
	size_t pos = input.find(' '); 
	if (pos != string::npos) { //finds the first thing in the list as the category name and the rest is seperated into the items
		cate = input.substr(0, pos);
		items = input.substr(pos + 1);
	}
	else { //if only a category name and has no items makes the items list empty
		cate = input;
		items = "";
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: parseItems
// Description: adds the list of items into the item linked list
// Parameters: passes the list of items and the linked list of the itmes
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void parseItems(string& items, Item*& pItemHead) {
	size_t pos = 0;
	string item;
	
	while ((pos = items.find(' ')) != string::npos) { //adds the item to the item linked list then erases to move to the next item until empty
		item = items.substr(0, pos);
		appendItem(pItemHead, item);
		items.erase(0, pos + 1);
	}

	if (!items.empty()) { //if list empty makes the item linked list empty
		appendItem(pItemHead, items);
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: destroyItemList
// Description: deallocates the memeory used item linked list
// Parameters: passes the item linked list
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void destroyItemList(Item*& pItemHead) {
	while (pItemHead) { //goes though the item linked list and deallocates all of the linked list
		Item* pItem = pItemHead;
		pItemHead = pItemHead->pNextItem;
		delete pItem;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: destroyCategegoryList
// Description: deallocates memoried used by categories linked list
// Parameters: passses the categegory linked list
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void destroyCategoryList(Category*& pHead) {
	while (pHead) { //deallocates the category linked list and the categories item list until empty
		destroyItemList(pHead->pItemHead);
		Category* pCate = pHead;
		pHead = pHead->pNextCat;
		delete pCate;
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: menu
// Description: displays menu of options 
// Parameters: none
// Returns: int the option chosen by input
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int menu() {
	int input;

	do { //will display menu list until an approptiate option in submitted to input
		cout << setfill('=') << setw(13) << "" << endl;
		cout << "1.Print Category" << endl;
		cout << "2.Print Items" << endl;
		cout << "3.Remove Category" << endl;
		cout << "4.Add Category" << endl;
		cout << "5.Quit" << endl;
		cout << "Choice: ";
		cin >> input;

		if (cin.fail() || input < 1 || input > 5) { //displays error if incorrect responce type or out of bound number option
			cout << "ERROR: invalid choice. Please re-enter." << endl;
			cin.clear();
			cin.ignore();
		}
	} while (input < 1 || input > 5);
	return input;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function: removeCategory
// Description: deallocate the memory of a single catgeory and its item linked list
// Parameters: passes the category linked list and the string of the category to be deallocated 
// Returns: void
// Author: Austin Fathbruckner
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void removeCategory(Category*& pHead, string cate) {
	if (pHead->name == cate) { //if the category being removed is the head moves the head then deletes its items and catgeory
		Category* pCate = pHead;
		pHead = pHead->pNextCat;
		destroyItemList(pCate->pItemHead);
		delete pCate;
		cout << cate << " was removed from list." << endl;
		return;
	}

	Category* pCurr = pHead; //look thorugh the linked list for the name
	while (pCurr->pNextCat && pCurr->pNextCat->name != cate) {
		pCurr = pCurr->pNextCat;
	}

	if (pCurr->pNextCat && pCurr->pNextCat->name == cate) { //when found removes the category and its item linked list and displays error if not found
		Category* pCate = pCurr->pNextCat;
		pCurr->pNextCat = pCate->pNextCat;
		destroyItemList(pCate->pItemHead);
		delete pCate;
		cout << cate << " was removed from list." << endl;
	}

	else {
		cout << cate << " is not in the list." << endl;
	}
}