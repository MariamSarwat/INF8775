#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <math.h>
#include <map>

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

// Inspire de https://www.geeksforgeeks.org/travelling-salesman-problem-greedy-approach/?ref=rp
// Function to find the minimum cost path using the greedy method
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
    route[counter] = 0;
    counter++;

	// Traverse vector cityArr
	while (i < cityArr.size() && j < cityArr.size())
	{
        // Visited all cities, close path (last node to first node)
        // Update the ending city in array from city which was last visited
        if (counter == cityArr.size() - 1) 
        {
            i = route[counter - 1];
            int euclDist = findEuclDist(cityArr[0], cityArr[i]);
            totMinDist += euclDist;
            route[counter] = 0;

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

    for(int k = 0; k < cityArr.size(); k++){
        std::cout << route[k];
        if(k != cityArr.size() - 1) std::cout << "->";
        else std::cout << endl;
    }

	// Started from the node where we finished as well.
	std::cout << "Minimum Cost is : " << (totMinDist) << std::endl;
}


//https://gist.github.com/jgcoded/d7ecba7aa3e210419471
std::vector<std::vector<int>> findDistMatrix(std::vector<Coord> cities){
    std::vector<std::vector<int>> citiesMatrix(cities.size());
    for(auto& neighbors : citiesMatrix)
        neighbors = vector<int>((1 << cities.size()) - 1, INT_MAX);

    for(int i = 0; i < cities.size(); i++){
        for (int j = 0; j < cities.size(); j++){
            if(i == j) {
                citiesMatrix[i][j] = 0;
            }
            else {
                int euclDist = findEuclDist(cities[i], cities[j]);
                citiesMatrix[i][j] = euclDist;
            }
        }
    }
    return citiesMatrix;
}

int DPAlgo(const vector<vector<int>>& cities, int pos, int visited, vector<vector<int>>& state)
{
    if(visited == ((1 << cities.size()) - 1))
        return cities[pos][0]; // return to starting city

    if(state[pos][visited] != INT_MAX)
        return state[pos][visited];

    for(int i = 0; i < cities.size(); ++i)
    {
        // can't visit ourselves unless we're ending & skip if already visited
        if(i == pos || (visited & (1 << i)))
            continue;

        int distance = cities[pos][i] + DPAlgo(cities, i, visited | (1 << i), state);
        if(distance < state[pos][visited])
            state[pos][visited] = distance;
    }

    return state[pos][visited];
}

// Print help menu
static void showUsage(std::string name) 
{
    std::cerr << "Usage: " << name << " -a {glouton, progdyn, approx} -e CHEMIN_EXEMPLAIRE [-p] [-t]"
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
        vector<vector<int>> citiesMatrix = findDistMatrix(cityArr);
        /*for(int i = 0; i < nbrCities - 1; i++){
            std::cout << "{";
            for (int j = 0; j < nbrCities - 1; j++){
                std::cout << citiesMatrix[i][j] << ", "; 
            }
            std::cout << "}, " << endl;
        }*/
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        vector<vector<int>> state(citiesMatrix.size());
        for(auto& neighbors : state)
            neighbors = vector<int>((1 << citiesMatrix.size()) - 1, INT_MAX);

        cout << "minimum: " << DPAlgo(citiesMatrix, 0, 1, state) << endl;
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
