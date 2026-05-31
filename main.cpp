#include <iostream>
#include <string>
#include <map> // associative array / dict
#include <vector>
#include <cmath> // for INFINITY and std:;round
#include <ranges> // for std::views
#include <algorithm> // for std::erase
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
// uses Dijkstra's algorithm for traversal (treat weight as distance)
// use adjacency table
// EXAMPLE:
//     01  02  03  04
// 01  0   12  ∞   4
// 02  ∞   0   3   ∞
// 03  ∞   6   0   9
// 04  5   2   3   0
// labels will be printed beforehand!
using namespace std;

namespace Graph {
  // this time i won't make it global
  class Node {
  public:
    string label;
    Node(string label1); // constructor
    void print_connections();
    float get_connection(unsigned short); // returns weight
    void set_connection(Node*, float);
    void remove_dead_connections();
  protected:
    map<Node*, float> connections = {}; // populate this with connections to other nodes (node pointer is key)
  };
  vector<Node*> nodes; // unwrapped vector to every node (use this to convert index to pointer)
  void update_connections();
  void add_node(string); // add node to nodes
  Node* lookup_node(string); // look up node by label
  int lookup_index(Node*); // look up index by node
  void connect_nodes(string, string, float);
  Node* find_smallest_distance(map<Node*, float>, const vector<Node*>);
  vector<Node*> dijkstra(Node*, Node*);
  float total_weight = 0.0;
  const string version = "1.6";
  const float default_weight = INFINITY; // untraversable weight
}

Graph::Node::Node(string label1){
  // node constructor
  label = label1;
  connections[this] = 0.0; // self by reference
  cout << WHITE << "Node \'" << label1 << "\' created." << RESET << endl;
}

float Graph::Node::get_connection(unsigned short index){
  // return this connection's weight, if it exists
  Node* current_node = nodes[index];
  if (connections.contains(current_node)) return connections[current_node];
  else return default_weight;
}

void Graph::Node::set_connection(Node* current_node, float weight){
  // set this connection, if it is valid and doesn't already exist
  if (current_node->label == label){
    // these are the same node (assume nobody will reuse labels)
    connections[current_node] = 0;
    return;
  }
  if (!connections.contains(current_node) || (connections[current_node] == default_weight || weight >= 1000)){
    connections[current_node] = weight;
    if (weight >= 1000)
      cout << WHITE << "Connection removed between \'" << label << "\' and \'" << current_node->label << "\'." << RESET << endl;
    else
      cout << WHITE << "Connection added between \'" << label << "\' and \'" << current_node->label << "\'." << RESET << endl;
  }
  else cout << RED << "There is already a connection between these nodes." << RESET << endl;
}

int Graph::lookup_index(Node* current_node){
  // O(n) search through nodes with node
  for (int i = 0; i < nodes.size(); i++)
    if (nodes[i] == current_node)
      return i;
  return -1;
}

Graph::Node* Graph::lookup_node(string label){
  // O(n) search through nodes with label
  for (Node* current_node : nodes)
    if (current_node->label == label)
      return current_node;
  return nullptr;
}

void Graph::Node::remove_dead_connections(){
  // find which keys don't exist
  vector<Node*> to_remove;
  for (Node* key : views::keys(connections)) // std::ranges is needed for this
    if (lookup_index(key) == -1) // this node doesn't exist anymore
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
	current_node->set_connection(nodes[i], default_weight); 
    // now, make sure no invalid connections exist
    current_node->remove_dead_connections();
  }
}

void Graph::add_node(string label){
  // add a node to nodes with this label, no starting connections
  Node* new_node = new Node(label);
  nodes.push_back(new_node);
}

void Graph::connect_nodes(string label1, string label2, float weight){
  Node* node1 = lookup_node(label1);
  Node* node2 = lookup_node(label2);
  if (node1 == nullptr || node2 == nullptr){
    cout << RED << "One or both of those nodes doesn't exist!" << RESET << endl;
    return;
  }
  node1->set_connection(node2, weight);
}

void ADD(){
  // ADD function
  string input;
  cout << GREEN << "$ What label do you want this node to have?: " << RESET << flush;
  cin.ignore();
  getline(cin, input);
  Graph::add_node(input);
  if (Graph::nodes.size() == 2)
    cout << YELLOW << "Remember to add connections between your nodes!" << RESET << endl;
  Graph::update_connections();
}

