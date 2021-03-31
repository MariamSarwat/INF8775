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



uint64_t findEuclDist(Coord initCoord, Coord finalCoord) {
    uint64_t x = pow(finalCoord.x - initCoord.x, 2);
    uint64_t y = pow(finalCoord.y - initCoord.y, 2);
    return round(sqrt(x + y));
}

// Inspire de https://www.geeksforgeeks.org/travelling-salesman-problem-greedy-approach/?ref=rp
// Function to find the minimum cost path using the greedy method
void greedyAlgo(std::vector<Coord> cityArr, vector<int>& shortestPath)
{
    uint64_t totMinDist = 0;
    int counter = 0;
    int j = 0, i = 0;
    uint64_t minDist = UINT64_MAX;
    map<int, int> visitedRouteList;

    // Starting from the 0th indexed city i.e., the first city
    visitedRouteList[0] = 1;
    shortestPath[counter] = 0;
    counter++;

    // Traverse vector cityArr
    while (i < cityArr.size() && j < cityArr.size())
    {
        // Visited all cities, close path (last node to first node)
        // Update the ending city in array from city which was last visited
        if (counter == cityArr.size())
        {
            i = shortestPath[counter - 1];
            uint64_t euclDist = findEuclDist(cityArr[i], cityArr[0]);
            totMinDist += euclDist;
            break;
        }

        if (j != i && visitedRouteList[j] == 0) {
            uint64_t euclDist = findEuclDist(cityArr[i], cityArr[j]);
            if (euclDist < minDist)
            {
                minDist = euclDist;
                shortestPath[counter] = j;
            }
        }
        j++;

        // Check all paths from the ith indexed city
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

    std::cout << "Minimum Cost is : " << (totMinDist) << std::endl;
}

std::vector<std::vector<int>> findDistMatrix(std::vector<Coord> cities) {
    std::vector<std::vector<int>> citiesMatrix(cities.size());
    for (auto& neighbors : citiesMatrix)
        neighbors = vector<int>((1 << cities.size()) - 1, 0);

    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            if (i == j) {
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

//https://gist.github.com/jgcoded/d7ecba7aa3e210419471
int DPAlgo(int pos, int visited, const vector<vector<int>>& citiesMatrix, vector<vector<int>>& state)
{
    if (visited == ((1 << citiesMatrix.size())) - 1)
        return citiesMatrix[pos][0]; // return to starting city

    if (state[pos][visited] != INT_MAX)
        return state[pos][visited];

    for (int i = 0; i < citiesMatrix.size(); i++)
    {
        // can't visit ourselves unless we're ending & skip if already visited
        if (i == pos || (visited & (1 << i))) {
            continue;
        }
        int distance = citiesMatrix[pos][i] + DPAlgo(i, visited | (1 << i), citiesMatrix, state);

        if (distance < state[pos][visited]) {
            state[pos][visited] = distance;
        }
    }
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

    // here you get the current city you need to visit
    shortestPath.push_back(chosenCity);
    getDPPath(chosenCity, mask | (1 << chosenCity), citiesMatrix, state, shortestPath);
}

//https://www.geeksforgeeks.org/travelling-salesman-problem-set-2-approximate-using-mst/?fbclid=IwAR1mcT7xs8ARV-fgmI7uAbIRTN1buN6i2ANK2HKPoEn3f6zvjG9Tkc-otgA
//http://www.mathcs.emory.edu/~cheung/Courses/171/Syllabus/11-Graph/prim2.html?fbclid=IwAR03NsIwqkDZUYQam7pWxX7dccTySCZobjWRxNshCH94tQrR2RmRXqRM5vs
void approx_old(std::vector<Coord> cityArr, vector<int>& shortestPath) {
    uint64_t totMinDist = 0;
    int counter = 0;

    uint64_t minDist = UINT64_MAX;
    Coord actualPoint = {};
    Coord chosenPoint = {};
    Coord firstPoint = {};
    vector<Coord> cityArrRef = cityArr;
    std::vector<Coord> visitedRouteList;

    actualPoint.x = cityArr[0].x;
    actualPoint.y = cityArr[0].y;
    firstPoint.x = cityArr[0].x;
    firstPoint.y = cityArr[0].y;
    shortestPath.push_back(0);

    visitedRouteList.push_back(cityArr[0]);
    cityArr.erase(cityArr.begin());

    while (cityArr.size() > 0)
    {
        for (int i = 0; i < cityArr.size(); i++) {
            uint64_t distEucl = findEuclDist(actualPoint, cityArr[i]);
            if (distEucl < minDist) {
                minDist = distEucl;
                chosenPoint.x = cityArr[i].x;
                chosenPoint.y = cityArr[i].y;
            }
        }
        totMinDist += minDist;
        visitedRouteList.push_back(chosenPoint);
        actualPoint.x = chosenPoint.x;
        actualPoint.y = chosenPoint.y;

        auto coord = std::find(cityArrRef.begin(), cityArrRef.end(), actualPoint);
        auto index = std::distance(cityArrRef.begin(), coord);
        shortestPath.push_back(index);

        cityArr.erase(find(cityArr.begin(), cityArr.end(), chosenPoint));
        minDist = UINT64_MAX;
        counter++;
    }
    uint64_t distEucl = findEuclDist(actualPoint, firstPoint);
    totMinDist += distEucl;
    visitedRouteList.push_back(firstPoint);
    std::cout << "Minimum Cost is : " << (totMinDist) << std::endl;

}

struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode* head; // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode {
    int v;
    int key;
};

// Structure to represent a min heap
struct MinHeap {
    int size; // Number of heap nodes present currently
    int capacity; // Capacity of min heap
    int* pos; // This is needed for decreaseKey()
    struct MinHeapNode** array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int key)
{
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->key = key;
    return minHeapNode;
}

// A utilit function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key)
        smallest = right;

    if (smallest != idx) {
        // The nodes to be swapped in min heap
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease key value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int key)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its key value
    minHeap->array[i]->key = key;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key) {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap* minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// A utility function used to print the constructed MST
void printArr(int arr[], int n)
{
    for (int i = 1; i < n; ++i)
        printf("%d - %d\n", arr[i], i);
}

// The main function that constructs Minimum Spanning Tree (MST)
// using Prim's algorithm
void PrimMST(struct Graph* graph)
{
    uint64_t totMinDist = 0;
    const int V = graph->V; // Get the number of vertices in graph
    int* parent = new int[V];
    for (int i = 0; i < V; i++)
        parent[i] = i;
    // Array to store constructed MST
   //int key[V]; 
    int* key = new int[V];
    for (int i = 0; i < V; i++)
        key[i] = i;// Key values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. Key value of
    // all vertices (except 0th vertex) is initially infinite
    for (int v = 1; v < V; ++v) {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, key[v]);
        minHeap->pos[v] = v;
    }

    // Make key value of 0th vertex as 0 so that it
    // is extracted first
    key[0] = 0;
    minHeap->array[0] = newMinHeapNode(0, key[0]);
    minHeap->pos[0] = 0;

    // Initially size of min heap is equal to V
    minHeap->size = V;
    vector<int> result;
    // In the following loop, min heap contains all nodes
    // not yet added to MST.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum key value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number
        result.push_back(u);
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their key values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;

            // If v is not yet included in MST and weight of u-v is
            // less than key value of v, then update key value and
            // parent of v
            if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) {
                key[v] = pCrawl->weight;
                

                parent[v] = u;
                decreaseKey(minHeap, v, key[v]);
            }
            pCrawl = pCrawl->next;
        }
        totMinDist += key[u];
    }

    // print edges of MST
    printArr(parent, V);
    for(int i: result)
        std::cout << i << " ";
    std::cout << endl;
    std::cout << totMinDist << endl;
} 

