#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// --- Process Structure ---
struct Process{
	int pid; //Process ID
	int arrivalTime; // Arrival Time 
	int burstTime; // Burst Time
	int remainingTime;
	int startTime;
	int completionTime;
	
	Process(int p, int arr, int burst){
		pid = p;
		arrivalTime = arr;
		burstTime = burst;
		remainingTime = burst;
		startTime = -1;
		completionTime = -1;
	}
};

// --- SJF Logic ---
void runSJF(vector<Process> &allProcesses){
	
	vector <Process*> readyQueue;
	
	 int currentTime = 0;
	 int completed = 0;
	 int total = allProcesses.size();
	 int cpuBusyTime = 0;
	 int startofSimulation = INT_MAX;
	 
	 for (int i = 0; i < allProcesses.size(); i++){
	 	if (allProcesses[i].arrivalTime < startofSimulation){
	 		startofSimulation = allProcesses[i].arrivalTime;
		 }
	 }
	 
	 currentTime = startofSimulation;
	 
	 while (completed < total){
	 	
	 	// Add arrivals
	 	for (int i = 0; i < allProcesses.size(); i++){
	 		Process &p = allProcesses[i];
	 		
	 		if (p.arrivalTime <= currentTime && p.completionTime == -1){
	 			
	 			bool found = false;
	 			for (int j = 0; j < readyQueue.size(); j++){
	 				if (readyQueue[j] -> pid == p.pid){
	 					found = true;
					 }
				 }
				 
				 if (!found){
				 	readyQueue.push_back(&p);
				 	cout << "Time" << currentTime
				 		<< ": P" << p.pid << " arrived." << endl;
				 }
			 }
		 }
		 
		 // Select shortest remaining job
		 Process *current = NULL;
		 int minBurst = INT_MAX;
		 int minIndex = -1;
		 
		 for (int i = 0; i < readyQueue.size(); i++){
		 	if (readyQueue[1] -> remainingTime < minBurst){
		 		minBurst = readyQueue[i] -> remainingTime;
		 		current = readyQueue[i];
		 		minIndex = i;
			 }
		 }
		 // CPU Idle
		 if (current == NULL){
		 	int nextArrival = INT_MAX;
		 	
		 	for (int i = 0; i < allProcesses.size(); i++){
		 		Process &p = allProcesses[i];
		 		if (p.arrivalTime >= currentTime && p.completionTime == -1 ){
		 			if (p.arrivalTime < nextArrival){
		 				nextArrival = p.arrivalTime;
					 }
				 }
			 }
			 
			cout << "Time" << currentTime
			 	 << ": CPU idle (Jumping to " << nextArrival << ")" << endl;
			currentTime = nextArrival;
			continue;
		 }
		 
		 //Run the selected job
		 if (current -> startTime == -1){
		 	current -> startTime = currentTime;
		 }
		 
		cout << "Time " << currentTime 
		 	 << ": Running P" << current->pid << endl;
		
		int runTime = current -> remainingTime;
		currentTime += runTime;
		current -> remainingTime = 0;
		cpuBusyTime += runTime;
		current -> completionTime = currentTime;
		completed++;
		readyQueue.erase(readyQueue.begin() + minIndex); 
	 }
	 
	 //Metrics
	 int endTime = currentTime;
	 double totalW = 0, totalT = 0, totalR = 0;
	 
	 cout << "\nProcess Summary: \n";
	 
	 for (int i = 0; i < allProcesses.size(); i++){
	 	Process &p = allProcesses[i];
	 	
	 	int turnaround = p.completionTime - p.arrivalTime;
	 	int waiting = turnaround - p.burstTime;
	 	int response = p.startTime - p.arrivalTime;
	 	
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
	 
	cout << "\nAverage Waiting Time: " << totalW / total << endl;
	cout << "\nAverage Turaround Time: " << totalT / total << endl;
	cout << "\nAverage Response Time: " << totalR / total << endl;
	
	int simTime = endTime - startofSimulation;
	cout << "CPU Utilization: " << (cpuBusyTime * 100.0 / simTime) << "%\n"; 
	cout << "Throughput: " << (double)total / simTime << endl;
}

int main(){
	int n;
	cout << "Enter number of processes: ";
	cin >> n;
	
	vector<Process> processes;
	
	for (int i = 0; i < n; i++){
		int arr, burst;
		cout << "Enter arrival and burst time for P" << i + 1 << ":";
		cin >> arr >> burst;
		processes.push_back(Process(i = 1, arr, burst));
	}
	
	runSJF(processes);
	return 0; 
}
