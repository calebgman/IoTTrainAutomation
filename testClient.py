import http.client
import time

conn = http.client.HTTPConnection("localhost",8001)

#add Locomotive Node
conn.request("GET", "/node/ae67/nn/loco")
r1 = conn.getresponse()

time.sleep(1)

#Move a locomotive through a few blocks
conn.request("GET", "/node/103E/lp/ae67")
r2 = conn.getresponse()

print(r2.read())

time.sleep(1)

conn.request("GET", "/node/117E/lp/ae67")
r2 = conn.getresponse()

print(r2.read())

time.sleep(1)

conn.request("GET", "/node/AA42/lp/ae67")
r2 = conn.getresponse()

print(r2.read())

#Test Tick function
conn.request("GET", "/node/0000/tn")
r2 = conn.getresponse()

print(r2.read())

time.sleep(1)

#Simulate Obstacle on track
conn.request("GET", "/node/BCDF/ob/1")
r2 = conn.getresponse()

print(r2.read())

time.sleep(1)

#Simulate Obstacle on track
conn.request("GET", "/node/BCDF/ob/0")
r2 = conn.getresponse()

print(r2.read())