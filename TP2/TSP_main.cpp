#include <iostream> 
#include <fstream>
//#include <vector>
//#include <string>
#include <chrono>
#include <math.h>
//#include <map>
#include <bits/stdc++.h>

using namespace std;

std::string GLOUTON_CODE = "0";
std::string PROGDYN_CODE = "1";
std::string APPROX_CODE = "2";

// A structure for x and y coordinates 
struct Coord {
    int x;
    int y;
};

bool operator == (Coord point1, Coord point2) {
    return (point1.x == point2.x) && (point1.y == point2.y);
}

int findEuclDist (Coord initCoord, Coord finalCoord){
    int x = pow(finalCoord.x - initCoord.x, 2);
    int y = pow(finalCoord.y - initCoord.y, 2);
    return (x + y);
}

// Inspire de https://www.geeksforgeeks.org/travelling-salesman-problem-greedy-approach/?ref=rp
// Function to find the minimum cost path using the greedy method
int* greedyAlgo(std::vector<Coord> cityArr, int * route)
{
	int totMinDist = 0;
	int counter = 0;
	int j = 0, i = 0;
	int minDist = INT_MAX;
	map<int, int> visitedRouteList;

	// Starting from the 0th indexed city i.e., the first city
	visitedRouteList[0] = 1;
    route[counter] = 0;
    counter++;

	// Traverse vector cityArr
	while (i < cityArr.size() && j < cityArr.size())
	{
        // Visited all cities, close path (last node to first node)
        // Update the ending city in array from city which was last visited
        if (counter == cityArr.size()) 
        {   
            i = route[counter - 1];
            int euclDist = findEuclDist(cityArr[0], cityArr[i]);
            totMinDist += euclDist;
            //route[counter] = 0;

            break;
        }

        if(j != i && visitedRouteList[j] == 0){
            int euclDist = findEuclDist(cityArr[i], cityArr[j]);
            if (euclDist < minDist)
			{   
				minDist = euclDist;
				route[counter] = j ;
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

    std::cout << "Minimum Cost is : " << (totMinDist) << std::endl;
    return route;
}

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

//vector<vector<int>> state;
//https://gist.github.com/jgcoded/d7ecba7aa3e210419471
int DPAlgo(int pos, int visited, const vector<vector<int>>& citiesMatrix, vector<vector<int>>& state)
{
    if(visited == ((1 << citiesMatrix.size())) - 1)
        return citiesMatrix[pos][0]; // return to starting city

    if(state[pos][visited] != INT_MAX)
        return state[pos][visited];
    
    for(int i = 0; i < citiesMatrix.size(); i++)
    {
        // can't visit ourselves unless we're ending & skip if already visited
        if(i == pos || (visited & (1 << i))){
            continue;
        }
        int distance = citiesMatrix[pos][i] + DPAlgo(i, visited | (1 << i), citiesMatrix, state);

        if(distance < state[pos][visited]) {
            state[pos][visited] = distance;
        }
    }
    return state[pos][visited];
}

//https://www.geeksforgeeks.org/travelling-salesman-problem-set-2-approximate-using-mst/?fbclid=IwAR1mcT7xs8ARV-fgmI7uAbIRTN1buN6i2ANK2HKPoEn3f6zvjG9Tkc-otgA
//http://www.mathcs.emory.edu/~cheung/Courses/171/Syllabus/11-Graph/prim2.html?fbclid=IwAR03NsIwqkDZUYQam7pWxX7dccTySCZobjWRxNshCH94tQrR2RmRXqRM5vs
void approx(std::vector<Coord> cityArr) {
    int totMinDist = 0;
    int counter = 0;
    
    int minDist = INT_MAX;
    Coord actualPoint = {};
    Coord choosenPoint = {};
    std::vector<Coord> visitedRouteList;
    
    actualPoint.x = cityArr[0].x;
    actualPoint.y = cityArr[0].y;
    
    visitedRouteList.push_back(cityArr[0]);
    cityArr.erase(cityArr.begin());   

    while (cityArr.size()>0)
    {
        for (int i=0; i < cityArr.size(); i++) {
            int distEucl = findEuclDist(actualPoint, cityArr[i]);
            if (distEucl < minDist) {
                minDist = distEucl;
                choosenPoint.x = cityArr[i].x;
                choosenPoint.y = cityArr[i].y;
            }
        }
        totMinDist += minDist;
        visitedRouteList.push_back(choosenPoint);
        actualPoint.x = choosenPoint.x;
        actualPoint.y = choosenPoint.y;
       
        cityArr.erase(find(cityArr.begin(),cityArr.end(),choosenPoint));
        minDist = INT_MAX;
        counter++;
    }  
    std::cout << "Min cost " << totMinDist << endl;
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
            //std::cout << line << endl;

            if (isFirstLine) {
                nbrCities = std::stoi(line);
                isFirstLine = false;
            }      
            int x = -1 , y = -1;
            inputFileStream >> x >> y;
            Coord coordinates = { x, y };
            //std::cout << x << " " << y << endl;
            if(x == -1 || y == -1) break;
            cityArr.push_back(coordinates);
        }
        inputFileStream.close();
    } else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";
    }
    return std::make_pair(nbrCities, cityArr);
}

// //https://stackoverflow.com/questions/61869112/travelling-salesman-problem-in-dynamic-programming
void path(int mask, int pos, const vector<vector<int>>& citiesMatrix, const vector<vector<int>>& state){
    if(mask == ((1 << citiesMatrix.size()) - 1)) {
        return;
    }
    
    int ans = INT_MAX, chosenCity = pos;

    for(int city = 0; city < citiesMatrix.size(); ++city)
    {
        if(city == pos || (mask & (1 << city))>0){
            continue;
        }
        int newAns = citiesMatrix[pos][city] + state[city][mask];
        if(newAns < ans){
            ans = newAns;
            chosenCity = city;
        }
    }
    printf("%d ", chosenCity); // here you get the current city you need to visit
    path(mask | (1 << chosenCity), chosenCity, citiesMatrix, state);
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
        int shortestPath[cityArr.size()];
        auto start = std::chrono::high_resolution_clock::now();
	    int *result = greedyAlgo(cityArr, shortestPath);
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count();  
        for(int k = 0; k < nbrCities; k++){
            std::cout << result[k];
            if(k != nbrCities - 1) std::cout << "->";
            else std::cout << endl;
        }
    } 
    else if (method == "progdyn" || method == PROGDYN_CODE) {
        vector<vector<int>> citiesMatrix = findDistMatrix(cityArr);
        vector<vector<int>> state(citiesMatrix.size());
        for(auto& neighbors : state)
            neighbors = vector<int>((1 << citiesMatrix.size()) - 1, INT_MAX);
        
        auto start = std::chrono::high_resolution_clock::now();
        cout << "minimum: " << DPAlgo(0, 1, citiesMatrix, state) << endl;
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(finish - start).count(); 
        path(1, 0, citiesMatrix, state);
        std::cout << endl;
    } 
    else if (method == "approx" || method == APPROX_CODE) {
        auto start = std::chrono::high_resolution_clock::now();
        //call algo
        approx(cityArr);
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
