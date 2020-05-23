#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include "swrace.h"
#include <random>

using namespace std;

#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'
#define MAX_LEN 256*256+1
#define END '#'

ofstream out("output.txt");
ofstream tuo("tuptuo.txt");

bool goUp(int *i, int *j, int N, int M, vector<vector<int>>& matrix, char *path, int *cursor, int *length, int *nRing){
    //cout << "ciao" << endl;
    bool found = false;
    int resetI, resetCursor, resetLength, resetNRing;
    resetI = *i;
    resetCursor = *cursor;
    resetLength = *length;
    resetNRing =  *nRing;
    *i = *i-1;
    while(*i >= 0 && !found){
        if(matrix[*i][*j] == BLACK){
            *nRing +=1;
            found = true;
        } else {
            if(matrix[*i][*j] == WHITE){
                *nRing +=1;
            }
            *i = *i-1;
        }
        path[*cursor] = UP;
        *cursor += 1;
    }
    if(!found){
        *i = resetI;
        *cursor = resetCursor;
        *length = resetLength;
        *nRing = resetNRing;
    } else {
        *length += resetI - *i;
    }
    return found;
}

bool goDown(int *i, int *j, int N, int M, vector<vector<int>>& matrix, char *path, int *cursor, int *length, int *nRing){
    //cout << "ciao 2" << endl;
    bool found = false;
    int resetI, resetCursor, resetLength, resetNRing;
    resetI = *i;
    resetCursor = *cursor;
    resetLength = *length;
    resetNRing =  *nRing;
    *i = *i+1;
    while(*i < N && !found){
        if(matrix[*i][*j] == BLACK){
            *nRing +=1;
            found = true;
        } else {
            if(matrix[*i][*j] == WHITE){
                *nRing +=1;
            }
            *i = *i+1;
        }
        path[*cursor] = DOWN;
        *cursor += 1;
    }
    if(!found){
        *i = resetI;
        *cursor = resetCursor;
        *length = resetLength;
        *nRing = resetNRing;
    } else {
        *length += *i - resetI;
    }
    return found;
}

bool goLeft(int *i, int *j, int N, int M, vector<vector<int>>& matrix, char *path, int *cursor, int *length, int *nRing){
    bool found = false;
    int resetJ, resetCursor, resetLength, resetNRing;
    resetJ = *j;
    resetCursor = *cursor;
    resetLength = *length;
    resetNRing =  *nRing;
    *j = *j-1;
    while(*j >= 0 && !found){
        if(matrix[*i][*j] == BLACK){
            *nRing +=1;
            found = true;
        } else {
            if(matrix[*i][*j] == WHITE){
                *nRing +=1;
            }
            *j = *j-1;
        }
        path[*cursor] = LEFT;
        *cursor += 1;
    }
    if(!found){
        *j = resetJ;
        *cursor = resetCursor;
        *length = resetLength;
        *nRing = resetNRing;
    } else {
        *length += resetJ - *j;
    }
    return found;
}

bool goRigth(int *i, int *j, int N, int M, vector<vector<int>>& matrix, char *path, int *cursor, int *length, int *nRing){
    bool found = false;
    int resetJ, resetCursor, resetLength, resetNRing;
    resetJ = *j;
    resetCursor = *cursor;
    resetLength = *length;
    resetNRing =  *nRing;
    *j = *j+1;
    while(*j < M && !found){
        if(matrix[*i][*j] == BLACK){
            *nRing +=1;
            found = true;
        } else {
            if(matrix[*i][*j] == WHITE){
                *nRing +=1;
            }
            *j = *j+1;
        }
        path[*cursor] = RIGHT;
        *cursor += 1;
    }
    if(!found){
        *j = resetJ;
        *cursor = resetCursor;
        *length = resetLength;
        *nRing = resetNRing;
    } else {
        *length += *j - resetJ;
    }
    return found;
}

void printMatrix(vector<vector<int>>& matrix, int N, int M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            //cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }
}

void printVisited(vector<vector<bool>>& matrix, int N, int M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            //cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }
}

/**
 * Links for reference
 * Unordered Set: https://www.geeksforgeeks.org/how-to-create-an-unordered_set-of-user-defined-class-or-struct-in-c/
 */

// Structure definition 
struct Cell {
    int row;
    int col;
    int type;
    vector<char> latestMove;
    bool valid = false;
    int priority;
    int id; 
  
    // This function is used by unordered_set to compare 
    // elements of Test. 
    bool operator==(const Cell& t) const
    { 
        return (this->id == t.id); 
    } 
}; 

bool stop(Cell& start, vector<vector<bool>>& visited, int N, int M, int nextRow, int nextCol){
    bool needToStop = false;
    if(nextRow >= N || nextCol >= M || nextRow < 0 || nextCol < 0 || visited[nextRow][nextCol]){
        needToStop = true;
    }
    return needToStop;
}
  
// class for hash function 
class MyHashFunction { 
    public: 
    // id is returned as hash function 
    size_t operator()(const Cell& t) const
    { 
        return t.id; 
    } 
};

bool sortVector (pair<int, char> i, pair<int, char> j) { return (i.first < j.first); }

unordered_set<Cell, MyHashFunction>::iterator nearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& us, int startRow, int startCol){
    int minDistance = INT_MAX;
    int minPriority = INT_MAX;
    int tmpMin;
    Cell min;
    min.row = N;
    min.col = M;
    unordered_set<Cell, MyHashFunction> :: iterator itr;
    itr = us.begin();
    for(auto it = us.begin(); it != us.end(); it++){
        tmpMin = abs(startRow - (*it).row) + abs(startCol - (*it).col);
        if((*it).priority < minPriority){
            minDistance = tmpMin;
            minPriority = (*it).priority;
            min = *it;
            itr = it;
        } else if((*it).priority == minPriority){
            if(minDistance > tmpMin){
                minDistance = tmpMin;
                minPriority = (*it).priority;
                min = *it;
                itr = it;
            }
        }
    }
    return itr;
}

