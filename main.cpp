#include <iostream>
#include <string>
#include <map> // associative array / dict
#include <vector>
#include "terminal_format.cpp"
// NOTE:
// this program uses several fairly new C++ features
// make sure your GCC (or any other compiler) is updated
// then, if your default C++ version is lower than C++20:
// g++ -std=c++20 main.cpp -o a
// you can also use C++23, 26, etc. (GCC already has experimental support for C++26)
//
// C++ Graph Creator
// github.com/TRVRNB/CppGraphCreator.git
// weighted graph (with one-way connections)
// uses Dijkstra's algorithm for traversal (treat weight as distance, similar-ish to Wandering Salesmand but nodes can be revisited?)
// use adjacency table
// EXAMPLE:
//     01  02  03  04
// 01  0   12  ∞   4
// 02  ∞   0   3   ∞
// 03  ∞   6   0   9
// 04  5   2   3   0
// replace the numbers with labels, but add a strict character limit so it can be printed out easily
using namespace std;

namespace Graph {
  // this time i won't make it global
  class Node {
  public:
    unsigned short self_index; // what position in the vector does this node belong to? (CHANGE THIS WHEN DELETING!)
    string label;
    Node(unsigned short self_index1, string label1); // constructor
    void print_connections();
    float get_connection(unsigned short index);
    void set_connection(unsigned short index, float strength); 
  protected:
    map<Node*, float> connections = {}; // populate this with connections to other nodes (node pointer is key)
  };
  vector<Node*> nodes; // unwrapped vector to every node (use this to convert index to pointer)
  string version = "1.0";
}

Graph::Node::Node(unsigned short self_index1, string label1){
  // node constructor
  self_index = self_index1;
  label = label1;
  cout << WHITE << "Node " << label1 << " created." << RESET << endl;
}

float Graph::Node::get_connection(unsigned short index){
  // return this connection's weight, if it exists
  if (index == self_index){
    return -10.0;
  }
  Node* current_node = Graph::nodes[index];
  if (connections.contains(current_node)){
    return connections[current_node];
  } else {
    return -1.0; // treat this as NaN
  }
}

void Graph::Node::set_connection(unsigned short index, float strength){
  // set this connection, if it is valid and doesn't exist
  if (index == self_index){
    cout << RED << "Can't set self-connection." << RESET << endl;
    return;
  }
  Node* current_node = Graph::nodes[index];
  if (!connections.contains(current_node)) connections[current_node] = strength;
  else cout << RED << "There is already a connection between these nodes." << RESET << endl;
}

int main(){
  cout << YELLOW << "C++ Graph Creator - " << RESET << RED << "Version " << Graph::version << RESET << endl;
}
