from cpu import run

class PriorityScheduler:
    def __init__(self):
        self.task_list = []

    def add(self, task):
        '''Add a task to the scheduler queue'''
        self.task_list.append(task)

    def schedule(self):
        '''Run all tasks based on priority (higher priority first)'''
        print("\n===== Priority Scheduling =====")

        if not self.task_list:
            print("No tasks to schedule.")
            return

        # sort tasks by priority (higher number = higher priority)
        sorted_tasks = sorted(self.task_list, key=lambda t: t.priority, reverse=True)

        total_time = 0
        waiting_times = []
        turnaround_times = []

        print("Order of execution:")
        for task in sorted_tasks:
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
        avg_turnaround = sum(turnaround_times)/ len(turnaround_times)

        print("\nStatistics:")
        print(f"Average Waiting Time: {avg_waiting:.2f} ms")
        print(f"Average Turnaround TIme: {avg_turnaround:.2f}")