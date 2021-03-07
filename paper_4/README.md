# [Paper 4 - An Efficient Dynamic Round Robin Algorithm for CPU Scheduling](#https://ieeexplore.ieee.org/document/7918936)

<details>
    <summary>Authors</summary>

    Muhammad Umar Farooq
    Aamna Shakoor
    Abu Bakar Siddique
    University of Engineering and Technology, Lahore
</details>
<details>
    <summary>Objective</summary>
    
    Improve on the efficiency of Dynamic Round Robin algorithms by improving the method used to select time quantum.

    When improving the efficiency of said algorithm, the paper focuses on the following:
    - maximising throughput
    - minimising context switches
    - minimising average waiting time (AWT)
    - minimising average turnaroung time (ATT)
</details>
<details>
    <summary>How does this algorithm differ from normal round robin?</summary>

    The efficiency of Round Robin is determined by the selected time quantum. 

    The paper studies one possible way of selecting the time quantum dynamically based on the burst times of processes that are ready. 
</details>
<details>
    <summary>How does this algorithm differ from other dynamic round robin?</summary>

    Other dynamic round robin algorithms make use of a combination of SJF (sorting the processes based on ascending burst time) and RR algorithms. The problem with this method -- sorting takes time.

    Hence, the paper instead tries to avoid using SJF with the hope of improving the efficiency of RR.
</details>
<details>
    <summary>Algorithm</summary>
<br>

- Calculate burst_time_maximum of processes in ready queue
- Calculate `time_quantum = burst_time_maximum * 0.8`
- For processes with `burst_time <= time_quantum`
    - Execute process
- For processes with `burst_time > time_quantum`
    - Place process at end of ready queue.
- Once all shorter processes are finished, set `time_quantum = burst_time_maximum`
- Execute all the longer processes that were previously placed at the end of the queue.
- Whenever a new process joins the queue, recalculate `time_quantum`
</details>