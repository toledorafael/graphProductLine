/**
 * @file Node.h
 * 
 * @author Rafael Toledo
 * @date 2021-10-01
 * 
 * @brief Defines the class Node and its attributes and methods
 */

#ifndef GRAPH_APP_NODE_H
#define GRAPH_APP_NODE_H

#include <string>
#include <vector>

class Node {
	public:
	/**	Constructors/Destructors */
	Node(std::string nodeName);
	~Node();

	/** Editing Node Methods */
	void addNeighbor(int neighborID);

	/** Accessor methods */
	int getID(){ return nodeID; };
	int getValue(){ return value; };
	std::string getName(){ return nodeName; };
	std::vector<int> getNeighbors() {return neighbors;}

	/** Mutator methods */
	void setValue(int newValue) {value = newValue;}

	private:
	int nodeID;
	int value;
	std::string nodeName;
	std::vector<int> neighbors;



	static int nodeCount;
};

#endif //GRAPH_APP_NODE_H