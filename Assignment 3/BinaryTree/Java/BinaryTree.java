/**
 * A runnable class that implements and tests an ordered binary tree of integer values.
 */
public class BinaryTree {

  /**
   * A node of the binary tree containing the node's integer value 
   * and pointers to its right and left children (or null).
   */
  static class Node {
    int  value;
    Node left   = null;
    Node right  = null;
    
    /**
     * Create a new node with no children.
     */
    Node (int value) {
      this.value = value;
    }
    
    /**
     * Insert the node n into the binary tree rooted by this node.
     */
    void insertNode (Node n) {
      if (n.value <= value) {
        if (left==null)
          left = n;
        else
          left.insertNode (n);
      } else {
        if (right==null)
          right = n;
        else
          right.insertNode (n);
      }
    }
    
    /**
     * Insert value into the binary tree rooted by this node, creating a new node for value.
     */
    void insert (int value) {
      Node n = new Node (value);
      insertNode (n);
    }
    
    /**
     * Print the contents entire binary tree in order of ascending integer value.
     */
    void printInOrder() {
      if (left != null)
        left.printInOrder();
      System.out.printf ("%d\n", value);
      if (right != null)
        right.printInOrder();
    }
    
  }
  
  /**
   * Create a node to root a new tree, populate it with a few values, and print it.
   */
  public static void main (String[] args) {
    Node root = new Node (100);
    root.insert (10);
    root.insert (120);
    root.insert (130);
    root.insert (90);
    root.insert (5);
    root.insert (95);
    root.insert (121);
    root.insert (131);
    root.insert (1);
    root.printInOrder();
  }

}