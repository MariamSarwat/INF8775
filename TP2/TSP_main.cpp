#include <iostream> 
#include <fstream>
#include <chrono>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

std::string GLOUTON_CODE = "0";
std::string PROGDYN_CODE = "1";
std::string APPROX_CODE = "2";

// A structure for x and y coordinates 
struct Coord 
{
    int x;
    int y;
};

//Returns rounded euclidien distance
uint64_t findEuclDist(Coord initCoord, Coord finalCoord) 
{
    uint64_t x = pow(finalCoord.x - initCoord.x, 2);
    uint64_t y = pow(finalCoord.y - initCoord.y, 2);
    return round(sqrt(x + y));
}

// Beginning of Greedy Algorithm
// Inspire de https://www.geeksforgeeks.org/travelling-salesman-problem-greedy-approach/?ref=rp
// Function to find the minimum cost path using the greedy method
uint64_t greedyAlgo(std::vector<Coord> cityArr, vector<int>& shortestPath)
{
    uint64_t totMinDist = 0;
    int counter = 0;
    int j = 0, i = 0;
    uint64_t minDist = UINT64_MAX;
    map<int, int> visitedRouteList;

    //Starting from the first city
    visitedRouteList[0] = 1;
    shortestPath[counter] = 0;
    counter++;

    //Traverse vector cityArr and choose city which the smallest distance (becomes our new starting city) 
    while (i < cityArr.size() && j < cityArr.size()) 
    {
        // Visited all cities, close path (last node to first node)
        if (counter == cityArr.size()) 
        {
            i = shortestPath[counter - 1];
            uint64_t euclDist = findEuclDist(cityArr[i], cityArr[0]);
            totMinDist += euclDist;
            break;
        }

        if (j != i && visitedRouteList[j] == 0) 
        {
            uint64_t euclDist = findEuclDist(cityArr[i], cityArr[j]);

            if (euclDist < minDist) 
            {
                minDist = euclDist;
                shortestPath[counter] = j;
            }
        }

        j++;

        // Checked all paths from the ith indexed city
        if (j == cityArr.size()) 
        {
            totMinDist += minDist;
            minDist = UINT64_MAX;
            visitedRouteList[shortestPath[counter]] = 1;
            j = 0;
            i = shortestPath[counter];
            counter++;
        }
    }

    return totMinDist;
}
//End Greedy Algorithm


//Beginning of Dynamic Programming Algorithm
//Store our distances in a a 2D Matrix
std::vector<std::vector<int>> findDistMatrix(std::vector<Coord> cities) 
{
    std::vector<std::vector<int>> citiesMatrix(cities.size());
    for (auto& neighbors : citiesMatrix)
        neighbors = vector<int>((1 << cities.size()) - 1, 0);

    for (int i = 0; i < cities.size(); i++) 
    {
        for (int j = 0; j < cities.size(); j++) 
        {
            int distance = (i != j)? findEuclDist(cities[i], cities[j]): 0;
            citiesMatrix[i][j] = distance;
        }
    }

    return citiesMatrix;
}

//https://gist.github.com/jgcoded/d7ecba7aa3e210419471
int DPAlgo(int pos, int visited, const vector<vector<int>>& citiesMatrix, vector<vector<int>>& state)
{
    if (visited == ((1 << citiesMatrix.size())) - 1)
        return citiesMatrix[pos][0]; // return to starting city

    if (state[pos][visited] != INT_MAX)
        return state[pos][visited];

    for (int i = 0; i < citiesMatrix.size(); i++)
    {
        //Can't visit ourselves unless we're at the end and skip those already visited
        if (i == pos || (visited & (1 << i))) {
            continue;
        }

        int distance = citiesMatrix[pos][i] + DPAlgo(i, visited | (1 << i), citiesMatrix, state);

        //Choose minimum between new distance calculated, and the one already stored in state
        if (distance < state[pos][visited]) {
            state[pos][visited] = distance;
        }
    }

    //Return minimum distance found
    return state[pos][visited];
}

//https://stackoverflow.com/questions/61869112/travelling-salesman-problem-in-dynamic-programming
void getDPPath(int pos, int mask, const vector<vector<int>>& citiesMatrix, const vector<vector<int>>& state, vector<int>& shortestPath) {
    int ans = INT_MAX, chosenCity;

    if (mask == ((1 << citiesMatrix.size()) - 1)) return;
    for (int city = 0; city < citiesMatrix.size(); city++)
    {
        if ((mask & (1 << city)) == 0) {
            int newAns = citiesMatrix[pos][city] + state[city][mask | (1 << city)];
            if (newAns < ans) {
                ans = newAns;
                chosenCity = city;
            }
        }
    }

    shortestPath.push_back(chosenCity);
    getDPPath(chosenCity, mask | (1 << chosenCity), citiesMatrix, state, shortestPath);
}
//End of Dynamic Programming Algorithm


//Beginning Approximatif Algorithm with Prim
//https://github.com/shawontafsir/Travelling-Salesman/blob/master/1305072_mst_preorder.cpp
int getMinKey(uint64_t key[], bool mstSet[], int nbrCities)
{
   // Initialize min value
   uint64_t min = UINT64_MAX;
   int min_index;

    for (int v = 0; v < nbrCities; v++){
        if (!mstSet[v] && key[v] < min){
            min = key[v];
            min_index = v;
        }
    }

   return min_index;
}