bool preferredDirection(Cell& start, vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, bool& changeDirection, char *path, int cursor, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn){
    int choice[2] = {-1, 1};
    int minDistance = INT_MAX;
    int tmpMin;
    bool toRtn = false;
    vector<pair<int, char>> preferredMove;
    for(int i = 0; i < 4; i++){
        pair<int, char> choice_dist;
        if(i%2 == 0){
            choice_dist.first = abs(currentCell.row+choice[i/2] - next.row) + abs(currentCell.col - next.col);
        } else {
            choice_dist.first = abs(currentCell.row - next.row) + abs(currentCell.col+choice[i/2] - next.col);
        }
        bool atLeastOneBlocked = false;
        switch(i){
            case 0:  choice_dist.second = UP;
                /*if(currentCell.col == next.col){
                    if(currentCell.row+choice[i/2] < currentCell.row){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            case 1: choice_dist.second = LEFT;
                /*if(currentCell.row == next.row){
                    if(currentCell.col+choice[i/2] < currentCell.col){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            case 2: choice_dist.second = DOWN;
                /*if(currentCell.col == next.col){
                    if(currentCell.row+choice[i/2] > currentCell.row){
                        choice_dist.first += 2;
                    }
                }*/
                break;
            case 3: choice_dist.second = RIGHT;
                /*if(currentCell.row == next.row){
                    if(currentCell.col+choice[i/2] > currentCell.col){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            default: printf("err\n"); break;
        }
        preferredMove.push_back(choice_dist);
    }
    //Pruning delle mosse
    std::sort(preferredMove.begin(), preferredMove.end(), sortVector);
    for(int i = 0; i < preferredMove.size(); i++){
    }
    for(int i = 0; i < 4; i++){
        int nextRow;
        int nextCol;
        switch(preferredMove[i].second){
            case LEFT:
                nextRow = currentCell.row;
                nextCol = currentCell.col - 1;
                break;
            case UP:
                nextRow = currentCell.row - 1;
                nextCol = currentCell.col;
                break;
            case RIGHT:
                nextRow = currentCell.row;
                nextCol = currentCell.col + 1;
                break;
            case DOWN:
                nextRow = currentCell.row + 1;
                nextCol = currentCell.col;
                break;
        }
        //Insert if it is valid
        if(!stop(start, visited, N, M, nextRow, nextCol)){
            bool toInsert = true;
            bool toReallyInsert = true;
            if(toInsert){
                if(changeDirection){
                    switch(path[cursor-1]){
                        case LEFT:
                            preferredMove[i].second != LEFT ? toInsert=true : toInsert=false;
                            break;
                        case UP:
                            preferredMove[i].second != UP ? toInsert=true : toInsert=false;
                            break;
                        case RIGHT:
                            preferredMove[i].second != RIGHT ? toInsert=true : toInsert=false;
                            break;
                        case DOWN:
                            preferredMove[i].second != DOWN ? toInsert=true : toInsert=false;
                            break;
                    }
                }
                if(toInsert){
                    if(currentCell.latestMove.size() > 0){
                        toReallyInsert = false;
                    } else {
                        toReallyInsert = true;
                    }    
                    for(int j = 0; j < currentCell.latestMove.size(); j++){
                        if(currentCell.latestMove[j] == preferredMove[i].second){
                            toReallyInsert = true;
                            break;
                        }
                    }
                    if(toReallyInsert){
                        Cell nextMove;
                        bool isRing = false;
                        nextMove.id = nextRow*M + nextCol;
                        if(us.find(nextMove) != us.end()){
                            nextMove = *(us.find(nextMove));
                            isRing = true;
                        } else {
                            //! NOT GONNA TELL YOU AGAIN
                        }
                        if(nextMove.latestMove.size() > 0){
                            toReallyInsert = false;
                        } else {
                            toReallyInsert = true;
                        } 
                        for(int j = 0; j < nextMove.latestMove.size(); j++){
                            if(nextMove.latestMove[j] == preferredMove[i].second){
                                toReallyInsert = true;
                                break;
                            }
                        }
                        if(toReallyInsert){
                            if(isRing){
                                if(nextMove.type == BLACK){
                                    if(cursor > 0){
                                        switch(path[cursor-1]){
                                            case LEFT:
                                                preferredMove[i].second == LEFT ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case UP:
                                                preferredMove[i].second == UP ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case RIGHT:
                                                preferredMove[i].second == RIGHT ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case DOWN:
                                                preferredMove[i].second == DOWN ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                        }
                                        if(toReallyInsert == false){
                                            toRtn = true;
                                        }
                                    }
                                }
                            }
                        }
                        if(toReallyInsert){
                            if(goStraightOn){
                                switch(path[cursor-1]){
                                    case LEFT:
                                        preferredMove[i].second == LEFT ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case UP:
                                        preferredMove[i].second == UP ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case RIGHT:
                                        preferredMove[i].second == RIGHT ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case DOWN:
                                        preferredMove[i].second == DOWN ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                }
                                if(toReallyInsert){
                                    prunedMoves.push_back(preferredMove[i].second);
                                    //cout << "inserisco NON ANELLO PT 1: " << preferredMove[i].second << endl;
                                }
                            } else {
                                prunedMoves.push_back(preferredMove[i].second);
                                //cout << "inserisco NON ANELLO PT 2: " << preferredMove[i].second << endl;
                            }
                        }
                    } 
                }
            }
        } else {
            //! AGAIN WITH AN EMPTY ELSE
        }
    }
    goStraightOn = false;
    changeDirection = false;
    return toRtn;
}

bool preferredRingDirectionMK2(Cell &start, vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, char *path, int cursor, bool& changeDirection, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn){
    int choice[2] = {-1, 1};
    int minDistance = INT_MAX;
    int tmpMin;
    bool toRtn = false;
    vector<pair<int, char>> preferredMove;
    for(int i = 0; i < 4; i++){
        pair<int, char> choice_dist;
        if(i%2 == 0){
            choice_dist.first = abs(currentCell.row + choice[i/2] - next.row) + abs(currentCell.col - next.col);
        } else {
            choice_dist.first = abs(currentCell.row - next.row) + abs(currentCell.col + choice[i/2] - next.col);
        }
        switch(i){
            case 0:  choice_dist.second = UP;
                /*if(currentCell.col == next.col){
                    if(currentCell.row+choice[i/2] < currentCell.row){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            case 1: choice_dist.second = LEFT;
                /*if(currentCell.row == next.row){
                    if(currentCell.col+choice[i/2] < currentCell.col){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            case 2: choice_dist.second = DOWN;
                /*if(currentCell.col == next.col){
                    if(currentCell.row+choice[i/2] > currentCell.row){
                        choice_dist.first += 2;
                    }
                }*/
                break;
            case 3: choice_dist.second = RIGHT;
                /*if(currentCell.row == next.row){
                    if(currentCell.col+choice[i/2] > currentCell.col){
                            choice_dist.first += 2;
                    }
                }*/
                break;
            default: printf("err\n"); break;
        }
        preferredMove.push_back(choice_dist);
    }
    std::sort(preferredMove.begin(), preferredMove.end(), sortVector);
    for(int i = 0; i < 4; i++){
        int nextRow;
        int nextCol;
        switch(preferredMove[i].second){
            case LEFT:
                nextRow = currentCell.row;
                nextCol = currentCell.col - 1;
                break;
            case UP:
                nextRow = currentCell.row - 1;
                nextCol = currentCell.col;
                break;
            case RIGHT:
                nextRow = currentCell.row;
                nextCol = currentCell.col + 1;
                break;
            case DOWN:
                nextRow = currentCell.row + 1;
                nextCol = currentCell.col;
                break;
        }
        if(!stop(start, visited, N, M, nextRow, nextCol)){
            bool toInsert = true;
            bool toReallyInsert = true;
            if(currentCell.type == BLACK){
                if(cursor > 0){
                    switch(path[cursor-1]){
                        case LEFT:
                            preferredMove[i].second != LEFT ? toInsert=true : toInsert=false;
                            break;
                        case UP:
                            preferredMove[i].second != UP ? toInsert=true : toInsert=false;
                            break;
                        case RIGHT:
                            preferredMove[i].second != RIGHT ? toInsert=true : toInsert=false;
                            break;
                        case DOWN:
                            preferredMove[i].second != DOWN ? toInsert=true : toInsert=false;
                            break;
                    }
                }
                //TODO... set go straight to true and if it is set then the next need to check the previous move
            } else {
                if(cursor > 1){
                    if(path[cursor-1] != path[cursor-2]){
                        currentCell.valid = true;
                    }
                }
                if(cursor > 0){
                    switch(path[cursor-1]){
                        case LEFT:
                            preferredMove[i].second == LEFT ? toInsert=true : toInsert=false;
                            break;
                        case UP:
                            preferredMove[i].second == UP ? toInsert=true : toInsert=false;
                            break;
                        case RIGHT:
                            preferredMove[i].second == RIGHT ? toInsert=true : toInsert=false;
                            break;
                        case DOWN:
                            preferredMove[i].second == DOWN ? toInsert=true : toInsert=false;
                            break;
                    }
                }
            }
            if(toInsert){
                if(changeDirection){
                    switch(path[cursor-1]){
                        case LEFT:
                            preferredMove[i].second != LEFT ? toInsert=true : toInsert=false;
                            break;
                        case UP:
                            preferredMove[i].second != UP ? toInsert=true : toInsert=false;
                            break;
                        case RIGHT:
                            preferredMove[i].second != RIGHT ? toInsert=true : toInsert=false;
                            break;
                        case DOWN:
                            preferredMove[i].second != DOWN ? toInsert=true : toInsert=false;
                            break;
                    }
                }
                if(toInsert){
                    if(currentCell.latestMove.size() > 0){
                        toReallyInsert = false;
                    } else {
                        toReallyInsert = true;
                    }    
                    for(int j = 0; j < currentCell.latestMove.size(); j++){
                        if(currentCell.latestMove[j] == preferredMove[i].second){
                            toReallyInsert = true;
                            break;
                        }
                    }
                    if(toReallyInsert){
                        Cell nextMove;
                        bool isRing = false;
                        nextMove.id = nextRow*M + nextCol;
                        if(us.find(nextMove) != us.end()){
                            nextMove = *(us.find(nextMove));
                            isRing = true;
                        }
                        if(nextMove.latestMove.size() > 0){
                            toReallyInsert = false;
                        } else {
                            toReallyInsert = true;
                        } 
                        for(int j = 0; j < nextMove.latestMove.size(); j++){
                            if(nextMove.latestMove[j] == preferredMove[i].second){
                                toReallyInsert = true;
                                break;
                            }
                        }
                        if(toReallyInsert){
                            if(isRing){
                                if(nextMove.type == BLACK){
                                    if(cursor > 0){
                                        switch(path[cursor-1]){
                                            case LEFT:
                                                preferredMove[i].second == LEFT ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case UP:
                                                preferredMove[i].second == UP ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case RIGHT:
                                                preferredMove[i].second == RIGHT ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                            case DOWN:
                                                preferredMove[i].second == DOWN ? toReallyInsert=true : toReallyInsert=false;
                                                break;
                                        }
                                        if(toReallyInsert == false){
                                            toRtn = true;
                                        }
                                    }
                                }
                            }
                        }
                        if(toReallyInsert){
                            if(goStraightOn){
                                switch(path[cursor-1]){
                                    case LEFT:
                                        preferredMove[i].second == LEFT ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case UP:
                                        preferredMove[i].second == UP ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case RIGHT:
                                        preferredMove[i].second == RIGHT ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                    case DOWN:
                                        preferredMove[i].second == DOWN ? toReallyInsert=true : toReallyInsert=false;
                                        break;
                                }
                                if(toReallyInsert){
                                    prunedMoves.push_back(preferredMove[i].second);
                                    //cout << "inserisco ANELLO PT 1: " << preferredMove[i].second << endl;
                                }
                            } else {
                                prunedMoves.push_back(preferredMove[i].second);
                                //cout << "inserisco ANELLO PT 2: " << preferredMove[i].second << endl;
                            }
                        }
                    } 
                }
            }
        } else {
            if(nextRow >= 0 && nextRow < N && nextCol >= 0 && nextCol < M){
            }
        }
    }
    changeDirection = false;
    goStraightOn = false;
    return toRtn;
}




