// sjf.cpp
#include "sjf.hpp"
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

void runSJF(vector<Process> &allProcesses) {

    vector<Process*> readyQueue;

    int currentTime = 0;
    int completed = 0;
    int total = (int)allProcesses.size();
    int cpuBusyTime = 0;
    int startofSimulation = INT_MAX;

    for (int i = 0; i < total; i++) {
        if (allProcesses[i].arrivalTime < startofSimulation) {
            startofSimulation = allProcesses[i].arrivalTime;
        }
    }

    currentTime = startofSimulation;

    while (completed < total) {

        // --- Add arrivals ---
        for (int i = 0; i < total; i++) {
            Process &p = allProcesses[i];

            if (p.arrivalTime <= currentTime && p.completionTime == -1) {

                bool found = false;
                for (int j = 0; j < (int)readyQueue.size(); j++) {
                    if (readyQueue[j]->pid == p.pid) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    readyQueue.push_back(&p);
                    cout << "Time " << currentTime
                         << ": P" << p.pid << " arrived." << endl;
                }
            }
        }

        // --- Select shortest job in ready queue ---
        Process *current = nullptr;
        int minBurst = INT_MAX;
        int minIndex = -1;

        for (int i = 0; i < (int)readyQueue.size(); i++) {
            if (readyQueue[i]->remainingTime < minBurst) {
                minBurst = readyQueue[i]->remainingTime;
                current  = readyQueue[i];
                minIndex = i;
            }
        }

        // --- CPU idle: jump to next arrival ---
        if (current == nullptr) {
            int nextArrival = INT_MAX;

            for (int i = 0; i < total; i++) {
                Process &p = allProcesses[i];
                if (p.arrivalTime > currentTime && p.completionTime == -1) {
                    if (p.arrivalTime < nextArrival) {
                        nextArrival = p.arrivalTime;
                    }
                }
            }

            cout << "Time " << currentTime
                 << ": CPU idle (jumping to " << nextArrival << ")\n";
            currentTime = nextArrival;
            continue;
        }

        // --- Run the selected job (non-preemptive) ---
        if (current->startTime == -1) {
            current->startTime = currentTime;
        }

        cout << "Time " << currentTime
             << ": Running P" << current->pid << endl;

        int runTime = current->remainingTime;
        currentTime += runTime;
        current->remainingTime = 0;
        cpuBusyTime += runTime;
        current->completionTime = currentTime;
        completed++;

        // Remove finished job from ready queue
        readyQueue.erase(readyQueue.begin() + minIndex);
    }

    // --- Metrics ---
    int endTime = currentTime;
    double totalW = 0, totalT = 0, totalR = 0;

    cout << "\nProcess Summary (SJF):\n";

    for (int i = 0; i < total; i++) {
        Process &p = allProcesses[i];

        int turnaround = p.completionTime - p.arrivalTime;
        int waiting    = turnaround - p.burstTime;
        int response   = p.startTime - p.arrivalTime;

        totalT += turnaround;
        totalW += waiting;
        totalR += response;

        cout << "P" << p.pid
             << " | Arrival: " << p.arrivalTime
             << " | Burst: " << p.burstTime
             << " | Start: " << p.startTime
             << " | Completion: " << p.completionTime
             << " | Waiting: " << waiting
             << " | Turnaround: " << turnaround
             << " | Response: " << response
             << endl;
    }

    cout << "\nAverage Waiting Time: "   << totalW / total << endl;
    cout << "Average Turnaround Time: " << totalT / total << endl;
    cout << "Average Response Time: "   << totalR / total << endl;

    int simTime = endTime - startofSimulation;
    cout << "CPU Utilization: " << (cpuBusyTime * 100.0 / simTime) << "%\n";
    cout << "Throughput: "      << (double)total / simTime << endl;
}
