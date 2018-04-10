//============================================================================
// Name        : Node.java
// Author      : Christopher Willoughby
// Course      : CS-499
// Date        : 28Mar18
// Version     : 1.0
// Description : Implements the Node data type class
//============================================================================

package algorithmProject;

public class Node {
	public Node leftTree;
	public Node rightTree;
	public Bid bid;
	
	/**
	 * Constructor for the Node data type
	 */
	public Node() {
		this.leftTree = null;
		this.rightTree = null;
		this.bid = null;
	}
}

