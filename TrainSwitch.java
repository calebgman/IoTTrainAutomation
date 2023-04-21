package t_switch_test;

import java.util.*;

public class TrainSwitch {
	 private static final int INF = Integer.MAX_VALUE; // Used to represent infinity
	    
	 private static int[][] adjacencyMatrix = {
			 //1  2  3   4  5   6     7    8    9    10 
			  {0, 1, 1, 1, INF, INF, INF, INF, INF, INF, 1}, // 1
			  {1, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF}, //2
			  {1, 1, 0, INF, INF, INF, INF, INF, INF, INF, INF}, //3
			  {1, INF, INF, 0, 1, INF, INF, INF, INF, 1, INF}, // 4
			  {INF, INF, INF, 1, 0, 1, INF, 1, INF, INF, INF}, //5
			  {INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, 1}, // 6
			  {INF, INF, INF, INF, INF, 1, 0, 1, INF, INF, INF}, //7
			  {INF, INF, INF, INF, 1, INF, 1, 0, 1, INF, INF}, //8
			  {INF, INF, INF, INF, INF, INF, INF, 1, 0, 1, INF}, //9
			  {INF, INF, INF, 1, INF, INF, INF, INF, 1, 0, INF}, //10
			  {1, INF, INF, INF, INF, 1, INF, INF, INF, INF, 0}
			};
	    
	    private static String[] stationNames = { 
	        "Station 1", "Station 2", "Station 3", "Station 4", "Station 5",
	        "Station 6", "Station 7", "Station 8", "Station 9", "Station 10", "Station 11"
	    };
	    
	    private static int[] distances = new int[11];
	    private static int[] previousNodes = new int[11];
	    private static boolean[] visited = new boolean[11];
	    
	    public static void main(String[] args) {
	        
	        Scanner scanner = new Scanner(System.in);
	        
	        while (true) {
	            
	            // Reset the algorithm's data structures
	            Arrays.fill(distances, INF);
	            Arrays.fill(previousNodes, -1);
	            Arrays.fill(visited, false);
	            
	            // Get the user's input for the starting station
	            System.out.print("Enter the starting station (1-11): ");
	            int startingStation = scanner.nextInt() - 1; // Subtract 1 to convert to 0-indexed array
	            
	            if (startingStation < 0 || startingStation >= 11) {
	                System.out.println("Invalid station number. Please try again.");
	                continue;
	            }
	            
	            // Get the user's input for the ending station
	            System.out.print("Enter the ending station (1-11): ");
	            int endingStation = scanner.nextInt() - 1; // Subtract 1 to convert to 0-indexed array
	            
	            if (endingStation < 0 || endingStation >= 11) {
	            	System.out.println("Invalid station number. Please try again.");
	                continue;
	            } else if (endingStation == 7 || startingStation == 1) {
	            
	            }
	            
	            // Run Dijkstra's algorithm on the adjacency matrix
	            distances[startingStation] = 0;
	            
	            for (int i = 0; i < 11; i++) {
	                int currentNode = getClosestUnvisitedNode();
	                visited[currentNode] = true;
	                
	                if (currentNode == endingStation) {
	                    break;
	                }
	                
	                updateDistances(currentNode);
	            }
	            
	            // Print the shortest path and distance from the starting station to the ending station
	            System.out.print("Shortest path from " + stationNames[startingStation] + " to " + stationNames[endingStation] + ": ");
	            printShortestPath(endingStation);
	            System.out.println("Distance: " + distances[endingStation] + "\n");
	            
	            // Ask the user if they want to continue or quit
	            System.out.print("Enter 'q' to quit or any other key to continue: ");
	            String userInput = scanner.next();
	            
	            if (userInput.equals("q")) {
	                break;
	            }
	        }
	        
	        scanner.close();
	    }
	    
	    private static int getClosestUnvisitedNode() {
	        int closestNode = -1;
	        int closestDistance = INF;
	        
	        for (int i = 0; i < 11; i++) {
	            if (!visited[i] && distances[i] < closestDistance) {
	                closestNode = i;
	                closestDistance = distances[i];
	            }
	        }
	        
	        return closestNode;
	    }
	    
	    private static void updateDistances(int currentNode) {
	        for (int i = 0; i < 11; i++) {
	            int distanceToNextNode = adjacencyMatrix[currentNode][i];
	            
	            if (distanceToNextNode != INF) {
	                int totalDistance = distances[currentNode] + distanceToNextNode;
	                
	                if (totalDistance < distances[i]) {
	                    distances[i] = totalDistance;
	                    previousNodes[i] = currentNode;
	                }
	            }
	        }
	    }
	    
	    private static void printShortestPath(int node) {
	        if (node == -1) {
	            return;
	        }
	        
	        printShortestPath(previousNodes[node]);
	        System.out.print(stationNames[node] + " ");
	    }
	
}
