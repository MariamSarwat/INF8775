
#include <fstream>
#include <chrono>
#include <math.h>
#include <string>
#include <sstream>
#include<vector>



using namespace std;



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
vector<vector<int>> readExempFile(std::string filePath)
{
    std::ifstream inputFileStream(filePath);
    std::string line;

    bool isFirstLine = true;
    int N = 0;
    int M = 0;
    vector<vector<int>> gold;
    vector<vector<int>> value;
    vector<vector<int>> profit;
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
            cout << gold[i][y] << '-' << value[i][y] << endl;
            profit[i][y] = gold[i][y] - value[i][y];
            cout << profit[i][y] << endl;
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
    vector<vector<int>> profit=readExempFile(filePath);


    return 0;
}

