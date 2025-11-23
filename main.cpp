#include <iostream>
#include <vector>
#include <limits>
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
        if (cin >> val && val >= minVal) {
            return val;
        }
        cout << "Invalid input. Please enter an integer >= " << minVal << ".\n";
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

    for (int i = 0; i < n; i++) {
        Process p;
        p.pid = i + 1;

        cout << "\nProcess P" << p.pid << ":\n";
        p.arrivalTime = readInt("  Arrival time: ", 0);
        p.burstTime   = readInt("  Burst time: ", 1);
        p.remainingTime = p.burstTime;
        p.priority    = readInt("  Priority (lower = higher): ", 0);

        int highFlag = readInt("  High priority for MLQ? (1=yes, 0=no): ", 0);
        p.highPriority = (highFlag == 1);

        procs.push_back(p);
    }
    return procs;
}

// Helper: reset processes before each run
void resetProcesses(vector<Process> &procs) {
    for (auto &p : procs) p.reset();
}

int main() {
    vector<Process> baseProcesses = inputProcesses();

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

        if (choice == 6) {
            baseProcesses = inputProcesses();
            continue;
        }

        vector<Process> procs = baseProcesses;
        resetProcesses(procs);

        switch (choice) {
            case 1:
                cout << "\n==============================\n";
                cout << "Algorithm: SJF\n";
                cout << "==============================\n";
                runSJF(procs);
                break;

            case 2: {
                int q = readInt("Enter time quantum for Round Robin: ", 1);
                cout << "\n==============================\n";
                cout << "Algorithm: Round Robin\n";
                cout << "==============================\n";
                runRoundRobin(procs, q);
                break;
            }

            case 3:
                cout << "\n==============================\n";
                cout << "Algorithm: Preemptive Priority\n";
                cout << "==============================\n";
                runPreemptivePriorityScheduling(procs);
                break;
 
            case 4: {
                int q = readInt("Enter time quantum for MLQ high-priority queue: ", 1);
                cout << "\n==============================\n";
                cout << "Algorithm: Multi-Level Queue\n";
                cout << "==============================\n";
                runMLQ(procs, q);
                break;
            }

            case 5: {
                int qRR  = readInt("Enter RR quantum: ", 1);
                int qMLQ = readInt("Enter MLQ quantum: ", 1);

                cout << "\n==============================\n";
                cout << "Algorithm: SJF\n";
                cout << "==============================\n";
                resetProcesses(procs);
                runSJF(procs);

                cout << "\n==============================\n";
                cout << "Algorithm: Round Robin\n";
                cout << "==============================\n";
                resetProcesses(procs);
                runRoundRobin(procs, qRR);

                cout << "\n==============================\n";
                cout << "Algorithm: Preemptive Priority\n";
                cout << "==============================\n";
                resetProcesses(procs);
                runPreemptivePriorityScheduling(procs);

                cout << "\n==============================\n";
                cout << "Algorithm: Multi-Level Queue\n";
                cout << "==============================\n";
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
