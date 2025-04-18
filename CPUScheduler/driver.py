from task import Task
from fcfs import FCFSScheduler
from priority import PriorityScheduler
from rr import RRScheduler
import sys

def read_tasks(filename):
    '''Read tasks from a file'''
    tasks = []
    try:
        with open(filename, "r") as file:
            for line in file:
                # remove whitespace and split by comma
                parts = [part.strip() for part in line.strip().split(',')]

                if len(parts) != 3:
                    continue # skip invalid lines

                name = parts[0]
                priority = int(parts[1])
                burst = int(parts[2])

                tasks.append(Task(name, priority, burst))

        return tasks
    except Exception as e:
        print(f"Error reading task file: {e}")
        return []
    
def main():
    if len(sys.argv) < 2:
        print("Usage: python driver.py <schedule_file>")
        return
    
    filename = sys.argv[1]
    tasks = read_tasks(filename)

    if not tasks:
        print("No tasks loaded.")
        return
    
    print(f"Loaded {len(tasks)} tasks:")
    for task in tasks:
        print(f"Task {task.name}: Priority={task.priority}, Burst={task.burst}ms")

    # run FCFS scheduler
    fcfs = FCFSScheduler()
    for task in tasks:
        fcfs.add(task)
    fcfs.schedule()

    # run priority scheduler
    priority = PriorityScheduler()
    for task in tasks:
        priority.add(task)
    priority.schedule()

    # run round-robin scheduler (time quantam = 10ms)
    rr = RRScheduler(time_quantam=10)
    for task in tasks:
        rr.add(task)
    rr.schedule()

if __name__ == "__main__":
    main()