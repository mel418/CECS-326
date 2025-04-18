class Task:
    def __init__(self, name, priority, burst):
        self.name = name
        self.priority = priority    # higher number means higher priority (1-10)
        self.burst = burst          # CPU burst time in milliseconds
        self.remaining = burst      # remaining burst time (used for RR scheduling)