void CONNECT(bool connection_added){
  string input;
  string input1;
  string input2;
  // CONNECT function
  if (Graph::nodes.size() < 2){
    cout << RED << "Add more nodes!" << RESET << endl;
    return;
  }
  if (!connection_added)
    cout << YELLOW << "Remember that connections are one-directional!" << RESET << endl;
  cout << GREEN << "$ What is the first node you want to connect?: " << RESET << flush;
  cin.ignore();
  getline(cin, input);
  cout << GREEN << "$ What is the second node you want to connect?: " << RESET << flush;
  getline(cin, input1);
  cout << GREEN << "$ What should the weight between these two nodes be?: " << RESET << flush;
  cin >> input2;
  float weight = stof(input2);
  if (round(weight) >= 1000 || weight < 1){
    cout << RED << "Weight must be between 1 and 999." << RESET << endl;
    return;
  }
  Graph::connect_nodes(input, input1, weight);
}

void REMOVE(){
  string input;
  // REMOVE function
  if (Graph::nodes.size() < 1){
    cout << RED << "Add more nodes!" << RESET << endl;
    return;
  }
  cout << GREEN << "$ What node do you want to remove?: " << RESET << flush;
  cin.ignore();
  getline(cin, input);
  Graph::Node* to_delete = Graph::lookup_node(input);
  if (to_delete == nullptr){
    cout << RED << "Node \'" << input << "\' not found." << RESET << endl;
    return;
  }
  erase(Graph::nodes, to_delete); // after C++20 this should work
  delete to_delete;
  Graph::update_connections(); // kill all dead connections to this node
  cout << WHITE << "Removed node \'" << input << "\'." << RESET << endl;
}

void DISCONNECT(){
  // DISCONNECT function
  // this is the same as CONNECT but it always uses default_weight
  if (Graph::nodes.size() < 2){
    cout << RED << "Add more nodes!" << RESET << endl;
    return;
  }
  string input;
  string input1;
  cin.ignore();
  cout << GREEN << "$ What is the first node you want to disconnect?: " << RESET << flush;
  getline(cin, input);
  cout << GREEN << "$ What is the second node you want to disconnect?: " << RESET << flush;
  getline(cin, input1);
  Graph::connect_nodes(input, input1, Graph::default_weight);
}

string formatted_index(unsigned short index){
  // format index to always fit the same amount of space
  index++;
  if (index < 10)
    return "  " + to_string(index);
  else if (index < 100)
    return " " + to_string(index);
  else
    return to_string(index);
  // this will break after 999 nodes have been added, but at that point (with adequate connections) the pathfinder will probably take years
}

string formatted_weight(float weight){
  // format weight to always fit the same amount of space
  if (weight >= 1000)
    return "  ∞";
  int iweight = round(weight); // weights will be displayed as int, so they should probably be entered as int
  if (weight < 10)
    return "  " + to_string(iweight);
  else if (weight < 100)
    return " " + to_string(iweight);
  else
    return to_string(iweight);
}

void PRINT(){
  // PRINT function
  for (int i = 0; i < Graph::nodes.size(); i++)
    // first, print the index of every node
    cout << BLUE << (i+1) << ") " << WHITE << Graph::nodes[i]->label << endl;
  cout << WHITE << "---";
  for (int i = 0; i < Graph::nodes.size(); i++)
    cout << "----";
  cout << endl;
  // print adjacency line for a nonexistent node
  cout << "   " << WHITE;
  for (int i = 0; i < Graph::nodes.size(); i++)
    cout << ' ' << formatted_index(i);
  cout << endl;
  // print adjacency line for nodes
  for (int i = 0; i < Graph::nodes.size(); i++){
    Graph::Node* current_node = Graph::nodes[i];
    cout << WHITE << formatted_index(i) << RESET;
    for (int j = 0; j < Graph::nodes.size(); j++)
      cout << ' ' << formatted_weight(current_node->get_connection(j));
    cout << endl;
  }
}

Graph::Node* Graph::find_smallest_distance(map<Graph::Node*, float> distances, const vector<Graph::Node*> unvisited_nodes){
  // finds the smallest distance out of any unvisited node, returns nullptr if it doesn't exst
  // i used 'distance' and 'weight' fairly interchangeably when making this program, but they do mean the same thing in this case
  if (unvisited_nodes.size() == 0)
    return nullptr;
  float best_distance = default_weight;
  Node* closest_node = nullptr;
  for (Node* current_node : unvisited_nodes)
    if (distances[current_node] < best_distance){
      best_distance = distances[current_node];
      closest_node = current_node;
    }
  if (best_distance >= 1000)
    return nullptr;
  return closest_node;
}

