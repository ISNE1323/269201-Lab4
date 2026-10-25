#include <iostream>
#include <string>
#include <random>
#include <limits>
#include <climits>
#include <cstdlib>
#include "tree.h"
using namespace std;

const int INITIAL_COUNT = 10000;   // how many random numbers the first demo inserts
const int MAX_VALUE = 100000;      // random numbers are between 1 and MAX_VALUE

// Random number generator (seeded differently on every run)
mt19937 rng{ random_device{}() };

int randomNumber() {
	uniform_int_distribution<int> dist(1, MAX_VALUE);
	return dist(rng);
}

// Smallest possible height for a tree with n nodes: the least h such that 2^h - 1 >= n.
int bestPossibleHeight(int n) {
	int h = 0;
	long long capacity = 0;          // how many nodes fit in a perfect tree of height h
	while (capacity < n) {
		h++;
		capacity = capacity * 2 + 1;
	}
	return h;
}

// Print how many nodes the tree has and how tall it is.
void printStatus(Tree<int>& tree) {
	cout << "  Nodes: " << tree.size()
	     << " | Height: " << tree.height()
	     << " (best possible for this many nodes: " << bestPossibleHeight(tree.size()) << ")"
	     << endl;
}

// Keep asking until the user types a whole number between low and high.
int readInt(const string& prompt, int low, int high) {
	while (true) {
		cout << prompt;
		int value;
		bool ok = (bool)(cin >> value);
		if (cin.eof()) {             // input ended (e.g. Ctrl+Z) -> stop the program
			cout << endl;
			exit(0);
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');   // throw away the rest of the line
		if (ok && value >= low && value <= high)
			return value;
		cout << "  Please type a whole number from " << low << " to " << high << "." << endl;
	}
}

// Insert 'count' random numbers.
void insertRandom(Tree<int>& tree, int count) {
	for (int i = 0; i < count; i++)
		tree.insert(randomNumber());
}

// Try 'attempts' times: make a random number, and if it is in the tree, delete it.
void deleteRandom(Tree<int>& tree, int attempts) {
	int deleted = 0;
	for (int i = 0; i < attempts; i++) {
		int number = randomNumber();
		if (tree.search(number) != 0) {
			tree.findAndDelete(number);
			deleted++;
		}
	}
	cout << "  Tried " << attempts << " random numbers, " << deleted
	     << " were in the tree and got deleted." << endl;
}

void printMenu() {
	cout << endl;
	cout << "===== Binary Tree Simulation =====" << endl;
	cout << " 1. Insert random numbers" << endl;
	cout << " 2. Search for a number" << endl;
	cout << " 3. Delete random numbers" << endl;
	cout << " 4. Balance the tree" << endl;
	cout << " 5. Show nodes and height" << endl;
	cout << " 0. Exit" << endl;
}

int main()
{
	Tree<int> tree;

	// ---- Part 1: insert 10000 random numbers, then balance ----
	cout << "Inserting " << INITIAL_COUNT << " random numbers (1 to " << MAX_VALUE << ")..." << endl;
	insertRandom(tree, INITIAL_COUNT);
	cout << "Before balancing:" << endl;
	printStatus(tree);

	tree.balance();
	cout << "After balancing:" << endl;
	printStatus(tree);

	// ---- Part 2: menu ----
	while (true) {
		printMenu();
		int choice = readInt("Choose (0-5): ", 0, 5);

		if (choice == 0) {
			cout << "Bye!" << endl;
			break;
		}
		else if (choice == 1) {
			int count = readInt("How many insertions? ", 1, 1000000);
			insertRandom(tree, count);
			cout << "  Inserted " << count << " random numbers." << endl;
			printStatus(tree);
		}
		else if (choice == 2) {
			int number = readInt("Number to search for: ", INT_MIN, INT_MAX);
			if (tree.search(number) != 0)
				cout << "  " << number << " IS in the tree." << endl;
			else
				cout << "  " << number << " is NOT in the tree." << endl;
		}
		else if (choice == 3) {
			int attempts = readInt("How many deletions? ", 1, 1000000);
			deleteRandom(tree, attempts);
			printStatus(tree);
		}
		else if (choice == 4) {
			tree.balance();
			cout << "  Tree balanced." << endl;
			printStatus(tree);
		}
		else if (choice == 5) {
			printStatus(tree);
		}
	}

	return 0;
}