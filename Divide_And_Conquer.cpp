#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// A divide and conquer based C++ program to find skyline of given buildings 
// https://www.geeksforgeeks.org/the-skyline-problem-using-divide-and-conquer-algorithm/

// A structure for building 
struct Building {
    // x coordinate of left side 
    int left;

    // x coordinate of right side 
    int right;

    // height 
    int height;
};

// A strip in skyline 
class Strip {
    // x coordinate of left side 
    int left;

    // height 
    int height;

public:
    Strip(int l = 0, int h = 0)
    {
        left = l;
        height = h;
    }
    friend class SkyLine;
};

// Skyline: To represent Output(An array of strips) 
class SkyLine {
    // Array of strips 
    Strip* arr;

    // Capacity of strip array 
    int capacity;

    // Actual number of strips in array 
    int n;

public:
    ~SkyLine() { delete[] arr; }
    int count() { return n; }

    // A function to merge another skyline to this skyline 
    SkyLine* Merge(SkyLine* other);

    // Constructor 
    SkyLine(int cap)
    {
        capacity = cap;
        arr = new Strip[cap];
        n = 0;
    }

    // Function to add a strip 'st' to array 
    void append(Strip* st)
    {
        // Check for redundant strip, a strip is 
        // redundant if it has same height or left as previous 
        if (n > 0 && arr[n - 1].height == st->height)
            return;
        if (n > 0 && arr[n - 1].left == st->left) {
            arr[n - 1].height = max(arr[n - 1].height, st->height);
            return;
        }

        arr[n] = *st;
        n++;
    }

    // A utility function to print all strips of skyline 
    void print()
    {
        for (int i = 0; i < n; i++) {
            cout << arr[i].left << " " << arr[i].height << endl;
        }
    }
};

// This function returns skyline for a given array of buildings arr[l..h]. 
// This function is similar to mergeSort(). 
SkyLine* divideAndConquer(Building arr[], int l, int h)
{
    if (l == h) {
        SkyLine* res = new SkyLine(2);
        res->append(new Strip(arr[l].left, arr[l].height));
        res->append(new Strip(arr[l].right, 0));
        return res;
    }

    int mid = (l + h) / 2;

    // Recur for left and right halves and merge the two results 
    SkyLine* sl = divideAndConquer(arr, l, mid);
    SkyLine* sr = divideAndConquer(arr, mid + 1, h);
    SkyLine* res = sl->Merge(sr);

    // To avoid memory leak 
    delete sl;
    delete sr;

    // Return merged skyline 
    return res;
}

// Similar to merge() in MergeSort 
// This function merges another skyline 'other' to the skyline for which it is called. 
// The function returns pointer to the resultant skyline 
SkyLine* SkyLine::Merge(SkyLine* other)
{
    // Create a resultant skyline with capacity as sum of two skylines 
    SkyLine* res = new SkyLine(this->n + other->n);

    // To store current heigheights of two skylines 
    int h1 = 0, h2 = 0;

    // Indexes of strips in two skylines 
    int i = 0, j = 0;
    while (i < this->n && j < other->n) {
        // Compare x coordinates of left sides of two skylines and put the smaller one in result 
        if (this->arr[i].left < other->arr[j].left) {
            int x1 = this->arr[i].left;
            h1 = this->arr[i].height;

            // Choose height as max of two heigheights 
            int maxh = max(h1, h2);

            res->append(new Strip(x1, maxh));
            i++;
        } else {
            int x2 = other->arr[j].left;
            h2 = other->arr[j].height;
            int maxh = max(h1, h2);
            res->append(new Strip(x2, maxh));
            j++;
        }
    }

    // If there are strips left in this skyline or other skyline 
    while (i < this->n) {
        res->append(&arr[i]);
        i++;
    }
    while (j < other->n) {
        res->append(&other->arr[j]);
        j++;
    }
    return res;
}
// End of divide and Conquer code.


// Brute force program to find skyline of given buildings 
// Inspired from : http://sandrasi-sw.blogspot.com/2012/12/the-skyline-problem.html
struct Point 
{
    int x, y;
    Point(int paramx, int paramy) : x(paramx), y(paramy) {}
};

bool compare(Point i1, Point i2)
{
    return (i1.x < i2.x);
}

int substract(int x, int y) 
{
    int result = x - y;
    return result;
}

int add(int x, int y) {
    int result = x + y;
    return result;
}

vector<Point> checkLinearity(vector<Point> points) 
{
    for (unsigned int i = 0; i < points.size() - 1; i++) {
        if (substract(points[i + 1].x, points[i].x) > 1) {
            points.insert(points.begin() + i + 1, Point(add(points[i].x, 1), 0));
        }
    }
    return points;
}

