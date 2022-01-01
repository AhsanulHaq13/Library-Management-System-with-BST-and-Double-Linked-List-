#include <iostream>
#include "BST.h"
#include"FinalGraph.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
string filename = "BookRecord.csv";
vector<string> readRow(int);
void displayDataBase()
{
    fstream finp(filename);
    string lined;
    vector<string> rowdata;
    if (!finp.is_open())
    {
        cout << "Couldn't open file." << endl;
        return;
    }
    else
    {
        int sz = 1;
        while (getline(finp, lined))
        {
            sz++;
        }
        for (int i = 1; i < sz; i++)
        {
            rowdata = readRow(i);
            cout << "title : " << rowdata[0] << endl;
            cout << "Author : " << rowdata[1] << endl;
            cout << "Edition : " << rowdata[2] << endl;
            cout << "Publisher : " << rowdata[3] << endl;
            cout << "Issue_Status : " << rowdata[4] << endl;
            cout << "Date_of_Issue : " << rowdata[5] << endl;
            cout << "Date_of_Return : " << rowdata[6] << endl;
            cout << "Person_ID : " << rowdata[7] << endl;

            cout << "----------" << endl;
        }
    }

}
vector<string> readRow(int n)
{
    vector<string> row;
    fstream finp(filename);
    if (!finp.is_open())
    {
        cout << "Couldn't open file." << endl;
        return row;
    }
    else
    {
        int idx = 0;
        string temprow;
        while (idx != n && finp.peek() != EOF)
        {
            string col;
            row.clear();
            getline(finp, temprow);
            stringstream ss(temprow);
            while (getline(ss, col, ','))
            {
                row.push_back(col);
            }
            idx++;
        }
    }
    finp.close();
}

void updateRecord(bool av, string title, int edition, bool removerow = false)
{
    vector<string> UpdateRecData;
    bool flag = true;
    int idx = 1;
    while (flag)
    {
        UpdateRecData = readRow(idx);
        if (UpdateRecData[0] == title && UpdateRecData[2] == to_string(edition))
        {
            flag = false;
        }
        idx++;
    }

    string oldData = "";
    for (int i = 0; i < 7; i++)
    {
        oldData += UpdateRecData[i];
        oldData += ",";
    }
    oldData += UpdateRecData[7];
    UpdateRecData[4] = (av == 1 ? "1" : "0");

    string newData = "";
    for (int i = 0; i < 7; i++)
    {
        newData += UpdateRecData[i];
        newData += ",";
    }
    newData += UpdateRecData[7];
    ifstream finp(filename);
    ofstream fout("temp.csv");

    string tempstr;
    // int idx = 1;
    while (finp.peek() != EOF)
    {
        getline(finp, tempstr);
        if (tempstr == oldData)
        {
            if (removerow)
            {
                continue;
            }
            else
            {
                tempstr = newData;
            }
        }
        tempstr += "\n";
        fout << tempstr;
        // idx++;
    }

    finp.close();
    fout.close();
    remove("BookRecord.csv");
    rename("temp.csv", "BookRecord.csv");
}

template <typename T>
struct Node
{
    Node() : next(NULL), prev(NULL) {}
    Node(T dt) : data(dt), next(NULL), prev(NULL) {}

    Node<T>* next;
    Node<T>* prev;

    T data;
};

struct BookInfo
{
    BookInfo() {}
    //Enter your book attributes here
    string title;
    string Author;
    int Edition;
    string Publisher;
    bool Issue_Status;
    string Date_of_Issue;
    string Date_of_Return;
    int Person_ID;
    BookInfo(string nam, string aut, int ed, string pub, bool iss, string DOI, string DOR, int PI)
    {
        title = nam;
        Author = aut;
        Edition = ed;
        Publisher = pub;
        Issue_Status = iss;
        Date_of_Issue = DOI;
        Date_of_Return = DOR;
        Person_ID = PI;
    }
    void printInfo()
    {
        cout << title << endl;
        cout << Author << endl;
        cout << Edition << endl;
        cout << Publisher << endl;
        cout << Issue_Status << endl;
        cout << Date_of_Issue << endl;
        cout << Date_of_Return << endl;
        cout << Person_ID << endl;
    } //Define a function to print info of a particular book
};

class LinkedList
{
    Node<BookInfo>* Head;
    Node<BookInfo>* Tail;
    int size;

public:
    LinkedList() : Head(NULL), Tail(NULL), size(0) {}

    void addNode(const BookInfo& data)
    {
        Node<BookInfo>* temp = new Node<BookInfo>(data);
        if (Head == NULL)
        {
            Head = temp;
            Tail = temp;
        }
        else
        {
            Tail->next = temp;
            temp->prev = Tail;
            Tail = Tail->next;
        }
        size++;
        fstream fout;

        // opens an existing csv file or creates a new file.
        fout.open("BookRecord.csv", ios::out | ios::app);
        fout << data.title << ","
            << data.Author << ","
            << data.Edition << ","
            << data.Publisher << ","
            << data.Issue_Status << ","
            << data.Date_of_Issue << ","
            << data.Date_of_Return << ","
            << data.Person_ID
            << "\n";
        fout.close();
    }

