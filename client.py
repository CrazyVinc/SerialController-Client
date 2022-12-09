import sys
import threading
import asyncio

import serial
import socketio

from src.config import config
from src.queue_1 import Queue


serial_port = serial.Serial(config['SerialPort'], 9600, timeout=0)
Ready = False


def runOnStart():
    if config["runOnStart"] != "":
        serial_port.write((config["runOnStart"]+"\n").encode())

def read_from_port(ser):
    while True:
        reading = ser.readline().decode()
        if reading != "":
            print(reading.strip())
            if reading == config["readyOutput"]:
                runOnStart()
                Ready = True
            elif Ready:
                if Queue.qsize() > 0: Queue.task_done()

thread = threading.Thread(target=read_from_port, args=(serial_port,))
thread.start()

def CLI(ser):
    while(True):
        line = sys.stdin.readline()
        ser.write(line.encode())
        print(line.strip())

thread = threading.Thread(target=CLI, args=(serial_port,))
thread.start()







sio = socketio.AsyncClient()
@sio.on('connect', namespace="/client")
async def on_connect():
    print("Connection established for /client")
    await sio.emit("init", config['data'], namespace="/client")


@sio.on('data', namespace="/client")
async def on_message(data = str):
    Queue.put(serial_port.write((data+"\n").encode()))


async def connect():
    await sio.connect(config["host"], namespaces='/client')
    await sio.wait()


asyncio.run(connect())