bool reachableCell(Cell& next, int N, int M, int nextRow, int nextCol){
    bool reachable = false;
    if(nextRow < N && nextCol < M && nextRow >= 0 && nextCol >= 0 && nextRow == next.row && nextCol == next.col){
        reachable = true;
    }
    return reachable;
}

Cell nearestRingV2(int N, int M, unordered_set<Cell, MyHashFunction>& us, int startRow, int startCol){
    int minDistance = INT_MAX;
    int minPriority = INT_MAX;
    int tmpMin;
    Cell min;
    min.row = N;
    min.col = M;
    for(auto it = us.begin(); it != us.end(); it++){
        tmpMin = abs(startRow - (*it).row) + abs(startCol - (*it).col);
        if((*it).priority < minPriority){
                minDistance = tmpMin;
                minPriority = (*it).priority;
                min.row = (*it).row;
                min.col = (*it).col;
                min.id = (*it).id;
                min.type = (*it).type;
                min.priority = (*it).priority;
                min.latestMove = (*it).latestMove;
                min.valid = (*it).valid;
        } else if((*it).priority == minPriority){
            if(minDistance > tmpMin){
                minDistance = tmpMin;
                minPriority = (*it).priority;
                min.row = (*it).row;
                min.col = (*it).col;
                min.id = (*it).id;
                min.type = (*it).type;
                min.priority = (*it).priority;
                min.latestMove = (*it).latestMove;
                min.valid = (*it).valid;
            }
        }
    }
    return min;
}