vector<int> primMST(vector<Coord> cityArr, int nbrCities)
{
    vector<int> parent(nbrCities);
    uint64_t key[nbrCities];
    bool mstSet[nbrCities];

    for (int i = 0; i < nbrCities; i++) {
        key[i] = UINT64_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < (nbrCities - 1); count++) { 
        int minKey = getMinKey(key, mstSet, nbrCities);
        mstSet[minKey] = true;

        for (int v = 0; v < nbrCities; v++){
            uint64_t minDist = findEuclDist(cityArr[minKey], cityArr[v]);

            if (!mstSet[v] && minDist <  key[v]){
                parent[v] = minKey;
                key[v] = minDist;
            }
        }
    }

    return parent;
}

queue<int> preOrder(vector<Coord> cityArr, int nbrCities){
    queue<int> preOrderPath;
    vector<int> parent = primMST(cityArr, nbrCities);
    stack<int> stck; 
    vector<int> cityPassed; 

    for(int i = 0; i < nbrCities; i++) 
        cityPassed.push_back(0);

    stck.push(0);
    preOrderPath.push(0);

    while(!stck.empty()){
        int p = stck.top();
        for(int i = cityPassed[p]; i < nbrCities; i++){
            if(parent[i] == p){
                parent[i] = -1; 
                cityPassed[p] = i;
                stck.push(i);
                preOrderPath.push(i);
                break;
            }

            if(i == (nbrCities - 1)) stck.pop();
        }
    }

    return preOrderPath;
}

uint64_t approxAlgo(vector<Coord> cityArr, int nbrCities, vector<int>& shortestPath)
{   
    queue<int> q = preOrder(cityArr, nbrCities);
    uint64_t totMinDist = 0;

    while(!q.empty()){
        int i = q.front();
        shortestPath.push_back(i);
        q.pop();
        if(!q.empty()){
            int j = q.front();
            totMinDist += findEuclDist(cityArr[i], cityArr[j]);
        }
        else {
            totMinDist += findEuclDist(cityArr[i], cityArr[0]);
        }
    }
    return totMinDist;
}
//End Approximatif Algorithm


//Print help menu
static void showUsage(std::string name)
{
    std::cerr << "Usage: " << name << " -a {glouton, progdyn, approx} -e CHEMIN_EXEMPLAIRE [-p] [-t]"
        << "Parametre optionel :\n"
        << "\t[-p] affiche dans l’ordre, sur chaque ligne, les indices des villes à visiter en commençant par 0 et en finissant par 0, sans texte superflu. Rapportez le chemin tel que la deuxième ville visitée ait un indice inférieur à celui de l’avant dernière ville affichée.\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
}

//Read File with City coordinates
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
            int x = -1, y = -1;
            inputFileStream >> x >> y;
            Coord coordinates = { x, y };
            if (x == -1 || y == -1) break; //In case of empty line at the end of file.
            cityArr.push_back(coordinates);
        }
        inputFileStream.close();
    }
    else {
        std::cerr << "Ne peux pas ouvrir le fichier specifie " << filePath << "\n";
        return std::make_pair(0, cityArr);
    }
    return std::make_pair(nbrCities, cityArr);
}

//Prints the shortest path so that the second city printed is smaller than the second to last city printed
void printShortestPath(vector<int> shortestPath) {
    shortestPath.push_back(0);
    if (shortestPath[1] > shortestPath[shortestPath.size() - 2]) {
        for (int k = shortestPath.size() - 1; k >= 0; k--) {
            std::cout << shortestPath[k] << endl;
        }
    }
    else {
        for (int k = 0; k < shortestPath.size(); k++) {
            std::cout << shortestPath[k] << endl;
        }
    }
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

    if (nbrCities == 0) return 1;

    std::chrono::high_resolution_clock::time_point start, finish;
    vector<int> shortestPath;
    uint64_t totMinDist = 0;

    if (method == "glouton" || method == GLOUTON_CODE) {
        shortestPath = vector<int>(nbrCities, -1);
        start = std::chrono::high_resolution_clock::now();
        totMinDist = greedyAlgo(cityArr, shortestPath);
        finish = std::chrono::high_resolution_clock::now();
    }
    else if (method == "progdyn" || method == PROGDYN_CODE) {
        vector<vector<int>> citiesMatrix = findDistMatrix(cityArr);
        vector<vector<int>> state(citiesMatrix.size());
        for (auto& neighbors : state)
            neighbors = vector<int>((1 << citiesMatrix.size()) - 1, INT_MAX);

        start = std::chrono::high_resolution_clock::now();
        totMinDist = DPAlgo(0, 1, citiesMatrix, state);
        finish = std::chrono::high_resolution_clock::now();

        shortestPath.push_back(0);
        getDPPath(0, 1, citiesMatrix, state, shortestPath);
    }
    else if (method == "approx" || method == APPROX_CODE) {
        start = std::chrono::high_resolution_clock::now();
        totMinDist = approxAlgo(cityArr, nbrCities, shortestPath);
        finish = std::chrono::high_resolution_clock::now();
    }

    std::cout << "Distance Minimum : " << totMinDist << endl << endl;

    if (printTime) {
        double executionTime = std::chrono::duration<double, std::milli>(finish - start).count();
        std::cout << executionTime << "\n" << endl;
    }
    if (printResult) {
        printShortestPath(shortestPath);
    }
    return 0;
}
