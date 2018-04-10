//================================================================================
// Name        : BinarySearchTree.java
// Author      : Christopher Willoughby
// Course      : CS-260
// Date        : 11Oct17
// Version     : 1.0
// Description : Implements a Binary Search Tree using the Bid and Node data types
//================================================================================

package algorithmProject;

public class BinarySearchTree {
	
	private static Node root;
	
	/**
	 * Add a bid to some node
	 *
	 * @param node Current node in tree
	 * @param bid The bid to be added
	 */
	private static void addNode(Node node, Bid bid) {
		
		// Searches left children to add a bid
		if (Integer.parseInt(bid.bidID) < Integer.parseInt(node.bid.bidID)) {
			
			// Adds a bid to the left child of a node
			if (node.leftTree == null) { 
				node.leftTree = new Node();
				node.leftTree.bid = bid;
				return;
			}
			
			else addNode(node.leftTree, bid); // Continues to search left
		}
		
		// Searches right children to add a bid
		else {
			
			// Adds a bid to the right child of a node
			if (node.rightTree == null) {
				node.rightTree = new Node();
				node.rightTree.bid = bid;
				return;
			}
			
			else addNode(node.rightTree, bid); // Continues to search left
		}
	}
	
	/**
	 * Traverses and prints the bids in the tree in numeric order
	 *
	 * @param node Current node in tree
	 */
	private static void inOrder(Node node) {
		if(node == null) return;
	    inOrder(node.leftTree); // Traverses the tree to the left
	    System.out.println(node.bid); // Prints the bid of the current node
	    inOrder(node.rightTree); // Traverses to the tree to the right		
	}
	
	/**
	 * Remove a bid from the tree node
	 *
	 * @param node Current node in tree
	 * @param bidID The bid Id of the bid to be removed
	 */
	private static Node removeNode(Node node, String bidID) {
		Node parent = null;
		Node current = node;
		
		while (current != null) { // Search for node
			if (Integer.parseInt(current.bid.bidID) == Integer.parseInt(bidID)) { // Node found
				if ((current.leftTree == null) && (current.rightTree == null)) { // Remove leaf
					if (parent == null) root = null; // Node is root
				
					else if (parent.leftTree == null) parent.leftTree = null;
				
					else parent.rightTree = null;
				}
			
				// Remove node with only left child
				else if ((current.leftTree != null) && (current.rightTree == null)) { 
					if (parent == null) root = current.leftTree; // Node is root 
        
					else if (parent.leftTree == current) parent.leftTree = current.leftTree;
                 
					else  parent.rightTree = current.leftTree;
                
				}
			
				// Remove node with only right child   
				else if ((current.leftTree == null) && (current.rightTree != null)) {    
					if (parent == null) root = current.rightTree; // Node is root
                
					else if (parent.leftTree == current) parent.leftTree = current.rightTree;
                
					else parent.rightTree = current.rightTree;
				}
			
				else { // Remove node with two children
					// Find successor (leftmost child of right subtree)
					Node successor = current.rightTree;
					while (successor.leftTree != null) {
						successor = successor.leftTree;
					}
					// Copy successor bid information to current node
					current.bid = successor.bid; 
					// Remove successor from right subtree and set current to the memory location of the 
					// successor node 
					current = removeNode(current.rightTree, successor.bid.bidID);     
				}
				return current; // Node found and removed
			}
			
	        else if (Integer.parseInt(current.bid.bidID) < Integer.parseInt(bidID)) { // Search right
	            parent = current;
	            current = current.rightTree;
	        }
			
	        else { // Search left
	            parent = current;
	            current = current.leftTree;
	        }
	    }
		
	    return null; // Node not found
	}
	
	/**
	 * Default constructor
	 */
	public BinarySearchTree() {
		root = null;
	}
	
	/**
	 * Traverse the tree in order
	 */
	public static void InOrder() {
		inOrder(root);
	}
	
	/**
	 * Insert a bid
	 * 
	 * @param bid which is to be added to the BST
	 */
	public static void Insert(Bid bid) {
	    if (root == null) { // Empty tree case
	        root = new Node();
	        root.bid = bid;
	    }
	    
	    else addNode(root, bid); // When a root node exists
	}
	
	/**
	 * Remove a bid
	 * 
	 * @param bidID To remove the node/bid containing the bidID string
	 */
	public static void Remove(String bidID) {
	    Node node = removeNode(root, bidID);
	    // Sets the node equal to null that is returned from removeNode if found
	    if (node != null) node = null;
	}
	
	/**
	 * Search for a bid
	 * 
	 * @param bidID The string to search for the corresponding node/bid
	 */
	public static Bid Search(String bidID) {
	    Node current = root;
	    
	    while(current != null) {
	        // Returns the bid if a matching bidId is founds
	        if (Integer.parseInt(bidID) == Integer.parseInt(current.bid.bidID)) return current.bid;
	        else if (Integer.parseInt(bidID) < Integer.parseInt(current.bid.bidID)) { // Searches left
	            current = current.leftTree;
	        }
	        else current = current.rightTree; // Searches right
	    }
	    Bid bid = new Bid();
	    return bid; // Returns an empty bid if the bid is not found
	}	
}

