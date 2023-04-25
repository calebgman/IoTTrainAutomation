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
Each vertex in the matrix corresponds to a train station in the order that is provided above. 
