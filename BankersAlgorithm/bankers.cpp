#include <iostream>
#include <vector>
#include <limits>
using namespace std;

// Global variables
const int n = 5; // Number of processes
const int m = 3; // Number of resource types

// Function to calculate the Need matrix (Need = Max - Allocation)
void calculateNeed(int need[n][m], int maxMatrix[n][m], int allocation[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = maxMatrix[i][j] - allocation[i][j];
        }
    }
}

// Function to print a matrix
void printMatrix(int matrix[n][m], string name) {
    cout << "# " << name << endl;
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << "[";
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j];
            if (j < m - 1) cout << ", ";
        }
        cout << "]";
        if (i < n - 1) cout << "," << endl;
    }
    cout << "]" << endl;
}

// Function to print a vector
void printVector(int vec[m], string name) {
    cout << name << endl;
    cout << "[";
    for (int i = 0; i < m; i++) {
        cout << vec[i];
        if (i < m - 1) cout << ", ";
    }
    cout << "]" << endl;
}

void displayState(int available[m], int need[n][m], int allocation[n][m], int maxMatrix[n][m]) {
    cout << endl;
    printVector(available, "# Available Vector (initially total resources available)");
    cout << endl;
    printMatrix(maxMatrix, "Maximum Matrix");
    cout << endl;
    printMatrix(allocation, "Allocation Matrix");
    cout << endl;
    printMatrix(need, "Need Matrix (Max - Allocation)");
}

// Safety Algorithm: Check if the system is in a safe state
bool isSafe(int available[m], int need[n][m], int allocation[n][m], vector<int>& safeSeq) {
    int work[m]; // Work vector (copy of Available)
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }

    bool finish[n] = {false}; // Track which processes have finished
    safeSeq.clear(); // Clear the safe sequence

    // Find a safe sequence
    int count = 0; // Number of processes that have finished
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) { // If process p is not finished
                // Check if resources are available for this process
                bool canProceed = true;
                for (int j = 0; j < m; j++) {
                    if (need[p][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                if (canProceed) { // If we can allocate resources to process p
                    // Add allocated resources back to work
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[p][j];
                    }
                    safeSeq.push_back(p); // Add process to safe sequence
                    finish[p] = true; // Mark process as finished
                    count++;
                    found = true;
                }
            }
        }
        if (!found) { // If no process can proceed, system is not safe
            return false;
        }
    }
    return true; // System is safe
}

