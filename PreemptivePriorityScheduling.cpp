#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Process {
    int pid;              // Process ID
    int arrivalTime;      // Arrival time of the process
    int burstTime;        // Burst time of the process
    int remainingTime;    // Remaining time for the process
    int priority;         // Priority (lower value means higher priority)
    
    int startTime;        // First time the process gets CPU
    int completionTime;   // Time when process finishes
    int waitingTime;      // Time the process spends waiting in the ready queue
    int turnaroundTime;   // Total time from arrival to completion
    int responseTime;     // Time from arrival to first CPU allocation

    Process(int p, int arr, int burst, int prio)
    {
        pid = p;
        arrivalTime = arr;
        burstTime = burst;
        remainingTime = burst;
        priority = prio;
        startTime = -1;
        completionTime = -1;
        waitingTime = 0;
        turnaroundTime = 0;
        responseTime = 0;
    }
};

// Comparison function to sort processes by arrival time, then by priority
bool compareArrival(Process &a, Process &b) {
    if (a.arrivalTime == b.arrivalTime) {
        return a.priority < b.priority; 
    }
    return a.arrivalTime < b.arrivalTime;
}

void runPreemptivePriorityScheduling(vector<Process> &allProcesses) {
    int currentTime = 0;
    int completed = 0;
    int total = allProcesses.size();
    int cpuBusyTime = 0;

    // Sort processes by arrival time
    sort(allProcesses.begin(), allProcesses.end(), compareArrival);

    vector<Process*> readyQueue; // Ready queue to hold processes
    int lastCompletionTime = 0;
    
    while (completed < total) {
        // Adding all processes that have arrived at the current time to the ready queue
        for (int i = 0; i < total; i++) {
            if (allProcesses[i].arrivalTime == currentTime && allProcesses[i].remainingTime > 0) {
                readyQueue.push_back(&allProcesses[i]);
                cout << "Time " << currentTime << ": P" << allProcesses[i].pid << " arrived." << endl;
            }
        }

        // If the ready queue is empty then the CPU is idled
        if (readyQueue.empty()) {
            cout << "Time " << currentTime << ": CPU idle" << endl;
            currentTime++;
            continue;
        }

        // Sorting ready queue by priority (lowest priority value comes first)
        sort(readyQueue.begin(), readyQueue.end(), [](Process* a, Process* b) {
            return a->priority < b->priority;
        });

        // Selecting the process with the highest priority (lowest priority value)
        Process* current = readyQueue[0];

        // Seting start time (if not already set)
        if (current->startTime == -1) {
            current->startTime = currentTime;
        }

        // Running the selected process
        cout << "Time " << currentTime << ": Running P" << current->pid << " (Priority " << current->priority << ")" << endl;
        current->remainingTime--;
        cpuBusyTime++;
        currentTime++;

        
        if (current->remainingTime == 0) {
            current->completionTime = currentTime;
            current->turnaroundTime = current->completionTime - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            current->responseTime = current->startTime - current->arrivalTime;
            completed++;
            readyQueue.erase(readyQueue.begin()); // Removing process from ready queue
        }
    }

    // Calculating and displays performance metrics
    double totalWaiting = 0.0;
    double totalTurnaround = 0.0;
    double totalResponse = 0.0;

    cout << "\nProcess Summary:\n";
    for (int i = 0; i < total; i++) {
        Process &p = allProcesses[i];

        totalTurnaround += p.turnaroundTime;
        totalWaiting += p.waitingTime;
        totalResponse += p.responseTime;

        cout << "P" << p.pid
             << " | Arrival: " << p.arrivalTime
             << " | Burst: " << p.burstTime
             << " | Priority: " << p.priority
             << " | Start: " << p.startTime
             << " | Completion: " << p.completionTime
             << " | Waiting: " << p.waitingTime
             << " | Turnaround: " << p.turnaroundTime
             << " | Response: " << p.responseTime
             << endl;
    }

    double avgW = totalWaiting / total;
    double avgT = totalTurnaround / total;
    double avgR = totalResponse / total;

    double utilization = (cpuBusyTime * 100.0) / currentTime;
    double throughput = (double)total / currentTime;

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
        int arr, burst, prio;
        cout << "Enter arrival time, burst time, and priority (lower value means higher priority): ";
        cin >> arr >> burst >> prio;
        processes.push_back(Process(i + 1, arr, burst, prio));
    }

    runPreemptivePriorityScheduling(processes);

    return 0;
}