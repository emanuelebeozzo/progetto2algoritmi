#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include "swrace.h"
#include <random>

using namespace std;

//Directives regarding cell content
#define EMPTY 0
#define WHITE 1
#define BLACK 2
//Directives for path contruction
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'
#define END '#'
//Define max path length 
#define MAX_LEN 256*256+1

//Output file where che results are saved
ofstream out("output.txt");

/**
 * Recerences: Unordered Set -  https://www.geeksforgeeks.org/how-to-create-an-unordered_set-of-user-defined-class-or-struct-in-c/
 */


/**
 * Struct cell contains ring informations
 *    row/col: position of the ring in the matrix
 *    type: type of ring
 *    latestMove: valid moves that indicate the possible moves from this ring
 *    valid: indicate if the conditions of the ring were aldready satisfied
 *    id: unique id that idetify the ring
 */
struct Cell {
  int row;
  int col;
  int type;
  vector<char> latestMove;
  bool valid = false;
  int id; 
  
  // This function is used by unordered_set to compare elements 
  bool operator==(const Cell& t) const{ 
    return (this->id == t.id); 
  } 
}; 

  
/*
 * Hash function used by ordered set to insert/find/remove element
 */
class MyHashFunction { 
  public: 
  // id is returned as hash function 
  size_t operator()(const Cell& t) const { 
    return t.id; 
  } 
};

/**
 * function used to sort the elements of the set
 * args:
 *    i: first pair to sort
 *    j: second pair to sort
 * return:
 *    true: if the first element is less than the second
 *    false: if the first element is greater or equal than the second
 */
bool sortVector (pair<int, char> i, pair<int, char> j) { return (i.first < j.first); }

/**
 * Function that returns true if a move is valid, false if it is invalid.
 * It checks if the move does not go out of the matrix and the cell has not already been visited.
 * args:
 *    visited: reference to the visited matrix
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    nextRow: next move row
 *    nextCol: next move column
 * return:
 *    true: if the move is valid
 *    false: if the first element is greater or equal than the second
 */
bool stop(vector<vector<bool>>& visited, int N, int M, int nextRow, int nextCol);


/**
 * Function that goes up until it founds a black ring
 * args:
 *    i: pointer to the current row index
 *    j: pointer to the current column index
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    length: lenght of the solution string
 *    nRing: number of encountered rings
 * return:
 *    true: if the black ring is found
 *    false: if the black ring is not found
 */
bool goUp(int *i, int *j, const int N, const int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing);

/**
 * Function that goes down until it founds a black ring
 * args:
 *    i: pointer to the current row index
 *    j: pointer to the current column index
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    length: lenght of the solution string
 *    nRing: number of encountered rings
 * return:
 *    true: if the black ring is found
 *    false: if the black ring is not found
 */
bool goDown(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing);

/**
 * Function that goes left until it founds a black ring
 * args:
 *    i: pointer to the current row index
 *    j: pointer to the current column index
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    length: lenght of the solution string
 *    nRing: number of encountered rings
 * return:
 *    true: if the black ring is found
 *    false: if the black ring is not found
 */
bool goLeft(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing);

/**
 * Function that goes left until it founds a black ring
 * args:
 *    i: pointer to the current row index
 *    j: pointer to the current column index
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    length: lenght of the solution string
 *    nRing: number of encountered rings
 * return:
 *    true: if the black ring is found
 *    false: if the black ring is not found
 */
bool goRigth(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing);

/**
 * Function that return the possible moves from a specific position to another position
 * args:
 *    visited: matrix of visited element
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    curretCell: current position where to start to find the possibile moves
 *    next: destination cell where we want to arrive
 *    pruntedMoves: array of valid moves we can take
 *    changeDirection: indicates if we have to turn in the next move 
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    us: set containing all the remaining rings
 *    goStraightOn: indicates if we have to go straight on in the next move 
 * return:
 *    true: if in the next moves we have to go straight on
 *    false: if we have no constraint in the next move
 */
bool preferredDirection(vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, bool& changeDirection, char *path, int cursor, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn);

/**
 * Function that return the possible moves from a specific ring to another position
 * args:
 *    visited: matrix of visited element
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    curretCell: current position of the ring where to start to find the possibile moves
 *    next: destination cell where we want to arrive
 *    pruntedMoves: array of valid moves we can take
 *    changeDirection: indicates if we have to turn in the next move 
 *    path: solution path
 *    cursor: cursor that points to the solution path's cell where to enter the next move
 *    us: set containing all the remaining rings
 *    goStraightOn: indicates if we have to go straight on in the next move 
 * return:
 *    true: if in the next moves we have to go straight on
 *    false: if we have no constraint in the next move
 */
