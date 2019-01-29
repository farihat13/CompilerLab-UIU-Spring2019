
#include<iostream>
#include<fstream>
#include <sstream>

using namespace std;

class SymbolInfo;

class SymbolTable;

#define MAX 7

struct Point {
    int i, j;

    Point(int x, int y) {
        this->i = x;
        this->j = y;
    }
};

struct SymbolInfo {
    string name;
    string type;

    SymbolInfo() {};

    SymbolInfo(string name, string type) {
        this->name = name;
        this->type = type;
    }

    SymbolInfo *next = NULL;
};

class SymbolTable {
    int length;
    SymbolInfo **list;
public:
    SymbolTable(int n) {
        length = n;
        list = new SymbolInfo *[n];
        for (int i = 0; i < length; i++)
            list[i] = NULL;
    }

    int getHashKey(SymbolInfo symbol);

    Point insert(SymbolInfo symbol);

    Point search(SymbolInfo &symbol);

    Point _delete(SymbolInfo &symbol);

    void show(ofstream &output);

    void show();

    Point update(SymbolInfo &oldSymbol, SymbolInfo &updatedSymbol);

};

SymbolTable symbolTable(MAX);

int SymbolTable::getHashKey(SymbolInfo symbol) {
    string name = symbol.name;
    int n = 0;
    for (int i = 0; i < name.length(); i++)
        n += name.at(i);
    n = n % length;
    return n;
}

Point SymbolTable::insert(SymbolInfo symbol) {
    int i = getHashKey(symbol);
    int j = 0;
    Point position(i, j);

    // if no entry in hash-key index in the symbol table, insert directly
    if (list[i] == NULL) {
        list[i] = new SymbolInfo(symbol.name, symbol.type);
        return position;
    }

    // if entry exists already in hash-key index, then chain
    // and keep track of the position in the chain
    SymbolInfo *temp = list[i];
    while (temp != NULL) {
        j++;
        if (!(temp->name).compare(symbol.name)) { // if the symbol already exists, do not re-enter
            i = -1;
            break;
        } else if (temp->next == NULL) {
            temp->next = new SymbolInfo(symbol.name, symbol.type);
            break;
        }
        temp = temp->next;
    }
    position.i = i;
    position.j = j;
    return position;

}

Point SymbolTable::search(SymbolInfo &symbol) {
    int i = getHashKey(symbol);
    int j = 0;

    if (symbol.name == ""){
        i = -1;
        Point pos(i,j);
        return pos;
    }

    SymbolInfo *temp = list[i];
    while (1) {
        if (temp == NULL) {
            // if we haven't found the symbol
            // and reached the end of the chain then break
            i = -1;
            break;
        } else if (!(temp->name).compare(symbol.name)) {
            symbol.type = temp->type;
            break;
        }
        j++;
        temp = temp->next; // traversing the chain
    }

    Point pos(i, j);
    return pos;

}

Point SymbolTable::_delete(SymbolInfo &symbol) {
    int i = getHashKey(symbol);
    int j = 0;

    if (list[i] == NULL || symbol.name == "") { // nothing exists in that hash-key entry to delete
        i = -1;
    } else {
        SymbolInfo *temp = list[i], *prev = NULL;
        while (1) {
            if (temp == NULL) {
                // if we haven't found the symbol
                // and reached the end of the chain then break
                i = -1;
                break;
            } else if (!(temp->name).compare(symbol.name)) {
                // if we have found a symbol with same name then we can delete it
                symbol.type = temp->type;
                if (prev == NULL) {
                    list[i] = temp->next;
                    delete temp;
                } else {
                    prev->next = temp->next;
                    delete temp;
                }
                break;
            }

            j++;

            prev = temp;
            temp = temp->next;
        }
    }

    Point position(i, j);
    return position;
}

void SymbolTable::show(ofstream &output) {
    for (int i = 0; i < length; i++) {
        SymbolInfo *temp = list[i];
        cout << i << " --> ";
        output << i << " --> ";
        while (temp != NULL) {
            cout << "< " << temp->name << " : " << temp->type << " >";
            output << "< " << temp->name << " : " << temp->type << " >";
            temp = temp->next;
        }
        cout << "\n";
        output << "\n";
    }
}

void SymbolTable::show() {
    for (int i = 0; i < length; i++) {
        SymbolInfo *temp = list[i];
        cout << i << " --> ";
        while (temp != NULL) {
            cout << "< " << temp->name << " : " << temp->type << " >";
            temp = temp->next;
        }
        cout << "\n";
    }
}


SymbolInfo takeSymbolAsInput() {
    while (1) {
        string line;
        cout << "Enter name,type: ";
        getline(cin, line);
//    cout << "entered " << line << ".\n";
        istringstream ins(line);
        SymbolInfo symbol;
        getline(ins, symbol.name, ',');
        getline(ins, symbol.type, ',');
//    cout << symbol.name << " " << symbol.type;
        if (symbol.name == "")
            continue;
        if (symbol.type == "")
            continue;
        return symbol;
    }
}


void insert() {
    SymbolInfo symbol = takeSymbolAsInput();
    Point pos = symbolTable.insert(symbol);
    if (pos.i == -1) {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " already exists.\n";
    } else {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " inserted at position " << pos.i << " , "
             << pos.j << "\n";
    }
}

