# IoTTrainAutomation

## Overview
Welcome to the IoTTrainAutomation Senior Project from Nolan G. Josh B. & Caleb G. This project was designed with efficiency 
and safety as our top priority. This design operates using a connected network of nodes ( a gateway node, a locomotive node, 
a level crossing node, & finally a switch node) that communicate with one another via LoRa communiation. This system allows 
for real-time data transfer and communication between trains, improving safety and reducing the risk of accidents. 
Additionally, the use of LoRa communication ensures that there is a reliable and secure connection between the trains, making 
it a safer means of travel, as well as moving large shipments. With our system, routing is also optimized to ensure the most
efficient and effective route is taken, reducing travel time and improving overall productivity. Our use of IoT technology 
also enabled us to have real time tracking and monitoring of trains, allowing for quick response to any issue or accident
that may arise during travel time. Our system features speed monitoring as well as location to ensure that all train 
operations are secure and reliable. 

## Train Routing Code
This is a program that uses Dijkstra's algorithm for shortest pathing. It uses a hard-coded graph that is represented by 
an adjacency matrix. The adjacency matrix uses a small area for train routing. We chose to use the North Eastern U.S.A
for our routing, which includes the cities of:
  1. Chicago
  2. Kansas
  3. St. louis
  4. Cleveland
  5. Pitsburg
  6. DC
  7. Philidelphia
  8. NY
  9. Boston
  10. Albany
  11. Cincinati

### How does it work?
This made for a simple and effective design for efficient routing. The program takes in a user input for a starting station 
( a number between 1-11 inclusive) and computes the shortest path and distance between them using Dijkstra's algorithm.
Each vertex in the matrix corresponds to a train station in the order that is provided above. 'INF' in this case represents
an infinite distance between two vertices that are not directly connected to each other. The algorithm starts with the 
'distances' array, which represents the shortest distance from the starting station provided by the user to every other
station. Initially, all distances are set to the value of 'INF' except for the starting station, which is set to 0. 
The 'previousNodes' array is used to keep track of the previous node in that path. The 'visited' array is used to keep 
track of which nodes have been visited so far in the path. The 'getClosestUnvisitedNode' method is used to find the node 
with the shortest distance from the starting node that hasn't been visited yet. The method iterates through all nodes, 
checks if it hasn't been visited and if the distance is shorter than the current closest distance. The 'updateDistances' 
method updates the distances of the neighboring nodes of the current node. It also iterates through all the nodes and 
checks if the distance to the next node is not infinite. If the total distance to the next node is shorter than the current 
distance to the next node, the distance is updated, and the previous node is set to the current node. The program then 
outputs the shortest path resulting from the starting user input to the finish station from user input. 

# What is the rest?
The rest of the code is everything that is needed to actually run our system, including test files.