void setSafeRing(Cell& currentRing, Cell& safeRing){
    safeRing.col = currentRing.col;
    safeRing.row = currentRing.row;
    safeRing.id = currentRing.id;
    safeRing.type = currentRing.type;
    safeRing.latestMove = currentRing.latestMove;
    safeRing.valid = currentRing.valid;
}

bool greedyV3(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& us, unordered_set<Cell, MyHashFunction>& validRing, unordered_set<Cell, MyHashFunction>& AHHHHHHHHHHHHHH, int currentRing, const int totalRing, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose){
    bool isRing = false;
    bool closed = false;
    Cell currentCell;
    currentCell.col = currentCol;
    currentCell.row = currentRow;
    currentCell.id = currentRow*M + currentCol;
    vector<char> prunedMoves;
    //cout << "goStraightOn settata a " << goStraightOn << endl;
    //cout << "sono " << currentRow << " " << currentCol << " voglio andare a " << nextRing.row << " " << nextRing.col << " e posso chiudermi tra " << turnToClose << endl;
    if(move != EMPTY){
        path[cursor] = move;
        cursor++;
        pendingMoves++;
        toUnlock.push(pair<int,int>(currentRow, currentCol));
    }
    //! PENDING MOVES
    if(nextRing.col == currentCol && nextRing.row == currentRow){
        if(us.find(nextRing) != us.end()){
            currentCell.type = nextRing.type;
            currentCell.latestMove = nextRing.latestMove;
            currentCell.valid = nextRing.valid;
            if(startRing.col != currentCol || startRing.row != currentRow){
                safeRing.col = currentCell.col;
                safeRing.row = currentCell.row;
                safeRing.id = currentCell.id;
                safeRing.type = currentCell.type;
                safeRing.latestMove = currentCell.latestMove;
                safeRing.valid = currentCell.valid;
                pendingMoves = 0;
            }
            currentRing++;
            turnToClose--;
            // TODO... check if all data are catafrattamente distrutti male
            Cell tmp;
            us.erase(nextRing);
            validRing.insert(nextRing);
            isRing = true;

            if(turnToClose == 0){
                nextRing.col = startRing.col;
                nextRing.row = startRing.row;
                nextRing.id = startRing.id;
                nextRing.type = startRing.type;
                nextRing.latestMove = startRing.latestMove;
                nextRing.valid = startRing.valid;
                nextRing.priority = startRing.priority;
                //TODO si può anche cambiare
                turnToClose = 1;
            }else{
                tmp = nearestRingV2(N, M, us, safeRing.row, safeRing.col);
                //cout << "nuovo target " << tmp.row << " " << tmp.col << endl;
                nextRing.col = tmp.col;
                nextRing.row = tmp.row;
                nextRing.id = tmp.id;
                nextRing.type = tmp.type;
                nextRing.latestMove = tmp.latestMove;
                nextRing.valid = tmp.valid;
                nextRing.priority = tmp.priority;
            }
            // TODO... if you are in the starting node the unlock the path you used until now to reach the start, set the safe ring to the last reached and the start another recursion
        }
    } else if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
            unordered_set<Cell, MyHashFunction> :: iterator itr = AHHHHHHHHHHHHHH.find(currentCell);
            currentCell.priority = (*itr).priority;
            currentCell.type = (*itr).type;
            currentCell.latestMove = (*itr).latestMove;
            currentCell.valid = (*itr).valid;

            safeRing.col = currentCell.col;
            safeRing.row = currentCell.row;
            safeRing.id = currentCell.id;
            safeRing.type = currentCell.type;
            safeRing.latestMove = currentCell.latestMove;
            safeRing.valid = currentCell.valid;
            if(currentCell.row == startRing.row && currentCell.col == startRing.col){
                return false;
            } else {
                us.erase(currentCell);
            }
            currentRing++;
            validRing.insert(currentCell);
            // TODO... check if all data are catafrattamente distrutti male
            isRing = true;
            pendingMoves = 0;
            turnToClose--;
            if(turnToClose == 0){
                nextRing.col = startRing.col;
                nextRing.row = startRing.row;
                nextRing.id = startRing.id;
                nextRing.type = startRing.type;
                nextRing.latestMove = startRing.latestMove;
                nextRing.valid = startRing.valid;
                nextRing.priority = startRing.priority;
                //TODO si può anche cambiare
                turnToClose = 1;
            }else{
                nextRing = nearestRingV2(N, M, us, currentCell.row, currentCell.col);
            }
    }

    float currentSolution = 5*((float)validRing.size()/totalRing);
    
    if(currentCell.row != startRing.row || currentCell.col != startRing.col){
        currentSolution /= 2.0;
    } else {
        //! EMPTY ELSE LMAO
    }
    
    if(partialSolution < currentSolution){
        if(!goStraightOn && !changeDirection){
            path[cursor] = '\0';
            if(currentRing == totalRing + 1){
                out << totalRing << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
            }else{
                out << validRing.size() << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
            }
            partialSolution = currentSolution;
        }
    } else {
        path[cursor] = '\0';
    }
    
    if(!(currentRow == startRing.row && currentCol == startRing.col)){
        visited[currentRow][currentCol] = true;
    } else {
        nextRing = nearestRingV2(N, M, us, safeRing.row, safeRing.col);
        currentCell.col = safeRing.col;
        currentCell.row = safeRing.row;
        currentCell.id = safeRing.id;
        currentCell.type = safeRing.type;
        currentCell.latestMove = safeRing.latestMove;
        currentCell.valid = safeRing.valid;
        for(int i = 0; i < pendingMoves; i++){
            pair<int, int> extracted = toUnlock.top();
            visited[extracted.first][extracted.second] = false;
            toUnlock.pop();
        }
        cursor = cursor - pendingMoves;
        pendingMoves = 0;
        isRing = true;
    }
    if(cursor > 0){
        if(isRing){
            if(currentCell.type == WHITE){
                if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                    AHHHHHHHHHHHHHH.erase(currentCell);
                    currentCell.latestMove.resize(1);
                    currentCell.latestMove[0] = path[cursor-1];
                    AHHHHHHHHHHHHHH.insert(currentCell);
                }
            } else {
                if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                    AHHHHHHHHHHHHHH.erase(currentCell);
                    currentCell.latestMove.resize(2);
                    switch(path[cursor-1]){
                        case LEFT:
                            currentCell.latestMove.push_back(UP);
                            currentCell.latestMove.push_back(DOWN);
                            break;
                        case UP:
                            currentCell.latestMove.push_back(LEFT);
                            currentCell.latestMove.push_back(RIGHT);
                            break;
                        case RIGHT:
                            currentCell.latestMove.push_back(UP);
                            currentCell.latestMove.push_back(DOWN);
                            break;
                        case DOWN:
                            currentCell.latestMove.push_back(LEFT);
                            currentCell.latestMove.push_back(RIGHT);
                            break;
                    }
                    AHHHHHHHHHHHHHH.insert(currentCell);
                }
            }
        }
    }
    
    if(isRing){
        if(preferredRingDirectionMK2(startRing, visited, N, M, currentCell, nextRing, prunedMoves, path, cursor, changeDirection, AHHHHHHHHHHHHHH, goStraightOn)){
            goStraightOn = true;
        }
        if(currentCell.type == WHITE){
            if(currentCell.valid == false){
                changeDirection = true;
            } else {
                changeDirection = false;
            }
        } else {
            goStraightOn = true;
        }
    } else {
        if(preferredDirection(startRing, visited, N, M, currentCell, nextRing, prunedMoves, changeDirection, path, cursor, AHHHHHHHHHHHHHH, goStraightOn)){
            goStraightOn = true;
        }
        changeDirection = false;
    }
    if(prunedMoves.size()>0){
        int nextRow;
        int nextCol;
        switch(prunedMoves[0]){
            case LEFT:
                nextRow = currentCell.row;
                nextCol = currentCell.col - 1;
                break;
            case UP:
                nextRow = currentCell.row - 1;
                nextCol = currentCell.col;
                break;
            case RIGHT:
                nextRow = currentCell.row;
                nextCol = currentCell.col + 1;
                break;
            case DOWN:
                nextRow = currentCell.row + 1;
                nextCol = currentCell.col;
                break;
        }
        if(cursor == 0){
            if(isRing){
                if(currentCell.type == WHITE){
                    if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                        AHHHHHHHHHHHHHH.erase(currentCell);
                        currentCell.latestMove.resize(1);
                        currentCell.latestMove[0] = prunedMoves[0];
                        AHHHHHHHHHHHHHH.insert(currentCell);
                    }
                    
                } else {
                    if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                        AHHHHHHHHHHHHHH.erase(currentCell);
                        currentCell.latestMove.resize(2);
                        switch(prunedMoves[0]){
                            case LEFT:
                                currentCell.latestMove.push_back(UP);
                                currentCell.latestMove.push_back(DOWN);
                                break;
                            case UP:
                                currentCell.latestMove.push_back(LEFT);
                                currentCell.latestMove.push_back(RIGHT);
                                break;
                            case RIGHT:
                                currentCell.latestMove.push_back(UP);
                                currentCell.latestMove.push_back(DOWN);
                                break;
                            case DOWN:
                                currentCell.latestMove.push_back(LEFT);
                                currentCell.latestMove.push_back(RIGHT);
                                break;
                        }
                        AHHHHHHHHHHHHHH.insert(currentCell);
                    }
                }
            }
        }
        
        if(greedyV3(nextRing, startRing, safeRing, nextRow, nextCol, us, validRing, AHHHHHHHHHHHHHH, currentRing, totalRing, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, prunedMoves[0], partialSolution, goStraightOn, turnToClose)){
            return true;
        } else {
            //! EMPTY AGAIN
        }
    }
}


