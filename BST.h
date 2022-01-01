#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct node
{
    node(string nm, string ad, int cn, int nmb) : Name(nm), Address(ad), CNIC(cn), Number(nmb) {}

    string Name;
    string Address;
    int CNIC;
    int Number;
    node* left;
    node* right;

    void display()
    {
        cout << Name << endl
            << Address << endl
            << CNIC << endl
            << Number << endl;
        cout << "->" << endl;
    }
};

// Create a node
struct node* newNode(string name, int cnic, int number, string address)
{
    node* temp = new node(name, address, cnic, number);
    return temp;
}

// Inorder Traversal
void inorder(struct node* root)
{
    if (root != NULL)
    {
        // Traverse left
        inorder(root->left);

        // Traverse root
        root->display();
        // Traverse right
        inorder(root->right);
    }
}

// Insert a node
struct node* insert(struct node* node, string name, int cnic, int number, string address)
{
    // Return a new node if the tree is empty
    if (node == NULL)
        return newNode(name, cnic, number, address);

    // Traverse to the right place and insert the node
    if (cnic < node->CNIC)
        node->left = insert(node->left, name, cnic, number, address);
    else
        node->right = insert(node->right, name, cnic, number, address);

    // node->display();
    return node;
}

// Find the inorder successor
struct node* minValueNode(struct node* nod)
{
    struct node* current = nod;

    // Find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct node* deleteNode(struct node* root, int cnic)
{
    // Return if the tree is empty
    if (root == NULL)
        return root;

    // Find the node to be deleted
    if (cnic < root->CNIC)
        root->left = deleteNode(root->left, cnic);
    else if (cnic > root->CNIC)
        root->right = deleteNode(root->right, cnic);
    else
    {
        // If the node is with only one child or no child
        if (root->left == NULL)
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        // If the node has two children
        struct node* temp = minValueNode(root->right);

        // Place the inorder successor in position of the node to be deleted
        root->Name = temp->Name;
        root->CNIC = temp->CNIC;
        root->Number = temp->Number;
        root->Address = temp->Address;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->CNIC);
    }
    return root;
}
