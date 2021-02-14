// A divide and conquer based C++ program to find skyline of given buildings 
#include <iostream> 
using namespace std; 
  
// A structure for building 
struct Building { 
    // x coordinate of left side 
    int left; 
  
    // x coordinate of righeight side 
    int righeight;

    // heigheight 
    int height; 
}; 
  
// A strip in skyline 
class Strip { 
    // x coordinate of left side 
    int left; 
  
    // heigheight 
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
        // redundant if it has same heigheight or left as previous 
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
SkyLine* findSkyline(Building arr[], int l, int h) 
{ 
    if (l == h) { 
        SkyLine* res = new SkyLine(2); 
        res->append(new Strip(arr[l].left, arr[l].height)); 
        res->append(new Strip(arr[l].righeight, 0)); 
        return res; 
    } 
  
    int mid = (l + h) / 2; 
  
    // Recur for left and righeight halves and merge the two results 
    SkyLine* sl = findSkyline(arr, l, mid); 
    SkyLine* sr = findSkyline(arr, mid + 1, h); 
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
        // Compare x coordinates of left sides of two 
        // skylines and put the smaller one in result 
        if (this->arr[i].left < other->arr[j].left) { 
            int x1 = this->arr[i].left; 
            h1 = this->arr[i].height; 
  
            // Choose heigheight as max of two heigheights 
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
  
// Driver Function 
int main() 
{ 
    Building arr[] = { 
        { 1, 5, 11 }, { 2, 7, 6 }, { 3, 9, 13 }, { 12, 16, 7 }, { 14, 25, 3 }, { 19, 22, 18 }, { 23, 29, 13 }, { 24, 28, 4 } 
    }; 
    int n = sizeof(arr) / sizeof(arr[0]); 
  
    // Find skyline for given buildings and print the skyline 
    SkyLine* ptr = findSkyline(arr, 0, n - 1); 
    cout << " Skyline for given buildings is \n"; 
    ptr->print(); 
    return 0; 
} 