Cell maybeNotNearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& us, int startRow, int startCol){
    int minDistance = INT_MAX;
    int minPriority = INT_MAX;
    int tmpMin;
    Cell min;
    min.row = N;
    min.col = M;
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();
    
    // Initialize a default_random_engine with the seed
    std::default_random_engine myRandomEngine(seed);

    // Initialize a uniform_int_distribution to produce values between -10 and 10
    std::uniform_int_distribution<int> myUnifIntDist(0, 100);

    int soMushRandomImGonnaCry = myUnifIntDist(myRandomEngine);
    if(soMushRandomImGonnaCry <= 50){
        std::uniform_int_distribution<int> myUnifIntDist2(0, us.size()-1);
        int toExtract = myUnifIntDist2(myRandomEngine);
        for(auto it = us.begin(); it != us.end(); it++){
            if(toExtract == 0){
                min.row = (*it).row;
                min.col = (*it).col;
                min.id = (*it).id;
                min.type = (*it).type;
                min.priority = (*it).priority;
                min.latestMove = (*it).latestMove;
                min.valid = (*it).valid;
                return min;
            }
            toExtract--;
        }
    } else {
        for(auto it = us.begin(); it != us.end(); it++){
            tmpMin = abs(startRow - (*it).row) + abs(startCol - (*it).col);
            if((*it).priority < minPriority){
                    minDistance = tmpMin;
                    minPriority = (*it).priority;
                    min.row = (*it).row;
                    min.col = (*it).col;
                    min.id = (*it).id;
                    min.type = (*it).type;
                    min.priority = (*it).priority;
                    min.latestMove = (*it).latestMove;
                    min.valid = (*it).valid;
            } else if((*it).priority == minPriority){
                if(minDistance > tmpMin){
                    minDistance = tmpMin;
                    minPriority = (*it).priority;
                    min.row = (*it).row;
                    min.col = (*it).col;
                    min.id = (*it).id;
                    min.type = (*it).type;
                    min.priority = (*it).priority;
                    min.latestMove = (*it).latestMove;
                    min.valid = (*it).valid;
                }
            }
        }
    }
    return min;
}


