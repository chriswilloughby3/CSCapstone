//============================================================================
// Name        : bstApplication.java
// Author      : Christopher Willoughby
// Course      : CS-499
// Date        : 30Mar18
// Version     : 1.0
// Description : Implements the user interface to interact with the bids and
//				 the Binary Search Tree data structure
//============================================================================

package algorithmProject;

import java.util.Scanner;

public class bstApplication {
	public static void main(String[] args){
	    // process command line arguments
	    String csvPath;
	    String bidKey;
	    
	    switch (args.length) {
	    
	    case 1:
	        csvPath = args[0];
	        bidKey = "98109";
	        break;
	        
	    case 2:
	        csvPath = args[0];
	        bidKey = args[1];
	        break;
	    default:
	        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
	        bidKey = "98109";
	    }
	    
	    // Define a timer variables
	    long nanoSeconds = 0;
	    double seconds = 0;

	    // Define a binary search tree to hold all bids
	    BinarySearchTree bst = new BinarySearchTree();;
	    Bid bid = new Bid();
	    
	    Scanner in = new Scanner(System.in);
	    
	    int choice = 0;
	    while (choice != 9) {
	        System.out.println("Menu:");
	        System.out.println("  1. Load Bids"); 
	        System.out.println("  2. Display All Bids"); 
	        System.out.println("  3. Find Bid");
	        System.out.println("  4. Remove Bid");
	        System.out.println("  9. Exit");
	        System.out.println("Enter choice: ");
	        choice = in.nextInt();
	        
	        switch (choice) {

	        case 1:          
	        	// Initialize a timer variable before loading bids
	        	nanoSeconds = System.nanoTime();

	        	// Complete the method call to load the bids
	        	Bid.loadBids(csvPath, bst);

	        	// Calculate elapsed time and display result
	        	nanoSeconds = System.nanoTime() - nanoSeconds; // current clock ticks minus starting clock ticks
	        	System.out.println("time: " + nanoSeconds + " nanoseconds");
            
	        	seconds = (double)nanoSeconds / 1000000000.0;
	        	System.out.println("time: " + seconds + " seconds");
	        	break;

	        case 2:
	        	BinarySearchTree.InOrder();
	        	break;
	        	

	        case 3:
	        	nanoSeconds = System.nanoTime();

	            bid = BinarySearchTree.Search(bidKey);
	            
	            // current clock ticks minus starting clock ticks
	            nanoSeconds = System.nanoTime() - nanoSeconds; 

	            if (!bid.bidID.isEmpty()) {
	            	System.out.println(bid);
	            }
	            else {
	            	System.out.println("Bid Id " + bidKey + " not found.");
	            }

	        	System.out.println("time: " + nanoSeconds + " nanoseconds");
	            
	        	seconds = (double)nanoSeconds / 1000000000.0;
	        	System.out.println("time: " + seconds + " seconds");
	            break;

	        case 4:
	            BinarySearchTree.Remove(bidKey);
	            break;
	        }
	    }
	    in.close(); // Closes scanner
	    System.out.println("Good bye.");	        
	}
}
