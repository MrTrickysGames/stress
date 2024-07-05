#include <bits/stdc++.h>
using namespace std;

struct Piesa {
    int piesa[7][7];
    int type;
};

const int TMAX = 14, NMAX = 91, ZMAX = 21;
int a[TMAX][TMAX];
int adv[TMAX][TMAX];
bool ban[ZMAX];
Piesa v[NMAX];
clock_t start_time;

void clear_board() {
    for (int i = 0; i < TMAX; ++i)
        for (int j = 0; j < TMAX; ++j)
            a[i][j] = 0;
    cout << "= \n\n";
}

void cputime() {
    cout << "= " << (clock() - start_time) / (double)CLOCKS_PER_SEC << "\n\n";
}

void initialize() {
    ifstream fin("pieces.txt");
    if (!fin) {
        cerr << "Error opening file!" << endl;
        return;
    }

    int aux, index = 0, type = 0;
    while (fin >> aux) {
        while (aux--) {
            for (int i = 0; i < 7; ++i) {
                for (int j = 0; j < 7; ++j) {
                    fin >> v[index].piesa[i][j];
                    v[index].type = type;
                }
            }
            index++;
        }
        type++;
    }

    fin.close();

    /*for (int aux1 = 0; aux1 < NMAX; ++aux1) {
        cout << aux1 << " " << v[aux1].type << "\n";
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                cout << v[aux1].piesa[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n\n\n";
    }*/
}

bool isValid(int x, int y, int S[7][7]) {
    int ii = 0, jj = 0, k = 0;
    for (int i = x; i < x + 7; ++i) {
        for (int j = y; j < y + 7; ++j) {
            if(i>=0 && j>=0){

            if ((i >= TMAX || j >= TMAX) && S[ii][jj] == 1)
                return false;
            if (S[ii][jj] == 1 && (a[i][j] == 1 || a[i][j] == 7))
                return false;
            if (S[ii][jj] == 1 && adv[i][j] == 1)
                return false;
            if (S[ii][jj] == 2 && a[i][j] == 1)
                k = 1;
            }
            jj++;
        }
        ii++;
        jj = 0;
    }
    return k == 1;
}

void mutare(int x, int y, int S[7][7]) {
    cout<<"= ";
    int ii = 0, jj = 0, first_l=1;
    for (int i = x; i < TMAX && i < x + 7; ++i) {
        for (int j = y; j < TMAX && j < y + 7; ++j) {
                    if (S[ii][jj]==1){
                            //cout<<i<<" "<<j<<"\n";
                            char litera = 'a'+j;
                            int num = 14 - i;

                            if (first_l){
                                cout << litera << num;
                                first_l=0;
                            }
                            else{
                                cout<< ","<<litera << num;
                            }
                    }
                jj++;
                }
        ii++;
        jj = 0;
    }
    cout<<"\n\n";
}

void stamp(int x, int y, int S[7][7]) {
    int ii = 0, jj = 0;
    for (int i = x; i < TMAX && i < x + 7; ++i) {
        for (int j = y; j < TMAX && j < y + 7; ++j) {
            if (S[ii][jj] != 0) {
                if (S[ii][jj] == 2) {
                    if (a[i][j] == 0)
                        a[i][j] = S[ii][jj];
                } else {
                    if (!(a[i][j] == 7 && S[ii][jj] == 2)) {
                        a[i][j] = S[ii][jj];
                    }
                }
            }
            jj++;
        }
        ii++;
        jj = 0;
    }
}

int scoreBoard() {
    int score = 0;
    for (int i = 0; i < TMAX; ++i) {
        for (int j = 0; j < TMAX; ++j) {
            if (a[i][j] == 1) {
                score++;
            }
        }
    }
    return score;
}

int simulateRandomGame() {
    int original_a[TMAX][TMAX];
    memcpy(original_a, a, sizeof(a)); // Backup the current board state

    while (true) {
        bool moveFound = false;
        for (int z = 0; z < NMAX; ++z) {
            if (!ban[v[z].type]) {
                int x = rand() % TMAX;
                int y = rand() % TMAX;
                if (isValid(x, y, v[z].piesa)) {
                    stamp(x, y, v[z].piesa);
                    ban[v[z].type] = 1;
                    moveFound = true;
                    break;
                }
            }
        }
        if (!moveFound) {
            break; // No more valid moves
        }
    }

    int score = scoreBoard();
    memcpy(a, original_a, sizeof(a)); // Restore the board state
    return score;
}

