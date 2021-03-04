// A divide and conquer based C++ program to find skyline of given buildings 
#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;
int init_time;
int final_time;

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

class StripBF {
public:
    int left;
    int height;
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
            cout << " (" << arr[i].left << ", "
                << arr[i].height << "), ";
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
    SkyLine* res = new SkyLine(
        this->n + other->n);

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
        }
        else {
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

void append(vector<StripBF>& skyline, StripBF& cur_strip)
{
    int size = skyline.size();

    /*If the current strip to be added and the last strip of the skyline have
    the  same left position, then pick the max height of the two strips and
    update the height of the last strip in the skyline and return*/
    if (size && skyline[size - 1].left == cur_strip.left) {
        skyline[size - 1].height = max(skyline[size - 1].height,
            cur_strip.height);
        return;
    }

    /*The current strip to be added has the same height as the last strip in
    skyline. So no need to add the current strip since neighboring strips
    in skyline should have unique heights. So simply return*/
    if (size && skyline[size - 1].height == cur_strip.height)
        return;

    /*The current strip to be added starts at a different position and has a
    different height than the last strip in the skyline. So add the current
    strip to the skyline*/
    skyline.push_back(cur_strip);
}

vector<StripBF> mergeBruteForce(vector<StripBF>& skyline1,
    vector<StripBF>& skyline2)
{
    vector<StripBF> result;
    int i = 0, j = 0;
    /*h1 keeps track of the current height in skyline1 and h2 keeps track of the
    current height in skyline2 as we traverse the two skylines*/
    int h1 = 0, h2 = 0;
    StripBF cur_strip;

    while (i < skyline1.size() && j < skyline2.size()) {
        /*Pick the strip which is leftmost in the two skylines*/
        if (skyline1[i].left < skyline2[j].left) {
            h1 = skyline1[i].height; /*update current height of skyline1*/

            /*The height of the strip to be added to the result will have
            the maximum of the current heights of the two skylines*/
            cur_strip.left = skyline1[i].left;
            cur_strip.height = max(h1, h2);

            append(result, cur_strip);
            ++i;
        }
        else if (skyline1[i].left == skyline2[j].left) {
            h1 = skyline1[i].height; /*update current height of the skylines*/
            h2 = skyline2[j].height;

            /*The height of the strip to be added to the result will have
            the maximum of the current heights of the two skylines*/
            cur_strip.left = skyline1[i].left;
            cur_strip.height = max(h1, h2);

            append(result, cur_strip);
            ++i;
            ++j;
        }
        else {
            h2 = skyline2[j].height; /*update current height of skyline2*/

            /*The height of the strip to be added to the result will have
            the maximum of the current heights of the two skylines*/
            cur_strip.left = skyline2[j].left;
            cur_strip.height = max(h1, h2);

            append(result, cur_strip);
            ++j;
        }
    }

    /*Append the remaining strips in skyline1 to the result*/
    while (i < skyline1.size()) {
        append(result, skyline1[i]);
        ++i;
    }

    /*Append the remaining strips in skyline2 to the result*/
    while (j < skyline2.size()) {
        append(result, skyline2[j]);
        ++j;
    }

    return result;
}

vector<StripBF> bruteForce(Building arr[], int low, int high) {
    if (low == high) {
        /*We have reached a single building. Create the result skyline*/
        vector<StripBF> result;
        StripBF cur_strip;

        /*Add the strip corresponding to left end of building to result*/
        cur_strip.left = arr[low].left,
        cur_strip.height = arr[low].height;
        result.push_back(cur_strip);

        /*Add the strip corresponding to right end of building to result.
        For the right end of the building, the height will be 0*/
        cur_strip.left = arr[low].right;
        cur_strip.height = 0;
        result.push_back(cur_strip);

        /*Terminate the recursion and return the result skyline*/
        return result;
    }
    int mid = (low + high) / 2;

    vector<StripBF> skyline1 = bruteForce(arr, low, mid);
    vector<StripBF> skyline2 = bruteForce(arr, mid + 1, high);
    vector<StripBF> result = mergeBruteForce(skyline1, skyline2);

    return result;
}

SkyLine* convertStripToSkyLine(vector<StripBF> strip){
    SkyLine* skyline = new SkyLine(strip.size());
    for (unsigned i = 0; i < strip.size(); i++) {
        skyline->append(new Strip(strip[i].left, strip[i].height));
    }
    return skyline;
}

SkyLine* divideAndConquerSeuil(Building arr[], int l, int h)
{
    int n = sizeof(arr);
    if(n <= 750) {
        vector<StripBF> result = bruteForce(arr, l, h);
        SkyLine* skyline = convertStripToSkyLine(result);
        return skyline;
    }

    SkyLine* skyline = divideAndConquer(arr, 0, n - 1);
    return skyline;
}

static void showUsage(std::string name)
{
    std::cerr << "Usage: " << name << " -a {brute, recursif, seuil} -e CHEMIN_EXEMPLAIRE <option(s)> SOURCES"
        << "Options:\n"
        << "\t[-p] affiche, sur chaque ligne, les couples définissant la silhouette de bâtiments, triés selon l’abscisse et sans texte superflu (les deux valeurs d’un couple sont séparées d’un espace)\n"
        << "\t[-t] affiche le temps d’exécution en millisecondes, sans unité ni texte superflu\n"
        << std::endl;
}

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
                cout << " n is :" << nbrBuildings << endl;
                isFirstLine = false;
            }
           
            int right, left, height;
            inputFileStream >> right >> left >> height;
            Building building = { right, left, height };
            buildingArray.push_back(building);
        }
        inputFileStream.close();
    }
    else {
        std::cerr << "Couldn't open " << filePath << " for reading\n";
    }
    return std::make_pair(nbrBuildings, buildingArray);
}

// Driver Function 
int main(int argc, char* argv[])
{
    if (argc < 5) {
        showUsage(argv[0]);
        return 1;
    }

    std::string method;
    std::string filePath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-a") {
            method = argv[++i];
        }
        else if (arg == "-e") {
            filePath = argv[++i];
        }
        else {
            //implement -p and -t options if we have time
        }
    }

    std::pair<int, std::vector<Building>> fileData = readExempFile(filePath);
    int n = fileData.first;
    Building* arr = new Building[fileData.second.size()];
    std::copy(fileData.second.begin(), fileData.second.end(), arr);

    if (method == "recursif") {
        // Find skyline for given buildings and print the skyline
        auto start = std::chrono::high_resolution_clock::now();
        SkyLine* ptr = divideAndConquer(arr, 0, n - 1);
        ptr->print();
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
    }
    else if (method == "brute") {
        //call brute force algo 
        vector<vector<StripBF>> result;
        
        auto start = std::chrono::high_resolution_clock::now();
        vector<StripBF> skyline= (bruteForce(arr, 0, n-1));
        SkyLine* ptr = convertStripToSkyLine(skyline);
        ptr->print();
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
    }
    else if (method == "seuil") {
        //call seuil algo
        auto start = std::chrono::high_resolution_clock::now();
        SkyLine* ptr = divideAndConquerSeuil(arr, 0, n - 1);
        ptr->print();
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
    }
    else {
        std::cerr << "Invalid algorithm type \n";
    }
    return 0;
}