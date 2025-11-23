#include "rr.hpp"
#include <iostream>
#include <vector>

using namespace std;

void runRoundRobin(vector<Process> &processes, int quantum) {
    int n = (int)processes.size();

    vector<int> bt(n), rt(n);   // burst time, remaining time
    vector<int> waiting(n, 0);  // waiting time
    vector<int> turnaround(n, 0);

    for (int i = 0; i < n; i++) {
        bt[i] = processes[i].burstTime;
        rt[i] = processes[i].burstTime;
    }

    int time = 0;

    cout << "\n=== Round Robin Execution Order ===\n";

    while (true) {
        bool done = true;

        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = false;

                cout << "Time " << time << ": P" << processes[i].pid << " running\n";

                if (rt[i] > quantum) {
                    time += quantum;
                    rt[i] -= quantum;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    turnaround[i] = time;  // completion time since all arrivals assumed at 0
                }
            }
        }

        if (done)
            break;
    }

    // Calculate waiting times
    for (int i = 0; i < n; i++) {
        waiting[i] = turnaround[i] - bt[i];
    }

    // Copy results back to Process objects
    for (int i = 0; i < n; i++) {
        processes[i].completionTime = turnaround[i];
        processes[i].turnaroundTime = turnaround[i];
        processes[i].waitingTime = waiting[i];
        processes[i].startTime = (waiting[i] > 0 ? 0 : 0); // simple model
        processes[i].responseTime = processes[i].startTime - processes[i].arrivalTime;
    }

    // Output results
    double avgW = 0, avgT = 0;

    cout << "\n=== Round Robin Results ===\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << processes[i].pid
             << " | Burst: " << bt[i]
             << " | Waiting: " << waiting[i]
             << " | Turnaround: " << turnaround[i]
             << endl;

        avgW += waiting[i];
        avgT += turnaround[i];
    }

    cout << "\nAverage Waiting Time: " << avgW / n << endl;
    cout << "Average Turnaround Time: " << avgT / n << endl;
}