vector<Graph::Node*> Graph::dijkstra(Graph::Node* start, Graph::Node* end){
  // dijkstra's algorithm for pathfinding
  // en.wikipedia.org/wiki/Dijkstra's_algorithm
  vector<Node*> unvisited_nodes = nodes;
  map<Node*, float> distances;
  map<Node*, Node*> previous; // stores the previous node in the current shortest chain
  for (Node* current_node : nodes){ // populate distances
    distances[current_node] = default_weight;
    previous[current_node] = nullptr;
  }
  distances[start] = 0.0; // start always has a distance of 0 from itself
  // start of loop
  float base_distance = 0.0;
  Node* current_node = find_smallest_distance(distances, unvisited_nodes);
  while (current_node != nullptr && current_node != end){ // while there is a visitable, unvisited node (besides the end node, if the end node is the closest one then there is guaranteed to be no faster path)
    base_distance = distances[current_node];
    for (int i = 0; i < nodes.size(); i++){
      Node* node = nodes[i];
      if (node == current_node) continue; // no need to check this node
      float distance = current_node->get_connection(i);
      if (distance >= 10e9) continue; // infinity
      distance += base_distance;
      if (distance < distances[node]){ // if this path's distance is shorter than the previous minimum
	distances[node] = distance; // update its distance with a new best
	previous[node] = current_node; // update the previous node
      }
    }
    erase(unvisited_nodes, current_node); // this node has been visited
    current_node = find_smallest_distance(distances, unvisited_nodes);
  }
  total_weight = distances[end];
  vector<Node*> path;
  // now, populate path by starting at end and repeatedly appending previous to it
  current_node = end;
  path.push_back(current_node);
  while (current_node != start){
    current_node = previous[current_node];
    if (current_node == nullptr) break;
    path.push_back(current_node);
  }
  return path; // this will be backwards
}

void PATHFIND(){
  // PATHFIND function
  if (Graph::nodes.size() < 2){
    cout << RED << "Add more nodes!" << RESET << endl;
    return;
  }
  string input;
  string input1;
  cin.ignore();
  cout << GREEN << "$ What is the starting node?: " << RESET << flush;
  getline(cin, input);
  cout << GREEN << "$ What is the ending node?: " << RESET << flush;
  getline(cin, input1);
  Graph::Node* start = Graph::lookup_node(input);
  Graph::Node* end = Graph::lookup_node(input1);
  if (start == nullptr || end == nullptr){
    cout << RED << "One or both of those nodes doesn't exist!" << RESET << endl;
    return;
  }
  cout << WHITE;
  vector<Graph::Node*> path = Graph::dijkstra(start, end);
  if (Graph::total_weight < 10e9){ // i can't ever get INFINITY to work consistently so i use a very large literal instead
    for (int i = path.size(); i > 0; i--){
      cout << path[i-1]->label;
      if (i > 1)
	cout << '>';
    }
    cout << endl;
    cout << BLUE << Graph::total_weight << RESET << endl;
  } else {
    cout << "No valid path exists." << RESET << endl;
  }
}

int main(){
  cout << YELLOW << "C++ Graph Creator - " << RESET << RED << "Version " << Graph::version << RESET << endl;
  cout << WHITE << "Type \'HELP\' for a list of commands." << endl;
  map<string, unsigned short> commands;
  commands["HELP"] = 1; commands["ADD"] = 2; commands["CONNECT"] = 3; commands["REMOVE"] = 4; commands["DISCONNECT"] = 5; commands["PRINT"] = 6; commands["PATHFIND"] = 7;
  // ^ could use an enum for these values, after some point i might be overengineering this, a long if/else statement would work anyway
  string input;
  bool connection_added = false;
  // main loop
  while (input != "QUIT"){
    cout << GREEN << "$ Enter a command: " << RESET << flush;
    cin >> input;
    string input1;
    string input2;
    string input3;
    float weight;
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
	"REMOVE: remove a node (vertex) from the graph" << '\n' <<
	"DISCONNECT: remove a connection (line) from two nodes" << '\n' <<
	"PRINT: print the adjacency table" << '\n' <<
	"PATHFIND: find the shortest path between two nodes" << '\n' <<
	endl;
      break;
      
    case 2: // ADD
      ADD();
      break;

    case 3: // CONNECT
      CONNECT(connection_added);
      connection_added = true;
      break;

    case 4: // REMOVE
      REMOVE();
      break;

    case 5: // DISCONNECT
      DISCONNECT();
      break;

    case 6: // PRINT
      PRINT();
      break;

    case 7: // PATHFIND
      PATHFIND();
      break;
      
    }
    // one thing about switch statements that made this slightly harder to make is that all cases share the same scope
    // so the compiler freaks out if i 'redefine' a variable between cases even though only one case will end up running
    // i ended up making this easier by using functions for the later cases
  }
  cout << "Goodbye!" << endl;
  return 0;
}
