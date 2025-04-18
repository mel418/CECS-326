from cpu import run

class FCFSScheduler:
    def __init__(self):
        self.task_list = []

    def add(self, task):
        '''Add a task to the scheduler queue'''
        self.task_list.append(task)
    
    def schedule(self):
        '''Run all tasks in FCFS order'''
        print("\n===== FCFS Scheduling =====")

        if not self.task_list:
            print("No tasks to schedule.")
            return

        total_time = 0
        waiting_times = []
        turnaround_times = []

        print("Order of execution:")
        for task in self.task_list:
            # this task starts at current total_time
            waiting_time = total_time
            waiting_times.append(waiting_time)

            # run the task for its full burst time
            run(task, task.burst)

            # update total time
            total_time += task.burst

            # calculate turnaround time (waiting time + burst time)
            turnaround_time = waiting_time + task.burst
            turnaround_times.append(turnaround_time)

        # print statistics
        avg_waiting = sum(waiting_times) / len(waiting_times)
        avg_turnaround = sum(turnaround_times) / len(turnaround_times)

        print("\nStatistics:")
        print(f"Average Waiting Time: {avg_waiting:.2f} ms")
        print(f"Average Turnaround Time: {avg_turnaround:.2f} ms")