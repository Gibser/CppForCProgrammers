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
#define MIN_WEIGHT 1
#define MAX_WEIGHT 10

using namespace std;

void swap(pair<int, float>* a, pair<int, float>* b){
    pair<int, float> tmp = *a;
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
};

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

Graph::Graph(int n_nodes){
    adjacency_matrix.assign(n_nodes, vector<float>(n_nodes));
}

Graph::Graph(){
    adjacency_matrix.assign(5, vector<float>(5));
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

/* 
This class holds a graph, the distances between every pair of nodes and the predecessor matrix prev, where prev[i][j] is the previous node of j in the shortest path from i to j, or -1 if no such path exists.
When we compute the shortest path from u to w, we update these matrices to avoid redundant calculations in the future.
*/
class ShortestPath{
    private:
        Graph *g;
        vector<vector<float>> dist;
        vector<vector<int>> prev;
    public:
        ShortestPath(float density, int n_nodes);
        ShortestPath(Graph *g);
        string path(int u, int w);
        float path_size(int u, int w);
        float get_dist(int u, int w);
        void print_graph();
        string create_path_string(int source, int dest);
        float average_path_length_from_source(int source);
        int finalise_path(int source, int dest);
};

float ShortestPath::path_size(int u, int w){
    return dist[u][w];
}

float ShortestPath::average_path_length_from_source(int source){
    float avg = 0.0;
    int n_paths = 0;
    for(int i = 0; i < g->n_nodes(); i++){
        if(i != source){
            path(source, i);
            if(path_size(source, i) != -1){
                avg += path_size(source, i);
                n_paths++;
            }
        }
    }
    if(avg == 0 && n_paths == 0)
        return 0;
    return avg / n_paths;
}

void ShortestPath::print_graph(){
    g->print_adjacency_matrix();
}

ShortestPath::ShortestPath(float density, int n_nodes){
    g = new Graph(n_nodes);
    g->randomize_graph(density);
    dist.assign(g->n_nodes(), vector<float>(g->n_nodes(), __FLT_MAX__));
    prev.assign(g->n_nodes(), vector<int>(g->n_nodes(), -1));
}

ShortestPath::ShortestPath(Graph *g){
    this->g = g;
    dist.assign(this->g->n_nodes(), vector<float>(this->g->n_nodes(), __FLT_MAX__));
    prev.assign(this->g->n_nodes(), vector<int>(this->g->n_nodes(), -1));
}

float ShortestPath::get_dist(int u, int w){
    return dist[u][w];
}

string ShortestPath::create_path_string(int source, int dest){
    int node = dest;
    string path = "";
    if(dist[source][dest] != -1){
        path = to_string(dest);
        while(node != source){
            path = to_string(prev[source][node]) + " -> " + path;
            node = prev[source][node];
        }
        path += "     Dist: " + to_string(dist[source][dest]);
    }
    return path;
}

int ShortestPath::finalise_path(int source, int dest){
    if(source == dest)
        return dest;
    if(prev[source][dest] == -1){
        dist[source][dest] = -1;
        dest = -1;   
    }
    else
        prev[source][dest] = finalise_path(source, prev[source][dest]); 
    return dest;
}

string ShortestPath::path(int source, int dest){
    string path = "";
    if(dist[source][dest] < __FLT_MAX__){
        // Path already calculated, we only have to return it
        return create_path_string(source, dest);
    }
    PriorityQueue queue;   // minHeap
    vector<bool> in_queue(g->n_edges(), true);
    
    dist[source][source] = 0;
    for(int i = 0; i < g->n_nodes(); i++){  // We put every vertex in the queue Q
        queue.insert(i, dist[source][i]);
    }
    while(!queue.empty()){
        int u = queue.minPriority();
        if(u == dest)
            break;
        for(auto& v : g->neighbors(u)){
            if(queue.contains(v) && dist[source][u] != -1){
                float alt = dist[source][u] + g->get_edge_value(u, v);
                if(alt < dist[source][v]){
                    dist[source][v] = alt;
                    prev[source][v] = u;
                    queue.chgPriority(v, alt);
                }
            }
        }
    }
    finalise_path(source, dest);
    return create_path_string(source, dest);
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

int main(){
    srand((time(0)));
    
    // Graph as input
    /*
    Graph g;
    ShortestPath dijkstra(&g);
    
    g.set_edge_value(0, 2, 1);
    g.set_edge_value(0, 3, 10);
    g.set_edge_value(2, 3, 2);
    g.set_edge_value(1, 2, 5);
    g.set_edge_value(1, 3, 3);
    g.set_edge_value(1, 4, 8);
    g.set_edge_value(3, 4, 6);
    */

    // Random graph
    
    ShortestPath dijkstra(0.1, 10);
    dijkstra.print_graph();

    int source, dest;
    while(1){
        cout << "Source: ";
        cin >> source;
        cout << "Dest: ";
        cin >> dest;
        cout << dijkstra.path(source, dest) << endl;
    }
    
    /*
    // Average path for the assignment
    ShortestPath dijkstra_20percent(0.2, 50);
    //dijkstra_20percent.print_graph();
    cout << "Avg path length from " << 0 << ": " << dijkstra_20percent.average_path_length_from_source(0) << endl;

    ShortestPath dijkstra_40percent(0.4, 50);
    //dijkstra_40percent.print_graph();
    cout << "Avg path length from " << 0 << ": " << dijkstra_40percent.average_path_length_from_source(0) << endl;
    */
    return 0;
}