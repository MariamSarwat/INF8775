#include <iostream> 
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <climits>

using namespace std;

struct Position {
    int row;
    int column;
};

int NBR_ROWS = 0;
int NBR_COLUMNS = 0;

//Regarde les voisins autour et trouve le max et retourne sa position
Position findMaxNeighbour(vector<vector<pair<int, int>>> profit, Position currentPosition) {
    Position newPosition = {0, 0};
    int row = currentPosition.row;
    int column = currentPosition.column;

    int x_min = (row != 0)? row - 1 : 0;
    int x_max = (row != NBR_ROWS - 1)?  row + 1 : row;

    int y_min = (column != 0)? column - 1 : 0;
    int y_max = (column != NBR_COLUMNS - 1)?  column + 1 : column;

    int maxElement = 0;

    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            if (profit[x][y].second == 0) {
                if (profit[x][y].first > maxElement) {
                    maxElement = profit[x][y].first;
                    newPosition = {x, y};
                }
            }
        }
    }

    if (newPosition.row == 0 && newPosition.column == 0) {
        for (int x = 0; x < NBR_ROWS; x++) {
            for (int y = 0; y < NBR_COLUMNS; y++) {
                if (profit[x][y].second == 0) {
                    if (profit[x][y].first > maxElement) {
                        maxElement = profit[x][y].first;
                        newPosition = {x, y};
                    }
                }
            }
        }
    }

    if (newPosition.row == 0 && newPosition.column == 0) {
        newPosition = {INT_MIN, INT_MIN};
    }

    return newPosition;
}

void verifyCondition(vector<vector<pair<int, int>>>& profit, vector<Position>& currentPath, Position newPosition) {
    vector<Position> positionsToVerify;
    vector<Position> tempPath;
    positionsToVerify.push_back(newPosition);
    tempPath.push_back(newPosition);

    while (positionsToVerify.size() > 0) {
        Position newPosition;
        
        Position pos = positionsToVerify.at(0);
        positionsToVerify.erase(positionsToVerify.begin());

        int row_min = 0;
        if (pos.row != 0) {
            row_min = pos.row - 1;
        }
        else break;

        int column_min = (pos.column != 0)? pos.column - 1 : pos.column;
        int column_max = (pos.column != NBR_COLUMNS - 1)? pos.column + 1 : pos.column;

        if(profit[row_min][column_max].second == 0) {
            profit[row_min][column_max].second = 1;
            newPosition = {row_min, column_max};
            positionsToVerify.push_back(newPosition);
            tempPath.push_back(newPosition);
        }

        if (profit[row_min][pos.column].second == 0) {
            profit[row_min][pos.column].second = 1;
            newPosition = {row_min, pos.column};
            positionsToVerify.push_back(newPosition);
            tempPath.push_back(newPosition);
        }
        
        if (profit[row_min][column_min].second == 0) { 
            profit[row_min][column_min].second = 1;
            newPosition = {row_min, column_min};
            positionsToVerify.push_back(newPosition);
            tempPath.push_back(newPosition);
        }
    }

    for(int i = tempPath.size() - 1; i >= 0; i--)
        currentPath.push_back(tempPath[i]);
}

int64_t getCurrentProfit(vector<vector<pair<int, int>>> profit, vector<Position> path) {
    int64_t currentProfit = 0;
    for (int i = 0; i < path.size(); i++) {
        currentProfit += profit[path[i].row][path[i].column].first;
    }
    return currentProfit;
}

pair<int64_t, Position> getBlock(vector<vector<pair<int, int>>>& profit, vector<Position>& currentPath, Position newPosition) {
    int64_t currentProfit = 0;

    newPosition = findMaxNeighbour(profit, newPosition);

    if (newPosition.row != INT_MIN && newPosition.column != INT_MIN) {
        profit[newPosition.row][newPosition.column].second = 1;
        
        verifyCondition(profit, currentPath, newPosition);
        currentProfit = getCurrentProfit(profit, currentPath);
    } else {
        currentProfit = INT64_MIN;
    }

    return make_pair(currentProfit, newPosition);
}

