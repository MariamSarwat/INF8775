#include <iostream> 
#include <fstream>
#include <chrono>
#include <math.h>
#include <string>
#include <sstream>
#include<vector>
#include <stdc++.h>
#include <limits>



using namespace std;

struct Position {
    int row;
    int column;

};


//Regarde les voisins autour et trouve le max et retourne la position
Position findMaxNeighbour(vector<vector<pair<int, int>>> profit, int row, int column) {
    int maxElement = 0;
    Position position = { 0,0 };
    int i_min=0, i_max=row, j_min=0, j_max = column;

    if (row != 0) {
        i_min = row - 1;
    }
    if (row != profit.size()) {
        i_max = row + 1;
    }
    if (column != 0) {
        j_min = column - 1;
    }
    if (column != profit.size()) {
        j_max = column + 1;
    }

    for (int i=i_min; i<=i_max; i++) {
        for (int j=j_min; j<=j_max; j++) {
           if (profit[i][j].second == 0) {
                if (profit[i][j].first > maxElement) {

                    maxElement = profit[i][j].first;
                    position.row = i;
                    position.column = j;
                    

                }
            }
        }
    }
    return position;
}

vector<vector<pair<int, int>>> verifyCondition(vector<vector<pair<int, int>>> profit, vector <Position> list_pos) {
    Position pos = { 0,0 };
    int row_min = 0, row_max = 0, column_min = 0, column_max = 0;
    while (list_pos.empty() == false) {
        //cout << gelist_pos.erase(list_pos.begin()). << endl;
        auto it = list_pos.at(0);
        list_pos.erase(list_pos.begin());
        pos.row = it.row;
        pos.column = it.column;

        if (pos.row != 0) {
            row_min = pos.row - 1;
        }
        else { row_min = pos.row; }
        if (pos.column != 0) {
            column_min = pos.column - 1;
        }
        else { column_min = pos.column; }
        if (pos.row != profit.size()) {
            row_max = pos.row + 1;
        }
        else{row_max = pos.row; }
        if (pos.column != 0) {
            column_max = pos.column + 1;
        }
        else { column_max = pos.column; }

       

        if (profit[row_min][column_min].second == 0) {
            
            profit[row_min][column_min].second = 1;
        }
        if (profit[row_min][pos.column].second == 0) {
            profit[row_min][pos.column].second = 1;
        }
        if(profit[row_min][column_max].second == 0) {
            profit[row_min][column_max].second = 1;
        }
    }
    return profit;
}

void algorythm(vector<vector<pair<int, int>>> profit) {
    int maxElement = 0;
    int firstRow = 0;
    Position position = { 0,0 };
    Position new_position = { 0,0 };
    std::vector <Position> list_pos;

    

    for (int j = 0; j < profit.size(); j++) {

        if (profit[firstRow][j].first > maxElement) {

             maxElement = profit[firstRow][j].first;
             
             position.row = firstRow;
             position.column = j;
             
        }
    }
    profit[position.row][position.column].second = 1;
    new_position = findMaxNeighbour(profit, position.row, position.column);
    
    
    profit[new_position.row][new_position.column].second = 1;
    list_pos.push_back(new_position);
    verifyCondition(profit, list_pos);
    new_position = findMaxNeighbour(profit, new_position.row, new_position.column);
    profit[new_position.row][new_position.column].second = 1;
    list_pos.push_back(new_position);
    verifyCondition(profit, list_pos);



    

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
    int N = 0;
    int M = 0;
    vector<vector<int>> gold;
    vector<vector<int>> value;
    vector<vector<pair<int, int>>> profit;
    int number;



    if (inputFileStream) {
        while (std::getline(inputFileStream, line)) {
            std::vector<int>   data;


            if (isFirstLine) {
                N = std::stoi(line.substr(0, line.find(' ')));
                M = std::stoi(line.substr(line.find(' ') + 1, line.length() - 1));
                
                isFirstLine = false;
            }
            gold.resize(M);
            for (int i = 0; i < M; i++) {
                gold[i].resize(N);
            }
            value.resize(M);
            for (int i = 0; i < M; i++) {
                value[i].resize(N);
            }
            profit.resize(M);
            for (int i = 0; i < M; i++) {
                profit[i].resize(N);
            }


            for (int i = 0; i < M; i++) {
                for (int y = 0; y < N; y++) {
                    inputFileStream >> number;
                    gold[i][y] = number;
                }
            }

            for (int i = 0; i < M; i++) {
                for (int y = 0; y < N; y++) {
                    inputFileStream >> number;
                    value[i][y] = number;
                }
            }
            
        }
        inputFileStream.close();

        
    }
    else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";

    }
    for (int i = 0; i < M; i++) {
        for (int y = 0; y < N; y++) {
            
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

    //bool printTime = (string(argv[3]) != "0");
    //bool printResult = (string(argv[4]) != "0");
    vector<vector<pair<int,int>>> profit=readExempFile(filePath);
    
    algorythm(profit);
      

    return 0;
}
