//////////////////////////////////
/* Ayşe Zeynep Subaşı */
/* 150190046          */
//////////////////////////////////

// Compilation: g++ -std=c++11 main.cpp -o main
// ./main VideoGames.csv

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Node{
    public:
    string publisher;
    float na_sales;
    float eu_sales;
    float other_sales;
    bool color; //false = red and true = black
    Node *right, *left, *parent;
    Node(string n_publisher, float n_na_sales, float n_eu_sales, float n_other_sales) : publisher(n_publisher), na_sales(n_na_sales), eu_sales(n_eu_sales), other_sales(n_other_sales), color(false), right(NULL), left(NULL), parent(NULL){}
    Node(): color(false), right(NULL), left(NULL), parent(NULL){}
    void destroy();
    void preorder(string);
};

void Node::destroy(){ // it is to called in the tree destructor, i didn't deleted any node manually
    if(left) left->destroy();
    if(right) right->destroy();
    delete left;
    delete right; 
}

void Node::preorder(string hyphen = "") { //this is preorder print for the tree
    if (this) {
        string n_color = color ? "(BLACK) " : "(RED) ";
        cout << hyphen << n_color << publisher << endl;
        hyphen += "-"; // increased hyphen by itself in each dept to be able toprint properly
        left->preorder(hyphen);
        right->preorder(hyphen);
    }
}

class RBTree{
    Node* root;
    Node* best_na; //kept best valued nodes as a node pointer in the tree
    Node* best_eu;
    Node* best_other;
    public:
    RBTree():root(NULL), best_na(NULL), best_eu(NULL), best_other(NULL){}
    void rotate_right(Node*);
    void rotate_left(Node*);
    void insert(Node*);
    Node* bst_insert(string, float, float, float); //wrote a bst function to insert the node before the rotations and recoloring properly
    void print_best_games(int);
    void print_games();
    ~RBTree();
};

void RBTree::rotate_left(Node* x){ // rotate left function rotates the node as "left child" of its child, takes the child as parent of if
    Node* child = x->right;
    x->right = child->left;

    if(child->left != NULL)
        child->left->parent = x;

    child->parent = x->parent;

    child->left = x;
    x->parent = child;
    
    if(child->parent == NULL)
        root = child; // equalized x to the root if there is no grand parent means that the if the before parent (now child) was the root
    else if (child->parent->left == x) //checked if the before parent node was the left or the right child of its parent
        child->parent->left = child; // take the new parent (before child) as the child of grand parent depending on this
    else
        child->parent->right = child;
    
}

void RBTree::rotate_right(Node* x){ // rotate right function rotates the node as "right child" of its child, takes the child as parent of if
    Node* child = x->left;
    x->left = child->right;

    if(child->right != NULL)
        child->right->parent = x;

    child->parent = x->parent;

    child->right = x;
    x->parent = child;
    
    if(child->parent == NULL)
        root = child; // equalized x to the root if there is no grand parent means that the if the before parent (now child) was the root
    else if(child->parent->right == x) //checked if the before parent node was the left or the right child of its parent
        child->parent->right = child; // take the new parent (before child) as the child of grand parent depending on this
    else
        child->parent->left = child;
}

Node* RBTree::bst_insert(string publisher, float na, float eu, float other){ // wrote a binary search insert function to insert the node before rotating
    if(root == NULL){
        root = new Node(publisher, na, eu, other);
        best_na = root;
        best_eu = root;
        best_other = root;
        return root;
    }

    Node* traverse = root;
    Node* point;

    while(traverse && publisher != traverse->publisher){
        point = traverse;
        if(traverse->publisher < publisher){
            traverse = traverse->right;
        } else if(traverse->publisher > publisher){
            traverse = traverse->left;
        }
    }

    if(traverse && publisher == traverse->publisher){ // if node exists, added the sales values of it to the alredy existings
        traverse->na_sales += na;
        traverse->eu_sales += eu;
        traverse->other_sales += other;

        if(traverse->na_sales > best_na->na_sales) //checked if the node has a best sales value
            best_na = traverse;
        if(traverse->eu_sales > best_eu->eu_sales)
            best_eu = traverse;
        if(traverse->other_sales > best_other->other_sales)
            best_other = traverse;
        
        return NULL;
    }

    Node* node = new Node(publisher, na, eu, other);

    if(point->publisher < publisher){
        point->right = node;
        node->parent = point;
    }
    else{
        point->left = node;
        node->parent = point;
    }
    
    if(na > best_na->na_sales) //checked if the node has a best sales value
        best_na = node;
    if(eu > best_eu->eu_sales)
        best_eu = node;
    if(other > best_other->other_sales)
        best_other = node;
    return node;
}