bool preferredRingDirection(vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, char *path, int cursor, bool& changeDirection, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn);

/**
 * Function that returns the nearest ring to a specific position based on distance
 * args:
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    startRow: starting row
 *    startCol: starting column
 * return:
 *    min: the cell that is closest to the current position
 */
Cell nearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, int startRow, int startCol);

/**
 * Function that returns the nearest ring from a specific position based on distance or return a random element where to go
 * The return value is choose based on probability
 * args:
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    ringsSet: set containing all the rings
 *    startRow: starting row
 *    startCol: starting column
 * return:
 *    min: the cell where to go from the current position
 */
Cell maybeNotNearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, int startRow, int startCol);

/**
 * Function that sets a ring as a safe ring
 * args:
 *    currentRing: current ring, the ring wich will be set as a safe one
 *    safeRing: the resulting safe ring
 */
void setSafeRing(Cell& currentRing, Cell& safeRing);

/**
 * Wrapper function for greedyRecursion function. Used to avoid explicit copy of unordered set
 * args:
 *    nextRing: ring where to start the path
 *    unvisitedRings: set containing all the rings to visit
 *    validRing: set of valid visited rings 
 *    totalRings: set containing all the rings of the problem
 *    ringAmount: total number of rings (B+W) of the problem
 *    path: current solution path
 *    cursor: current solution cursor (move index)
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    visited: visited cells matrix
 *    partialPoint: worth of the curret path
 *    turnToClose: number of moves to do before try to close the path and get back to the starting ring
 * return:
 *    number of valid visited rings in the path
 */
int greedyWrapper(Cell nextRing, unordered_set<Cell, MyHashFunction> unvisitedRings, unordered_set<Cell, MyHashFunction> validRings, unordered_set<Cell, MyHashFunction> totalRings, int ringsAmount, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose);

/**
 * Wrapper function for notSoGreedyRecursion function. Used to avoid explicit copy of unordered set
 * args:
 *    nextRing: ring where to start the path
 *    unvisitedRings: set containing all the rings to visit
 *    validRing: set of valid visited rings 
 *    totalRings: set containing all the rings of the problem
 *    ringAmount: total number of rings (B+W) of the problem
 *    path: current solution path
 *    cursor: current solution cursor (move index)
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    visited: visited cells matrix
 *    partialPoint: worth of the curret path
 *    turnToClose: number of moves to do before try to close the path and get back to the starting ring
 * return:
 *    number of valid visited rings in the path
 */
int notSoGreedyWrapper(Cell nextRing, unordered_set<Cell, MyHashFunction> unvisitedRings, unordered_set<Cell, MyHashFunction> validRings, unordered_set<Cell, MyHashFunction> totalRings, int ringsAmount, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose);

/**
 *  THE GREAT WALL OF CHINA
 */

/**
 * Main function: ONE OF THE BRICK
 * It is based on an unproven but easily understood greedy principle, that is "scegli sempre la strada più facile" that means "always take the shortest way".
 * In fact we noticed that in some cases where there is a "low density of rings" it is very convenient to choose the ring closest to the current one as a target ring.
 * This function basically implements the choice to try to always reach the nearest ring by following the problem's rules
 * args:
 *    nextRing: ring chosen as target
 *    startRing: starting ring1
 *    safeRing: latest safe ring
 *    currentRow: current row in the matrix
 *    currentCol: current col in the matrix
 *    unvisitedRings: set of unvisited rings
 *    validRings: set of valid rings
 *    totalRings: set that contains all the rings 
 *    currentRing: number of encountered rings 
 *    ringsAmount: number of total rings
 *    path: current solution path
 *    cursor: current solution cursor (move index)
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    visited: visited cells matrix
 *    pendingMoves: number of pending moves
 *    toUnlock: stack which contains the pending moves that can be unlocked if a better path is found
 *    changeDirection: indicates whether it is necessary to change direction
 *    move: previous move
 *    partialSolution: score of the partial solution that is found
 *    goStraightOn: indicates whether it is necessary to go straight on
 *    turnToClose: number of moves that the algorithm will consider the starting ring as the target ring for creating a closed path
 * returns:
 *    true: it can be either because he has found the best solution, or because he has no more moves available considering the path taken so far
 *    false: the recursion is not finished yet 
 */
bool greedyRecursion(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& unvisitedRings, unordered_set<Cell, MyHashFunction>& validRings, unordered_set<Cell, MyHashFunction>& totalRings, int currentRings, const int ringsAmount, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose);

