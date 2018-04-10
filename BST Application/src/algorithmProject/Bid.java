//============================================================================
// Name        : Bid.java
// Author      : Christopher Willoughby
// Course      : CS-499
// Date        : 28Mar18
// Version     : 1.0
// Description : Implements the Bid data type class
//============================================================================
package algorithmProject;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Vector;

public class Bid {
	
	public String bidID;
	public String title;
	public String fund;
	public double amount;
	
	/**
	 * Constructor for the Bid data type
	 */
	public Bid() {
		this.bidID = "";
		this.title = "";
		this.fund = "";
		this.amount = 0.0;
	}

	/**
	 * Display the bid information to the console
	 *
	 * @param bid data type containing the bid info
	 */
	public String toString() {	
		return this.bidID + ": " + this.title + " | " + this.amount + " | " + this.fund;
		}
	
	/**
	 * Simple function to convert a string to a double after stripping out unwanted characters
	 *
	 * @param str is the string to be converted
	 * @return a double data type of the converted string
	 */
	public static double stringToDouble(String str) {
		
		Vector<Integer> charRemoveLocations =  new Vector<Integer>(0);
		StringBuilder tempString = new StringBuilder(str);
		int periodCount = 0;
		for (int i = 0; i < str.length(); i++) {
			boolean removeChar = true;
			
			if ((str.charAt(i) == '.') && (periodCount == 0)) {
				removeChar = false;
				periodCount++;
			}
			
			else {
				// Looping through ASCII values for numbers 0-9
				for(int j = 48; j < 58; j++) {
					if ((int) str.charAt(i) ==  j) {
						removeChar = false;					
					}
				}
			}
			
			if (removeChar == true) {
				charRemoveLocations.addElement(i);
			}
		}

		for (int i = charRemoveLocations.size(); i > 0; i--) {
			tempString.deleteCharAt(charRemoveLocations.get(i - 1));
		}
		
		str = tempString.toString();
		return Double.parseDouble(str);
		
	}
	
	/**
	 * Load a CSV file containing bids into a container
	 *
	 * @param csvPath the path to the CSV file to load and the BST data type
	 * @return a container holding all the bids read
	 */	
	public static void loadBids(String csvPath, BinarySearchTree bst) {
		System.out.println("Loading CSV file " + csvPath);
		
		try {
			File csvData = new File(csvPath);
			Scanner scanner = new Scanner(csvData);
			scanner.nextLine().split(",");  // Consumes the document header
			String[] recordEntry;
	
			while (scanner.hasNextLine()) {
				// Creates a data structure and adds to the collection of bids
				Bid bid = new Bid();
				recordEntry = scanner.nextLine().split(",");
				bid.bidID = recordEntry[1];
				bid.title = recordEntry[0];
				bid.fund = recordEntry[8];
				bid.amount = stringToDouble(recordEntry[4]);

				// Insert the bid into the BST
				BinarySearchTree.Insert(bid);
			}
			
			scanner.close();
		} 
		  
		catch (FileNotFoundException e) {
			e.printStackTrace();
		} 
	}
}