bool notSoGreedyAnymoreBitch(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& us, unordered_set<Cell, MyHashFunction>& validRing, unordered_set<Cell, MyHashFunction>& AHHHHHHHHHHHHHH, int currentRing, const int totalRing, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose){
    bool isRing = false;
    bool closed = false;
    Cell currentCell;
    //cout << "sono " << currentRow << " " << currentCol << " voglio andare a " << nextRing.row << " " << nextRing.col << " e posso chiudermi tra " << turnToClose << endl;
    currentCell.col = currentCol;
    currentCell.row = currentRow;
    currentCell.id = currentRow*M + currentCol;
    vector<char> prunedMoves;
    if(move != EMPTY){
        path[cursor] = move;
        cursor++;
        pendingMoves++;
        toUnlock.push(pair<int,int>(currentRow, currentCol));
    }
    //! PENDING MOVES
    if(nextRing.col == currentCol && nextRing.row == currentRow){
        if(us.find(nextRing) != us.end()){
            currentCell.type = nextRing.type;
            currentCell.latestMove = nextRing.latestMove;
            currentCell.valid = nextRing.valid;
            if(startRing.col != currentCol || startRing.row != currentRow){
                safeRing.col = currentCell.col;
                safeRing.row = currentCell.row;
                safeRing.id = currentCell.id;
                safeRing.type = currentCell.type;
                safeRing.latestMove = currentCell.latestMove;
                safeRing.valid = currentCell.valid;
                pendingMoves = 0;
            }
            currentRing++;
            turnToClose--;
            Cell tmp;
            isRing = true;
            us.erase(nextRing);
            validRing.insert(nextRing);
            // TODO... check if all data are catafrattamente distrutti male
            if(turnToClose == 0){
                nextRing.col = startRing.col;
                nextRing.row = startRing.row;
                nextRing.id = startRing.id;
                nextRing.type = startRing.type;
                nextRing.latestMove = startRing.latestMove;
                nextRing.valid = startRing.valid;
                nextRing.priority = startRing.priority;
                //TODO si può anche cambiare
                turnToClose = 1;
            }else{
                tmp = maybeNotNearestRing(N, M, us, currentCell.row, currentCell.col);
                nextRing.col = tmp.col;
                nextRing.row = tmp.row;
                nextRing.id = tmp.id;
                nextRing.type = tmp.type;
                nextRing.latestMove = tmp.latestMove;
                nextRing.valid = tmp.valid;
                nextRing.priority = tmp.priority;
            }
            // TODO... if you are in the starting node the unlock the path you used until now to reach the start, set the safe ring to the last reached and the start another recursion
        }
    } else if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
            unordered_set<Cell, MyHashFunction> :: iterator itr = AHHHHHHHHHHHHHH.find(currentCell);
            currentCell.priority = (*itr).priority;
            currentCell.type = (*itr).type;
            currentCell.latestMove = (*itr).latestMove;
            currentCell.valid = (*itr).valid;

            safeRing.col = currentCell.col;
            safeRing.row = currentCell.row;
            safeRing.id = currentCell.id;
            safeRing.type = currentCell.type;
            safeRing.latestMove = currentCell.latestMove;
            safeRing.valid = currentCell.valid;
            if(currentCell.row == startRing.row && currentCell.col == startRing.col){
                return false;
            } else {
                us.erase(currentCell);
            }
            currentRing++;
            validRing.insert(currentCell);
            isRing = true;
            pendingMoves = 0;
            turnToClose--;
            if(turnToClose == 0){
                nextRing.col = startRing.col;
                nextRing.row = startRing.row;
                nextRing.id = startRing.id;
                nextRing.type = startRing.type;
                nextRing.latestMove = startRing.latestMove;
                nextRing.valid = startRing.valid;
                nextRing.priority = startRing.priority;
                //TODO si può anche cambiare
                turnToClose = 1;
            }else{
                nextRing = maybeNotNearestRing(N, M, us, currentCell.row, currentCell.col);
            }
    }
    float currentSolution = 5*((float)validRing.size()/totalRing);
    if(currentCell.row != startRing.row || currentCell.col != startRing.col){
        currentSolution /= 2.0;
    } else {
        //! WHY NOT AN EMPTY ELSE
    }
    if(partialSolution < currentSolution){
        if(!goStraightOn && !changeDirection){
            path[cursor] = '\0';
            if(currentRing == totalRing + 1){
                out << totalRing << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
            }else{
                out << validRing.size() << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
            }
            partialSolution = currentSolution;
        }
    } else {
        path[cursor] = '\0';
    }
    if(!(currentRow == startRing.row && currentCol == startRing.col)){
        visited[currentRow][currentCol] = true;
    } else {
        nextRing = maybeNotNearestRing(N, M, us, safeRing.row, safeRing.col);
        currentCell.col = safeRing.col;
        currentCell.row = safeRing.row;
        currentCell.id = safeRing.id;
        currentCell.type = safeRing.type;
        currentCell.latestMove = safeRing.latestMove;
        currentCell.valid = safeRing.valid;
        for(int i = 0; i < pendingMoves; i++){
            pair<int, int> extracted = toUnlock.top();
            visited[extracted.first][extracted.second] = false;
            toUnlock.pop();
        }
        cursor = cursor - pendingMoves;
        pendingMoves = 0;
        //printVisited(visited, N, M);
        isRing = true;
    }
    if(cursor > 0){
        if(isRing){
            if(currentCell.type == WHITE){
                if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                    AHHHHHHHHHHHHHH.erase(currentCell);
                    currentCell.latestMove.resize(1);
                    currentCell.latestMove[0] = path[cursor-1];
                    AHHHHHHHHHHHHHH.insert(currentCell);
                }  
            } else {
                if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                    AHHHHHHHHHHHHHH.erase(currentCell);
                    currentCell.latestMove.resize(2);
                    switch(path[cursor-1]){
                        case LEFT:
                            currentCell.latestMove.push_back(UP);
                            currentCell.latestMove.push_back(DOWN);
                            break;
                        case UP:
                            currentCell.latestMove.push_back(LEFT);
                            currentCell.latestMove.push_back(RIGHT);
                            break;
                        case RIGHT:
                            currentCell.latestMove.push_back(UP);
                            currentCell.latestMove.push_back(DOWN);
                            break;
                        case DOWN:
                            currentCell.latestMove.push_back(LEFT);
                            currentCell.latestMove.push_back(RIGHT);
                            break;
                    }
                    AHHHHHHHHHHHHHH.insert(currentCell);
                }
            }
        }
    }
    if(isRing){
        if(preferredRingDirectionMK2(startRing, visited, N, M, currentCell, nextRing, prunedMoves, path, cursor, changeDirection, AHHHHHHHHHHHHHH, goStraightOn)){
            goStraightOn = true;
        }
        if(currentCell.type == WHITE){
            if(currentCell.valid == false){
                changeDirection = true;
            } else {
                changeDirection = false;
            }
        } else {
            goStraightOn = true;
        }
    } else {
        if(preferredDirection(startRing, visited, N, M, currentCell, nextRing, prunedMoves, changeDirection, path, cursor, AHHHHHHHHHHHHHH, goStraightOn)){
            goStraightOn = true;
        }
        changeDirection = false;
    }
    if(prunedMoves.size()>0){
        int nextRow;
        int nextCol;
        switch(prunedMoves[0]){
            case LEFT:
                nextRow = currentCell.row;
                nextCol = currentCell.col - 1;
                break;
            case UP:
                nextRow = currentCell.row - 1;
                nextCol = currentCell.col;
                break;
            case RIGHT:
                nextRow = currentCell.row;
                nextCol = currentCell.col + 1;
                break;
            case DOWN:
                nextRow = currentCell.row + 1;
                nextCol = currentCell.col;
                break;
        }
        if(cursor == 0){
            if(isRing){
                if(currentCell.type == WHITE){
                    if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                        AHHHHHHHHHHHHHH.erase(currentCell);
                        currentCell.latestMove.resize(1);
                        currentCell.latestMove[0] = prunedMoves[0];
                        AHHHHHHHHHHHHHH.insert(currentCell);
                    }
                    
                } else {
                    if(AHHHHHHHHHHHHHH.find(currentCell) != AHHHHHHHHHHHHHH.end()){
                        AHHHHHHHHHHHHHH.erase(currentCell);
                        currentCell.latestMove.resize(2);
                        switch(prunedMoves[0]){
                            case LEFT:
                                currentCell.latestMove.push_back(UP);
                                currentCell.latestMove.push_back(DOWN);
                                break;
                            case UP:
                                currentCell.latestMove.push_back(LEFT);
                                currentCell.latestMove.push_back(RIGHT);
                                break;
                            case RIGHT:
                                currentCell.latestMove.push_back(UP);
                                currentCell.latestMove.push_back(DOWN);
                                break;
                            case DOWN:
                                currentCell.latestMove.push_back(LEFT);
                                currentCell.latestMove.push_back(RIGHT);
                                break;
                        }
                        AHHHHHHHHHHHHHH.insert(currentCell);
                    }
                }
            }
        }
        
        if(notSoGreedyAnymoreBitch(nextRing, startRing, safeRing, nextRow, nextCol, us, validRing, AHHHHHHHHHHHHHH, currentRing, totalRing, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, prunedMoves[0], partialSolution, goStraightOn, turnToClose)){
            return true;
        } else {
            //! BEST ELSE IS EMPTY ELSE
        }
    }
}


