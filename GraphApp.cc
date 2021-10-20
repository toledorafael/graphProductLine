/**
 * @file GraphApp.cc
 * 
 * @author Rafael Toledo
 * @date 2021-10-01
 * 
 * @brief Class that manages the GraphApp and handles the commands 
 */

#include "GraphApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool kWeighted;
bool kDirected;
bool kUndirected;
bool kSearch;
bool kBFS;
bool kDFS;
bool kCycle;
bool kConnectedComps;
bool kPrim;


/**
 * @brief Construct a new Graph App:: Graph App object
 * 
 * @param filename 
 */
GraphApp::GraphApp(string configFilename, string graphFilename) {
    loadConfig(configFilename);
    
    loadGraph(graphFilename);

    setupMenu();

}


/**
 * @brief Destroy the Graph App:: Graph App object
 * 
 */
GraphApp::~GraphApp() {
    //First, deletes the recipes.
    for (int i = 0; i < nodes.size(); i++)
        delete nodes.at(i);
}

/**
 * @brief Sets up the menu of active commands for 
 * the current software product
 * 
 */
void GraphApp::setupMenu() {
    if (kCycle){
        activeCommands.push_back(CYCLE);
    }
    
    if (kConnectedComps){
        activeCommands.push_back(CC);
    } 
        
    if (kPrim){
        activeCommands.push_back(PRIM);
    } 
    
    activeCommands.push_back(HELP);
    activeCommands.push_back(EXIT);
}

/**
 * @brief Loads the feature configuration from provided file
 * 
 * @param configFilename 
 */
void GraphApp::loadConfig (string configFilename){
    ifstream configFile (configFilename);

    if (configFile.is_open()) {
        string line;

        while ( getline (configFile, line) )
        {
            stringstream ss(line);
            vector<string> vect;

            for (string i; ss >> i;) {
                vect.push_back(i);
            }

            string feature = vect[0];
            bool toggleValue = (vect[1][0] == '1');

            if (feature == "kWeighted" ){
                kWeighted = toggleValue;
            } else if (feature == "kDirected" ){
                kDirected = toggleValue;
            } else if (feature == "kUndirected" ){
                kUndirected = toggleValue;
            } else if (feature == "kSearch" ){
                kSearch = toggleValue;
            } else if (feature == "kBFS" ){
                kBFS = toggleValue;
            } else if (feature == "kDFS" ){
                kDFS = toggleValue;
            } else if (feature == "kCycle" ){
                kCycle = toggleValue;
            } else if (feature == "kConnectedComps" ){
                kConnectedComps = toggleValue;
            } else if (feature == "kPrim" ){
                kPrim = toggleValue;
            }

        }
        configFile.close();
    } else cout << "Unable to open configFile" << endl;
}


/**
 * @brief Loads the graph from the provided file
 * 
 * @param graphFilename 
 */
void GraphApp::loadGraph (string graphFilename) {
    ifstream graphFile (graphFilename);
    int nodeCount = 0;
    int edgeCount = 0;

    if (graphFile.is_open()) {
        string line;

        // Read nodes until empty line
        getline (graphFile, line);

        while ( line.size() != 0 )
        {
            Node* node = new Node(line);
            nodes.push_back(node);

            getline (graphFile, line);
        }

        // Read edges until end of file
        while (getline (graphFile, line)) {
            //Extract name of node in the edge
            stringstream ss(line);
            vector<string> vect;

            for (string i; ss >> i;) {
                vect.push_back(i);
            }

            string startNode = vect[0];
            string endNode = vect[1];
            int weight;
            if (kWeighted) {
                weight = stoi(vect[2]);
            }

            for (Node * n1 : nodes){
                for (Node * n2: nodes){
                    if (n1->getName() == startNode &&
                        n2->getName() == endNode) {
                        if (!kWeighted) {
                            n1->addNeighbor(n2->getID());
                            
                            if (kUndirected){
                                n2->addNeighbor(n1->getID());
                            }                    
                        }

                        if (kWeighted) {
                            Edge* edge = new Edge(n1->getID(), n2->getID(), weight);
                            edges[n1->getID()].push_back(edge);

                            if (kUndirected){
                                edges[n2->getID()].push_back(edge);
                            }
                        }
                    
                    }    
                    
                }

            }          
        }
        
        graphFile.close();

    } else cout << "Unable to graphFile" << endl;
}