void RBTree::insert(Node* node){
    if (node!=NULL){
        while(root != node && !node->parent->color){
            if(node->parent == node->parent->parent->left){
                if(node->parent->parent->right != NULL && !node->parent->parent->right->color){
                    Node* parent_parent = (node->parent)->parent;
                    parent_parent->color = false;
                    parent_parent->right->color = true;
                    parent_parent->left->color = true;
                    node = parent_parent;
                }else{
                    if(node == node->parent->right){
                        rotate_left(node->parent);
                        node = node->left;
                    }
                    bool color = node->parent->color;
                    node->parent->color = node->parent->parent->color;
                    node->parent->parent->color = color;
                    rotate_right(node->parent->parent);
                }
            } else{
                if(node->parent->parent->left != NULL && !node->parent->parent->left->color){
                    Node* parent_parent = (node->parent)->parent;
                    parent_parent->color = false;
                    parent_parent->right->color = true;
                    parent_parent->left->color = true;
                    node = parent_parent;
                }else{
                    if(node == node->parent->left){
                        rotate_right(node->parent);
                        node = node->right;
                    }
                    bool color = node->parent->color;
                    node->parent->color = node->parent->parent->color;
                    node->parent->parent->color = color;
                    rotate_left(node->parent->parent);
                }
            }
        }
        root->color = true;
    }
}

RBTree::~RBTree(){ // destructor of the tree
    root->destroy();
    delete root;
    root = NULL;
}

void RBTree::print_best_games(int year){ // print the best values untill the wanted year
    cout << "End of the " << year << " Year" << endl;
    cout << "Best seller in North America : " << best_na->publisher << " - " << best_na->na_sales << " million" << endl;
    cout << "Best seller in Europe : " << best_eu->publisher << " - " << best_eu->eu_sales << " million" << endl;
    cout << "Best seller rest of the World : " << best_other->publisher << " - " << best_other->other_sales << " million" << endl;
}

void RBTree::print_games(){
    root->preorder();
}

void read_from_file(RBTree& videoGames, char* file_name){
    ifstream fin;
    fin.open(file_name); //opened the input file to read
    string name, platform, year, publisher, na, eu, other, line;
    int print_year = 1990;

    getline(fin, line); // get the first line of the file, since there are headers, didn't write into vehicles array

    while(getline(fin, line)){
        istringstream l(line);
        getline(l, name, ',');
        getline(l, platform, ',');
        getline(l, year, ',');
        getline(l, publisher, ',');
        getline(l, na, ',');
        getline(l, eu, ',');
        getline(l, other, ','); // assigned the values from the line to the variables
        
        if(stoi(year) > print_year){
            videoGames.print_best_games(print_year); // print 1990, 2000, 2010 values
            print_year += 10;
        }

        Node* node = videoGames.bst_insert(publisher, stof(na), stof(eu), stof(other)); //inserted the node depending on bst
        videoGames.insert(node); // roated the node depending on red-black tree
    }

    videoGames.print_best_games(print_year); // print 2020 values
    fin.close();
}

int main(int argc, char** argv){
    if(argc != 2) // checked if the number of the input arguments of the main is equal to 2
        return 1;
    
    RBTree video_games;
    read_from_file(video_games, argv[1]);
    video_games.print_games();
    return 0;
}