// Resource-Request Algorithm
bool requestResources(int pid, int request[m], int available[m], int need[n][m], int allocation[n][m]) {
    // Step 1: Check if request <= need
    for (int i = 0; i < m; i++) {
        if (request[i] > need[pid][i]) {
            cout << "Error: Request exceeds maximum need." << endl;
            return false;
        }
    }

    // Step 2: Check if request <= available
    for (int i = 0; i < m; i++) {
        if (request[i] > available[i]) {
            cout << "Error: Not enough resources available." << endl;
            return false;
        }
    }

    // Step 3: Simulate granting the request
    int tempAvailable[m], tempAllocation[n][m], tempNeed[n][m];
    for (int i = 0; i < m; i++) {
        tempAvailable[i] = available[i] - request[i];
        tempAllocation[pid][i] = allocation[pid][i] + request[i];
        tempNeed[pid][i] = need[pid][i] - request[i];
    }
    // Copy allocation and need for other processes
    for (int i = 0; i < n; i++) {
        if (i != pid) {
            for (int j = 0; j < m; j++) {
                tempAllocation[i][j] = allocation[i][j];
                tempNeed[i][j] = need[i][j];
            }
        }
    }

    // Step 4: Check if the new state is safe
    vector<int> safeSeq;
    if (isSafe(tempAvailable, tempNeed, tempAllocation, safeSeq)) {
        // If safe, update the actual state
        for (int i = 0; i < m; i++) {
            available[i] = tempAvailable[i];
            allocation[pid][i] = tempAllocation[pid][i];
            need[pid][i] = tempNeed[pid][i];
        }
        cout << "System is in a safe state." << endl;
        cout << "Safe Sequence: [";
        for (int i = 0; i < safeSeq.size(); i++) {
            cout << safeSeq[i];
            if (i < safeSeq.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "Resources allocated to process " << pid << "." << endl;
        return true;
    } else {
        cout << "Error: Request would lead to an unsafe state." << endl;
        return false;
    }
}

// Function to safely get integer input
int getIntInput() {
    int value;
    while (!(cin >> value)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter a number: ";
    }
    return value;
}

// Function to handle user-defined resource request
void userRequest(int available[m], int need[n][m], int allocation[n][m], int maxMatrix[n][m]) {
    int pid;
    int request[m];
    
    // Get valid process ID
    bool validPid = false;
    while (!validPid) {
        cout << "Enter the process ID (0 to " << n-1 << "): ";
        pid = getIntInput();
        if (pid < 0 || pid >= n) {
            cout << "Invalid process ID. Please enter a value between 0 and " << n-1 << "." << endl;
        } else {
            validPid = true;
        }
    }

    // Get valid resource request
    cout << "Enter the resource request for process " << pid << " (format: r1 r2 r3): ";
    for (int i = 0; i < m; i++) {
        request[i] = getIntInput();
        if (request[i] < 0) {
            cout << "Invalid request: Resource values must be non-negative." << endl;
            cout << "Re-enter value for resource " << i << ": ";
            i--; // Re-enter this resource
        }
    }

    // Display the request
    cout << "# Process " << pid << " requests resources [";
    for (int i = 0; i < m; i++) {
        cout << request[i];
        if (i < m - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "request = [";
    for (int i = 0; i < m; i++) {
        cout << request[i];
        if (i < m - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "request_resources(" << pid << ", request)" << endl;
    cout << "Output:" << endl;

    // Process the request
    requestResources(pid, request, available, need, allocation);
}

int main() {
    // Initialize the system with the given data
    int available[m] = {3, 3, 2}; // Available vector
    int maxMatrix[n][m] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    }; // Maximum matrix
    int allocation[n][m] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    }; // Allocation matrix

    // Calculate the Need matrix
    int need[n][m];
    calculateNeed(need, maxMatrix, allocation);

    // Print the initial state
    cout << "n = " << n << " # Number of processes" << endl;
    cout << "m = " << m << " # Number of resources types" << endl;
    displayState(available, need, allocation, maxMatrix);

    // Menu-driven interface
    int choice;
    do {
        cout << "\nBanker's Algorithm Test Menu:" << endl;
        cout << "1. Test Case 1: Check initial safety" << endl;
        cout << "2. Test Case 2: Process 1 requests [1, 0, 2]" << endl;
        cout << "3. Test Case 3: Process 4 requests [3, 3, 1]" << endl;
        cout << "4. User-defined resource request" << endl;
        cout << "5. Display Current State" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        
        choice = getIntInput();

        vector<int> safeSeq;
        switch (choice) {
            case 1: { 
                cout << "\nTest case 1: Run safe test:" << endl;
                if (isSafe(available, need, allocation, safeSeq)) {
                    cout << "System is in a safe state." << endl;
                    cout << "Safe Sequence: [";
                    for (int i = 0; i < safeSeq.size(); i++) {
                        cout << safeSeq[i];
                        if (i < safeSeq.size() - 1) cout << ", ";
                    }
                    cout << "]" << endl;
                } else {
                    cout << "System is not in a safe state." << endl;
                }
                break;
            }

            case 2: { 
                cout << "\nTest case 2:" << endl;
                cout << "# Process 1 requests resources [1, 0, 2]" << endl;
                int request1[m] = {1, 0, 2};
                cout << "request = [1, 0, 2]" << endl;
                cout << "request_resources(1, request)" << endl;
                cout << "Output:" << endl;
                requestResources(1, request1, available, need, allocation);
                break;
            }

            case 3: { 
                cout << "\nTest case 3:" << endl;
                cout << "# Process 4 requests resources [3, 3, 1]" << endl;
                int request2[m] = {3, 3, 1};
                cout << "request = [3, 3, 1] # Process 4's resource request" << endl;
                cout << "request_resources(4, request)" << endl;
                cout << "Output:" << endl;
                requestResources(4, request2, available, need, allocation);
                break;
            }

            case 4: { 
                userRequest(available, need, allocation, maxMatrix);
                break;
            }

            case 5: { 
                displayState(available, need, allocation, maxMatrix);
                break;
            }

            case 6: { 
                cout << "Exiting program." << endl;
                break;
            }

            default: { 
                cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
                break;
            }
        }
    } while (choice != 6); // Changed condition to exit when 6 is entered

    return 0;
}