#include "fcfs.hpp"
#include <iostream>
#include <vector>

using namespace std;

// First Come First Served Scheduling
void runFCFS(vector<Process> &procs)
{
    cout << "\n===== Running FCFS Scheduling =====\n";

    int n = procs.size();
    int currentTime = 0;
    int cpuBusyTime = 0;

    // Sort by arrival time (stable)
    sort(procs.begin(), procs.end(),
         [](const Process &a, const Process &b) {
             return a.arrivalTime < b.arrivalTime;
         });

    // Start at earliest arrival
    int simulationStart = procs.front().arrivalTime;
    currentTime = simulationStart;

    cout << "\n=== FCFS Execution Order ===\n";

    for (int i = 0; i < n; i++) {
        Process &p = procs[i];

        // CPU idle until process arrives
        if (currentTime < p.arrivalTime) {
            cout << "Time " << currentTime << ": CPU idle\n";
            currentTime = p.arrivalTime;
        }

        // First time the process runs
        p.startTime = currentTime;

        cout << "Time " << currentTime
             << ": Running P" << p.pid << endl;

        // Run until completion
        currentTime += p.burstTime;
        cpuBusyTime += p.burstTime;

        p.completionTime = currentTime;

        // Metrics
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        p.responseTime = p.startTime - p.arrivalTime;
    }

    // Final metrics
    double totalW = 0, totalT = 0, totalR = 0;

    cout << "\n=== FCFS Process Summary ===\n";
    for (auto &p : procs) {
        totalW += p.waitingTime;
        totalT += p.turnaroundTime;
        totalR += p.responseTime;

        cout << "P" << p.pid
             << " | Arrival: " << p.arrivalTime
             << " | Burst: " << p.burstTime
             << " | Start: " << p.startTime
             << " | Completion: " << p.completionTime
             << " | Waiting: " << p.waitingTime
             << " | Turnaround: " << p.turnaroundTime
             << " | Response: " << p.responseTime
             << endl;
    }

    cout << "\n[FCFS Metrics]\n";
    cout << "Average Waiting Time: " << totalW / n << endl;
    cout << "Average Turnaround Time: " << totalT / n << endl;
    cout << "Average Response Time: " << totalR / n << endl;

    int simDuration = currentTime - simulationStart;
    double utilization = (cpuBusyTime * 100.0) / simDuration;
    double throughput = (double)n / simDuration;

    cout << "CPU Utilization: " << utilization << "%\n";
    cout << "Throughput: " << throughput << " processes/time unit\n";
}
