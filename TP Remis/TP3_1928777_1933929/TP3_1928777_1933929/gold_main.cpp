#include <iostream> 
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <climits>
#include <chrono>

using namespace std;

struct Position {
    int row;
    int column;
};

int NBR_ROWS = 0;
int NBR_COLUMNS = 0;
const int MIN_PRINT_INTERVAL = 7;

//Regarde les voisins autour et trouve le max et retourne sa position. 
//Si un voisin maximale et positif n'est pas retrouvé, une nouvelle voisinnage sera visité (case maximale qui n'a pas encore été déterré).
//Si encore une fois, aucun case est retourné, il reste seulement des cases de profit négative (on retourne une condition d'arrêt).
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

//Selon la case choisis avec findMaxNeighbour, les conditions de déterrage seront vérifier. Si les cases nécessaire à déterrer newPosition 
//ne sont pas encore visité, ces dernier seront déterrés.
//Finalement, le path suivis sera ajouté au currentPath afin de pouvoir l'afficher.
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

//Retourne le profit correspondant au path passé en paramètre.
int64_t getCurrentProfit(vector<vector<pair<int, int>>> profit, vector<Position> path) {
    int64_t currentProfit = 0;
    for (int i = 0; i < path.size(); i++) {
        currentProfit += profit[path[i].row][path[i].column].first;
    }
    return currentProfit;
}

//Retrouve le newPosition à déterrer et, si une position valide est retournée, les conditions sont vérifier. 
//Finalement, le profit correspondant au nouveau path est calculé et sera retourné à la fin.
pair<int64_t, Position> getBlock(vector<vector<pair<int, int>>>& profit, vector<Position>& currentPath, Position newPosition) {
    int64_t currentProfit = INT64_MIN;

    newPosition = findMaxNeighbour(profit, newPosition);

    if (newPosition.row != INT_MIN && newPosition.column != INT_MIN) {
        profit[newPosition.row][newPosition.column].second = 1;
        
        verifyCondition(profit, currentPath, newPosition);
        currentProfit = getCurrentProfit(profit, currentPath);
    }

    return make_pair(currentProfit, newPosition);
}

//Permet d'aller chercher la case de début, soit la case ayant le plus grand profit de la première ligne de la matrice.
//Ce point sera par la suite retourné par la méthode.
Position getStartingBlock(vector<vector<pair<int, int>>>& profit, vector<Position>& currentPath){
    int maxElement = INT_MIN;
    Position newPosition = {0, 0};

    for (int y = 0; y < NBR_COLUMNS; y++) {
        if (profit[0][y].first > maxElement) {
            maxElement = profit[0][y].first;
            newPosition = {0, y};
        }
    }
    profit[newPosition.row][newPosition.column].second = 1;
    currentPath.push_back(newPosition);

    return newPosition;
}

//Affichage du menu aide
static void showUsage(std::string execName)
{
    std::cerr << "Usage: " << execName << " -e CHEMIN_EXEMPLAIRE" << std::endl;
}

//Lire fichier de l'exemplaire et initier les variables.
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

//Affiche le chemin passé en paramètre selon l'énoncé.
void printPath(vector<Position> path){
    string stringifiedPath = "";
    for(int i = 0; i < path.size(); i++) {
        string newPos = to_string(path[i].row) + ' ' + to_string(path[i].column) + '\n';
        stringifiedPath = stringifiedPath + newPos;
    }
    std::cout << stringifiedPath << endl;
}

//set le currentMaxPath.
void setMaxPath(vector<Position> currentPath, vector<Position>& currentMaxPath){
    for (int i = currentMaxPath.size(); i < currentPath.size(); i++) {
        currentMaxPath.push_back(currentPath[i]);
    }
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
    int64_t previousPrintedProfit = 0;

    vector<Position> currentMaxPath;
    vector<Position> currentPath;

    pair<int64_t, Position> result;

    //Retourne la case de début.
    Position currentPosition = getStartingBlock(profit, currentPath);

    //Initialise notre chronomètre de temps.
    long previous_print_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    //Tant qu'il reste des cases positive à déterrer. 
    //INT64_MAX est retourné par getBlock lorsqu'il ne reste plus case avec un profit positif à déterrer.
    while (currentProfit != INT64_MIN) {
        result = getBlock(profit, currentPath, currentPosition);
        currentProfit = result.first;
        currentPosition = result.second;

        if(currentProfit > currentMaxProfit) {
            currentMaxProfit = currentProfit;
            setMaxPath(currentPath, currentMaxPath);
        } 

        long current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        long time_since_last_print = current_time - previous_print_time;

        if (currentMaxProfit != previousPrintedProfit && time_since_last_print >= MIN_PRINT_INTERVAL){
            printPath(currentMaxPath);
            previous_print_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
            previousPrintedProfit = currentMaxProfit;
        }
    }
    
    //On affiche la solution maximale retrouvé avec l'algorithme s'il n'a pas été déjà affichée.
    if(currentMaxProfit != previousPrintedProfit)
       printPath(currentMaxPath);

    return 0;
}