int wrapper(Cell nextRing, unordered_set<Cell, MyHashFunction> us, unordered_set<Cell, MyHashFunction> validRing, unordered_set<Cell, MyHashFunction> AHHHHHHHHHHHHHH, int totalRing, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose){
    Cell safeRing;
    Cell startRing;
    startRing.col = safeRing.col = nextRing.col;
    startRing.row = safeRing.row = nextRing.row;
    startRing.id = safeRing.id = nextRing.id;
    startRing.type = safeRing.type = nextRing.type;
    startRing.latestMove = safeRing.latestMove = nextRing.latestMove;
    startRing.valid = safeRing.valid = nextRing.valid;
    int currentRing = 0;
    int pendingMoves = 0;
    cursor = 0;
    path[0] = 0;
    stack<pair<int,int>> toUnlock;
    bool changeDirection = false;
    bool goStraightOn = false;
    greedyV3(nextRing, startRing,  safeRing, nextRing.row, nextRing.col, us, validRing, AHHHHHHHHHHHHHH, currentRing, totalRing, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, EMPTY, partialPoint, goStraightOn, turnToClose);
    //TODO... keep going maybe there's hope
    if(startRing.row == nextRing.row && startRing.col == nextRing.col){
        return validRing.size();
    } else {
        return 0;
    }
}

int wrapper2(Cell nextRing, unordered_set<Cell, MyHashFunction> us, unordered_set<Cell, MyHashFunction> validRing, unordered_set<Cell, MyHashFunction> AHHHHHHHHHHHHHH, int totalRing, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose){
    Cell safeRing;
    Cell startRing;
    startRing.col = safeRing.col = nextRing.col;
    startRing.row = safeRing.row = nextRing.row;
    startRing.id = safeRing.id = nextRing.id;
    startRing.type = safeRing.type = nextRing.type;
    startRing.latestMove = safeRing.latestMove = nextRing.latestMove;
    startRing.valid = safeRing.valid = nextRing.valid;
    int currentRing = 0;
    int pendingMoves = 0;
    cursor = 0;
    path[0] = 0;
    stack<pair<int,int>> toUnlock;
    bool changeDirection = false;
    bool goStraightOn = false;
    notSoGreedyAnymoreBitch(nextRing, startRing,  safeRing, nextRing.row, nextRing.col, us, validRing, AHHHHHHHHHHHHHH, currentRing, totalRing, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, EMPTY, partialPoint, goStraightOn, turnToClose);
    if(startRing.row == nextRing.row && startRing.col == nextRing.col){
        return validRing.size();
    } else {
        return 0;
    }
    //TODO... keep going maybe there's hope
}



