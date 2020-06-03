#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <limits>
using namespace std;

enum Val {EMPTY, IND, BOMB};

int itest = 0;
#define ITEST cout << "Test #" << itest++ << endl;
#define NL cout << endl; // newline
#define NP system("clear");

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
    bool isFlagged();

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

bool Tile::isFlagged() {
    return flagged;
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
    int flags;

    // gets
    bool isDone();
    bool isBomb(int, int);
    vector<vector<Tile>> getBoard();

    // functionality
    int reveal(int, int);
    void revealAll();
    int flagTile(int, int);
    int unFlagTile(int, int);

    // prints
    void printBoard();
    void printBoardSpecial(int, int);
};

// constructors
bool isIn(vector<int> v, int n) { // a helper function essential to the constructor
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

    flags = p;

    for (int i = 0; i < p; i++) { // p tiles set to be randomly placed mines
        int spot = rand() % (n*m); if (spot == (n*m)) spot--;
        spots.push_back(spot);
    }
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
            if (!bd[i][j].isFound() && !bd[i][j].isFlagged()) return false; // we aren't done until all tiles are discovered, or flagged.
        }
    }
    return true;
}

bool Board::isBomb(int n, int m) {
    return (bd[n][m].getVal() == BOMB);
}

vector<vector<Tile>> Board::getBoard() {
    return bd;
}

