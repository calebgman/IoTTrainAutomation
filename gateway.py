import http.client
import serial
import time
#New Gateway code for Pi to integrate with new Server Stuff

#Establish the Connection
conn = http.client.HTTPConnection("localhost",8001)
ser = serial.Serial("COM6", 9600, timeout=0.5)

tick = 100000

print("Begin Gateway")

nodeInput = ""

inByte = ""

validPacket = False
readIn = False

while 1:
    if ser.in_waiting > 0:
        inByte = ser.read().decode(encoding="ascii")
        print(inByte)

        if inByte == "?":
            readIn = True
        elif readIn == True and inByte != " ":
            nodeInput = nodeInput + inByte
            print(nodeInput)
        elif readIn == True and inByte == " ":
            validPacket = True
            readIn = False
            inByte = ""

    
    
        
        

    if validPacket == True:

        nodes = nodeInput.split(";")

        print(nodes)

        nodeInput = ""

        ##Locomotive Pass command
        if nodes[2] == "LP":
            print(nodes[2])
            getS = "/node/%s/lp/%s" % (nodes[1], nodes[3])
            conn.request("GET", getS)
            rComm = conn.getresponse()
            iCommands = rComm.read()
                
            iCommands = iCommands.decode()

            commands = iCommands.split(" ")

            ss1 = commands[0].split("/")
            ss2 = commands[1].split("/")
            ss3 = commands[2].split("/")
            lx = commands[3].split("/")

            ss1m = "0;%s;SS;%s" % (ss1[2], ss1[4])
            ss1m = ss1m.encode()

            ss2m = "0;%s;SS;%s" % (ss2[2], ss2[4])
            ss2m = ss2m.encode()

            ss3m = "0;%s;SS;%s" % (ss3[2], ss3[4])
            ss3m = ss3m.encode()

            lxm = "0;%s;lx" % lx[2]
            lxm = lxm.encode()

            ser.write(ss1m)
            ser.write(ss2m)
            ser.write(ss3m)
            ser.write(lxm)

            ##Obstacle Command
        elif nodes[2] == "OB":
            print(nodes[2])
            getS = "/node/%s/ob/%s" % (nodes[1], nodes[3])
            print("Beginning HTTP Request")
            conn.request("GET", getS)
            rComm = conn.getresponse()
            print("HTTP Response Recieved")
            iCommands = rComm.read()
            print(iCommands)

            iCommands = iCommands.decode()

            commands = iCommands.split(" ")

            ss1 = commands[0].split("/")
            ss2 = commands[1].split("/")

            ss1m = "0;%s;SS;%s " % (ss1[2], ss1[4])
            ss1m = ss1m.encode()

            ss2m = "0;%s;SS;%s" % (ss2[2], ss2[4])
            ss2m = ss2m.encode()

            ser.write(ss1m)
                
            time.sleep(1)
                
            ser.write(ss2m)

            #No Code Yet
        elif nodes[2] == "CS":
            print(nodes[2])

            #No Code Yet
        elif nodes[2] == "SW":
            print(nodes[2])
    
        #Tick Control
        nodeInput = ""
        validPacket = False
        ser.reset_input_buffer()
        ser.reset_output_buffer()
    """
    if tick == 0:
        conn.request("GET", "/node/0000/tn")
        rComm = conn.getresponse()
        commands = rComm.read()
        print(commands)
        commands = commands.decode()

        lc = commands.split("/")
        print(lc)
        
        sendB = "0;%s;LC;%s|%s|%s" % (lc[2], lc[4], lc[5], lc[6])
        
        print(sendB)

        sendB = sendB.encode(encoding = "ascii")
        
        ser.write(sendB)

        tick = 100000

    tick = tick - 1
    """
            

        