/**
 * Do or do not. There is no try.
 */
int main(){

    //! CERCARE DI FERMARLO IL PRIMA POSSIBILE QUANDO NON PUO' CHIUDERSI, SCELTE RANDOM
    //! L'ALGORITMO DEVE PROVARE A CHIUDERSI SOLO DOPO AVER ATTRAVERSATO UN NUMERO MAGGIORE DI ANELLI RISPETTO A UN PERCORSO GIA' CHIUSO


    int N, M, B, W;
    int cursor = 0;
    int length = 0;
    int nRing = 0;
    int soluzRing = 0;
    char path[MAX_LEN];
    char soluzpath[MAX_LEN];
    bool isPreviousWhite = false;
    int lastBlackRow, lastBlackCol;

    ifstream in("input.txt");
    in >> N >> M >> B >> W;

    unordered_set<Cell, MyHashFunction> us;
    unordered_set<Cell, MyHashFunction> validRing;
    unordered_set<Cell, MyHashFunction> AHHHHHHHHHHHHHH;

    vector<vector<int>> matrix;
    matrix.resize(M, vector<int>(N));
    vector<vector<bool>> visited;
    visited.resize(M, vector<bool>(N));
    int tmpI, tmpJ, startI, startJ;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            matrix[i][j] = EMPTY;
            visited[i][j] = false;
        }
    }
    for(int i = 0; i < B; i++){
        Cell c;
        in >> c.row >> c.col;
        c.type = BLACK;
        c.priority = 0;
        c.id = c.row*M + c.col;
        c.valid = true;
        us.insert(c);
        AHHHHHHHHHHHHHH.insert(c);
        matrix[c.row][c.col] = BLACK;
        lastBlackCol = c.col;
        lastBlackRow = c.row;
    }

    for(int i = 0; i < W; i++){
        Cell c;
        in >> c.row >> c.col;
        c.type = WHITE;
        if(c.row == 0 || c.row == N-1){
            c.latestMove.push_back(LEFT);
            c.latestMove.push_back(RIGHT);
        } else if(c.col == 0 || c.col == M-1){
            c.latestMove.push_back(UP);
            c.latestMove.push_back(DOWN);
        }
        c.priority = 0;
        c.id = c.row*M + c.col;
        us.insert(c);
        AHHHHHHHHHHHHHH.insert(c);
        matrix[c.row][c.col] = WHITE;
    }

    bool squared = false;
    if(B == 4) {
        squared = true;
        for(int i=0; i<4 && squared; i++){
            if(i%2==0) {
                if(!goRigth(&lastBlackRow,&lastBlackCol, N, M, matrix, path, &cursor, &length, &nRing)){
                    if(!goLeft(&lastBlackRow,&lastBlackCol, N, M, matrix, path, &cursor, &length, &nRing)){
                        squared = false;
                    } else {
                        squared = true;
                    }
                }
            } else {
                if(!goUp(&lastBlackRow,&lastBlackCol, N, M, matrix, path, &cursor, &length, &nRing)){
                    if(!goDown(&lastBlackRow,&lastBlackCol, N, M, matrix, path, &cursor, &length, &nRing)){
                        squared = false;
                    } else {
                        squared = true;
                    }
                }
            }
        }
        if(squared){
            path[cursor] = 0;
            ofstream out("output.txt");
            out << nRing << " " << length << " " << lastBlackRow << " " << lastBlackCol << " " << path << END;
        } 
    }

    if(!squared) {

        std::random_device myRandomDevice;
        unsigned seed = myRandomDevice();
        
        std::default_random_engine myRandomEngine(seed);

        std::uniform_int_distribution<int> myUnifIntDist(0, N-1);

        Cell nextRing = nearestRingV2(N, M, us, 0, 0);
        double partialPoint = 0.0;
        int turnToClose = us.size()/2;
        int maxRingReached = -1;
        int currentRings = 0;

        while(true){
            currentRings = wrapper(nextRing, us, validRing, AHHHHHHHHHHHHHH, B+W, path, cursor, N, M, visited, partialPoint, turnToClose);
            //cout << "PRIMA: turn to close pari a " << turnToClose << " anelli massimi raggiunti " << maxRingReached << " anelli trovati ora " << currentRings << endl;
            if(maxRingReached < currentRings){
                maxRingReached = currentRings;
            } else {
                if(maxRingReached > turnToClose){
                    turnToClose = maxRingReached;
                }
            }

            if(maxRingReached >= turnToClose){
                turnToClose = maxRingReached;
            } else {
                turnToClose /= 2;
                if(turnToClose < maxRingReached){
                    turnToClose = maxRingReached;
                }
            }

            nextRing = maybeNotNearestRing(N, M, us, myUnifIntDist(myRandomEngine), myUnifIntDist(myRandomEngine));
            //cout << "DOPO: turn to close pari a " << turnToClose << "anelli massimi raggiunti " << maxRingReached << " anelli trovati ora " << currentRings << endl;
            
            currentRings = wrapper2(nextRing, us, validRing, AHHHHHHHHHHHHHH, B+W, path, cursor, N, M, visited, partialPoint, turnToClose);

            if(maxRingReached < currentRings){
                maxRingReached = currentRings;
            } else {
                if(maxRingReached > turnToClose){
                    turnToClose = maxRingReached;
                }
            }

            if(maxRingReached >= turnToClose){
                turnToClose = maxRingReached;
            } else {
                turnToClose /= 2;
                if(turnToClose < maxRingReached){
                    turnToClose = maxRingReached;
                }
            }
            
            nextRing = maybeNotNearestRing(N, M, us, myUnifIntDist(myRandomEngine), myUnifIntDist(myRandomEngine));
            
        }
    }
    return 0;
}