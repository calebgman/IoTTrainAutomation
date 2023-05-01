#Nolan's Terrible last minute server

import http.server
from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver
import numpy as np
import pandas as pd

hostname = "192.168.130.18"
PORT = 8001

nodeData  = pd.read_csv("nodes.csv").set_index("ID")
print(nodeData)

nodeMap = pd.read_csv("nodeMap.csv").set_index("NODE")
print(nodeMap)

## Horrible Server Code
class CentralHub(BaseHTTPRequestHandler):

    def AddNode(self, nid, ntype):

        global nodeData
        #declare temp dataframe to concat with nodeData
        nn = pd.DataFrame(
                {"TYPE" : ntype,
                "BLOCKING" : 0,
                "SIG_STATE" : 0,
                "OBSTACLE" : 0,
                "SW_STATE" : 0,
                "SPEED" : 0,
                "STOPPED" : 0,
                "CURR_LOC" : "NULL",
                "PREV_LOC" : "NULL",
                "PREV_PREV_LOC" : "NULL"},
                index = [nid])
        nodeData = pd.concat([nodeData, nn])
        #self.send_response(200)


    def setSignal(self, nid, sState):

        global nodeData

        #Sets the node Signals
        self.wfile.write(bytes("/hub/%s/ss/%d " % (nid, sState), "utf-8"))
        #update nodeData
        nodeData.loc[nid, "SIG_STATE"] = sState
        print("Setting Signal State of %s to %d" % (nid, sState))

    def startStop(self, lid):
        print("WOOT")

        cBlock = nodeData.loc[lid, "CURR_LOC"]

        nBlock = nodeMap.loc[cBlock, "NEXT"]

        if nodeData.loc[nBlock, "SIG_STATE"] < 2:
            nodeData.loc[lid, "STOPPED"] = 1

        elif nodeData.loc[nBlock, "SIG_STATE"] == 2:
            nodeData.loc[lid, "STOPPED"] = 0

    #Manages locomotives
    def tick(self):
        global nodeData
        lData = nodeData.index.to_numpy()

        for i in lData:
            print(i)
            if nodeData.loc[i, "TYPE"] == "loco":
                self.startStop(i)
                self.wfile.write(bytes("/hub/%s/lc/%s/%s/%s " % (i, nodeData.loc[i, "SPEED"], nodeData.loc[i, "STOPPED"], nodeData.loc[i, "CURR_LOC"] ), "utf-8"))


    def LocoPass(self, cPath):

        global nodeData

        lid = cPath[4]
        print("Loco Pass")
        #Perform the Signal State Changes
        self.setSignal(nodeData.loc[lid, "PREV_PREV_LOC"], 2) #Prev Prev to CLEAR
        self.setSignal(nodeData.loc[lid, "PREV_LOC"], 1) #Prev to CAUTION
        self.setSignal(cPath[2], 0) #Current to DANGER

        #inform previous node that Locomotive has left
        self.wfile.write(bytes("/hub/%s/lx " % nodeData.loc[lid, "CURR_LOC"], "utf-8"))

        #Edit the information in nodeData

        #Update loco node location
        nodeData.loc[lid, "PREV_PREV_LOC"] = nodeData.loc[lid, "PREV_LOC"]
        nodeData.loc[lid, "PREV_LOC"] = nodeData.loc[lid, "CURR_LOC"]
        nodeData.loc[lid, "CURR_LOC"] = cPath[2]

        #change which node is blocking
        nodeData.loc[nodeData.loc[cPath[4],"PREV_LOC"], "BLOCKING"] = 0
        nodeData.loc[cPath[2], "BLOCKING"] = 1

        

    def obstacleBlocking(self, cPath):
        if cPath[4] == "1":
            nodeData.loc[cPath[2], "OBSTACLE"] = 1
            nodeData.loc[cPath[2], "BLOCKING"] = 1
            self.setSignal(cPath[2], 0)

            pBlock = nodeMap.loc[cPath[2], "PREV"]
            self.setSignal(pBlock, 1)


        else:
            nodeData.loc[cPath[2], "OBSTACLE"] = 0
            nodeData.loc[cPath[2], "BLOCKING"] = 0
            self.setSignal(cPath[2], 2)

            pBlock = nodeMap.loc[cPath[2], "PREV"]
            self.setSignal(pBlock, 2)


    def do_GET(self):

        global nodeData

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        if self.path == "/testing":
            self.wfile.write(bytes("<p>Request: If blocks do work yo!</p>", "utf-8"))

        cPath = self.path.split("/")

        #Terrible Command Parser for Terrible Server

        #New Node: /NODE_ID/nn/type/
        if cPath[3] == "nn":
            self.AddNode(cPath[2], cPath[4])
        
        #Locomotive Pass: /NODE_ID/lp/LOCO_ID
        elif cPath[3] == "lp":
            self.LocoPass(cPath)

        #System Tick: /node/0000/tn
        elif cPath[3] == "tn":
            self.tick()

        elif cPath[3] == "ob":
            self.obstacleBlocking(cPath)

        print("Table Change:")
        print(nodeData)


        




            



if __name__ == "__main__":
    webServer = HTTPServer((hostname, PORT), CentralHub)
    print("Server started http:..%s:%s" % (hostname, PORT))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server Stopped")

