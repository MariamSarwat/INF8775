#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <math.h>

using namespace std;

string GLOUTON_CODE = "0";
string PROGDYN_CODE = "1";
string APPROX_CODE = "2";

// A structure for x and y coordinates 
struct Coord {
    int x;
    int y;
};

int findEuclDist (Coord initCoord, Coord finalCoord){
    int x = pow(finalCoord.x - initCoord.x, 2);
    int y = pow(finalCoord.y - initCoord.y, 2);
    return sqrt(x + y);
}

#include <bits/stdc++.h>
// Function to find the minimum cost path for all the paths
void greedyAlgo(std::vector<Coord> cityArr)
{
	int totMinDist = 0;
	int counter = 0;
	int j = 0, i = 0;
	int minDist = INT_MAX;
	map<int, int> visitedRouteList;

	// Starting from the 0th indexed city i.e., the first city
	visitedRouteList[0] = 1;
	int route[cityArr.size()];

	// Traverse the adjacency vector cityArr[]
	while (i < cityArr.size())
	{
        // Visited all cities
        if (counter >= cityArr.size() - 1)
        {
            break;
        }

        if(j != i && visitedRouteList[j] == 0){
            int euclDist = findEuclDist(cityArr[i], cityArr[j]);
            if (euclDist < minDist)
			{
				minDist = euclDist;
				route[counter] = j;
			}
        } 
        j++;

		// Check all paths from the ith indexed city
		if (j == cityArr.size())
		{
			totMinDist += minDist;
			minDist = INT_MAX;
			visitedRouteList[route[counter]] = 1;
			j = 0;
			i = route[counter];
			counter++;
		}
	}

	// Update the ending city in array from city which was last visited
	i = route[counter - 1];

    int euclDist = findEuclDist(cityArr[0], cityArr[i]);
    totMinDist += euclDist;

	// Started from the node where we finished as well.
	std::cout << ("Minimum Cost is : ");
	std::cout << (totMinDist) << std::endl;
}
// This code is contributed by grand_master.

// Print help menu
static void showUsage(std::string name) 
{
    std::cerr << "Usage: " << name << " -a -a {glouton, progdyn, approx} -e CHEMIN_EXEMPLAIRE [-p] [-t]"
        << "Parametre optionel :\n"
        << "\t[-p] affiche dans l’ordre, sur chaque ligne, les indices des villes à visiter en commençant par 0 et en finissant par 0, sans texte superflu. Rapportez le chemin tel que la deuxième ville visitée ait un indice inférieur à celui de l’avant dernière ville affichée.\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
}

// Read File with City coordinates
std::pair<int, std::vector<Coord>> readExempFile(std::string filePath) 
{
    std::ifstream inputFileStream(filePath);
    std::vector<Coord> cityArr;
    std::string line;

    bool isFirstLine = true;
    int nbrCities = 0;

    if (inputFileStream) {
        while (std::getline(inputFileStream, line)) {
            if (isFirstLine) {
                nbrCities = std::stoi(line);
                isFirstLine = false;
            }

            int x, y;
            inputFileStream >> x >> y;
            Coord coordinates = { x, y };
            cityArr.push_back(coordinates);
        }
        inputFileStream.close();
    } else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";
    }
    return std::make_pair(nbrCities, cityArr);
}

// Main function
int main(int argc, char* argv[]) 
{
    if (argc < 5) {
        showUsage(argv[0]);
        return 1;
    }

    std::string filePath = argv[1];
    std::string method = argv[2];
    bool printTime = (string(argv[3]) != "0");
    bool printResult = (string(argv[4]) != "0");

    std::pair<int, std::vector<Coord>> fileData = readExempFile(filePath);
    int nbrCities = fileData.first;
    std::vector<Coord> cityArr = fileData.second;

    double executionTime;

    if (method == "glouton" || method == GLOUTON_CODE) {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
	    greedyAlgo(cityArr);
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();  
    } 
    else if (method == "progdyn" || method == PROGDYN_CODE) {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();    
    } 
    else if (method == "approx" || method == APPROX_CODE) {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();
    } 

    if (printResult) {
        //ptr->print();
    }
    if (printTime) {
        std::cout << executionTime << "\n";
    }
    return 0;
}