/**
 * Main function part 2. 
 * It is based on a simple expression, that is "non sempre la strada più giusta è la più semplice" that means "The right way is not always the easiest".
 * In fact we noticed that in some cases where there is an "high density of rings" it is more convenient to choose the not always the nearest. The target could be either the closest or a random one.
 * This function basically chooses with 50% probability the nearest ring, with the other 50% a random one.
 * args:
 *    nextRing: ring chosen as target
 *    startRing: starting ring1
 *    safeRing: latest safe ring
 *    currentRow: current row in the matrix
 *    currentCol: current col in the matrix
 *    unvisitedRings: set of unvisited rings
 *    validRings: set of valid rings
 *    totalRings: set that contains all the rings 
 *    currentRing: number of encountered rings 
 *    ringsAmount: number of total rings
 *    path: current solution path
 *    cursor: current solution cursor (move index)
 *    N: number of rows in the table
 *    M: number of columns in the table
 *    visited: visited cells matrix
 *    pendingMoves: number of pending moves
 *    toUnlock: stack which contains the pending moves that can be unlocked if a better path is found
 *    changeDirection: indicates whether it is necessary to change direction
 *    move: previous move
 *    partialSolution: score of the partial solution that is found
 *    goStraightOn: indicates whether it is necessary to go straight on
 *    turnToClose: number of moves that the algorithm will consider the starting ring as the target ring for creating a closed path
 * returns:
 *    true: it can be either because he has found the best solution, or because he has no more moves available considering the path taken so far
 *    false: the recursion is not finished yet 
 */
bool notSoGreedyRecursion(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& unvisitedRings, unordered_set<Cell, MyHashFunction>& validRings, unordered_set<Cell, MyHashFunction>& totalRings, int currentRings, const int ringsAmount, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose);

bool stop(vector<vector<bool>>& visited, int N, int M, int nextRow, int nextCol){
  bool needToStop = false;
  if(nextRow >= N || nextCol >= M || nextRow < 0 || nextCol < 0 || visited[nextRow][nextCol]){
    needToStop = true;
  }
  return needToStop;
}

