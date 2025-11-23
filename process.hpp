#ifndef PROCESS_HPP
#define PROCESS_HPP

struct Process {
    int pid;               // process id P1, P2, ...
    int arrivalTime;       // when it enters the system
    int burstTime;         // original CPU burst
    int remainingTime;     // remaining CPU burst

    int priority;          // used in priority scheduling (lower = higher priority)

    bool inForeground;     // used by MLQ: true = high queue, false = low queue
    bool hasEnteredQueue;  // internal flag for MLQ to avoid double-enqueue

    int startTime;         // first time it gets CPU
    int completionTime;    // time when it finishes

    Process(int id = 0,
            int arrival = 0,
            int burst = 0,
            int prio = 0,
            bool foreground = false)
    {
        pid             = id;
        arrivalTime     = arrival;
        burstTime       = burst;
        remainingTime   = burst;
        priority        = prio;
        inForeground    = foreground;
        hasEnteredQueue = false;
        startTime       = -1;
        completionTime  = -1;
    }
};

#endif
