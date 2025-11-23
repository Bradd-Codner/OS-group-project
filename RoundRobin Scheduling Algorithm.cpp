#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, quantum;

    cout << "Enter number of processes: ";
    cin >> n;

    vector<int> bt(n), rt(n);   // burst time, remaining time
    vector<int> waiting(n, 0);  // waiting time
    vector<int> turnaround(n, 0);

    for (int i = 0; i < n; i++) {
        cout << "Enter Burst Time for P" << i+1 << ": ";
        cin >> bt[i];
        rt[i] = bt[i];
    }

    cout << "Enter Time Quantum: ";
    cin >> quantum;

    int time = 0;

    cout << "\n=== Round Robin Execution Order ===\n";

    while (true) {
        bool done = true;

        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = false;

                cout << "Time " << time << ": P" << i+1 << " running\n";

                if (rt[i] > quantum) {
                    time += quantum;
                    rt[i] -= quantum;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    turnaround[i] = time;
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

    // Output results
    double avgW = 0, avgT = 0;

    cout << "\n=== Results ===\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i+1
             << " | Burst: " << bt[i]
             << " | Waiting: " << waiting[i]
             << " | Turnaround: " << turnaround[i]
             << endl;

        avgW += waiting[i];
        avgT += turnaround[i];
    }

    cout << "\nAverage Waiting Time: " << avgW / n;
    cout << "\nAverage Turnaround Time: " << avgT / n << endl;

    return 0;
}