    void removeNode(Node<BookInfo>* node)
    {
        if (Head == NULL || node == NULL)
        {
            return;
        }
        string name = node->data.title;
        int edt = node->data.Edition;
        if (node == Head)
        {
            Head = node->next;
        }
        if (node->next != NULL)
        {
            node->next->prev = node->prev;
        }
        if (node->prev != NULL)
        {
            node->prev->next = node->next;
        }
        delete node;
        updateRecord(0, name, edt, true);
        size--;
    }

    void removeFromEnd()
    {
        if (Head != NULL)
        {
            Node<BookInfo>* temp = Tail;
            Tail = Tail->prev;
            Tail->next = NULL;
            delete temp;
            size--;
        }
    }

    Node<BookInfo>* searchList(const string& title)
    {
        Node<BookInfo>* temp = Head;
        while (temp != NULL)
        {
            if (temp->data.title == title)
            {
                return temp;
            }
            temp = temp->next;
        }
        cout << "Not found in the list.";
        temp = NULL;
        return temp;
    }

    int getSize() { return size; }
    void display()
    {
        Node<BookInfo>* temp = Head;
        while (temp != NULL)
        {
            temp->data.printInfo();
            cout << std::endl;
            temp = temp->next;
        }
    }

    void updateStatus(string title, int edi, bool st)
    {
        Node<BookInfo>* temp = searchList(title);
        if (temp != NULL)
        {
            temp->data.Issue_Status = st;
            updateRecord(st, title, edi);
        }
    }
};
int main()
{
    struct node* root = NULL;

    int cnic;
    int number;

    string name;
    string address;

    ifstream input("borrower.txt");
    // input.open("borrower.txt");
    root = NULL;
    if (input.is_open())
    {
        while (input.peek() != EOF)
        {
            string temp;
            getline(input, temp);
            cnic = stoi(temp);
            getline(input, name);
            getline(input, temp);
            number = stoi(temp);
            getline(input, address);
            // cout << "cnic : " << cnic << endl;
            // cout << "name : " << name << endl;
            // cout << "number : " << number << endl;
            // cout << "address : " << address << endl;
            root = insert(root, name, cnic, number, address);
        }
        input.close();
    }

    Graph* g = new Graph(false);
    int parent[MAXV + 1];
    int distance[MAXV + 1];
    int start = 5;
    int arr[30][3];
    ifstream is("graph.txt");
    int cnt = 0;
    int cnt2 = 0;
    int x;
    // check that array is not already full
    while (cnt < arr[30][3] && is >> x) {
        // and read integer from file
        if (cnt2 == 3)
        {
            cnt++;
            cnt2 = 0;
        }
        arr[cnt][cnt2++] = x;
    }
    int z = 0;
    int cnt3 = 0;

    while (z < cnt + 1)
    {
        cnt2 = 0;
        g->insert_edge(arr[cnt3][cnt2], arr[cnt3][cnt2 + 1], arr[cnt3][cnt2 + 2], false);
        cnt3++;
        z++;
    }
    LinkedList BooksList;
    BookInfo data("CVT", "KYZUG", 1, "Advanced Mathematics", true, "10-10-21", "30-10-21", 1);
    BookInfo data1("DSA", "M Tauqeer", 1, "Aikmen Series", true, "20-10-21", "20-11-21", 1);
    BookInfo data3("C++ Programming", "D S Malik ", 2, "Cengage Learning", false, "10-10-21", "20-11-21", 2);
    BooksList.addNode(data);
    BooksList.addNode(data1);
    BooksList.addNode(data3);
    //close the file
    is.close();
    int choice=-1;
    do {
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "            Welcome to Library Management System" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "Enter 1 to see the shortest path to an address\n";
        cout << "Enter 2 to display book data:\n";
        cout << "Enter 3 to display Borrower data\n";
        cout << "Enter 4 to update status of a book\n";
        cout << "Enter 5 to display map data:\n";
        cout << "Enter 0 to exit:\n";
        cout << "\n";
        cin >> choice;
        switch (choice)
        {
        case 1:

            dijkstra_shortest_path(g, parent, distance, start);
            //print shortest path from vertex 1 to 5
            print_shortest_path(3, parent);
            //print_distances(start, distance);
            delete g;
            break;
        case 2:
            displayDataBase();
            break;
        case 3:
            cout << "Inorder traversal: " << endl;
            inorder(root);
            break;
        case 4:
            BooksList.updateStatus("book1", 1, 0);
            displayDataBase();
            //BooksList.removeNode(BooksList.searchList("book2"));
            displayDataBase();
            break;
        case 5:
            for (int i = 0; i < cnt; i++) {
                for (int j = 0; j < cnt2; j++) {
                    cout << arr[i][j] << ' ';
                }
            }
            break;
        default:
            cout << "Enter again!!!!\n";
            break;
        }
    } while (choice != 0);
    cout << endl;
}
