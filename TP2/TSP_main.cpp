#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// Print help menu
static void showUsage(std::string name) 
{
    std::cerr << "Usage: " << name << " -a -a {glouton, progdyn, approx} -e CHEMIN_EXEMPLAIRE [-p] [-t]"
        << "Parametre optionel :\n"
        << "\t[-p] affiche dans l’ordre, sur chaque ligne, les indices des villes à visiter en commençant par 0 et en finissant par 0, sans texte superflu. Rapportez le chemin tel que la deuxième ville visitée ait un indice inférieur à celui de l’avant dernière ville affichée.\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
}
/*
// Read File with building coordinates
std::pair<int, std::vector<Building>> readExempFile(std::string filePath) 
{
    std::ifstream inputFileStream(filePath);
    std::vector<Building> buildingArray;
    std::string line;
    bool isFirstLine = true;
    int nbrBuildings = 0;
    if (inputFileStream) {
        while (std::getline(inputFileStream, line)) {
            if (isFirstLine) {
                nbrBuildings = std::stoi(line);
                isFirstLine = false;
            }

            int right, left, height;
            inputFileStream >> right >> left >> height;
            Building building = { right, left, height };
            buildingArray.push_back(building);
        }
        inputFileStream.close();
    } else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";
    }
    return std::make_pair(nbrBuildings, buildingArray);
}*/

// Main function
int main(int argc, char* argv[]) 
{
    if (argc < 5) {
        showUsage(argv[0]);
        return 1;
    }

    std::string method;
    std::string filePath;
    bool printResult = false, printTime = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-a") {
            method = argv[++i];
        } else if (arg == "-e") {
            filePath = argv[++i];
        } else if (arg == "-p") {
            printResult = true;
        } else if (arg == "-t") {
            printTime = true;
        }
    }
/*
    std::pair<int, std::vector<Building>> fileData = readExempFile(filePath);
    int n = fileData.first;
    Building* arr = new Building[fileData.second.size()];
    std::copy(fileData.second.begin(), fileData.second.end(), arr);

    SkyLine* ptr;
*/
    double executionTime;
    if (method == "glouton") {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();  
    } 
    else if (method == "progdyn") {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();    
    } 
    else if (method == "approx") {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();
    } 
    else {
        std::cerr << "Algorithme de type invalide \n";
        return 1;
    }

    if (printResult) {
        //ptr->print();
    }
    if (printTime) {
        std::cout << executionTime << "\n";
    }
    return 0;
}