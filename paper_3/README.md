# Paper 3

Tasks are first sorted by arrival time.
Tasks that have arrived in the ready queue are then split into either a light queue or heavy queue
Tasks that have a burst time less than the median burst time of tasks in the ready queue gets moved to the light queue
Tasks that have a burst time more than the median burst time of tasks in the ready queue gets moved to the heavy queue

Program will keep running until the waiting queue has been emptied
  While the waiting queue is not empty; check if tasks in the waiting queue have arrival time less than or equals to the current turnaround time and add the tasks to ready queue
  If tasks in waiting queue have a higher arrival time compared to current turnaround time, move on to process tasks in the ready queue
  
  While ready queue is not empty
    Find the median burst time of all tasks in the ready queue
    Split tasks into two queues according to their burst time

    Light queue: Burst times less than or equal to median burst time
    Time quantum is set to the median burst time of all tasks
    Tasks are processed in a round robin fashion until queue is cleared

    When light queue has been cleared, move on to heavy queue for processing
    Heavy queue: Burst times greater than median burst time
    Time quantum is set to the median burst time of all tasks
    Tasks are processed in a round robin fashion until queue is cleared
