#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>

using namespace std;

void swap(int* a, int* b){
    *a = *b - *a;
    *b = *b - *a;
    *a = *a + *b;
}

void swap(pair<int, float>* a, pair<int, float>* b){
    pair<int, float> tmp = *a;
    *a = *b;
    *b = tmp;
}

class MinHeap{
    public:
        //inline MinHeap();
        //inline Heap(bool max_heap, vector<int> array);
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
    private:
        vector<pair<int, float>> heap_array;
        //int heap_size;
};

void MinHeap::print_heap()
{
    for (auto i = heap_array.cbegin(); i != heap_array.cend(); ++i)
        cout << "(" << (*i).first << ", " << (*i).second << ")";
    cout << "\n";
}

void MinHeap::insert_key(int key, float value){
    int i = heap_array.size() - 1;      // Si inserisce alla fine
    heap_array.push_back(pair<int, float>(key, value));
    while(i != 0 && heap_array[parent(i)].second > heap_array[i].second){
        swap(&heap_array[parent(i)], &heap_array[i]);
        i = parent(i);
    }
}

void MinHeap::decrease_key(int i, float new_key){
    heap_array[i].second = new_key;
    while(i != 0 && heap_array[parent(i)].second > heap_array[i].second){
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
    heap_array[0] = heap_array[heap_array.size() - 1];
    heap_array.erase(heap_array.end());
    heapify(0);
    return root;
}

void MinHeap::delete_key(int i){
    decrease_key(i, INT_MIN);
    extract_min();
}

int main(){
    srand((unsigned)time(0));
    MinHeap h;
    h.insert_key(0, 3);
    h.insert_key(1, 2);
    h.insert_key(2, 1);
    h.insert_key(3, 15);
    h.insert_key(4, 5);
    h.insert_key(5, 4);
    h.insert_key(6, 45);
    //cout << h.extract_min() << " " << h.get_min() << endl;
    h.print_heap();
    h.extract_min();
    h.print_heap();
    return 0;
}
