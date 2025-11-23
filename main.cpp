#include <iostream>
#include <vector>
#include "process.hpp"
#include "fcfs.hpp"
#include "sjf.hpp"
#include "rr.hpp"
#include "priority.hpp"
#include "mlq.hpp"

using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> baseProcesses;
    baseProcesses.reserve(n);

    cout << "\nEnter process details:\n";
    cout << "(pid will be P1, P2, ... in order of input)\n";

    for (int i = 0; i < n; i++) {
        Process p;
        p.pid = i + 1;

        cout << "\nProcess P" << p.pid << ":\n";
        cout << "  Arrival time: ";
        cin >> p.arrivalTime;
        cout << "  Burst time: ";
        cin >> p.burstTime;
        p.remainingTime = p.burstTime;

        cout << "  Priority (for Preemptive Priority, lower = higher): ";
        cin >> p.priority;

        cout << "  High priority for MLQ? (1 = yes, 0 = no): ";
        int highFlag;
        cin >> highFlag;
        p.highPriority = (highFlag == 1);

        // other fields use default values
        baseProcesses.push_back(p);
    }

    int choice = -1;
    while (choice != 0) {
        cout << "\n===== CPU Scheduling Menu =====\n";
        cout << "1. FCFS\n";
        cout << "2. Shortest Job First (SJF)\n";
        cout << "3. Round Robin\n";
        cout << "4. Preemptive Priority\n";
        cout << "5. Multi-Level Queue (MLQ)\n";
        cout << "6. Run all algorithms\n";
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }

        if (choice < 0 || choice > 6) {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        // fresh copy each time so runs don't affect each other
        vector<Process> procs = baseProcesses;

        switch (choice) {
            case 1: {
                runFCFS(procs);
                break;
            }
            case 2: {
                runSJF(procs);
                break;
            }
            case 3: {
                int q;
                cout << "Enter time quantum for Round Robin: ";
                cin >> q;
                runRoundRobin(procs, q);
                break;
            }
            case 4: {
                runPreemptivePriorityScheduling(procs);
                break;
            }
            case 5: {
                int q;
                cout << "Enter time quantum for high-priority queue (MLQ): ";
                cin >> q;
                runMLQ(procs, q);
                break;
            }
            case 6: {
                int qRR, qMLQ;
                cout << "\nEnter time quantum for Round Robin: ";
                cin >> qRR;
                cout << "Enter time quantum for MLQ high-priority queue: ";
                cin >> qMLQ;

                {
                    vector<Process> p1 = baseProcesses;
                    runFCFS(p1);
                }
                {
                    vector<Process> p2 = baseProcesses;
                    runSJF(p2);
                }
                {
                    vector<Process> p3 = baseProcesses;
                    runRoundRobin(p3, qRR);
                }
                {
                    vector<Process> p4 = baseProcesses;
                    runPreemptivePriorityScheduling(p4);
                }
                {
                    vector<Process> p5 = baseProcesses;
                    runMLQ(p5, qMLQ);
                }
                break;
            }
        }
    }

    return 0;
}