Graph* createGraph(std::vector<Coord> cityArr) {
    int V = cityArr.size();
    struct Graph* graph = createGraph(V);
    for (int i = 0; i < cityArr.size(); i++) {
        for (int j = i + 1; j < cityArr.size(); j++) {
            addEdge(graph, i, j, findEuclDist(cityArr[i], cityArr[j]));
        }
    }

    return graph;
    
}

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

    if (method == "glouton" || method == GLOUTON_CODE) {
        shortestPath = vector<int>(nbrCities, -1);
        start = std::chrono::high_resolution_clock::now();
        greedyAlgo(cityArr, shortestPath);
        finish = std::chrono::high_resolution_clock::now();
    }
    else if (method == "progdyn" || method == PROGDYN_CODE) {
        vector<vector<int>> citiesMatrix = findDistMatrix(cityArr);
        vector<vector<int>> state(citiesMatrix.size());
        for (auto& neighbors : state)
            neighbors = vector<int>((1 << citiesMatrix.size()) - 1, INT_MAX);

        start = std::chrono::high_resolution_clock::now();
        cout << "minimum: " << DPAlgo(0, 1, citiesMatrix, state) << endl;
        finish = std::chrono::high_resolution_clock::now();

        shortestPath.push_back(0);
        getDPPath(0, 1, citiesMatrix, state, shortestPath);
    }
    else if (method == "approx" || method == APPROX_CODE) {
        Graph* graph = createGraph(cityArr);
        start = std::chrono::high_resolution_clock::now();
        PrimMST(graph);
        finish = std::chrono::high_resolution_clock::now();
    }

    if (printTime) {
        double executionTime = std::chrono::duration<double, std::milli>(finish - start).count();
        std::cout << executionTime << "\n" << endl;
    }
    if (printResult) {
        printShortestPath(shortestPath);
    }
    return 0;
}