/**
 * @brief Prints neighbors in the adjacency list all the nodes
 * 
 */
void GraphApp::printNeighbors() {
    for (Node * node : nodes){
        cout << node->getID() << "-" << node->getName() << endl;
        cout << "Neighbors: ";
        for (int neighbor : node->getNeighbors()) {
            cout << neighbor << ",";
        }
        cout << endl;
    }
}

/**
 * @brief Prints all edges elements
 * 
 */
void GraphApp::printEdges() {
    for (size_t i = 0; i < nodes.size(); i++) {
        cout << "Edges from node: " << nodes[i]->getID() << endl; 
        for (Edge * edge : edges[i]){
            cout << edge->getID() << ":" << edge->getStartNodeID() << "-";
            cout << edge->getWeight() << "-";
            cout << edge->getEndNodeID();
            cout << endl;
        }
    }
}

// /**
//  * @brief Traverses the graph in Depth-First search
//  * 
//  * @param nodeID: starting node for the traversal
//  * @param command: operation performed at each traversed node
//  */
// void GraphApp::DFS(int nodeID, string command) {
//     visited[nodeID] = true;

//     if (kWeighted) {
//         for (Edge* edge : edges[nodeID]) {
//             int endNodeID = edge->getEndNodeID();

//             if (kDirected) {
//                 if (visited[endNodeID] == false) {
//                     DFS(endNodeID, command);
//                 }
//             }

//             if (kUndirected) {
//                 int startNodeID = edge->getStartNodeID();
//                 if (startNodeID != nodeID && visited[startNodeID] == false) {
//                     DFS(startNodeID, command);
//                 } else if (endNodeID != nodeID && visited[endNodeID] == false) {
//                     DFS(endNodeID, command);
//                 }
//             }

//         }
        
//     }
    
//     if (!kWeighted){
//         for (int neighborID : nodes[nodeID]->getNeighbors()) {
//             if (visited[neighborID] == false) {
//                 DFS(neighborID, command);
//             }
//         }
//     }
    
// }

/**
 * @brief Traverses the graph in Depth-First search while executing the command
 * 
 * @param nodeID: starting node for the traversal
 * @param command: operation performed at each traversed node
 * @param parent: id of the parent node
 */
bool GraphApp::DFS(int nodeID, int parent, string command) {
    visited[nodeID] = true;

    if (kDirected) {
        if (command == CYCLE) {
            recurStack[nodeID] = true;
        }
    }

    if (kUndirected) {
        if (command == CC) {
            cout << nodeID << " ";
        }
    }

    if (kWeighted) {
        for (Edge* edge : edges[nodeID]) {
            int endNodeID = edge->getEndNodeID();
            
            if (kDirected) {
                if (command == CYCLE){
                    if (visited[endNodeID] == false) {
                        if(DFS(endNodeID, nodeID, CYCLE)) return true;
                    } else if (recurStack[endNodeID]) {
                        return true;
                    }
                }
            }

            if (kUndirected) {
                int startNodeID = edge->getStartNodeID();
                int next;
                if (startNodeID != nodeID) {
                    next = startNodeID;
                } else if (endNodeID != nodeID) {
                    next = endNodeID;
                }

                if (command == CYCLE) {
                    if (visited[next] == false) {
                        if (DFS(next, nodeID, CYCLE)) return true;
                    } else if (next != parent) {
                        return true;
                    }
                }

                if (command == CC) {
                    if (visited[next] == false) {
                        if (DFS(next, 0, CC)) return true;
                    }
                }
            }

        }
    }

    if (!kWeighted){
        for (int neighborID : nodes[nodeID]->getNeighbors()) {
            if (kDirected) {
                if (command == CYCLE) {
                    if (visited[neighborID] == false) {
                        if(DFS(neighborID, nodeID, CYCLE)) return true;
                    } else if (recurStack[neighborID]) {
                        return true;
                    }
                }
            }

            if (kUndirected) {
                if (command == CYCLE) {
                    if (visited[neighborID] == false) {
                        if(DFS(neighborID, nodeID, CYCLE)) return true;
                    } else if (neighborID != parent) {
                        return true;
                    }
                }

                if (command == CC) {
                    if (visited[neighborID] == false) {
                        DFS(neighborID, 0, CC);
                    }
                }
            }

        }
    }

    if(kDirected) {
        if(command == CYCLE){
            recurStack[nodeID] = false;
        }
    }
    return false;

}

