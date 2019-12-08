#include <iostream>
#include <cassert>
#include <ostream>
#include <cmath>
#include <functional>
#include "TreeMap.h"


TreeMap<int, std::string> *tree = new TreeMap<int, std::string>();


void testConstructorEmpty() {
	assert (tree->isEmpty());
	assert (tree->length() == 0);
	std::cout << "Run constructor test\n";
}

void testPut() {
	tree->put(5, "A");
	tree->put(1, "B");
	tree->put(8, "C");
	tree->put(8, "D");
	assert (tree->isEmpty() == false);
	assert (tree->length() == 3);
	std::cout << "Run put test\n";
}

void testGet() {
	assert(tree->get(1).value() == "B");
	assert(tree->get(5).value() == "A");
	assert(tree->get(8).value() == "D");
	assert(tree->get(11).validation() == false);
	std::cout << "Run get test\n";
}

void testPutBalanced() {
	TreeMap<int, std::string> tree1 = TreeMap<int, std::string>();
	tree1.put(6, "A");
	tree1.put(8, "B");
	tree1.put(9, "C");
	tree1.put(11, "D");
	tree1.put(12, "E");
	tree1.put(5, "F");
	tree1.put(4, "G");
	tree1.put(3, "H");
	std::cout << "Run put balanced test\n";
	//tree1.printTree();
}


void testSorted() {
	TreeMap<int, std::string> tree1 = TreeMap<int, std::string>();
	tree1.put(6, "A");
	tree1.put(8, "B");
	tree1.put(9, "C");
	tree1.put(11, "D");
	std::cout << "Run sorted test\n";
	tree1.printTree();
	std::cout << "print sorted values\n";
	for (auto e : tree1.sorted())
		std::cout << e.first <<" "<< e.second<<" ";
}

void testCopyConstructor() {
	TreeMap<int, std::string> tree1 = TreeMap<int, std::string>();
	tree1.put(6, "A");
	tree1.put(8, "B");
	tree1.put(9, "C");
	tree1.put(11, "D");
	std::cout << "\nRun Copy constructor test\n";
	TreeMap<int, std::string> copiedTree = tree1;
	copiedTree = *tree;
	copiedTree.printTree();
}



int main() {
	std::cout << "---------------------\n";
	testConstructorEmpty();
	testPut();
	testGet();
	testPutBalanced();
	testSorted();
	testCopyConstructor();
	
	std::cout << "\n";
	printf("tests passed\n");
	std::cout << "---------------------\n";
	delete tree;
	return 0;
}
