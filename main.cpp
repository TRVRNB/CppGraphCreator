#include <iostream>
#include <string>
#include <map> // associative array / dict
#include <vector>
#include <cmath> // for INFINITY
#include <ranges> // lets me iterate over map keys
#include "terminal_format.cpp"
// NOTE:
// this program uses some fairly new C++ features
// make sure your GCC (or any other compiler) is updated
// then, if your default C++ version is lower than C++20 (mine is C++17):
// g++ -std=c++20 main.cpp -o a
// you can also use C++23, 26, etc. (GCC already has experimental support for C++26 to my knowledge)
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
    float get_connection(unsigned short index); // returns weight
    void set_connection(unsigned short index, float weight);
    void remove_dead_connections();
  protected:
    map<Node*, float> connections = {}; // populate this with connections to other nodes (node pointer is key)
  };
  vector<Node*> nodes; // unwrapped vector to every node (use this to convert index to pointer)
  void update_connections();
  const string version = "1.1";
  const float default_weight = INFINITY; // untraversable weight
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
  Node* current_node = nodes[index];
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
  Node* current_node = nodes[index];
  if (!connections.contains(current_node)) connections[current_node] = strength;
  else cout << RED << "There is already a connection between these nodes." << RESET << endl;
}

bool contains(vector<Graph::Node*> to_check, Graph::Node* to_find){
  // O(n) vector search
  // i wanted to use std::any, which works in C++17 and after, this already requires C++20, but any doesn't support direct comparison (even if both data types are identical)
  for (Graph::Node* x : to_check)
    if (x == to_find)
      return true;
  return false;
}

void Graph::Node::remove_dead_connections(){
  while (connections.size() > nodes.size()){
    // find which keys don't exist
    for (Node* key : views::keys(connections)) // std::ranges is needed for this
      if (!contains(nodes, key)) // this node doesn't exist anymore
	  connections.erase(key);
  }
}


  
void Graph::update_connections(){
  // call this when a node is added or deleted
  // iterates over every node in Graph::nodes, adds missing connections (with default weight) and removes connections that should not exist
  for (Node* current_node : nodes){
    // first, make sure every single connection exists
    for (int i = 0; i < nodes.size(); i++)
      if (current_node->get_connection(i) == -1.0)
	current_node->set_connection(i, default_weight); 
    // now, make sure no invalid connections exist
    current_node->remove_dead_connections();
  }
}

int main(){
  cout << YELLOW << "C++ Graph Creator - " << RESET << RED << "Version " << Graph::version << RESET << endl;
}
