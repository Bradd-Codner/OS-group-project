#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include "process.hpp"
#include "sjf.hpp"
#include "rr.hpp"
#include "priority.hpp"
#include "mlq.hpp"

using namespace std;


// Helper: input validation
int readInt(const string &prompt, int minVal = 0) {
    int val;
    while (true) {
        cout << prompt;
        //Ensure valid integer input
        if (cin >> val && val >= minVal) {
            return val;
        }
        cout << "Invalid input. Please enter an integer >= " << minVal << ".\n";
        // Clear error state and ignore rest of line
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Helper: enter processes
vector<Process> inputProcesses() {
    int n = readInt("Enter number of processes: ", 1);
    vector<Process> procs;
    procs.reserve(n);

    cout << "\nEnter process details:\n";
    cout << "(pid will be P1, P2, ... in order of input)\n";
    //Allow user to input each process data
    for (int i = 0; i < n; i++) {
        Process p;
        p.pid = i + 1;

        cout << "\nProcess P" << p.pid << ":\n";
        p.arrivalTime   = readInt("  Arrival time: ", 0);
        p.burstTime     = readInt("  Burst time: ", 1);
        p.remainingTime = p.burstTime;
        p.priority      = readInt("  Priority (lower = higher): ", 0);

        int highFlag    = readInt("  High priority for MLQ? (1=yes, 0=no): ", 0);
        p.highPriority  = (highFlag == 1);

        procs.push_back(p);
    }
    return procs;
}

// Hard-coded demo process data 
vector<Process> demoProcesses() {
    vector<Process> procs;
    procs.push_back({1, 0, 5, 5, 2, true});
    procs.push_back({2, 1, 3, 3, 1, true});
    procs.push_back({3, 2, 8, 8, 3, false});
    procs.push_back({4, 3, 6, 6, 4, false});
    return procs;
}

// Load processes from file
vector<Process> loadProcessesFromFile(const string &filename) {
    ifstream in(filename);
    vector<Process> procs;
    // Check if file opened successfully
    if (!in) {
        cout << "Could not open file '" << filename << "'. Falling back to manual input.\n";
        //Return manual input if file fails to open
        return inputProcesses();
    }

    int n;
    // Read number of processes
    if (!(in >> n) || n <= 0) {
        cout << "Invalid file format. Falling back to manual input.\n";
        //Return manual input if first line on file is not formatted correctly
        return inputProcesses();
    }
    // Read each process line
    procs.reserve(n);
    for (int i = 0; i < n; i++) {
        Process p;
        int highFlag;
        // Each line: arrivalTime burstTime priority highPriorityFlag   
        if (!(in >> p.arrivalTime >> p.burstTime >> p.priority >> highFlag)) {
            // Handle invalid lines
            cout << "Not enough valid process lines in file. Read " << i << " processes.\n";
            break;
        }
        p.pid = i + 1;
        p.remainingTime = p.burstTime;
        p.highPriority = (highFlag == 1);
        procs.push_back(p);
    }
    // If no valid processes were read, fallback to manual input
    if (procs.empty()) {
        cout << "No valid processes loaded. Falling back to manual input.\n";
        return inputProcesses();
    }
    return procs;
}

// Reset processes
void resetProcesses(vector<Process> &procs) {
    for (auto &p : procs) p.reset();
}

// Unified input mode selection with Hard coded and file input
vector<Process> selectInputMode() {
    cout << "\nSelect input mode:\n";
    cout << " 1. Hard-coded demo data\n";
    cout << " 2. Manual input\n";
    cout << " 3. Load from file\n";

    int mode;
    while (true) {
        mode = readInt("Choice (1–3): ", 1);
        if (mode >= 1 && mode <= 3) break;
        cout << "Please enter 1, 2, or 3.\n";
    }

    if (mode == 1) {
        cout << "\nUsing hard-coded demo processes.\n";
        return demoProcesses();
    } else if (mode == 2) {
        cout << "\nUsing manual input.\n";
        return inputProcesses();
    } else {
        string filename;
        cout << "Enter input file name (e.g. processes.txt): ";
        cin >> filename;
        return loadProcessesFromFile(filename);
    }
}

// Main
int main() {
    // Initial process input
    vector<Process> baseProcesses = selectInputMode();

    int choice = -1;
    while (choice != 0) {
        cout << "\n===== CPU Scheduling Menu =====\n";
        cout << "1. Shortest Job First (SJF)\n";
        cout << "2. Round Robin\n";
        cout << "3. Preemptive Priority\n";
        cout << "4. Multi-Level Queue (MLQ)\n";
        cout << "5. Run all algorithms\n";
        cout << "6. Re-enter processes\n";
        cout << "0. Exit\n";

        choice = readInt("Select an option: ", 0);

        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }
        //If you want to use the input mode selection again
        if (choice == 6) {
            baseProcesses = selectInputMode();
            continue;
        }

        vector<Process> procs = baseProcesses;
        resetProcesses(procs);
        // Execute selected algorithm
        switch (choice) {
            //SJF selection
            case 1:
                cout << "\nAlgorithm: SJF\n";
                runSJF(procs);
                break;
            //Round Robin selection
            case 2: {
                int q = readInt("Enter time quantum for Round Robin: ", 1);
                cout << "\nAlgorithm: Round Robin\n";
                runRoundRobin(procs, q);
                break;
            }
            //Preemptive Priority selection
            case 3:
                cout << "\nAlgorithm: Preemptive Priority\n";
                runPreemptivePriorityScheduling(procs);
                break;
            //MLQ selection
            case 4: {
                int q = readInt("Enter time quantum for MLQ high-priority queue: ", 1);
                cout << "\nAlgorithm: Multi-Level Queue\n";
                runMLQ(procs, q);
                break;
            }
            //Run all algorithms Simultaaneously selection
            case 5: {
                int qRR  = readInt("Enter RR quantum: ", 1);
                int qMLQ = readInt("Enter MLQ quantum: ", 1);

                cout << "\nAlgorithm: SJF\n";
                resetProcesses(procs);
                runSJF(procs);

                cout << "\nAlgorithm: Round Robin\n";
                resetProcesses(procs);
                runRoundRobin(procs, qRR);

                cout << "\nAlgorithm: Preemptive Priority\n";
                resetProcesses(procs);
                runPreemptivePriorityScheduling(procs);

                cout << "\nAlgorithm: Multi-Level Queue\n";
                resetProcesses(procs);
                runMLQ(procs, qMLQ);
                break;
            }

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