vector<Point> bruteForce(Building arr[], int n) 
{
    std::vector<Point> points;
    for (int i = 0; i < n; i++) {
        int first_x = arr[i].left;
        int last_x = arr[i].right;
        for (int j = first_x; j < last_x + 1; j++) {
            points.push_back(Point(j, arr[i].height));
        }
    }

    std::sort(points.begin(), points.end(), compare);
    std::vector<Point> currentPoint = { Point(0,0),Point(1,0) };
    std::vector<Point> sortedPoints;
    
    //find for same x max y value
    for (unsigned int i = 0; i < points.size(); i++) {
        if (points[i].x > currentPoint[0].x) {
            sortedPoints.push_back(Point(currentPoint[0].x, currentPoint[0].y));
            currentPoint[0].x = points[i].x;
            currentPoint[0].y = points[i].y;
        }
        if (points[i].x == currentPoint[0].x) {
            if (points[i].y > currentPoint[0].y) {
                currentPoint[0].y = points[i].y;
            }
        }
    }
    sortedPoints.push_back(Point(points[points.size() - 1].x, 0));

    //check linearity to have a (x,y) for all range of x
    sortedPoints = checkLinearity(sortedPoints);

    //find skyline
    std::vector<Point> finalPoints;
    for (unsigned int i = 0; i < sortedPoints.size() - 1; i++) {
        if (currentPoint[1].y != sortedPoints[i].y) {
            if (currentPoint[1].y > sortedPoints[i].y) {
                finalPoints.push_back(Point(sortedPoints[i - 1].x, sortedPoints[i].y));
                currentPoint[1].y = sortedPoints[i].y;
            }
            else {
                finalPoints.push_back(Point(sortedPoints[i].x, sortedPoints[i].y));
                currentPoint[1].y = sortedPoints[i].y;
            }
        }
    }
    finalPoints.push_back(Point(sortedPoints[sortedPoints.size() - 1].x, sortedPoints[sortedPoints.size() - 1].y));
    return finalPoints;
}

SkyLine* convertStripToSkyLine(vector<Point> points) 
{
    SkyLine* skyline = new SkyLine(points.size());
    for (unsigned i = 0; i < points.size(); i++) {
        skyline->append(new Strip(points[i].x, points[i].y));
    }
    return skyline;
}
// End of brute force code.


// Divide and Conquer with resursion threshold.
SkyLine* divideAndConquerSeuil(Building arr[], int l, int h) 
{
    int n = h + 1;
    if (n <= 20) {
        vector<Point> result = bruteForce(arr, h);
        SkyLine* skyline = convertStripToSkyLine(result);
        return skyline;
    }

    SkyLine* skyline = divideAndConquer(arr, 0, h);
    return skyline;
}
// End Divide and Conquer with resursion threshold code.


// Print help menu
static void showUsage(std::string name) 
{
    std::cerr << "Usage: " << name << " -a {brute, recursif, seuil} -e CHEMIN_EXEMPLAIRE <option(s)> SOURCES"
        << "Parametre optionel pa:\n"
        << "\t[-p] affiche, sur chaque ligne, les couples définissant la silhouette de bâtiments, triés selon l’abscisse et sans texte superflu (les deux valeurs d’un couple sont séparées d’un espace)\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
}

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

    std::pair<int, std::vector<Building>> fileData = readExempFile(filePath);
    int n = fileData.first;
    Building* arr = new Building[fileData.second.size()];
    std::copy(fileData.second.begin(), fileData.second.end(), arr);

    SkyLine* ptr;
    double executionTime;

    if (method == "recursif") {
        auto start = std::chrono::high_resolution_clock::now();
        ptr = divideAndConquer(arr, 0, n - 1);
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double>(finish - start).count();  
    } 
    else if (method == "brute") {
        auto start = std::chrono::high_resolution_clock::now();
        vector<Point> skyline = (bruteForce(arr, n - 1));
        ptr = convertStripToSkyLine(skyline);
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double>(finish - start).count();    
    } 
    else if (method == "seuil") {
        auto start = std::chrono::high_resolution_clock::now();
        ptr = divideAndConquerSeuil(arr, 0, n - 1);
        auto finish = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double>(finish - start).count();
    } 
    else {
        std::cerr << "Algorithme de type invalide \n";
        return 1;
    }

    if (printResult) {
        ptr->print();
    }
    if (printTime) {
        std::cout << executionTime << " ms\n";
    }
    return 0;
}