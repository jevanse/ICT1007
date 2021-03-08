# ICT1007 Paper 5

### TODO:
- Testing
- Cleanup 
- Documentation



### Implementation

- Prof assumes that there is an implicit priority, 1 > 2 > 3 > 4 > 5 (processes are always run in this order no matter what)
- Run all processes in a round robin fashion
- Each process has a priority - either 1 (low), 2 (medium), 3 (high)
  - The priority determines the allocated time slice
  - i.e. TQ = 500, for low priority TQ = 500 * 0.8, med priority TQ = 500, high priority TQ = 500 * 1.2
  
- If a process is low priority/med priority
  - Check if its remaining burst time is 1.2 * allocated time quantum
  - Run it to completion if it is
- If a process is high priority
  - Check if its remaining burst time is 1.3 * allocated time quantum
    - i.e. allocated TQ = 600, remaining burst time = 750, **run it till completion**

### Goals
- Reduce number of context switches
- Give higher priority processes more execution time
- 