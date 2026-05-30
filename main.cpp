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
    string label;
    Node(string label1); // constructor
    void print_connections();
    float get_connection(unsigned short index); // returns weight
    void set_connection(unsigned short index, float weight);
    void remove_dead_connections();
  protected:
    map<Node*, float> connections = {}; // populate this with connections to other nodes (node pointer is key)
  };
  vector<Node*> nodes; // unwrapped vector to every node (use this to convert index to pointer)
  void update_connections();
  void add_node(string); // add node to nodes
  Node* lookup_node(string); // look up node by label
  void connect_nodes(string, string);
  const string version = "1.3";
  const float default_weight = INFINITY; // untraversable weight
}

Graph::Node::Node(string label1){
  // node constructor
  label = label1;
  cout << WHITE << "Node \'" << label1 << "\' created." << RESET << endl;
}

float Graph::Node::get_connection(unsigned short index){
  // return this connection's weight, if it exists
  Node* current_node = nodes[index];
  if (connections.contains(current_node)){
    return connections[current_node];
  } else {
    return -1.0; // treat this as NaN
  }
}

void Graph::Node::set_connection(unsigned short index, float strength){
  // set this connection, if it is valid and doesn't already exist
  Node* current_node = nodes[index];
  if (!connections.contains(current_node)) connections[current_node] = strength;
  else cout << RED << "There is already a connection between these nodes." << RESET << endl;
}

bool contains(const vector<Graph::Node*> to_check, Graph::Node* to_find){
  // O(n) vector search
  // i wanted to use std::any, which works in C++17 and after, this already requires C++20, but any doesn't support direct comparison (even if both data types are identical)
  // std::ranges probably has something for this, too
  // i think using a const vector will, in most compilers, pass it by reference but also automatically dereference it in this scope, which should actually improve stack memory performance somewhat
  for (Graph::Node* x : to_check)
    if (x == to_find)
      return true;
  return false;
}

void Graph::Node::remove_dead_connections(){
  // find which keys don't exist
  vector<Node*> to_remove;
  for (Node* key : views::keys(connections)) // std::ranges is needed for this
    if (!contains(nodes, key)) // this node doesn't exist anymore
    to_remove.push_back(key);
  // remove these keys now
  for (Node* key : to_remove)
    connections.erase(key);
}
  
void Graph::update_connections(){
  // call this when a node is added or deleted
  // iterates over every node in Graph::nodes, adds missing connections (with default weight) and removes connections that should not exist
  for (Node* current_node : nodes){
    // first, make sure every single connection exists
    for (int i = 0; i < nodes.size(); i++)
      if (current_node->get_connection(i) == -1.0)
	if (nodes[i] == current_node)
	  current_node->set_connection(i, 0.0);
	else
	  current_node->set_connection(i, default_weight); 
    // now, make sure no invalid connections exist
    current_node->remove_dead_connections();
  }
}

void Graph::add_node(string label){
  // add a node to nodes with this label, no starting connections
  Node* new_node = new Node(label);
  nodes.push_back(new_node);
  update_connections();
}

Graph::Node* Graph::lookup_node(string label){
  // O(n) search through nodes
  for (Node* current_node : nodes)
    if (label == current_node->label)
      return current_node;
  return nullptr;
}

void Graph::connect_nodes(string label1, string label2){
  return;
}

int main(){
  cout << YELLOW << "C++ Graph Creator - " << RESET << RED << "Version " << Graph::version << RESET << endl;
  cout << WHITE << "Type \'HELP\' for a list of commands." << endl;
  map<string, unsigned short> commands;
  commands["HELP"] = 1; commands["ADD"] = 2; commands["CONNECT"] = 3;
  // ^ could use an enum for these values, after some point i might be overengineering this, a long if/else statement would work anyway
  string input;
  bool connection_added = false;
  // main loop
  while (input != "QUIT"){
    string input1;
    string input2;
    cout << GREEN << "$ Enter a command: " << RESET << flush;
    cin >> input;
    switch (commands[input]){ // i don't think C++ switch statements will be any faster than if/else, especially on O3 compilation but i haven't used them much so i might as well try (it might actually be slower since it's checking it against a map but whatever)

    case 0: // ???
      cout << WHITE << "What?" << RESET << endl;
      break;
      
    case 1: // HELP
      cout << WHITE <<
	"HELP: print a list of commands" << '\n' <<
	"QUIT: end the program" << '\n' <<
	"ADD: add a node (vertex) to the graph" << '\n' <<
	"CONNECT: add a connection (line) between two vertices" << '\n' <<
	endl;
      break;
      
    case 2: // ADD
      cout << GREEN << "$ What label do you want this node to have?: " << RESET << flush;
      cin.ignore();
      getline(cin, input1);
      Graph::add_node(input1);
      // add here
      if (Graph::nodes.size() == 2)
	cout << YELLOW << "Remember to add connections between your nodes!" << RESET << endl;
      break;

    case 3: // CONNECT
      if (Graph::nodes.size() < 2){
	cout << RED << "Add more nodes!" << RESET << endl;
	break;
      }
      if (!connection_added)
	cout << YELLOW << "Remember that connections are one-directional!" << RESET << endl;
      cout << GREEN << "$ What is the first node you want to connect?: " << RESET << flush;
      cin.ignore();
      getline(cin, input1);
      cout << GREEN << "$ What is the second node you want to connect?: " << RESET << flush;
      cin.ignore();
      getline(cin, input2);
      Graph::connect_nodes(input1, input2);
      connection_added = true;
      break;
      
    }
  }
  cout << "Goodbye!" << endl;
  return 0;
}
