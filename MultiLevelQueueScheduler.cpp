#include <iostream>
#include <vector>
#include <climits>  // for INT_MAX

using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;

    bool highPriority; // true = high queue (RR), false = low queue (FCFS)

    int startTime;      // first time it gets CPU
    int completionTime; // time when it finishes

    Process(int p, int arr, int burst, bool high)
    {
        pid = p;
        arrivalTime = arr;
        burstTime = burst;
        remainingTime = burst;
        highPriority = high;
        startTime = -1;
        completionTime = -1;
    }
};

void runMLQ(vector<Process> &allProcesses, int timeQuantumHigh) {

    vector<Process*> highQueue; // Round Robin queue (store pointers)
    vector<Process*> lowQueue;  // FCFS queue (store pointers)

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

    while (completed < total) {

        // 1. Add arrivals
        for (int i = 0; i < total; i++) {
            Process &p = allProcesses[i];

            if (p.arrivalTime == currentTime && p.remainingTime > 0) {

                if (p.highPriority) {
                    highQueue.push_back(&p);
                    cout << "Time " << currentTime
                         << ": P" << p.pid << " arrived (High Queue)" << endl;
                } else {
                    lowQueue.push_back(&p);
                    cout << "Time " << currentTime
                         << ": P" << p.pid << " arrived (Low Queue)" << endl;
                }
            }
        }

        // 2. Choose queue
        Process *current = nullptr;
        bool fromHigh = false;

        if (!highQueue.empty()) {
            current = highQueue[0];
            fromHigh = true;
        } else if (!lowQueue.empty()) {
            current = lowQueue[0];
        }

        // 3. CPU idle
        if (current == nullptr) {
            cout << "Time " << currentTime << ": CPU idle" << endl;
            currentTime++;
            continue;
        }

        // 4. Set start time (response time)
        if (current->startTime == -1) {
            current->startTime = currentTime;
        }

        // 5. HIGH QUEUE (RR)
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

                // handle arrivals during RR
                for (int i = 0; i < total; i++) {
                    Process &p = allProcesses[i];
                    if (p.arrivalTime == currentTime && p.remainingTime > 0) {
                        if (p.highPriority) {
                            highQueue.push_back(&p);
                        } else {
                            lowQueue.push_back(&p);
                        }
                    }
                }
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

        // 6. LOW QUEUE (FCFS)
        else {
            cout << "Time " << currentTime
                 << ": Running P" << current->pid << " (Low Queue)" << endl;

            while (current->remainingTime > 0) {
                current->remainingTime--;
                currentTime++;
                cpuBusyTime++;

                // arrivals during FCFS
                for (int i = 0; i < total; i++) {
                    Process &p = allProcesses[i];
                    if (p.arrivalTime == currentTime && p.remainingTime > 0) {
                        if (p.highPriority) {
                            highQueue.push_back(&p);
                        } else {
                            lowQueue.push_back(&p);
                        }
                    }
                }
            }

            current->completionTime = currentTime;
            completed++;

            // remove from front of lowQueue
            lowQueue.erase(lowQueue.begin());
        }
    }

    int endOfSimulation = currentTime;

    // 7. Calculate metrics
    double totalWaiting = 0.0;
    double totalTurnaround = 0.0;
    double totalResponse = 0.0;

    cout << "\nProcess Summary:\n";
    for (int i = 0; i < total; i++) {
        Process &p = allProcesses[i];

        int turnaround = p.completionTime - p.arrivalTime;
        int waiting = turnaround - p.burstTime;
        int response = p.startTime - p.arrivalTime;

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

    cout << "\nAverage Waiting Time: " << avgW << endl;
    cout << "Average Turnaround Time: " << avgT << endl;
    cout << "Average Response Time: " << avgR << endl;
    cout << "CPU Utilization: " << utilization << "%" << endl;
    cout << "Throughput: " << throughput << " processes per time unit" << endl;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;

    for (int i = 0; i < n; i++) {
        int arr, burst;
        int highFlag;

        cout << "Enter arrival, burst, highPriorityFlag (1 for high, 0 for low): ";
        cin >> arr >> burst >> highFlag;

        bool high = (highFlag == 1);
        processes.push_back(Process(i + 1, arr, burst, high));
    }

    int q;
    cout << "Enter quantum for high queue: ";
    cin >> q;

    runMLQ(processes, q);

    return 0;
}
