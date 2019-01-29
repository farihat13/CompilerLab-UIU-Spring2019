
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
    SymbolInfo list[MAX];
public:
    SymbolTable() {
    }

    int getHashKey(SymbolInfo symbol);

    int insert(SymbolInfo symbol);

    int search(SymbolInfo &symbol);

    int _delete(SymbolInfo &symbol);

    void show(ofstream &output);

    void show();

    int update(SymbolInfo &updatedSymbol);

};

SymbolTable symbolTable;

int SymbolTable::getHashKey(SymbolInfo symbol) {
    string name = symbol.name;
    int n = 0;
    for (int i = 0; i < name.length(); i++)
        n += name.at(i);
    n = n % MAX;
    return n;
}

int SymbolTable::insert(SymbolInfo symbol) {
    int i = getHashKey(symbol);

    // if no entry in hash-key index in the symbol table, insert directly
    if (list[i].name == "") {
        cout << "inserting <" << symbol.name << " : " << symbol.type << " > in index " << i << "\n";
    } else {
        cout << "inserting <" << symbol.name << " : " << symbol.type << " in index " << i << " by replacing\n";
    }
    list[i] = symbol;
    return i;
}

int SymbolTable::search(SymbolInfo &symbol) {
    if (symbol.name == "")
        return -1;
    int i = getHashKey(symbol);

    if (!(list[i].name).compare(symbol.name)) {
        symbol.type = list[i].type;
        return i;
    }
    return -1;

}

int SymbolTable::_delete(SymbolInfo &symbol) {
    if (symbol.name == "")
        return -1;
    int i = getHashKey(symbol);

    if (!(list[i].name).compare(symbol.name)) {
        symbol.type = list[i].type;
        list[i].name = "";
        return i;
    }
    return -1;
}

void SymbolTable::show(ofstream &output) {
    for (int i = 0; i < MAX; i++) {
        cout << i << " --> ";
        output << i << " --> ";

        if (list[i].name.compare("")) {
            cout << " < " << list[i].name << " : " << list[i].type << " > ";
            output << " < " << list[i].name << " : " << list[i].type << " > ";
        }

        cout << "\n";
        output << "\n";
    }
}

void SymbolTable::show() {
    for (int i = 0; i < MAX; i++) {
        cout << i << " --> ";

        if (list[i].name.compare("")) {
            cout << " < " << list[i].name << " : " << list[i].type << " > ";
        }

        cout << "\n";
    }
}

int SymbolTable::update(SymbolInfo &updatedSymbol) {
    if (updatedSymbol.name == "")
        return -1;
    int i = getHashKey(updatedSymbol);

    // if no entry in hash-key index in the symbol table, insert directly
    if (!list[i].name.compare(updatedSymbol.name)) {
        return -1;
    }
    list[i] = updatedSymbol;
    return i;
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
    int i = symbolTable.insert(symbol);
    if (i == -1) {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " already exists.\n";
    } else {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " inserted at position " << i << "\n";
    }
}

void search() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);

    SymbolInfo symbol(name, "");
    int i = symbolTable.search(symbol);
    if (i == -1) {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " not found.\n";
    } else {
        cout << "< " << symbol.name << " : " << symbol.type << " >" << " found at position " << i << "\n";
    }
}

void _delete() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);

    SymbolInfo symbolToDelete(name, "");
    int i = symbolTable._delete(symbolToDelete);
    if (i == -1) {
        cout << "< " << symbolToDelete.name << " not found. So couldn't delete\n";
    } else {
        cout << "< " << symbolToDelete.name << " : " << symbolToDelete.type << " >" << " deleted from position " << i
             << "\n";
    }
}

void update() {
    string name;
    cout << "Enter name: ";
    getline(cin, name);
    SymbolInfo symbolToUpdate(name, "");
    SymbolInfo updatedSymbol = takeSymbolAsInput();
    int i = symbolTable._delete(symbolToUpdate);
    if (i == -1) {
        cout << symbolToUpdate.name << " do not exist. So couldn't update\n";
    } else {

        i = symbolTable.update(updatedSymbol);
        if (i == -1) {
            cout << updatedSymbol.name << " already exists.\n";
        } else {
            cout << "< " << updatedSymbol.name << " : " << updatedSymbol.type << " >" << " updated at position "
                 << i << "\n";
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


