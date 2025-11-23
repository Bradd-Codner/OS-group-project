#include "mlq.hpp"
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// helper: add any processes that arrive at this exact time
static void addArrivals(vector<Process> &allProcesses,
                        int currentTime,
                        vector<Process*> &highQueue,
                        vector<Process*> &lowQueue)
{
    for (int i = 0; i < (int)allProcesses.size(); i++) {
        Process &p = allProcesses[i];

        if (!p.addedToQueue &&
            p.arrivalTime == currentTime &&
            p.remainingTime > 0) {

            if (p.highPriority) {
                highQueue.push_back(&p);
                cout << "Time " << currentTime
                     << ": P" << p.pid << " arrived (High Queue)" << endl;
            } else {
                lowQueue.push_back(&p);
                cout << "Time " << currentTime
                     << ": P" << p.pid << " arrived (Low Queue)" << endl;
            }
            p.addedToQueue = true; // make sure we never add it again
        }
    }
}

void runMLQ(vector<Process> &allProcesses, int timeQuantumHigh)
{
    vector<Process*> highQueue; // Round Robin queue (pointers to processes)
    vector<Process*> lowQueue;  // FCFS queue (pointers to processes)

    int currentTime = 0;
    int completed = 0;
    int total = (int)allProcesses.size();
    int cpuBusyTime = 0;

    // Start at earliest arrival
    int startOfSimulation = INT_MAX;
    for (int i = 0; i < total; i++) {
        if (allProcesses[i].arrivalTime < startOfSimulation) {
            startOfSimulation = allProcesses[i].arrivalTime;
        }
    }
    currentTime = startOfSimulation;

    // first arrivals at start time
    addArrivals(allProcesses, currentTime, highQueue, lowQueue);

    while (completed < total) {

        // 1. Choose queue
        Process *current = nullptr;
        bool fromHigh = false;

        if (!highQueue.empty()) {
            current = highQueue[0];
            fromHigh = true;
        } else if (!lowQueue.empty()) {
            current = lowQueue[0];
        }

        // 2. CPU idle
        if (current == nullptr) {
            cout << "Time " << currentTime << ": CPU idle" << endl;
            currentTime++;
            addArrivals(allProcesses, currentTime, highQueue, lowQueue);
            continue;
        }

        // 3. Set start time (response time)
        if (current->startTime == -1) {
            current->startTime = currentTime;
        }

        // 4. HIGH QUEUE (RR)
        if (fromHigh) {
            int run = 0;
            int slice = timeQuantumHigh;

            while (run < slice && current->remainingTime > 0) {
                cout << "Time " << currentTime
                     << ": Running P" << current->pid << " (High Queue)" << endl;

                current->remainingTime--;
                currentTime++;
                cpuBusyTime++;
                run++;

                // check for new arrivals at this new time
                addArrivals(allProcesses, currentTime, highQueue, lowQueue);
            }

            if (current->remainingTime == 0) {
                current->completionTime = currentTime;
                completed++;

                // remove from front of highQueue
                highQueue.erase(highQueue.begin());
            } else {
                // move this process pointer to the end of highQueue
                highQueue.erase(highQueue.begin());
                highQueue.push_back(current);
            }
        }

        // 5. LOW QUEUE (FCFS)
        else {
            cout << "Time " << currentTime
                 << ": Running P" << current->pid << " (Low Queue)" << endl;

            while (current->remainingTime > 0) {
                current->remainingTime--;
                currentTime++;
                cpuBusyTime++;

                // check for new arrivals at this new time
                addArrivals(allProcesses, currentTime, highQueue, lowQueue);
            }

            current->completionTime = currentTime;
            completed++;

            // remove from front of lowQueue
            lowQueue.erase(lowQueue.begin());
        }
    }

    int endOfSimulation = currentTime;

    // 6. Calculate metrics
    double totalWaiting = 0.0;
    double totalTurnaround = 0.0;
    double totalResponse = 0.0;

    cout << "\nProcess Summary (MLQ):\n";
    for (int i = 0; i < total; i++) {
        Process &p = allProcesses[i];

        int turnaround = p.completionTime - p.arrivalTime;
        int waiting = turnaround - p.burstTime;
        int response = p.startTime - p.arrivalTime;

        p.turnaroundTime = turnaround;
        p.waitingTime = waiting;
        p.responseTime = response;

        totalTurnaround += turnaround;
        totalWaiting += waiting;
        totalResponse += response;

        cout << "P" << p.pid
             << " | Arrival: " << p.arrivalTime
             << " | Burst: " << p.burstTime
             << " | High? " << (p.highPriority ? "true" : "false")
             << " | Start: " << p.startTime
             << " | Completion: " << p.completionTime
             << " | Waiting: " << waiting
             << " | Turnaround: " << turnaround
             << " | Response: " << response
             << endl;
    }

    double avgW = totalWaiting / total;
    double avgT = totalTurnaround / total;
    double avgR = totalResponse / total;

    int simTime = endOfSimulation - startOfSimulation;
    double utilization = (cpuBusyTime * 100.0) / simTime;
    double throughput = (double)total / simTime;

    cout << "\n[MLQ Metrics]\n";
    cout << "Average Waiting Time: " << avgW << endl;
    cout << "Average Turnaround Time: " << avgT << endl;
    cout << "Average Response Time: " << avgR << endl;
    cout << "CPU Utilization: " << utilization << "%" << endl;
    cout << "Throughput: " << throughput << " processes per time unit" << endl;
}
