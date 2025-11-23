#ifndef PROCESS_HPP
#define PROCESS_HPP

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;

    bool highPriority;    // used by MLQ: true = high queue, false = low queue
    bool addedToQueue;    // used by MLQ to avoid double-enqueue

    int startTime;        // first time it gets CPU
    int completionTime;   // time when it finishes

    Process(int p = 0,
            int arr = 0,
            int burst = 0,
            bool high = false)
    {
        pid           = p;
        arrivalTime   = arr;
        burstTime     = burst;
        remainingTime = burst;
        highPriority  = high;
        addedToQueue  = false;
        startTime     = -1;
        completionTime= -1;
    }
};

#endif
