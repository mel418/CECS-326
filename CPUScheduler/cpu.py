def run(task, time):
    '''
    Simulates running a task on the CPU for the specified time.

    Args:
        task (Task): the task to run
        time (int): time to run the task for (in milliseconds) 

    '''
    print(f"Running task '{task.name}' for {time} ms.")