void search() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);

    SymbolInfo symbol(name, "");
    Point pos = symbolTable.search(symbol);
    if (pos.i == -1) {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " not found.\n";
    } else {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " found at position " << pos.i << " , "
             << pos.j << "\n";
    }
}

void _delete() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);

    SymbolInfo symbolToDelete(name, "");
    Point pos = symbolTable._delete(symbolToDelete);
    if (pos.i == -1) {
        cout << "< " << symbolToDelete.name << " not found. So couldn't delete\n";
    } else {
        cout << "< " << symbolToDelete.name << " : " << symbolToDelete.type << " >" << " deleted from position "
             << pos.i << " , "
             << pos.j << "\n";
    }
}

void update() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);
    SymbolInfo symbolToUpdate(name, "");
    SymbolInfo updatedSymbol = takeSymbolAsInput();
    Point pos = symbolTable._delete(symbolToUpdate);
    if (pos.i == -1) {
        cout << symbolToUpdate.name << " do not exist. So couldn't update\n";
    } else {

        Point pos = symbolTable.insert(updatedSymbol);
        if (pos.i == -1) {
            cout << updatedSymbol.name << " already exists.\n";
        } else {
            cout << "< " << updatedSymbol.name << " : " << updatedSymbol.type << " >" << " inserted at position "
                 << pos.i << " , " << pos.j << "\n";
        }

    }
}

void show() {
    symbolTable.show();
}

void getHashKey() {
    SymbolInfo symbol = takeSymbolAsInput();
    int hashKey = symbolTable.getHashKey(symbol);
    cout << "<" << symbol.name << " : " << symbol.type << " > " << " hash key: " << hashKey;
}

int main() {

    string line;
    cout << "To take input from file press y, for command line press any other key\n";
    getline(cin, line);
    if (line.at(0) != 'y') {
        while (1) {
            cout
                    << "\nPRESS 1 for insert\n\t2 for search\n\t3 for delete\n\t4 for show \n\t5 for update \n\t6 for getHashKey\nPress any other key to exit.\n";
            getline(cin, line);
            switch (line.at(0)) {
                case '1':
                    insert();
                    break;
                case '2':
                    search();
                    break;
                case '3':
                    _delete();
                    break;
                case '4':
                    show();
                    break;
                case '5':
                    update();
                    break;
                case '6':
                    getHashKey();
                    break;
                default:
                    exit(0);
            }
        }
        return 0;
    }

    ifstream input("input.txt");
    ofstream output("output.txt");

    if (input.is_open()) {

//get the length of hash table
        getline(input, line);
        int hashTable_length = line.at(0) - '0';
        SymbolTable symbolTable(hashTable_length);
//get instructions
        while (getline(input, line)) {
            cout << "\n" << line << endl;
            output << "\n" << line << endl;

            istringstream ins(line);
            string word;
            //get instructions i.e I, D ,L ,P
            ins >> word;
            //insert
            if (!word.compare("I")) {
                SymbolInfo symbol;
                ins >> word;
                symbol.name = (word);
                ins >> word;
                symbol.type = (word);
                Point pos = symbolTable.insert(symbol);

                if (pos.i == -1) {
                    cout << "< " << symbol.name << " : " << symbol.type << " >" << " already exists.\n";
                    output << "< " << symbol.name << " : " << symbol.type << " >" << " already exists.\n";
                } else {
                    cout << "< " << symbol.name << " : " << symbol.type << " >"\
 << " inserted at position " << pos.i << " , " << pos.j << "\n";
                    output << "< " << symbol.name << " : " << symbol.type << " >"\
 << " inserted at position " << pos.i << " , " << pos.j << "\n";
                }

            }
                //look up

            else if (!word.compare("L")) {
                SymbolInfo symbol;
                ins >> word;
                symbol.name = (word);
                Point pos = symbolTable.search(symbol);
                if (pos.i == -1) {
                    cout << symbol.name << " not found.\n";
                    output << symbol.name << " not found\n";
                } else {
                    cout << "< " << symbol.name << " : " << symbol.type << " >"\
 << " found at position " << pos.i << " , " << pos.j << "\n";
                    output << "< " << symbol.name << " : " << symbol.type << " >"\
 << " found at position " << pos.i << " , " << pos.j << "\n";
                }
            }
                //delete
            else if (!word.compare("D")) {
                SymbolInfo symbol;
                ins >> word;
                symbol.name = (word);
                Point pos = symbolTable._delete(symbol);
                if (pos.i == -1) {
                    cout << symbol.name << " doesnot exist.\n";
                    output << symbol.name << " doesnot exist.\n";
                } else {
                    cout << "< " << symbol.name << " : " << symbol.type << " >"\
 << " deleted from position " << pos.i << " , " << pos.j << "\n";
                    output << "< " << symbol.name << " : " << symbol.type << " >"\
 << " deleted from position " << pos.i << " , " << pos.j << "\n";
                }
            }
                //print
            else if (!word.compare("P")) {
                symbolTable.show(output);
            } else {
                cout << "instruction is not recognized.\n";
                output << "instruction is not recognized.\n";
            }

        }

    } else {
        cout << "no file named input.txt was found.\n";
    }


}