//Print help menu
static void showUsage(std::string name)
{
    std::cerr << "Usage: " << name << " -e CHEMIN_EXEMPLAIRE" << std::endl;
}

//Read File
vector<vector<pair<int,int>>> readExempFile(std::string filePath)
{
    std::ifstream inputFileStream(filePath);
    std::string line;

    bool isFirstLine = true;
    bool isGoldMatrix = false, isCostMatrix = false;

    vector<vector<int>> gold;
    vector<vector<int>> value;
    vector<vector<pair<int, int>>> profit;
    int number;

    if (inputFileStream) {
        while (std::getline(inputFileStream, line)) {

            if (isFirstLine) {
                NBR_ROWS = std::stoi(line.substr(0, line.find(' ')));
                NBR_COLUMNS = std::stoi(line.substr(line.find(' ') + 1, line.length() - 1));

                gold.resize(NBR_ROWS);
                for (int i = 0; i < NBR_ROWS; i++) {
                    gold[i].resize(NBR_COLUMNS);
                }
                value.resize(NBR_ROWS);
                for (int i = 0; i < NBR_ROWS; i++) {
                    value[i].resize(NBR_COLUMNS);
                }
                profit.resize(NBR_ROWS);
                for (int i = 0; i < NBR_ROWS; i++) {
                    profit[i].resize(NBR_COLUMNS);
                }

                isFirstLine = false;
                isGoldMatrix = true;
            }

            if(isGoldMatrix){
                for (int i = 0; i < NBR_ROWS; i++) {
                    for (int y = 0; y < NBR_COLUMNS; y++) {
                        inputFileStream >> number;
                        gold[i][y] = number;
                    }
                }
                isGoldMatrix = false;
                isCostMatrix = true;
            }
        
            if(isCostMatrix){
                for (int i = 0; i < NBR_ROWS; i++) {
                    for (int y = 0; y < NBR_COLUMNS; y++) {
                        inputFileStream >> number;
                        value[i][y] = number;
                    }
                }
                isCostMatrix = false;
            }
        }
        inputFileStream.close();
    }
    else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";
    }

    for (int i = 0; i < NBR_ROWS; i++) {
        for (int y = 0; y < NBR_COLUMNS; y++) { 
            profit[i][y] = std::make_pair(gold[i][y] - value[i][y],0);
        }
    }

    return profit;
}

void printPath(vector<Position> path){
    for(int i = 0; i < path.size(); i++) {
        std::cout << path[i].row << " " << path[i].column << endl;
    }
    std::cout << endl;
}

// Main function
int main(int argc, char* argv[])
{
    if (argc < 1) {
        showUsage(argv[0]);
        return 1;
    }

    std::string filePath = argv[2];

    vector<vector<pair<int,int>>> profit = readExempFile(filePath);

    int64_t currentMaxProfit = 0;
    int64_t currentProfit = 0;
    int maxElement = INT_MIN;

    Position newPosition = {0, 0};
    vector<Position> currentMaxPath;
    vector<Position> currentPath;

    for (int y = 0; y < NBR_COLUMNS; y++) {
        if (profit[0][y].first > maxElement) {
            maxElement = profit[0][y].first;
            newPosition = {0, y};
        }
    }
    profit[newPosition.row][newPosition.column].second = 1;
    currentPath.push_back(newPosition);

    pair<int64_t, Position> result;
    bool printNewMax = true;

    while (currentProfit != INT64_MIN) {
        result = getBlock(profit, currentPath, newPosition);
        currentProfit = result.first;
        newPosition = result.second;

        if(currentProfit > currentMaxProfit) {
            currentMaxProfit = currentProfit;
            
            for (int i = currentMaxPath.size(); i < currentPath.size(); i++) {
                currentMaxPath.push_back(currentPath[i]);
            }

            if(printNewMax){
                printPath(currentMaxPath);
                printNewMax = false;
            }
        } 
        else {
            printNewMax = true;
        }
    }
    
    printPath(currentMaxPath);

    return 0;
}
