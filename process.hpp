#ifndef PROCESS_HPP
#define PROCESS_HPP

struct Process {
    int pid;              // Process ID
    int arrivalTime;      // Arrival time of the process
    int burstTime;        // Original burst time
    int remainingTime;    // Remaining time during execution

    int priority;         // For Priority Scheduling (lower = higher priority)
    bool highPriority;    // For MLQ: true = high queue (RR), false = low queue
    bool addedToQueue;    // For MLQ: has this process been enqueued yet?

    int startTime;        // First time the process gets CPU
    int completionTime;   // Time when process finishes
    int waitingTime;      // Time spent waiting in the ready queue
    int turnaroundTime;   // Total time from arrival to completion
    int responseTime;     // Time from arrival to first CPU allocation

    // Constructor for hard-coded and file input
    Process(int pid, int arrival, int burst, int remaining, int priority, bool highFlag)
        : pid(pid), arrivalTime(arrival), burstTime(burst), remainingTime(remaining),
          priority(priority), highPriority(highFlag), addedToQueue(false),
          startTime(-1), completionTime(-1), waitingTime(0),
          turnaroundTime(0), responseTime(0) {}

    // Default constructor for manual input
    Process()
        : pid(0), arrivalTime(0), burstTime(0), remainingTime(0),
          priority(0), highPriority(false), addedToQueue(false),
          startTime(-1), completionTime(-1), waitingTime(0),
          turnaroundTime(0), responseTime(0) {}

    // Reset metrics so the same process set can be reused
    void reset() {
        remainingTime = burstTime;
        addedToQueue = false;
        startTime = -1;
        completionTime = -1;
        waitingTime = 0;
        turnaroundTime = 0;
        responseTime = 0;
    }

    // Metric helpers
    int turnaround() const { return completionTime - arrivalTime; }
    int waiting() const { return turnaround() - burstTime; }
    int response() const { return startTime - arrivalTime; }
};

#endif
