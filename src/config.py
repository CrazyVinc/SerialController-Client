import json

f = open('config.json', "r") # Open Config file
config = json.loads(f.read()) # Reading from file
f.close() # Close File