bool goUp(int *i, int *j, const int N, const int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing){
  Cell currentCell;
  bool found = false;
  int resetI, resetCursor, resetLength, resetNRing;
  resetI = *i;
  resetCursor = *cursor;
  resetLength = *length;
  resetNRing =  *nRing;
  *i = *i-1;
  while(*i >= 0 && !found){
    currentCell.id = (*i)*M + (*j);
    currentCell.type = EMPTY;
    if(ringsSet.find(currentCell) != ringsSet.end()){
      currentCell = *(ringsSet.find(currentCell));
    }
    if(currentCell.type == BLACK){
      *nRing +=1;
      found = true;
    } else {
      if(currentCell.type == WHITE){
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


bool goDown(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing){
  Cell currentCell;
  bool found = false;
  int resetI, resetCursor, resetLength, resetNRing;
  resetI = *i;
  resetCursor = *cursor;
  resetLength = *length;
  resetNRing =  *nRing;
  *i = *i+1;
  while(*i < N && !found){
    currentCell.id = (*i)*M + (*j);
    currentCell.type = EMPTY;
    if(ringsSet.find(currentCell) != ringsSet.end()){
      currentCell = *(ringsSet.find(currentCell));
    }
    if(currentCell.type == BLACK){
      *nRing +=1;
      found = true;
    } else {
      if(currentCell.type == WHITE){
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


bool goLeft(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing){
  Cell currentCell;
  bool found = false;
  int resetJ, resetCursor, resetLength, resetNRing;
  resetJ = *j;
  resetCursor = *cursor;
  resetLength = *length;
  resetNRing =  *nRing;
  *j = *j-1;
  while(*j >= 0 && !found){
    currentCell.id = (*i)*M + (*j);
    currentCell.type = EMPTY;
    if(ringsSet.find(currentCell) != ringsSet.end()){
      currentCell = *(ringsSet.find(currentCell));
    }
    if(currentCell.type == BLACK){
      *nRing +=1;
      found = true;
    } else {
      if(currentCell.type == WHITE){
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

bool goRigth(int *i, int *j, int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, char *path, int *cursor, int *length, int *nRing){
  Cell currentCell;
  bool found = false;
  int resetJ, resetCursor, resetLength, resetNRing;
  resetJ = *j;
  resetCursor = *cursor;
  resetLength = *length;
  resetNRing =  *nRing;
  *j = *j+1;
  while(*j < M && !found){
    currentCell.id = (*i)*M + (*j);
    currentCell.type = EMPTY;
    if(ringsSet.find(currentCell) != ringsSet.end()){
      currentCell = *(ringsSet.find(currentCell));
    }
    if(currentCell.type == BLACK){
      *nRing +=1;
      found = true;
    } else {
      if(currentCell.type == WHITE){
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


bool preferredDirection(vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, bool& changeDirection, char *path, int cursor, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn){
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
      case 0: choice_dist.second = UP;
        break;
      case 1: choice_dist.second = LEFT;
        break;
      case 2: choice_dist.second = DOWN;
        break;
      case 3: choice_dist.second = RIGHT;
        break;
      default: printf("err\n"); break;
    }
    preferredMove.push_back(choice_dist);
  }
  //Moves pruning
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
    if(!stop(visited, N, M, nextRow, nextCol)){
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
                }
              } else {
                prunedMoves.push_back(preferredMove[i].second);
              }
            }
          } 
        }
      }
    }
  }
  goStraightOn = false;
  changeDirection = false;
  return toRtn;
}

bool preferredRingDirection(vector<vector<bool>>& visited, int N, int M, Cell& currentCell, Cell& next, vector<char>& prunedMoves, char *path, int cursor, bool& changeDirection, unordered_set<Cell, MyHashFunction>& us, bool& goStraightOn){
  int choice[2] = {-1, 1};
  int minDistance = INT_MAX;
  int tmpMin;
  bool toRtn = false;
  vector<pair<int, char>> preferredMove;
  for(int i = 0; i < 4; i++){
    //Mimimum distance and move calculus
    pair<int, char> choice_dist;
    if(i%2 == 0){
      choice_dist.first = abs(currentCell.row + choice[i/2] - next.row) + abs(currentCell.col - next.col);
    } else {
      choice_dist.first = abs(currentCell.row - next.row) + abs(currentCell.col + choice[i/2] - next.col);
    }
    switch(i){
      case 0:  choice_dist.second = UP;
        break;
      case 1: choice_dist.second = LEFT;
        break;
      case 2: choice_dist.second = DOWN;
        break;
      case 3: choice_dist.second = RIGHT;
        break;
      default: printf("err\n"); break;
    }
    preferredMove.push_back(choice_dist);
  }
  //Sort by lower cost
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

    //Moves puning
    if(!stop(visited, N, M, nextRow, nextCol)){
      bool toInsert = true;
      bool toReallyInsert = true;
      //RULES IMPLEMENTATION
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
      //RULES IMPLEMENTATION
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
                }
              } else {
                prunedMoves.push_back(preferredMove[i].second);
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

Cell nearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, int startRow, int startCol){
  int minDistance = INT_MAX;
  int tmpMin;
  Cell min;
  min.row = N;
  min.col = M;
  for(auto it = ringsSet.begin(); it != ringsSet.end(); it++){
    tmpMin = abs(startRow - (*it).row) + abs(startCol - (*it).col);
    if(minDistance > tmpMin){
      minDistance = tmpMin;
      min.row = (*it).row;
      min.col = (*it).col;
      min.id = (*it).id;
      min.type = (*it).type;
      min.latestMove = (*it).latestMove;
      min.valid = (*it).valid;
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

bool greedyRecursion(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& unvisitedRings, unordered_set<Cell, MyHashFunction>& validRings, unordered_set<Cell, MyHashFunction>& totalRings, int currentRings, const int ringsAmount, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose){
  bool isRing = false;
  bool closed = false;
  Cell currentCell;
  currentCell.col = currentCol;
  currentCell.row = currentRow;
  currentCell.id = currentRow*M + currentCol;
  vector<char> prunedMoves;
  //The move is added in the pending moves
  if(move != EMPTY){
    path[cursor] = move;
    cursor++;
    pendingMoves++;
    toUnlock.push(pair<int,int>(currentRow, currentCol));
  }
  //I am on the target ring
  if(nextRing.col == currentCol && nextRing.row == currentRow){
    if(unvisitedRings.find(nextRing) != unvisitedRings.end()){
      currentCell.type = nextRing.type;
      currentCell.latestMove = nextRing.latestMove;
      currentCell.valid = nextRing.valid;
      //If the ring is not the starting one
      //It is set as safe ring
      if(startRing.col != currentCol || startRing.row != currentRow){
        safeRing.col = currentCell.col;
        safeRing.row = currentCell.row;
        safeRing.id = currentCell.id;
        safeRing.type = currentCell.type;
        safeRing.latestMove = currentCell.latestMove;
        safeRing.valid = currentCell.valid;
        pendingMoves = 0;
      }
      //Not unvisited anymore
      currentRings++;
      turnToClose--;
      Cell tmp;
      unvisitedRings.erase(nextRing);
      validRings.insert(nextRing);
      isRing = true;
      //If I have to close then I set the target ring as the starting ring
      //Otherwise the next ring is the nearest ring based on the current position
      if(turnToClose == 0){
        nextRing.col = startRing.col;
        nextRing.row = startRing.row;
        nextRing.id = startRing.id;
        nextRing.type = startRing.type;
        nextRing.latestMove = startRing.latestMove;
        nextRing.valid = startRing.valid;
        turnToClose = 1;
      }else{
        tmp = nearestRing(N, M, unvisitedRings, safeRing.row, safeRing.col);
        nextRing.col = tmp.col;
        nextRing.row = tmp.row;
        nextRing.id = tmp.id;
        nextRing.type = tmp.type;
        nextRing.latestMove = tmp.latestMove;
        nextRing.valid = tmp.valid;
      }
    }
  } else if(totalRings.find(currentCell) != totalRings.end()){
    //The path ends up in a random ring, then it must be counted as a safe one
    unordered_set<Cell, MyHashFunction> :: iterator itr = totalRings.find(currentCell);
    currentCell.type = (*itr).type;
    currentCell.latestMove = (*itr).latestMove;
    currentCell.valid = (*itr).valid;

    safeRing.col = currentCell.col;
    safeRing.row = currentCell.row;
    safeRing.id = currentCell.id;
    safeRing.type = currentCell.type;
    safeRing.latestMove = currentCell.latestMove;
    safeRing.valid = currentCell.valid;

    //The path is closed
    if(currentCell.row == startRing.row && currentCell.col == startRing.col){
      return false;
    } else {
      unvisitedRings.erase(currentCell);
    }

    currentRings++;
    validRings.insert(currentCell);
    isRing = true;
    pendingMoves = 0;
    turnToClose--;

    //If I have to close then I set the target ring as the starting ring
    //Otherwise the next ring is the nearest ring based on the current position
    if(turnToClose == 0){
      nextRing.col = startRing.col;
      nextRing.row = startRing.row;
      nextRing.id = startRing.id;
      nextRing.type = startRing.type;
      nextRing.latestMove = startRing.latestMove;
      nextRing.valid = startRing.valid;
      turnToClose = 1;
    }else{
      nextRing = nearestRing(N, M, unvisitedRings, currentCell.row, currentCell.col);
    }
  }

  //Score of the current solution
  float currentSolution = 5*((float)validRings.size()/ringsAmount);
  if(currentCell.row != startRing.row || currentCell.col != startRing.col){
    currentSolution /= 2.0;
  }
  
  //Writing the solution only if it is valid and if it is better than the previous one
  if(partialSolution < currentSolution){
    if(!goStraightOn && !changeDirection){
      path[cursor] = '\0';
      if(currentRings == ringsAmount + 1){
        out << ringsAmount << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
      }else{
        out << validRings.size() << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
      }
      partialSolution = currentSolution;
    }
  } else {
    path[cursor] = '\0';
  }
  
  //Solution rollback. If the currentCell is the startinRing then pending moves are considered valid and the cursor is reset
  if(!(currentRow == startRing.row && currentCol == startRing.col)){
    visited[currentRow][currentCol] = true;
  } else {
    nextRing = nearestRing(N, M, unvisitedRings, safeRing.row, safeRing.col);
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

  //RULES IMPLEMENTATION
  if(cursor > 0){
    if(isRing){
      if(currentCell.type == WHITE){
        if(totalRings.find(currentCell) != totalRings.end()){
          totalRings.erase(currentCell);
          currentCell.latestMove.resize(1);
          currentCell.latestMove[0] = path[cursor-1];
          totalRings.insert(currentCell);
        }
      } else {
        if(totalRings.find(currentCell) != totalRings.end()){
          totalRings.erase(currentCell);
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
          totalRings.insert(currentCell);
        }
      }
    }
  }
  
  //If we are in a ring then we try to reach our target in the direction that is consider the best one, RULE IMPLEMENTATION PART 2
  if(isRing){
    if(preferredRingDirection(visited, N, M, currentCell, nextRing, prunedMoves, path, cursor, changeDirection, totalRings, goStraightOn)){
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
    if(preferredDirection(visited, N, M, currentCell, nextRing, prunedMoves, changeDirection, path, cursor, totalRings, goStraightOn)){
      goStraightOn = true;
    }
    changeDirection = false;
  }

  //If we are allowed to do a move
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

    //First move
    if(cursor == 0){
      if(isRing){
        if(currentCell.type == WHITE){
          if(totalRings.find(currentCell) != totalRings.end()){
            totalRings.erase(currentCell);
            currentCell.latestMove.resize(1);
            currentCell.latestMove[0] = prunedMoves[0];
            totalRings.insert(currentCell);
          }
          
        } else {
          if(totalRings.find(currentCell) != totalRings.end()){
            totalRings.erase(currentCell);
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
            totalRings.insert(currentCell);
          }
        }
      }
    }
    //Recursion
    if(greedyRecursion(nextRing, startRing, safeRing, nextRow, nextCol, unvisitedRings, validRings, totalRings, currentRings, ringsAmount, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, prunedMoves[0], partialSolution, goStraightOn, turnToClose)){
      return true;
    }
  }
  return false;
}

Cell maybeNotNearestRing(int N, int M, unordered_set<Cell, MyHashFunction>& ringsSet, int startRow, int startCol){
  int minDistance = INT_MAX;
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
    std::uniform_int_distribution<int> myUnifIntDist2(0, ringsSet.size()-1);
    int toExtract = myUnifIntDist2(myRandomEngine);
    for(auto it = ringsSet.begin(); it != ringsSet.end(); it++){
      if(toExtract == 0){
        min.row = (*it).row;
        min.col = (*it).col;
        min.id = (*it).id;
        min.type = (*it).type;
        min.latestMove = (*it).latestMove;
        min.valid = (*it).valid;
        return min;
      }
      toExtract--;
    }
  } else {
    for(auto it = ringsSet.begin(); it != ringsSet.end(); it++){
      tmpMin = abs(startRow - (*it).row) + abs(startCol - (*it).col);
      if(minDistance > tmpMin){
        minDistance = tmpMin;
        min.row = (*it).row;
        min.col = (*it).col;
        min.id = (*it).id;
        min.type = (*it).type;
        min.latestMove = (*it).latestMove;
        min.valid = (*it).valid;
      }
    }
  }
  return min;
}


bool notSoGreedyRecursion(Cell& nextRing, Cell& startRing, Cell& safeRing, int currentRow, int currentCol, unordered_set<Cell, MyHashFunction>& unvisitedRings, unordered_set<Cell, MyHashFunction>& validRings, unordered_set<Cell, MyHashFunction>& totalRings, int currentRings, const int ringsAmount, char *path, int& cursor, const int N, const int M, vector<vector<bool>> &visited, int pendingMoves, stack<pair<int, int>>& toUnlock, bool changeDirection, char move, double &partialSolution, bool& goStraightOn, int turnToClose){
  bool isRing = false;
  bool closed = false;
  Cell currentCell;
  currentCell.col = currentCol;
  currentCell.row = currentRow;
  currentCell.id = currentRow*M + currentCol;
  vector<char> prunedMoves;
  //The move is added in the pending moves
  if(move != EMPTY){
    path[cursor] = move;
    cursor++;
    pendingMoves++;
    toUnlock.push(pair<int,int>(currentRow, currentCol));
  }
  //I am on the target ring
  if(nextRing.col == currentCol && nextRing.row == currentRow){
    if(unvisitedRings.find(nextRing) != unvisitedRings.end()){
      currentCell.type = nextRing.type;
      currentCell.latestMove = nextRing.latestMove;
      currentCell.valid = nextRing.valid;
      //If the ring is not the starting one
      //It is set as safe ring
      if(startRing.col != currentCol || startRing.row != currentRow){
        safeRing.col = currentCell.col;
        safeRing.row = currentCell.row;
        safeRing.id = currentCell.id;
        safeRing.type = currentCell.type;
        safeRing.latestMove = currentCell.latestMove;
        safeRing.valid = currentCell.valid;
        pendingMoves = 0;
      } 
      //Not unvisited anymore
      currentRings++;
      turnToClose--;
      Cell tmp;
      isRing = true;
      //If I have to close then I set the target ring as the starting ring
      //Otherwise the next ring is the nearest ring based on the current position
      unvisitedRings.erase(nextRing);
      validRings.insert(nextRing);
      if(turnToClose == 0){
        nextRing.col = startRing.col;
        nextRing.row = startRing.row;
        nextRing.id = startRing.id;
        nextRing.type = startRing.type;
        nextRing.latestMove = startRing.latestMove;
        nextRing.valid = startRing.valid;
        turnToClose = 1;
      }else{
        tmp = maybeNotNearestRing(N, M, unvisitedRings, currentCell.row, currentCell.col);
        nextRing.col = tmp.col;
        nextRing.row = tmp.row;
        nextRing.id = tmp.id;
        nextRing.type = tmp.type;
        nextRing.latestMove = tmp.latestMove;
        nextRing.valid = tmp.valid;
      }
    }
  } else if(totalRings.find(currentCell) != totalRings.end()){
    //The path ends up in a random ring, then it must be counted as a safe one
    unordered_set<Cell, MyHashFunction> :: iterator itr = totalRings.find(currentCell);
    currentCell.type = (*itr).type;
    currentCell.latestMove = (*itr).latestMove;
    currentCell.valid = (*itr).valid;

    safeRing.col = currentCell.col;
    safeRing.row = currentCell.row;
    safeRing.id = currentCell.id;
    safeRing.type = currentCell.type;
    safeRing.latestMove = currentCell.latestMove;
    safeRing.valid = currentCell.valid;

    //The path is closed
    if(currentCell.row == startRing.row && currentCell.col == startRing.col){
      return false;
    } else {
      unvisitedRings.erase(currentCell);
    }

    currentRings++;
    validRings.insert(currentCell);
    isRing = true;
    pendingMoves = 0;
    turnToClose--;

    //If I have to close then I set the target ring as the starting ring
    //Otherwise the next ring is the nearest ring based on the current position
    if(turnToClose == 0){
      nextRing.col = startRing.col;
      nextRing.row = startRing.row;
      nextRing.id = startRing.id;
      nextRing.type = startRing.type;
      nextRing.latestMove = startRing.latestMove;
      nextRing.valid = startRing.valid;
      turnToClose = 1;
    }else{
      nextRing = maybeNotNearestRing(N, M, unvisitedRings, currentCell.row, currentCell.col);
    }
  }

  //Score of the current solution
  float currentSolution = 5*((float)validRings.size()/ringsAmount);
  if(currentCell.row != startRing.row || currentCell.col != startRing.col){
    currentSolution /= 2.0;
  } 

  //Writing the solution only if it is valid and if it is better than the previous one
  if(partialSolution < currentSolution){
    if(!goStraightOn && !changeDirection){
      path[cursor] = '\0';
      if(currentRings == ringsAmount + 1){
        out << ringsAmount << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
      }else{
        out << validRings.size() << " " << cursor << " " << startRing.row << " " << startRing.col << " " << path << END << endl;
      }
      partialSolution = currentSolution;
    }
  } else {
    path[cursor] = '\0';
  }

  //Solution rollback. If the currentCell is the startinRing then pending moves are considered valid and the cursor is reset
  if(!(currentRow == startRing.row && currentCol == startRing.col)){
    visited[currentRow][currentCol] = true;
  } else {
    nextRing = maybeNotNearestRing(N, M, unvisitedRings, safeRing.row, safeRing.col);
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

  //RULES IMPLEMENTATION
  if(cursor > 0){
    if(isRing){
      if(currentCell.type == WHITE){
        if(totalRings.find(currentCell) != totalRings.end()){
          totalRings.erase(currentCell);
          currentCell.latestMove.resize(1);
          currentCell.latestMove[0] = path[cursor-1];
          totalRings.insert(currentCell);
        }  
      } else {
        if(totalRings.find(currentCell) != totalRings.end()){
          totalRings.erase(currentCell);
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
          totalRings.insert(currentCell);
        }
      }
    }
  }

  //If we are in a ring then we try to reach our target in the direction that is consider the best one, RULE IMPLEMENTATION PART 2
  if(isRing){
    if(preferredRingDirection(visited, N, M, currentCell, nextRing, prunedMoves, path, cursor, changeDirection, totalRings, goStraightOn)){
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
    if(preferredDirection(visited, N, M, currentCell, nextRing, prunedMoves, changeDirection, path, cursor, totalRings, goStraightOn)){
      goStraightOn = true;
    }
    changeDirection = false;
  }

  //If we are allowed to do a move
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

    //First move
    if(cursor == 0){
      if(isRing){
        if(currentCell.type == WHITE){
          if(totalRings.find(currentCell) != totalRings.end()){
            totalRings.erase(currentCell);
            currentCell.latestMove.resize(1);
            currentCell.latestMove[0] = prunedMoves[0];
            totalRings.insert(currentCell);
          }
          
        } else {
          if(totalRings.find(currentCell) != totalRings.end()){
            totalRings.erase(currentCell);
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
            totalRings.insert(currentCell);
          }
        }
      }
    }
    
    if(notSoGreedyRecursion(nextRing, startRing, safeRing, nextRow, nextCol, unvisitedRings, validRings, totalRings, currentRings, ringsAmount, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, prunedMoves[0], partialSolution, goStraightOn, turnToClose)){
      return true;
    }
  }
  return false;
}


int greedyWrapper(Cell nextRing, unordered_set<Cell, MyHashFunction> unvisitedRings, unordered_set<Cell, MyHashFunction> validRings, unordered_set<Cell, MyHashFunction> totalRings, int ringsAmount, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose){
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
  greedyRecursion(nextRing, startRing,  safeRing, nextRing.row, nextRing.col, unvisitedRings, validRings, totalRings, currentRing, ringsAmount, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, EMPTY, partialPoint, goStraightOn, turnToClose);
  if(startRing.row == nextRing.row && startRing.col == nextRing.col){
    return validRings.size();
  } else {
    return 0;
  }
}


int notSoGreedyWrapper(Cell nextRing, unordered_set<Cell, MyHashFunction> unvisitedRings, unordered_set<Cell, MyHashFunction> validRings, unordered_set<Cell, MyHashFunction> totalRings, int ringsAmount, char *path, int cursor, int N, int M, vector<vector<bool>> visited, double &partialPoint, int turnToClose){
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
  notSoGreedyRecursion(nextRing, startRing,  safeRing, nextRing.row, nextRing.col, unvisitedRings, validRings, totalRings, currentRing, ringsAmount, path, cursor, N, M, visited, pendingMoves, toUnlock, changeDirection, EMPTY, partialPoint, goStraightOn, turnToClose);
  if(startRing.row == nextRing.row && startRing.col == nextRing.col){
    return validRings.size();
  } else {
    return 0;
  }
}



/**
 * Do or do not. There is no try. 
 */
int main(){
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

  vector<vector<bool>> visited;
  visited.resize(M, vector<bool>(N));
  int tmpI, tmpJ, startI, startJ;

  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      visited[i][j] = false;
    }
  }
  for(int i = 0; i < B; i++){
    Cell c;
    in >> c.row >> c.col;
    c.type = BLACK;
    c.id = c.row*M + c.col;
    c.valid = true;
    us.insert(c);
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
    c.id = c.row*M + c.col;
    us.insert(c);
  }

  //SQUARE CASE
  bool squared = false;
  if(B == 4) {
    squared = true;
    for(int i=0; i<4 && squared; i++){
      if(i%2==0) {
        if(!goRigth(&lastBlackRow,&lastBlackCol, N, M, us, path, &cursor, &length, &nRing)){
          if(!goLeft(&lastBlackRow,&lastBlackCol, N, M, us, path, &cursor, &length, &nRing)){
            squared = false;
          } else {
            squared = true;
          }
        }
      } else {
        if(!goUp(&lastBlackRow,&lastBlackCol, N, M, us, path, &cursor, &length, &nRing)){
          if(!goDown(&lastBlackRow,&lastBlackCol, N, M, us, path, &cursor, &length, &nRing)){
            squared = false;
          } else {
            squared = true;
          }
        }
      }
    }
    if(squared){
      path[cursor] = 0;
      out << nRing << " " << length << " " << lastBlackRow << " " << lastBlackCol << " " << path << END;
    } 
  }

  //NOT SQUARE CASE
  if(!squared) {
    std::random_device myRandomDevice;
    unsigned seed = myRandomDevice();
    
    std::default_random_engine myRandomEngine(seed);

    std::uniform_int_distribution<int> myUnifIntDist(0, N-1);

    Cell nextRing = nearestRing(N, M, us, 0, 0);
    double partialPoint = 0.0;
    int turnToClose = us.size()/2;
    int maxRingReached = -1;
    int currentRings = 0;

    //We call both the greedy and the random functions
    while(true){
      /**
       * GREEDY CALL
       */
      currentRings = greedyWrapper(nextRing, us, validRing, us, B+W, path, cursor, N, M, visited, partialPoint, turnToClose);
      
      //Updating of the number of rings to reach to try to close the path
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
      
      /**
       * NOT GREEDY CALL
       */
      currentRings = notSoGreedyWrapper(nextRing, us, validRing, us, B+W, path, cursor, N, M, visited, partialPoint, turnToClose);

      //Updating of the number of rings to reach to try to close the path
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