/**
 * @brief Traverses the graph in Breadth-First search while executing the command
 * 
 * @param nodeID: starting node for the traversal
 * @param command: operation performed at each traversed node
 */
void GraphApp::BFS(int nodeID, string command) {
    visited[nodeID] = true;

    list<int> queue;
    queue.push_back(nodeID);

    int currentNodeID;

    while(!queue.empty()) {
        currentNodeID = queue.front();
            
        if (kUndirected) {
            if (command == CC) {
                cout << currentNodeID << " ";
            }
        }

        queue.pop_front();
        
        if (kWeighted) {
            for (Edge* edge : edges[currentNodeID]) {
                int endNodeID = edge->getEndNodeID();
                if (kDirected) {
                    if (visited[endNodeID] == false) {
                        visited[endNodeID] = true;
                        queue.push_back(endNodeID);
                    }
                }

                if (kUndirected) {
                    int startNodeID = edge->getStartNodeID();
                    if (startNodeID != nodeID && visited[startNodeID] == false) {
                        visited[startNodeID] = true;
                        queue.push_back(startNodeID);
                    } else if (endNodeID != nodeID && visited[endNodeID] == false) {
                        visited[endNodeID] = true;
                        queue.push_back(endNodeID);
                    }
                }

            }
        }
        
        if (!kWeighted){
            for (int neighborID : nodes[currentNodeID]->getNeighbors()) {
                if (visited[neighborID] == false) {
                    visited[neighborID] = true;
                    queue.push_back(neighborID);
                }
            }
        }
    }
    

}

// /**
//  * @brief Traverses the all the nodes in the graph performing a command(DFS or BFS)
//  * 
//  * @param command: operation that is supposed to perform during traversal
//  */
// void GraphApp::traverse(string command) {
//     //reset visited map
//     clearVisited();
    
//     for (Node * node : nodes) {
//         if (visited[node->getID()] == false) {
//             if (kDFS) {
//                 DFS(node->getID(), command);
//             }
//             if (kBFS) {
//                 BFS(node->getID(), command);
//             }
//         }
//     }

//     clearVisited();
// }

/**
 * @brief Clears the visited map by assigning false to all position
 * 
 */
void GraphApp::clearVisited() {
    //reset visited map
    for (Node *node : nodes) {
        visited[node->getID()] = false;
    }
}

/**
 * @brief Prints the header that is shown when the
 * program is started.
 * 
 * @return int 
 */
int GraphApp::printHeader(){
    string header = "------------------------------------------------------\n";
    header += "| Graph Product Line v0.1                            |\n";
    header += "| C++ Version                                        |\n";
    header += "|                                                    |\n";
    header += "| University of Waterloo, 2021                       |\n";
    header += "------------------------------------------------------\n";

    //Prints the header.
    cout << header;

    //Prints the help text.
    cout << "Sample C++ program that is used to demonstrate how a " <<
         endl << "graph application would work. The program allows for" <<endl << "the execution of graph algorithms " <<
         endl << endl << "Available Commands:" << endl;
    for (string command : activeCommands) {
        if (command != HELP) {
            cout << "- " << command;
            if (command == CYCLE) {
                cout << ": Checks whether the graph include cycles." << endl;
            } else if (command == CC) {
                cout << ": Computes the connected components" << 
                endl << "of an undirected graph, which are equivalence classes under" << 
                endl <<"the reachable-from relation." <<
                endl;  
            } else if (command == SCC) {
                cout << ": Computes the strongly connected " << 
                endl <<"components of a directed graph, which are equivalence" << 
                endl <<"classes under the reachable-from relation." << endl;
            } else if (command == PRIM) {
                cout << ": Computes a Minimum Spanning Tree (MST) using Prim's Algorithm." << endl;
            } else if (command == EXIT) {
                cout << ": Exits the program." << endl;
            }
        }
    }
  
    return 1;
}

