#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;

// ======================================================
//              FIFO PAGE REPLACEMENT ALGORITHM
// ======================================================
void fifo(vector<int>& pages, int framesize) {
    unordered_set<int> frames;   // To hold pages currently in memory
    queue<int> pageOrder;        // Maintains insertion order (FIFO)
    int pageFaults = 0;          // Count page faults

    cout << "\n--- FIFO Page Replacement ---\n";

    // Traverse through all page requests
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        // If page not found in current frames -> PAGE FAULT occurs
        if (frames.find(page) == frames.end()) {
            pageFaults++; // Increase fault count

            // If there is free space -> add page directly
            if (frames.size() < framesize) {
                frames.insert(page);
                pageOrder.push(page);
            } 
            else {
                // Otherwise, remove the oldest page (front of queue)
                int oldestPage = pageOrder.front();
                pageOrder.pop();
                frames.erase(oldestPage);

                // Add the new page
                frames.insert(page);
                pageOrder.push(page);
            }
        }

        // Print current frame status after each page reference
        cout << "Step " << i + 1 << " -> Frames: ";
        for (const auto &f : frames)
            cout << f << " ";
        cout << endl;
    }

    cout << "Total Page Faults (FIFO): " << pageFaults << "\n";
}

// ======================================================
//              LRU PAGE REPLACEMENT ALGORITHM
// ======================================================
void lru(vector<int>& pages, int framesize) {
    list<int> frames;                       // Holds pages (front = least recently used)
    unordered_map<int, list<int>::iterator> position; // Keeps track of each page’s position
    int pageFaults = 0;

    cout << "\n--- LRU Page Replacement ---\n";

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        // If page is not found in memory -> PAGE FAULT
        if (position.find(page) == position.end()) {
            pageFaults++;

            // If memory is full -> remove least recently used page (front)
            if (frames.size() == framesize) {
                int lruPage = frames.front();
                frames.pop_front();
                position.erase(lruPage);
            }

            // Add current page to the back (most recently used)
            frames.push_back(page);
            position[page] = prev(frames.end());
        }
        else {
            // Page already in memory -> move it to the end (most recently used)
            frames.erase(position[page]);
            frames.push_back(page);
            position[page] = prev(frames.end());
        }

        // Print current frame contents
        cout << "Step " << i + 1 << " -> Frames: ";
        for (const auto &f : frames)
            cout << f << " ";
        cout << endl;
    }

    cout << "Total Page Faults (LRU): " << pageFaults << "\n";
}

// ======================================================
//              OPTIMAL PAGE REPLACEMENT ALGORITHM
// ======================================================
void optimal(vector<int>& pages, int framesize) {
    vector<int> frames;    // Holds pages currently in memory
    int pageFaults = 0;    // Counter for faults

    cout << "\n--- Optimal Page Replacement ---\n";

    // Process each page request
    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];

        // If page is already present in frames -> no fault
        if (find(frames.begin(), frames.end(), page) != frames.end()) {
            cout << "Step " << i + 1 << " -> Frames: ";
            for (auto f : frames) cout << f << " ";
            cout << endl;
            continue;
        }

        // Page not found -> PAGE FAULT
        pageFaults++;

        // If memory not full -> directly insert page
        if (frames.size() < framesize) {
            frames.push_back(page);
        } 
        else {
            // Need to replace a page -> choose the one used farthest in future
            int farthest = i + 1;  // Default next use index
            int replaceIndex = -1; // Index to replace

            // For every page currently in memory
            for (int j = 0; j < frames.size(); j++) {
                int nextUse = INT_MAX; // Track next use of this page

                // Find next occurrence of this page in the future
                for (int k = i + 1; k < pages.size(); k++) {
                    if (pages[k] == frames[j]) {
                        nextUse = k;
                        break;
                    }
                }

                // The page with the farthest next use will be replaced
                if (nextUse > farthest) {
                    farthest = nextUse;
                    replaceIndex = j;
                }
            }

            // If all pages will be used soon, replace the first one
            if (replaceIndex == -1) replaceIndex = 0;

            // Replace selected page with the new one
            frames[replaceIndex] = page;
        }

        // Display current frame contents
        cout << "Step " << i + 1 << " -> Frames: ";
        for (auto f : frames) cout << f << " ";
        cout << endl;
    }

    cout << "Total Page Faults (Optimal): " << pageFaults << "\n";
}

// ======================================================
//                       MAIN FUNCTION
// ======================================================
int main() {
    int n, framesize;

    // Input number of pages
    cout << "Enter number of pages: ";
    cin >> n;

    // Input the reference string
    vector<int> pages(n);
    cout << "Enter the page reference string (space-separated): ";
    for (int i = 0; i < n; i++)
        cin >> pages[i];

    // Input number of frames
    cout << "Enter number of frames (memory size): ";
    cin >> framesize;

    // Run all three algorithms
    fifo(pages, framesize);
    lru(pages, framesize);
    optimal(pages, framesize);

    cout << "\nSimulation Finished.\n";
    return 0;
}
