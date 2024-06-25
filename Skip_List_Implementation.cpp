#include <iostream>
#include <cstdlib>
#include <climits>
#include <cmath>
using namespace std;

int MAX_LEVEL; // Maximum Level of the Skip-List 

struct Node {
    int data;
    Node** next;
};

struct SkipList {
    int level; // current level of the skip list
    Node* header;
    int size;
};

void create_skipList(SkipList* sl);
int rand_lvl();
int insert_element(SkipList* sl, int data);
bool skiplist_search(SkipList* sl, int data);
int delete_element(SkipList* sl, int data);
void skiplist_display(SkipList* sl);
int get_skiplist_size(SkipList* sl);

int main() {
    SkipList* sl = new SkipList();
    int max_num;
    cout << "Enter the maximum number of elements in the list: ";
    cin >> max_num;
    MAX_LEVEL = static_cast<int>(ceil(log(max_num) / log(2))); // Maximum Level of Skip List = log2(Number of elements in the List)
    create_skipList(sl);

    int k, n;
    while (true) {
        cout << "\n-----------------------\n";
        cout << "\nOperations on Skip list\n";
        cout << "\n-----------------------\n";
        cout << "1. Insert Element\n";
        cout << "2. Search Element\n";
        cout << "3. Delete Element\n";
        cout << "4. Display List\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> k;
        switch (k) {
            case 1:
                if(get_skiplist_size(sl) == max_num){
                    cout<<"Skip List Full\n";
                    break;
                }
                cout << "Enter the element to be inserted in the list: ";
                cin >> n;
                insert_element(sl, n);
                break;
            case 2:
                cout << "Enter the element to be searched: ";
                cin >> n;
                if (skiplist_search(sl, n))
                    cout << n << " is present in the list\n";
                else
                    cout << "Element not present\n";
                break;
            case 3:
                cout << "Enter the element to be deleted from the list: ";
                cin >> n;
                if (!skiplist_search(sl, n)) {
                    cout << "Element not present\n";
                    break;
                }
                delete_element(sl, n);
                cout << "Element Deleted\n";
                break;
            case 4:
                cout<<"Number of elements in Skip List are :\n";
                cout<<get_skiplist_size(sl)<<"\n";
                cout << "The elements in the Skip List are:\n";
                skiplist_display(sl);
                break;
            case 5:
                exit(0);
            default:
                cout << "Enter a valid input.\n";
        }
    }
    return 0;
}

void create_skipList(SkipList* sl) {
    Node* header = new Node;
    sl->header = header;
    header->data = INT_MAX;
    header->next = new Node * [MAX_LEVEL + 1]; // Allocating memory for pointers to nodes of different levels
    for (int i = 0; i <= MAX_LEVEL; i++) {
        header->next[i] = sl->header; // Initializing all nodes to header (can also be initialized to NULL)
    }
    sl->level = 1; // Updating current level to 1
    sl->size = 0;  // Initialize size to 0
}

int rand_lvl() { // Function to generate random level for a node
    int level = 1;
    while (rand() < RAND_MAX / 2 && level <= MAX_LEVEL) // Probability for increasing level is 0.5
        level++;
    return level;
}

int insert_element(SkipList* sl, int data) {
    Node* x = sl->header;
    Node* update[MAX_LEVEL + 1]; // Creating an array to store address of previous nodes
    for (int i = sl->level; i >= 1; i--) { // Starting from highest level of skip list 
        while (x->next[i]->data < data)
            x = x->next[i];
        update[i] = x;
    }
    x = x->next[1];

    if (data == x->data) {
        return 0;
    } else {
        int level = rand_lvl();
        // Checking if random level generated is greater than skiplist's current level 
        // If so, initializing update value with pointer to header for further use
        // and updating sl->level to level.
        if (level > sl->level) {
            for (int i = sl->level + 1; i <= level; i++) {
                update[i] = sl->header;
            }
            sl->level = level;
        }

        x = new Node; // Allocating memory to new node
        x->data = data; // Updating data of new node
        x->next = new Node * [level + 1];
        for (int i = 1; i <= level; i++) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
         sl->size++;  // Increment size
    }
    return 0;
}

// Function to search element in SkipList 
bool skiplist_search(SkipList* sl, int data) {
    Node* x = sl->header;

    /*  Starting from current maximum level of skip list and moving the pointer forward while data 
        is greater than data of node next to the current node(named x). Otherwise inserting x in update and 
        moving one level down and continuing search
    */
    for (int i = sl->level; i >= 1; i--) {
        while (x->next[i]->data < data)
            x = x->next[i];
    }
    if (x->next[1]->data == data) {
        return true;
    } else {
        return false;
    }
}

// Function to delete elements from SkipList
int delete_element(SkipList* sl, int data) {
    Node* update[MAX_LEVEL + 1];
    Node* x = sl->header;
    /*  Starting from current maximum level of skip list and moving the pointer forward while data 
        is greater than data of node next to the current node(named x). Otherwise inserting x in update and 
        moving one level down and continuing search
    */
    for (int i = sl->level; i >= 1; i--) {
        while (x->next[i]->data < data)
            x = x->next[i];
        update[i] = x;
    }
    x = x->next[1];
    if (x->data == data) {
        for (int i = 1; i <= sl->level; i++) {
            // If at level i, next node is not target node, no need to move ahead
            if (update[i]->next[i] != x)
                break;
            update[i]->next[i] = x->next[i];
        }
        if (x) {
            delete[] x->next;
            delete x;
        }
        sl->size--;  // Decrement size
        // Removing levels with no elements
        while (sl->level > 1 && sl->header->next[sl->level] == sl->header)
            sl->level--;
        return 0;
    }
    return 1;
}

// Function to display elements of SkipList
void skiplist_display(SkipList* sl) {
    for (int i = 1; i <= MAX_LEVEL; i++) { // Starting from level 1 and printing elements at each level 
        Node* x = sl->header;
        cout << "LEVEL " << i << ": ";
        while (x && x->next[i] != sl->header) {
            cout << x->next[i]->data << "->";
            x = x->next[i];
        }
        cout << "NULL\n";
    }
}

//Function to find size of SkipList
int get_skiplist_size(SkipList* sl) {
    return sl->size;
}

