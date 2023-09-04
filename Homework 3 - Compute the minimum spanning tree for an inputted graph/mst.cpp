/*
This exercise was an awesome way to refresh my knowledge of Dijkstra’s shortest path algorithm, which I studied in the 
“Algorithms and Data Structures” and “Operations Research” courses. It is a very important algorithm in computer science and graph theory.
Therefore, these kinds of homework are really important because you have to do your own research about libraries, useful algorithms 
and language-specific features, and it’s way better than little guided exercises.
*/


#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib> 
#include <ctime>
#include <queue>
#include <list>
#include <numeric>
#include <fstream>
#define MIN_WEIGHT 1
#define MAX_WEIGHT 10

using namespace std;

void swap(pair<int, float>* a, pair<int, float>* b){
    pair<int, float> tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// reference: https://www.geeksforgeeks.org/binary-heap/
class MinHeap{
    public:
        void heapify(int);
        void insert_key(int, float);
        int parent(int i) { return (i-1)/2; }
        // to get index of left child of node at index i
        int left(int i) { return (2*i + 1); }
        // to get index of right child of node at index i
        int right(int i) { return (2*i + 2); }
        void decrease_key(int, float);
        pair<int, float> extract_min();
        void delete_key(int);
        pair<int, float> get_min() { return heap_array[0]; }
        void print_heap();
        bool empty();
        int get_node_heaparray_index(int node);
        bool is_present(int key){ return !(node_heapindex_map.find(key) == node_heapindex_map.end()); }
        int size(){ return heap_array.size(); }
        float get_value(int heap_array_index){ return heap_array[heap_array_index].second; }
        void print_node_heapindex_map();
    private:
        vector<pair<int, float>> heap_array;
        unordered_map<int, int> node_heapindex_map;
        //int heap_size;
};

int MinHeap::get_node_heaparray_index(int node){
    return node_heapindex_map[node];
}

bool MinHeap::empty(){
    return heap_array.size() == 0;
}

void MinHeap::print_node_heapindex_map(){
    for(auto it = node_heapindex_map.begin(); it != node_heapindex_map.end(); ++it){
        cout << it->first << ": " << it->second << " ";
    }
    cout << endl;
}

void MinHeap::print_heap()
{
    for(auto i = heap_array.cbegin(); i != heap_array.cend(); ++i)
        cout << "(" << (*i).first << ", " << (*i).second << ")";
    cout << "\n";
}

void MinHeap::insert_key(int key, float value){
    int i = heap_array.size();      // Append
    heap_array.push_back(pair<int, float>(key, value));
    node_heapindex_map[key] = i;
    while(i != 0 && heap_array[parent(i)].second > heap_array[i].second){
        node_heapindex_map[key] = parent(i);
        node_heapindex_map[heap_array[parent(i)].first] = i;
        swap(&heap_array[parent(i)], &heap_array[i]);
        i = parent(i);
    }
}

void MinHeap::decrease_key(int i, float new_key){
    heap_array[i].second = new_key;
    while(i != 0 && heap_array[parent(i)].second > heap_array[i].second){
        node_heapindex_map[heap_array[i].first] = parent(i);
        node_heapindex_map[heap_array[parent(i)].first] = i;
        swap(&heap_array[parent(i)], &heap_array[i]);
        i = parent(i);
    }
}

void MinHeap::heapify(int i){
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if(l < heap_array.size() && heap_array[l].second < heap_array[i].second)
        smallest = l;
    if(r < heap_array.size() && heap_array[r].second < heap_array[smallest].second)
        smallest = r;
    if(smallest != i){
        node_heapindex_map[heap_array[i].first] = smallest;
        node_heapindex_map[heap_array[smallest].first] = i;
        //swap(&node_heapindex_map[heap_array[i].first], &node_heapindex_map[heap_array[smallest].first]);
        swap(&heap_array[i], &heap_array[smallest]);
        heapify(smallest);
    }    
}

pair<int, float> MinHeap::extract_min(){
    if(heap_array.size() <= 0)
        return pair<int, float>(-1, __FLT_MAX__);
    if(heap_array.size() == 1){
        pair<int, float> elem = heap_array[0];
        heap_array.clear();
        return elem;
    }
    pair<int, float> root = heap_array[0];
    node_heapindex_map.erase(heap_array[0].first);
    heap_array[0] = heap_array[heap_array.size() - 1];
    node_heapindex_map[heap_array[0].first] = 0;
    heap_array.erase(heap_array.end());
    heapify(0);
    return root;
}

void MinHeap::delete_key(int i){
    decrease_key(i, INT_MIN);
    extract_min();
}

class PriorityQueue{
    private:
        MinHeap queue;
    public:
        void chgPriority(int queue_element, float priority){ queue.decrease_key(queue.get_node_heaparray_index(queue_element), priority); }
        int minPriority(){ return queue.extract_min().first; }
        bool contains(int queue_element) { return queue.is_present(queue_element); }
        void insert(int key, float value){ queue.insert_key(key, value); }
        int top(){ return queue.get_min().first; }
        int size(){ return queue.size(); }
        float get_elem_priority(int elem);
        bool empty(){ return queue.empty(); }
        void print_queue(){ queue.print_heap();}
};

float PriorityQueue::get_elem_priority(int elem){
    if(this->contains(elem))
        return queue.get_value(queue.get_node_heaparray_index(elem));
    else
        return -1;
}


class Graph{
    private:
        vector<vector<float>> adjacency_matrix;
    public:
        Graph();
        Graph(int n_nodes);
        Graph(string file_name);
        int n_nodes();
        int n_edges();
        bool adjacent(int i, int j);
        vector<int> neighbors(int i);
        void add_edge(int i, int j, float weight);
        void delete_edge(int i, int j);
        float get_edge_value(int i, int j);
        void set_edge_value(int i, int j, float weight);
        void randomize_graph(float density);
        void print_adjacency_matrix();
        void init_graph_from_file(ifstream &graph_file);
        void init_graph(int n_nodes);
};

void Graph::add_edge(int i, int j, float weight){
    set_edge_value(i, j, weight);
}

void Graph::delete_edge(int i, int j){
    set_edge_value(i, j, 0);
}

// Undirected
void Graph::set_edge_value(int i, int j, float weight){
    adjacency_matrix[i][j] = adjacency_matrix[j][i] = weight;
}

void init_array_with_value(vector<float> &array, float value, int n_elements){
    for(int i = 0; i < n_elements; i++){
        array.push_back(value);
    }
}

void init_array_with_value(vector<int> &array, int value, int n_elements){
    for(int i = 0; i < n_elements; i++){
        array.push_back(value);
    }
}

int Graph::n_nodes(){
    return adjacency_matrix.size();
}

int Graph::n_edges(){
    int count = 0;
    for(auto& row : adjacency_matrix){
        for(auto& col : row){
            if(col != 0)
                count++;
        }
    }
    return count;
}

bool Graph::adjacent(int i, int j){
    return adjacency_matrix[i][j] != 0;     // Undirected, so we don't need to check the edge j->i
}

vector<int> Graph::neighbors(int i){
    vector<int> neighs;
    for(int j = 0; j < adjacency_matrix.size(); j++){
        if(adjacency_matrix[i][j] != 0)
            neighs.push_back(j);
    }
    return neighs;
}

void Graph::init_graph_from_file(ifstream &graph_file){
    int n_nodes;
    string line;      
    graph_file >> n_nodes;      // First line, for the number of nodes
    init_graph(n_nodes);
    //cout << this->n_nodes();
    int i, j;
    float weight;
    while(graph_file >> i >> j >> weight){
        if(weight <= 0.0){
            cout << "Invalid edge, skipping...";
            continue;
        }
        add_edge(i, j, weight);
    }
    print_adjacency_matrix();
}

Graph::Graph(string file_name){
    ifstream graph_file;
    graph_file.open(file_name);
    if(graph_file.is_open())
        init_graph_from_file(graph_file);
    else
        cout << "File error" << endl;
    /*
    graph_file.exceptions(ifstream::failbit | ifstream::badbit);
    try {       // File opening error check
        graph_file.open(file_name);
        init_graph_from_file(graph_file);
    } catch (std::system_error& e) {
        std::cerr << e.code().message() << std::endl;
    }
    */

}

void Graph::init_graph(int n_nodes){
    adjacency_matrix.assign(n_nodes, vector<float>(n_nodes));
}

Graph::Graph(int n_nodes){
    //adjacency_matrix.assign(n_nodes, vector<float>(n_nodes));
    init_graph(n_nodes);
}

Graph::Graph(){
    //adjacency_matrix.assign(5, vector<float>(5));
    init_graph(5);
}

void Graph::print_adjacency_matrix(){
    cout << "Adjacency matrix:" << endl;
    for(auto& row : adjacency_matrix){
        for(auto& col : row){
            cout << col << " ";
        }
        cout << endl;
    }
    cout << endl;
}

float Graph::get_edge_value(int i, int j){
    return adjacency_matrix[i][j];
}


float random_number_min_max(float min, float max){
    return min + static_cast<float>(rand()) / (static_cast <float>(RAND_MAX/(max-min)));
}

void Graph::randomize_graph(float density){
    for(int i = 0; i < adjacency_matrix.size(); i++){
        for(int j = 0; j < adjacency_matrix.size(); j++){
            if(i != j && (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < density)){
                set_edge_value(i, j, random_number_min_max(MIN_WEIGHT, MAX_WEIGHT));
            }
        }
    }
}

class MinimumSpanningTree{    // We use Prim's algorithm (https://en.wikipedia.org/wiki/Prim%27s_algorithm)
    private:
        Graph *g;
        vector<float> cost;
        vector<int> prev;
        vector<list<int>> tree;     // Adjacency list of the MST
    public:
        MinimumSpanningTree(Graph *g);
        void compute_mst(int root);
        void init_tree();
        void init_queue(PriorityQueue &queue);
        void build_tree();
        void print_tree(int root);
        void print_tree_as_adj_matrix();
};

void MinimumSpanningTree::init_tree(){
    cost.assign(g->n_nodes(), __FLT_MAX__);
    prev.assign(g->n_nodes(), -1);
}

void MinimumSpanningTree::init_queue(PriorityQueue &queue){
    for(int i = 0; i < g->n_nodes(); i++){
        queue.insert(i, cost[i]);
    }
}

void MinimumSpanningTree::build_tree(){
    tree = vector<list<int>>(g->n_nodes(), list<int>());
    for(int i = 0; i < g->n_nodes(); i++){
        if(prev[i] != -1){
            tree[prev[i]].push_back(i);
        }
    }
}

void MinimumSpanningTree::compute_mst(int root){
    PriorityQueue queue;   // minHeap
    init_tree();
    cost[root] = 0;
    init_queue(queue);
    int tree_n_edges = -1;
    while(!queue.empty()){
        int u = queue.minPriority();
        tree_n_edges++;
        for(auto v : g->neighbors(u)){
            if(queue.contains(v) && g->get_edge_value(u, v) < cost[v]){
                cost[v] = g->get_edge_value(u, v);
                prev[v] = u;
                queue.chgPriority(v, g->get_edge_value(u, v));
            }
        }
    }
    if(tree_n_edges != g->n_nodes()-1){
        cout << "The graph is not connected!" << endl;
        return;    
    }

    build_tree();
    cout << "MST cost is " << accumulate(cost.begin(), cost.end(), 0.0) << endl;
}

void MinimumSpanningTree::print_tree_as_adj_matrix(){
    for(int i = 0; i < g->n_nodes(); ++i){
        vector<float> row(g->n_nodes(), 0);
        for(auto it = tree[i].begin(); it != tree[i].end(); ++it){
            row[*it] = cost[*it];
        }
        for(int j = 0; j < g->n_nodes(); ++j){
            cout << row[j] << " ";
        }
        cout << endl;
    }
}

void MinimumSpanningTree::print_tree(int root){     // We pass the root so we can print it first with his children
    cout << "Adjacency list of the MST:" << endl;
    if(tree.size() == 0){
        cout << "Empty tree" << endl;
        return;
    }
    cout << root;
    for(auto it = tree[root].begin(); it != tree[root].end(); ++it){
        cout << "->" << *it;
    }
    cout << endl;
    for(int i = 0; i < g->n_nodes(); i++){
        if(i == root || tree[i].size() == 0)
            continue;
        cout << i;
        for(auto it = tree[i].begin(); it != tree[i].end(); ++it){
            cout << "->" << *it;
        }
        cout << endl;
    }
}

MinimumSpanningTree::MinimumSpanningTree(Graph *g){
    this->g = g;
}

int main(){
    srand((time(0)));
    Graph g("graph.txt");           // Use the sample test data file from coursera
    //Graph g(6);
    //g.randomize_graph(0.3);
    //g.print_adjacency_matrix();
    MinimumSpanningTree mst(&g);
    /*
    g.set_edge_value(0, 1, 7);
    g.set_edge_value(0, 3, 5);
    g.set_edge_value(1, 2, 8);
    g.set_edge_value(1, 3, 9);
    g.set_edge_value(1, 4, 7);
    g.set_edge_value(2, 4, 5);
    g.set_edge_value(3, 4, 15);
    g.set_edge_value(3, 5, 6);
    g.set_edge_value(4, 5, 8);
    g.set_edge_value(4, 6, 9);
    g.set_edge_value(5, 6, 11);
    */
    mst.compute_mst(0);
    mst.print_tree(0);
    //mst.print_tree_as_adj_matrix();
    return 0;
}