// functionality
int Board::reveal(int n, int m) {
    if (bd[n][m].isFlagged()) {
        return 1;
    }
    else if (bd[n][m].getVal() != EMPTY) {
        bd[n][m].find();
        return 0;
    }
    else {
        bd[n][m].find();
        vector<vector<int>> adj = findAdjacent(n, m);
        for (auto s: adj) {
            if (bd[s[0]][s[1]].getVal() != BOMB && !bd[s[0]][s[1]].isFound() && !bd[s[0]][s[1]].isFlagged()) {
                reveal(s[0],s[1]); // cursed(?) I guess not.
            }
        }
        return 0;
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
    else if (bd[n][m].isFlagged()) {
        return -1; // we're trying to flag a tile that already has a flag
    }
    flags--;
    bd[n][m].flag();
    return 0;
}

int Board::unFlagTile(int n, int m) {
    if (!bd[n][m].isFlagged()) {
        return -1; // we're trying to unflag a tile with no flag on it
    }
    flags++;
    bd[n][m].unFlag();
    return 0;
}

// prints
void Board::printBoard() {
    if (w < 51) {
        // numbering each column
        cout << "\t";
        for (int i = 0; i < w; i++) {
            if (i < 10) cout << i << "  ";
            else cout << i << " ";
        } cout << endl << endl;

        // the rows
        for (int i = 0; i < h; i++) {
            cout << i << "\t";
            for (int j = 0; j < w; j++) {
                bd[i][j].printVal(); cout << "  ";
            }
            cout << endl;
        }
    }
    else {
        // special numbering system
        cout << "\t";
        for (int i = 0; i < w; i++) {
            if (!(i % 10)) cout << i/10 << " ";
            else cout << "  ";
        } cout << "\n\t";
        for (int i = 0; i < w; i++) {
            cout << i % 10 << " ";
        } cout << endl << endl;

        // the rows
        for (int i = 0; i < h; i++) {
            cout << i << "\t";
            for (int j = 0; j < w; j++) {
                bd[i][j].printVal(); cout << " ";
            }
            cout << endl;
        }
    }
}

void Board::printBoardSpecial(int n, int m) {
    if (w < 51) { // WIDTH NOT HEIGHT DUMMY
        // numbering each column
        cout << "\t";
        for (int i = 0; i < w; i++) {
            if (i < 10) cout << i << "  ";
            else cout << i << " ";
        } cout << endl << endl;

        // the rest of it
        for (int i = 0; i < h; i++) {
            cout << i << "\t";
            for (int j = 0; j < w; j++) {
                if (bd[i][j].getVal() == BOMB) cout << "B  ";
                else cout << "   ";
            }
            cout << endl;
        }
    }
    else {
        // special numbering system
        cout << "\t";
        for (int i = 0; i < w; i++) {
            if (!(i % 10)) cout << i/10 << " ";
            else cout << "  ";
        } cout << "\n\t";
        for (int i = 0; i < w; i++) {
            cout << i % 10 << " ";
        } cout << endl << endl;

        // the rows
        for (int i = 0; i < h; i++) {
            cout << i << "\t";
            for (int j = 0; j < w; j++) {
                if (bd[i][j].getVal() == BOMB) cout << "B ";
                else cout << "  ";
            }
            cout << endl;
        }
    }
}

// -------------------------------------------------------------------- MAIN --------------------------------------------------------------------

int stringToInt(string s) {
    int ret = 0;
    int m = 1;
    for (int i = s.size()-1; i >= 0; i--) {
        ret = ret + ((int)s[i]-48)*m;
        m *= 10;
    }
    return ret;
}

int insAreOk(vector<string> inp) {
    for (int i = 0; i < inp.size(); i++) {
        if (stringToInt(inp[i]) < 1 && i != 2) { // if one of the dimensions is less than 1
            return 1;
        } 
        else if (stringToInt(inp[2]) > 100 || stringToInt(inp[2]) < 1) { // if the percentage exceeds 100, or is less than 1
            return 2;
        }
    }
    return 0;
}

void printArgs() {
    cout << "\nAvailable args are:" << endl << endl;
    cout << ".\tto reveal the tile," << endl;
    cout << "f/F\tto place a flag," << endl;
    cout << "r/R\tto remove a flag," << endl;
    cout << "w/W\tto win the game," << endl;
    cout << "h/H\tto just display arguments again, and" << endl;
    cout << "q/Q\tto quit." << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: ./mine height width percent" << endl;
        return 1;
    }

    vector<string> inp;
    for (int i = 1; i < argc; i++) {
        inp.push_back(argv[i]);
    }

    if (insAreOk(inp) == 1) {
        cout << "Invalid height or width. Cannot be less than 1." << endl;
        return 1;
    }
    else if (insAreOk(inp) == 2) {
        cout << "Invalid percentage." << endl;
        return 2;
    }

    int h = stringToInt(inp[0]);
    int w = stringToInt(inp[1]);
    int p = stringToInt(inp[2]);

    bool first = true;
    bool errorLast = false;
    bool showArgs = false;
    bool flagError = false;
    bool manualWin = false;
    bool lost = false;
    bool invalidReveal = false;
    srand(time(NULL));

    p = int((h*w)*(p*0.01));
    if (p < 1) {
        p = 1;
    }

    Board b(h, w, p); 
    int n, m;

    while (!b.isDone()) {
        if (lost || manualWin) break;

        NP;
        b.printBoard();

        if (first) { 
            cout << "\nThe board has " << p << " mines on it." << endl;
            if (w > 50) cout << "Because your board is wider than 50, the formatting is more compact. The top row of numbers is tens." << endl;
            cout << "Enter your target coordinates, followed by arg, all separated by spaces." << endl;

            printArgs();

            cout << "\nFor example, to flag the 7th tile in the 15th row, type: \"15 7 f\"." << endl;
            first = false;
        }
        else if (errorLast) {
            cout << "\nInvalid argument(s), try again. " << endl;
            cout << flush;
            n = 0; m = 0;
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
        else if (invalidReveal) {
            cout << "\nCannot reveal a flagged tile." << endl;
            invalidReveal = false;
        }
        
        char f;

        if (b.flags != 1) cout << "\nYou have " << b.flags << " flags remaining." << endl;
        else cout << "\nYou have " << b.flags << " flag remaining." << endl;
        cout << "row col arg: ";

        // checking for invalid input
        while (!(cin >> n >> m >> f)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Invalid argument(s), try again. ";
        }
        
        if (n >= h || m >= w) {
            errorLast = true;
        }
        else if (f == '.') {
            if (b.getBoard()[n][m].isFlagged()) invalidReveal = true; // since it only seems to work sometimes.
            else if (b.reveal(n, m) == 1) invalidReveal = true;
            else if (b.isBomb(n, m)) lost = true;
        }
        else if (f == 'f' || f == 'F') {
            if (b.getBoard()[n][m].isFlagged()) flagError = true; // since it only seems to work sometimes.
            else if (b.flagTile(n, m) == 1) flagError = true;
        }
        else if (f == 'r' || f == 'R') {
            b.unFlagTile(n, m);
        }
        else if (f == 'q' || f == 'Q') {
            system("PAUSE");
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

    if (lost) {
        NP; b.printBoardSpecial(n,m);
        cout << "\nYou lost!" << endl;
        return 0;
    }

    // won
    NP; b.printBoard();
    cout << "\nYou win!" << endl;

    return 0;
}