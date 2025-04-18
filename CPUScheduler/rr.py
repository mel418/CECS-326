from cpu import run
from collections import deque

class RRScheduler:
    def __init__(self, time_quantam=10):
        self.task_list = []
        self.time_quantam = time_quantam

    def add(self, task):
        '''Add a task to the scheduler queue'''
        self.task_list.append(task)

    def schedule(self):
        '''Run all tasks using Round-Robin scheduling'''
        print(f"\n===== Round-Robin Scheduling (Time Quantum: {self.time_quantam} ms) =====")

        if not self.task_list:
            print("No tasks to schedule.")
            return
        
        # create a copy of tasks to preserve original task_list
        tasks = [task for task in self.task_list]
        for task in tasks:
            task.remaining = task.burst # reset remaining time

        ready_queue = deque(tasks)
        total_time = 0
        completed_tasks = []
        waiting_times = {}
        last_execution = {task.name: 0 for task in tasks} # track when each task last ran

        print("Order of execution:")
        while ready_queue:
            current_task = ready_queue.popleft()

            # calculate waiting time since last execution
            waiting_time = total_time - last_execution[current_task.name]
            if current_task.name not in waiting_times:
                waiting_times[current_task.name] = waiting_time
            else:
                waiting_times[current_task.name] += waiting_time

            # determine how much time to run
            run_time = min(self.time_quantam, current_task.remaining)

            # run the task
            run(current_task, run_time)

            # update timing info
            total_time += run_time
            last_execution[current_task.name] = total_time
            current_task.remaining -= run_time

            # check if task is complete
            if current_task.remaining > 0:
                ready_queue.append(current_task)    # put back in queue
            else:
                completed_tasks.append((current_task, total_time))    # task is done
            
        # calculate statistics
        turnaround_times = {task.name: completion_time for task, completion_time in completed_tasks}

        print("\nStatistics:")
        avg_waiting = sum(waiting_times.values()) / len(waiting_times)
        avg_turnaround = sum(turnaround_times.values()) / len(turnaround_times)

        print(f"Average Waiting Time: {avg_waiting:.2f} ms")
        print(f"Average Turnaround time: {avg_turnaround:.2f} ms")