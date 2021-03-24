#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// A structure for x and y coordinates 
struct Coord {
    int x;
    int y;
};

#include <bits/stdc++.h>
// Function to find the minimum cost path for all the paths
void findMinRoute(vector<vector<int> > tsp)
{
	int sum = 0;
	int counter = 0;
	int j = 0, i = 0;
	int min = INT_MAX;
	map<int, int> visitedRouteList;

	// Starting from the 0th indexed city i.e., the first city
	visitedRouteList[0] = 1;
	int route[tsp.size()];

	// Traverse the adjacency matrix tsp[][]
	while (i < tsp.size() && j < tsp[i].size())
	{
		// Corner of the Matrix
		if (counter >= tsp[i].size() - 1) break;

		// If this path is unvisited then and if the cost is less then update the cost
		if (j != i && (visitedRouteList[j] == 0))
		{
			if (tsp[i][j] < min)
			{
				min = tsp[i][j];
				route[counter] = j + 1;
			}
		}
		j++;

		// Check all paths from the ith indexed city
		if (j == tsp[i].size())
		{
			sum += min;
			min = INT_MAX;
			visitedRouteList[route[counter] - 1] = 1;
			j = 0;
			i = route[counter] - 1;
			counter++;
		}
	}

	// Update the ending city in array from city which was last visited
	i = route[counter - 1] - 1;

	for (j = 0; j < tsp.size(); j++)
	{
		if ((i != j) && tsp[i][j] < min)
		{
			min = tsp[i][j];
			route[counter] = j + 1;
		}
	}
	sum += min;

	// Started from the node where we finished as well.
	std::cout << ("Minimum Cost is : ");
	std::cout << (sum) << std::endl;
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

    std::pair<int, std::vector<Coord>> fileData = readExempFile(filePath);
    int nbrCities = fileData.first;
    std::vector<Coord> cityArr = fileData.second;

    double executionTime;

    if (method == "glouton") {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        // Input Matrix
        vector<vector<int> > tsp = { { -1, 10, 15, 20 },
                                    { 10, -1, 35, 25 },
                                    { 15, 35, -1, 30 },
                                    { 20, 25, 30, -1 } };

        // Function Call
	    findMinRoute(tsp);
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