void genmove(char color) {
    static bool FIRST = true;
    if (FIRST) {
        if (color == 'w') {
            cout << "= j6, j5, j7, i6, k6 \n\n";
            stamp(7, 7, v[0].piesa);
        } else if (color == 'b') {
            cout << "= e11,e10,e9,d10,f10 \n\n";
            stamp(2, 2, v[0].piesa);
        }
        FIRST = false;
        ban[v[0].type] = 1;
    } else {
        int bestScore = -1;
        int bestPiece = -1;
        int bestX = -1, bestY = -1;
        int original_a[TMAX][TMAX];
        bool original_ban[ZMAX];
        memcpy(original_a, a, sizeof(a)); // Backup the current board state
        memcpy(original_ban, ban, sizeof(ban)); // Backup the ban state

        for (int z = 0; z < NMAX; ++z) {
            if (!ban[v[z].type]) {
                for (int i = 0; i < TMAX; ++i) {
                    for (int j = 0; j < TMAX; ++j) {
                        if (isValid(i, j, v[z].piesa)) {
                            stamp(i, j, v[z].piesa);
                            ban[v[z].type] = 1;
                            int totalScore = 0;

                            // Run simulations
                            int numSimulations = 1000; // Adjust as needed
                            for (int sim = 0; sim < numSimulations; ++sim) {
                                totalScore += simulateRandomGame();
                            }

                            int avgScore = totalScore / numSimulations;
                            if (avgScore > bestScore) {
                                bestScore = avgScore;
                                bestPiece = z;
                                bestX = i;
                                bestY = j;
                            }

                            // Restore board state and ban state
                            memcpy(a, original_a, sizeof(a));
                            memcpy(ban, original_ban, sizeof(ban));
                        }
                    }
                }
            }
        }

        if (bestPiece != -1) {
            stamp(bestX, bestY, v[bestPiece].piesa);
            ban[v[bestPiece].type] = 1;
            //cout << "= "<<bestPiece<<" "<<bestX<<" "<<bestY<<"\n";
            mutare(bestX, bestY, v[bestPiece].piesa);

            /*cout<<"a: \n";
            for (int i=0; i<TMAX; ++i){
                for (int j=0; j<TMAX; ++j){
                    cout<<a[i][j]<<" ";
                }
                cout<<"\n";
            }
            cout<<"\n\n";*/
        }
    }
}

vector<pair<char, int>> separateLettersAndNumbers(const string &input) {
    vector<pair<char, int>> result;
    istringstream stream(input);
    string segment;

    while (getline(stream, segment, ',')) {
        char letter = segment[0];
        int number = stoi(segment.substr(1));
        result.emplace_back(letter, number);
    }

    return result;
}

void translate(char letter, int Num) {
    int y = letter - 'a';
    int x = 14 - Num;
    //cout << x << " " << y << "\n";
    adv[x][y] = 1;
}

int main() {
    initialize();

    start_time = clock();
    string command;
    // Main loop to listen for commands
    while (true) {
        getline(cin, command);
        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "set_game") {
            std::string game;
            iss >> game;
            cout << "= \n\n";
        } else if (cmd == "clear_board") {
            clear_board();
        } else if (cmd == "cputime") {
            cputime();
        } else if (cmd == "genmove") {
            char color;
            iss >> color;
            genmove(color);
        } else if (cmd == "play") {
            char color;
            string Move;
            iss >> color >> Move;
            vector<pair<char, int>> out_m = separateLettersAndNumbers(Move);
            for (const auto &p : out_m) {
                translate(p.first, p.second);
            }
            cout << "= \n\n";

            /*cout<<"adv: \n";
            for (int i = 0; i < TMAX; ++i) {
                for (int j = 0; j < TMAX; ++j) {
                    cout << adv[i][j] << " ";
                }
                cout << "\n";
            }
            cout<<"\n\n";*/
        } else if (cmd == "quit") {
            return 0;
        } else {
            cerr << "? unknown command" << endl;
            return 0;

        }
    }
    return 0;
}
