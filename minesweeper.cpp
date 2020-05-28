#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

enum Val {EMPTY, IND, BOMB};

int itest = 0;
#define ITEST cout << "Test #" << itest++ << endl;
#define NL cout << endl; // newline
#define NP for (int i = 0; i < 100; i++) NL; // newpage; a little barbaric but it works

// -------------------------------------------------------------------- TILE --------------------------------------------------------------------

class Tile {
private:
    Val val;
    bool found;
    bool flagged;
    int vnum;

public:
    // construction
    Tile();
    Tile(Val);

    // gets
    char letterEq();
    bool isFound();
    int getVNum();
    Val getVal();

    // functionality
    void setVal(Val);
    void incVal();
    void find();
    void flag();
    void unFlag();

    // prints
    void printVal();
};

// construction
Tile::Tile() {
    val = EMPTY;
    found = false;
    flagged = false;
    vnum = 0;
}

Tile::Tile(Val v) {
    val = v;
    found = false;
    flagged = false;
    vnum = 0;
}

// gets
char Tile::letterEq() {
    if (flagged) {
        return '?';
    }
    else if (found) {
        if (val == EMPTY) {
            return ' ';
        }
        else if (val == IND) {
            return vnum+48;
        }
        else {
            return 'B';
        }
    }
    else return '.';
}

bool Tile::isFound() {
    return found;
}

int Tile::getVNum() {
    return vnum;
}

Val Tile::getVal() {
    return val;
}

// functionality
void Tile::setVal(Val v) {
    val = v;
}

void Tile::incVal() {
    if (val != BOMB) {
        val = IND;
        vnum++;
    }
}

void Tile::find() {
    found = true;
}

void Tile::flag() {
    flagged = true;
}

void Tile::unFlag() {
    flagged = false;
}

// prints
void Tile::printVal() {
    cout << letterEq();
}

// -------------------------------------------------------------------- BOARD --------------------------------------------------------------------
class Board {
private:
    vector<vector<Tile>> bd;
    vector<int> spots;
    int h, w;

public:
    // constructors
    vector<vector<int>> findAdjacent(int, int);
    void setAdjacent(int, int);
    Board(int, int, int);
    ~Board();

    // gets
    bool isDone();
    bool isBomb(int, int);

    // functionality
    void reveal(int, int);
    void revealAll();
    int flagTile(int, int);
    void unFlagTile(int, int);

    // prints
    void printBoard();
};

// constructors
bool isIn(vector<int> v, int n) { // a helper function essential to the constructors
    for (auto x: v) {
        if (n == x) return true;
    }
    return false;
}

vector<vector<int>> Board::findAdjacent(int n, int m) {
    vector<vector<int>> ret;
    for (int i = n-1; i < n+2; i++) {
        for (int j = m-1; j < m+2; j++) {
            if (i == n && j == m) continue; // we're on the same tile
            else if ((i < 0) || (j < 0) || (i >= h) || (j >= w)) continue; // we're out of bounds 
            else {
                vector<int> temp;
                temp.push_back(i); temp.push_back(j);
                ret.push_back(temp);
            }
        }
    }
    return ret;
}

void printNestedVec(vector<vector<int>> v) {
    for (auto x: v) {
        for (auto y: x) {
            cout << y << " ";
        }
        NL;
    }
}

void printVec(vector<int> v) {
    for (auto x: v) {
        cout << x << " ";
    }
    NL;
}

void Board::setAdjacent(int n, int m) { // another helper function
    vector<vector<int>> adj = findAdjacent(n, m);
    for (auto s: adj) {
        bd[s[0]][s[1]].incVal(); 
    }
    NL;
}

Board::Board(int n, int m, int p) {
    h = n;
    w = m;

    int teddy = 0;
    for (int i = 0; i < int((n*m)*(p*0.01)); i++) { // p% of tiles set to be randomly placed mines
        spots.push_back(rand()%(n*m)); 
        teddy++;
    }
    cout << teddy << " mines placed." << endl;
    int count = 0;

    for (int i = 0; i < h; i++) {
        vector<Tile> temp;
        for (int j = 0; j < w; j++) {
            Tile t;

            if (isIn(spots, count++)) {
                t.setVal(BOMB); // if the tile in question should have a bomb, assign it.
            }

            temp.push_back(t);
        }
        bd.push_back(temp);
    }

    count = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (isIn(spots, count++)) {
                setAdjacent(i, j);
            }
        }
    }
}

