#include <iostream> 
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <bits\stdc++.h>

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

void verifyCondition(vector<vector<pair<int, int>>>& profit, vector<Position> list_pos) {

    while (list_pos.size() > 0) {
        Position pos = list_pos.at(0);
        Position newPosition;

        list_pos.erase(list_pos.begin());

        int row_min = 0;
        if (pos.row != 0) {
            row_min = pos.row - 1;
        }
        else break;

        int column_min = (pos.column != 0)? pos.column - 1 : pos.column;
        int column_max = (pos.column != NBR_COLUMNS - 1)? pos.column + 1 : pos.column;

        if (profit[row_min][column_min].second == 0) { 
            profit[row_min][column_min].second = 1;
            newPosition = {row_min, column_min};
            list_pos.push_back(newPosition);
        }

        if (profit[row_min][pos.column].second == 0) {
            profit[row_min][pos.column].second = 1;
            newPosition = {row_min, pos.column};
            list_pos.push_back(newPosition);
        }

        if(profit[row_min][column_max].second == 0) {
            profit[row_min][column_max].second = 1;
            newPosition = {row_min, column_max};
            list_pos.push_back(newPosition);
        }
    }
}

pair<int, Position> algorithm(vector<vector<pair<int, int>>>& profit, Position new_position) {
    int profitFound = 0;
    std::vector <Position> list_pos;

    new_position = findMaxNeighbour(profit, new_position);
    if (new_position.row != INT_MIN && new_position.column != INT_MIN) {
        profit[new_position.row][new_position.column].second = 1;
        list_pos.push_back(new_position);
        verifyCondition(profit, list_pos);
        list_pos.erase(list_pos.begin(), list_pos.end());
    } else {
        profitFound = INT_MIN;
    }

    if(profitFound == 0) {
        for(int x = 0; x < NBR_ROWS; x++){
            for (int y = 0; y < NBR_COLUMNS; y++)
            {
                if(profit[x][y].second == 1){
                    profitFound += profit[x][y].first;
                }
            }
        }
    }

    return make_pair(profitFound, new_position);
}

//Print help menu
static void showUsage(std::string name)
{
    std::cerr << "Usage: " << name << " -e CHEMIN_EXEMPLAIRE [-p] [-t]"
        << "Parametre optionel :\n"
        << "\t[-p] affiche dans l’ordre, sur chaque ligne, les indices des villes à visiter en commençant par 0 et en finissant par 0, sans texte superflu. Rapportez le chemin tel que la deuxième ville visitée ait un indice inférieur à celui de l’avant dernière ville affichée.\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
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

// Main function
int main(int argc, char* argv[])
{
    if (argc < 1) {
        showUsage(argv[0]);
        return 1;
    }

    std::string filePath = argv[2];

    vector<vector<pair<int,int>>> profit = readExempFile(filePath);

    int MaxProfit = 0;
    int profitFound = 0;
    int iteration = 1;

    int maxElement = 0;
    int firstRow = 0;
    Position new_position = { 0,0 };

    for (int j = 0; j < profit[profit.size() - 1].size(); j++) {
        if (profit[firstRow][j].first > maxElement) {
            maxElement = profit[firstRow][j].first;
             
            new_position.row = firstRow;
            new_position.column = j; 
        }
    }
    profit[new_position.row][new_position.column].second = 1;
   
    pair<int, Position> result;

    while (profitFound != INT_MIN) {
        result = algorithm(profit, new_position);
        profitFound = result.first;
        new_position = result.second;

        if(profitFound > MaxProfit) {
            MaxProfit = profitFound;
            //print
            std::cout << MaxProfit << endl;
        }

        if(profitFound == INT_MIN) std::cout << "done"<<endl;
    }
    

    return 0;
}
