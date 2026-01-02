#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESS 25

typedef struct{
  int pid;
  int arrival_time;
  int burst_time;
  int remaining_time;
  int start_time;
  int completion_time;
  int waiting_time;
  int turnaround_time;
  int response_time;
  bool is_completed;
} Process;

void printGanttChart(int pids[], int times[], int count){
  printf("\n\n-- Gantt Chart ---\n");

  printf(" ");
  
  for(int i = 0; i < count; i++){
    printf("\n|");
  }

  for(int i = 0; i < count; i++){
    printf("  P%d  |", pids[i]);
  }
  printf("\n ");

  for(int i = 0; i < count; i++) printf("------");
  printf("\n");

  printf("%d", times[0]);
  for(int i = 0; i < count; i++){
  	printf("      %d", times[i + 1]);
  }
  printf("\n");
}

void printTable(Process p[], int n){
	printf("-------------------------------------------------------------------------------------------\n");
	printf("|  Process  |  Arrival  |  Burst  |  Completion  |  Turaround  |  Waiting  |  Response  |\n");
	printf("-------------------------------------------------------------------------------------------\n");

	float avg_tat =  0, avg_wt = 0, avg_rt = 0;

	for(int i = 0; i < n; i++){
	  printf("|  P%d  |  %2d  |  %2d  |  %2d  |  %2d  |  %2d  |  %2d  |\n",
	    p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].completion_time, 
	    p[i].turnaround_time, p[i].waiting_time, p[i].response_time);
	  avg_tat += p[i].turnaround_time;
	  avg_wt += p[i].waiting_time;
	  avg_rt += p[i].response_time;
	}
	
	printf("------------------------------------------------------------\n");
	printf("Average Turn Around time: %.2f\n", avg_tat / n);
	printf("Average Waiting Time: %.2f\n", avg_wt / n);
	printf("Average Response Time: %.2f", avg_rt / n);
}

void calculateFCFS(Process p[], int n){
  int current_time = 0;
  int gantt_pid[MAX_PROCESS];
  int gantt_times[MAX_PROCESS + 1];
  int gantt_index = 0;

  for(int i = 0; i < n; i++){
  	if(current_time < p[i].arrival_time){
  	  current_time = p[i].arrival_time;
  	}
  	p[i].start_time = current_time;
  	p[i].response_time = p[i].start_time - p[i].arrival_time;

    current_time += p[i].burst_time;

    p[i].completion_time = current_time;
    p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

    gantt_pid[gantt_index] = p[i].pid;
    gantt_times[gantt_index + 1] = current_time;
    gantt_index++;
  }
  
  printGanttChart(gantt_pid, gantt_times, gantt_index);
  printTable(p, n);
}

void CalculateRoundRobin(Process p[], int n, int quant){
  int current_time = 0;
  int completed = 0;
  int queue[MAX_PROCESS * 10];
  int front = 0, rear = 0;
  bool in_queue[MAX_PROCESS] = {false};

  int gantt_pid[200];
  int gantt_times[201];
  int gantt_index = 0;
  gantt_times[0] = 0;

  if(n > 0 && p[0].arrival_time <= current_time){
    queue[rear++] = 0;
    in_queue[0] = true;
  }

  while(completed != n){
    if(front == rear){
      int next_arrival_idx = -1;
      int min_arrival = 99999;
      for(int i = 0; i < n; i++){
        if(!p[i].is_completed && !in_queue[i] && p[i].arrival_time < min_arrival){
          min_arrival = p[i].arrival_time;
          next_arrival_idx = i;
        }
      }
      if(next_arrival_idx != -1){
        current_time = p[next_arrival_idx].arrival_time;
        queue[rear++] = next_arrival_idx;
        in_queue[next_arrival_idx] = true;
        gantt_times[gantt_index] = current_time;
      } else break;
    }

    int idx = queue[front++];
    in_queue[idx] = false;

    if(p[idx].remaining_time == p[idx].burst_time){
      p[idx].start_time = current_time;
      p[idx].response_time = p[idx].start_time - p[idx].arrival_time;
    }

    int exec_time;
    exec_time = (p[idx].remaining_time > quant) ? quant : p[idx].remaining_time;

    current_time += exec_time;
    p[idx].remaining_time -= exec_time;

    gantt_pid[gantt_index] = p[idx].pid;
    gantt_times[gantt_index + 1] = current_time;
    gantt_index++;

    for(int i = 0; i < n; i++){
      if(i != idx && !p[i].is_completed && !in_queue[i] && p[i].arrival_time <= current_time){
        queue[rear++] = i;
        in_queue[i] = true;
      }
    }

    if(p[idx].remaining_time > 0){
      queue[rear++] = idx;
      in_queue[idx] = true;
    } else{
        p[idx].completion_time = current_time;
        p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
        p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
        p[idx].is_completed = true;
        completed++;
    }
  }
  printGanttChart(gantt_pid, gantt_times, gantt_index);
  printTable(p, n);
}

int main(){
  int choice, n, tq;
  Process p[MAX_PROCESS];
  Process p_copy[MAX_PROCESS];

  printf("Enter number of Process: ");
  scanf("%d", &n);

  printf("Enter Arrival Time and Burst Time for %d processes: \n", n);
  for(int i = 0; i < n; i++){
    p[i].pid = i + 1;
    printf("Process P%d (AT BT): ", i + 1);
    scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
  }

  while(1){
    printf("\n\n--- Scheduling Menu ---\n");
    printf("1. FCFS\n");
    printf("2. Round Robin\n");
    printf("3. Exit\n");
    printf("Enter Choice:");
    scanf("%d", &choice);

    for(int i = 0; i < n; i++) p_copy[i] = p[i];

    switch(choice){
      case 1:
        calculateFCFS(p_copy, n);
        break;
      case 2:
        printf("Enter Quant time");
        scanf("%d", &tq);
        CalculateRoundRobin(p_copy, n, tq);
        break;
      case 3:
        printf("Exiting Program!\n");
        return 0;
      default:
        printf("Invalid Chocie\n");
    }
  }
}