Board::~Board() {
    for (int i = 0; i < h; i++) {
        bd[i].clear();
    }
    bd.clear();
}

// gets
bool Board::isDone() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!bd[i][j].isFound()) return false;
        }
    }
    return true;
}

bool Board::isBomb(int n, int m) {
    return (bd[n][m].getVal() == BOMB);
}

// functionality
void Board::reveal(int n, int m) {
    if (bd[n][m].getVal() != EMPTY) {
        bd[n][m].find();
    }
    else {
        bd[n][m].find();
        vector<vector<int>> adj = findAdjacent(n, m);
        for (auto s: adj) {
            if (bd[s[0]][s[1]].getVal() != BOMB && !bd[s[0]][s[1]].isFound()) {
                reveal(s[0],s[1]); // cursed(?) I guess not.
            }
        }
    }
}

void Board::revealAll() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            bd[i][j].find();
        }
    }
}

int Board::flagTile(int n, int m) {
    if (bd[n][m].getVal() != BOMB && bd[n][m].isFound()) {
        return -1; // we're trying to flag a revealed empty tile
    }
    bd[n][m].flag();
    return 0;
}

void Board::unFlagTile(int n, int m) {
    bd[n][m].unFlag();
}

// prints
void Board::printBoard() {
    // lettering each column
    cout << "\t";
    for (int i = 0; i < w; i++) {
        if (i < 10) cout << i << "  ";
        else cout << i << " ";
    } cout << endl << endl;

    // the rest of it
    for (int i = 0; i < h; i++) {
        cout << i << "\t";
        for (int j = 0; j < w; j++) {
            bd[i][j].printVal(); cout << "  ";
        }
        cout << endl;
    }
}

// -------------------------------------------------------------------- MAIN --------------------------------------------------------------------

void printArgs() {
    cout << "\nAvailable args are:" << endl << endl;
    cout << ".\tto reveal the tile," << endl;
    cout << "f/F\tto place a flag," << endl;
    cout << "r/R\tto remove a flag," << endl;
    cout << "w/W\tto win the game," << endl;
    cout << "h/H\tto just display arguments again, and" << endl;
    cout << "q/Q\tto quit." << endl;
}

int main() {
    int h, w, p;
    bool first = true;
    bool errorLast = false;
    bool showArgs = false;
    bool flagError = false;
    bool manualWin = false;
    bool lost = false;
    srand(time(NULL));

    NP; cout << "Enter height and width, separated by a space: ";
    cin >> h >> w;
    cout << "Enter mine placement percentage (0 for random): ";
    cin >> p; if (p == 0) p = rand() % 100;

    Board b(h, w, p); 

    while (!b.isDone()) {
        if (lost) break;

        NP;
        b.printBoard();
        if (first) { 
            cout << "\nThe board has " << int((h*w)*(p*0.01)) << " mines on it." << endl;
            cout << "Enter your target coordinates, followed by arg, all separated by spaces." << endl;

            printArgs();

            cout << "\nFor example, to flag the 7th tile on the 15th row, type: \"15 7 f\"." << endl;
            first = false;
        }
        else if (errorLast) {
            cout << "\nInvalid argument(s), try again. " << endl;
            errorLast = false;
        }
        else if (showArgs) {
            printArgs();
            showArgs = false;
        }
        else if (flagError) {
            cout << "\nAttempted to flag invalid tile." << endl;
            flagError = false;
        }

        int n, m;
        char f;
        cout << "\nrow col arg: ";
        cin >> n >> m >> f;
        
        if (n >= h || m >= w) {
            errorLast = true;
        }
        else if (f == '.') {
            b.reveal(n, m);
            if (b.isBomb(n, m)) lost = true;
        }
        else if (f == 'f' || f == 'F') {
            if (b.flagTile(n, m) == -1) flagError = true;
        }
        else if (f == 'r' || f == 'R') {
            b.unFlagTile(n, m);
        }
        else if (f == 'q' || f == 'Q') {
            return 0;
        }
        else if (f == 'w' || f == 'W') {
            b.revealAll();
            manualWin = true;
        }
        else if (f == 'h' || f == 'H') {
            showArgs = true;
        }
        else {
            errorLast = true;
        }
    }

    if (manualWin) {
        char temp;
        NL; b.printBoard();
        cout << "\nEnter q to quit. ";
        cin >> temp;
    }
    else if (lost) {
        char temp;
        NL; b.printBoard();
        cout << "\nYou lost! Enter q to quit. ";
        cin >> temp;
    }

    return 0;
}