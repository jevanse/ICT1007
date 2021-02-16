# A k-Factor CPU Scheduling Algorithm
According to a [IEEE Paper](https://ieeexplore.ieee.org/document/8493662) published by [Sumedha Garg](https://ieeexplore.ieee.org/author/37086484690) and [Deepak Kumar](https://ieeexplore.ieee.org/author/37086482202), one of the shortcomings of the Shortest Job First (SJF) CPU scheduling system is that processes that require a larger time quantum of CPU burst time tend to be under prioritized. This is due to the inherent nature of the SJF algorithm. SJF gives priority to processes that require the least amount of CPU burst time.

Under certain conditions, processes with exceptionally large burst times never get allocated the neccessary resources to be resolved.

The solution proposed by Garg and Kumar aims to solve the issue by using a factor K which is initialised at 2 and incremented alternatively. It is then used to alter the priority in which a process gets resolved using a formula of :
```C
  if (K* burst_time[process1]) > (t + burst_time[process 2]){
    // Process 2 gets the CPU
  }
  else{
    // Process 1 gets the CPU
  }
```
Where t is the amount of time the CPU has been running, represented in a Gantt chart

<hr>

## Implementation
To implement this solution, i first generate a dataset of randomly generated process time aggregated to a scale of 1-100.
Following that, i set up a control test with a standard SJF Algorithm. I then pass the aforementioned dataset through the control algorithm and collected 4 data points crucial for analysis:
- Average waiting time
- Average turn around time
- Average waiting time for **Low Priority Processes**
- Average turn around time for **Low Priority Processes**

Next, i set up the proposed algorithm and passed the same data set through the algorithm and collected the same 4 data points for comparison

<hr>

## Analysis
*TBC*

## Conclusion
One of the flaws in the referenced conference paper was the absence of empirical data supporting the statement that the proposed algorithm "removes starvation of longer processes by becoming fair to all the processes with similar turn around time and waiting time as SJF." With the collected data points we have been able to prove/disprove **[TBD]** that the algorithm removes the inherent unfairness in SJF and prevents starvation of longer processes.
