import threading
import queue

Queue = queue.Queue()

def worker():
    while True:
        item = Queue.get()


threading.Thread(target=worker, daemon=True, args=()).start()