/**
 * @brief Traverses the graph looking for cycles. It requires kDFS
 * 
 * @return true if the graph has cycles
 * @return false if the graph doesn't have cycles
 */
bool GraphApp::isCyclic() {
    //reset visited map
    if (kDFS) {
        clearVisited();
        for (Node * node : nodes) {
            if(visited[node->getID()] == false){
                if (DFS(node->getID(), -1, CYCLE)) {
                    return true;
                }
            }
        }
        return false;
    } else {
        cout << "DFS not enabled!" << endl;
        return false;
    }
    
}

/**
 * @brief Traverses the graph printing the connected components
 * 
 */
void GraphApp::connectedComponents() {
    if (kSearch && kUndirected) {
        clearVisited();
        int compNum = 0;
        for (Node * node : nodes) {
            if(visited[node->getID()] == false){
                cout << "Component " << compNum+1 << ": ";
                if (kDFS) {
                    DFS(node->getID(), 0, CC);;
                }

                if (kBFS) {
                    BFS(node->getID(), CC);
                }
                cout << endl;
                compNum++;
            }
        }
    } else {
        cout << "Feature not enabled!" << endl;
    }
}

/**
 * @brief Find the Minimum Spanning Tree of the Graph using Prim's Algorithm
 * 
 */
void GraphApp::MSTPrim() {
    if (kWeighted && kUndirected) {
        vector<Edge*> MST;
        
        clearVisited();
        
        // Initialize node values
        for (int i=0; i < nodes.size(); i++){
            if (i == 0){
                nodes[i]->setValue(0);
            } else {
                nodes[i]->setValue(INT_MAX);
            }
        }

        int currentNodeID = 0;
        visited[currentNodeID] = true;
        int count = 1;
        int next;
        int total = 0;

        while (count < nodes.size()) {
            int min = INT_MAX;
            int minIndex;
            Edge * minEdge;
            for (Node * node : nodes) {
                currentNodeID = node->getID();
                if (visited[currentNodeID]) {
                    for (Edge * edge : edges[currentNodeID]) {
                        
                        next = edge->getNext(currentNodeID);

                        if(visited[next] == false && edge->getWeight() < nodes[next]->getValue()) {
                            nodes[next]->setValue(edge->getWeight());
                        }

                        if(visited[next] == false && nodes[next]->getValue() < min) {
                            min = nodes[next]->getValue();
                            minIndex = next;
                            minEdge = edge;
                        }
                    }
                }
            }
            

            visited[minIndex] = true;
            total += min;

            MST.push_back(minEdge);

            count++;
        }

        cout << "MST edges:" << endl;
        for (Edge * edge : MST){
            cout << edge->getStartNodeID() << "-";
            cout << edge->getWeight() << "-";
            cout << edge->getEndNodeID();
            cout << endl;
        }

        cout << "Total MST weight: " << total << endl;
    } else {
        cout << "Feature not enabled!" << endl;
    }
}

/**
 * @brief Runs the main portion of the program.
 *        Takes in commands from the user and
 *        performs the appropriate action. 
 */
void GraphApp::handleCommands() {
    string command;

    //Start by printing the header.
    printHeader();

    //Iterate until the stop command is reached.
    bool iterate = true;
    while(iterate){
        //Places a new line.
        cout << endl;

        //Prompt for input.
        cout << "> ";
        getline(cin, command);

        //Check commands
        if (command == HELP){
            printHeader();
        } else if (command == CYCLE) {
            if (kCycle) {
                if (isCyclic()) {
                    cout << "Graph contains cycle!" << endl;
                } else {
                    cout << "Graph doesn't contain cycle" << endl;
                }
            } else {
                cout << "Feature not enabled!" << endl;
            }
        } else if (command == CC) {
            if (kConnectedComps) {
                connectedComponents();
            } else {
                cout << "Feature not enabled!" << endl;
            }
        } else if (command == PRIM && kPrim) {
            if (kPrim) {
                MSTPrim();
            } else {
                cout << "Feature not enabled!" << endl;
            }
        } else if (command == LOAD) {
            //TODO: implement loading a graph from specified file
        } else if (command == EXIT) {
            iterate = false;
        } else if (command == "") {
            continue;
        } else {
            //Default
            cout << command << ": command not recognized." << endl;
        }
    }
}
