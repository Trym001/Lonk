class timer:
    def __init__(self):
        self.start_time = 0
        self.stop_time = 0
        self.spent = 0

    def start(self):
        self.start_time = time.perf_counter()
        return self.start_time

    def stop(self):
        self.stop_time = time.perf_counter()
        self.spent = self.stop_time - self.start_time
        return self.spent
