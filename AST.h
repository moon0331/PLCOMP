#include <iostream>
#include <vector>
using namespace std;

typedef struct _node{
    string str;
    vector<struct _node*> children;
}Node;

class AST{
private:
    Node* root;
public:
    AST();
    void create_ast(vector<string>& inputTape);
    void add_node(string val);
};
