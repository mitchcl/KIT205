#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

// create an empty bst
BST new_bst() {
	BST temp;
	temp.root = NULL;
	return temp;
}

// recursive function to find a value and return the containing node
BSTNodePtr find_bst_node(BSTNodePtr self, int n) {
	if (self == NULL || (n == self->data_item)) { /* not present, or here */
		return self;
	}
	else if (n < self->data_item) {
		return find_bst_node(self->left, n);
	}
	else {
		return find_bst_node(self->right, n);
	}
}

// find a value in the tree and return the node
BSTNodePtr find_bst(BST* self, int n) {
	return find_bst_node(self->root, n);
}

// recursive function to insert a value
BSTNodePtr insert_bst_node(BSTNodePtr self, int n) {
	if (self == NULL) { /* found where to put it*/
		self = malloc(sizeof * self);
		self->data_item = n;
		self->left = self->right = NULL;
	}
	else if (n < self->data_item) {
		self->left = insert_bst_node(self->left, n);
	}
	else if (n > self->data_item) {
		self->right = insert_bst_node(self->right, n);
	}
	return self;
}

// insert a value into the tree
void insert_bst(BST* self, int n) {
	self->root = insert_bst_node(self->root, n);
}

// helper function for delete
BSTNodePtr min_node(BSTNodePtr self) {
	BSTNodePtr current = self;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

// recursive function to delete a value
BSTNodePtr delete_bst_node(BSTNodePtr self, int n) {
	if (self != NULL) {
		if (n == self->data_item) { // found item 
			if (self->left != NULL && self->right != NULL) {
				// two child case 
				BSTNodePtr successor = min_node(self->right);
				self->data_item = successor->data_item;
				self->right = delete_bst_node(self->right, self->data_item);
			}
			else { // one or zero child case 
				BSTNodePtr to_free = self;
				if (self->left) {
					self = self->left;
				}
				else {
					self = self->right;
				}
				free(to_free);
			}
		}
		else if (n < self->data_item) {
			self->left = delete_bst_node(self->left, n);
		}
		else {
			self->right = delete_bst_node(self->right, n);
		}
	}
	return self;
}

// delete a value from the tree
void delete_bst(BST* self, int n) {
	self->root = delete_bst_node(self->root, n);
}

// recursive function to print in order
void print_in_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf("(");
		print_in_order_bst_node(self->left);
		printf(" %d ", self->data_item);
		print_in_order_bst_node(self->right);
		printf(")");
	}
}

// print the tree in order
void print_in_order_bst(BST* self) {
	print_in_order_bst_node(self->root);
}

void print_pre_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		printf(" %d ", self->data_item);
		print_pre_order_bst_node(self->left);
		print_pre_order_bst_node(self->right);
	}
}

void print_pre_order_bst(BST* self) {
	print_pre_order_bst_node(self->root);
}

void print_post_order_bst_node(BSTNodePtr self) {
	if (self == NULL) {
		printf("_");
	}
	else {
		print_post_order_bst_node(self->left);
		print_post_order_bst_node(self->right);
		printf(" %d ", self->data_item);
	}
}

void print_post_order_bst(BST* self) {
	print_post_order_bst_node(self->root);
}


// recursive function to detroy all node
void destroy_bst_node(BSTNodePtr self) {
	if (self != NULL) {
		destroy_bst_node(self->left);
		self->left = NULL;
		destroy_bst_node(self->right);
		self->right = NULL;
		free(self);
	}
}

// destroy the tree
void destroy_bst(BST* self) {
	destroy_bst_node(self->root);
	self->root = NULL;
}

void bst_adhoc_test() {
	BST tree = new_bst();
	int quit = 0;
	int data;
	while (quit == 0) {
		printf("Enter some data: ");
		scanf_s("%d", &data);
		if (data != 0) {
			insert_bst(&tree, data);
		}
		else {
			quit = 1;
		}
	}
	printf("In-order: ");
	print_in_order_bst(&tree);
	printf("\n");

	printf("Pre-order: ");
	print_pre_order_bst(&tree);
	printf("\n");

	printf("Post-order: ");
	print_post_order_bst(&tree);
	printf("\n");
}

void bst_test() {
    printf("\n========== BST UNIT TEST ==========\n");

    // Test 1: Creating a new BST
    printf("\nTest 1: Creating a new BST...\n");
    BST tree = new_bst();
    if (tree.root == NULL) {
        printf("PASS: Created empty BST successfully\n");
    }
    else {
        printf("FAIL: New BST should have a NULL root\n");
    }

    // Test height of empty tree
    printf("\nTest height of empty tree...\n");
    int h = height_bst(&tree);
    printf("Height of empty tree: %d (Expected: -1) %s\n", h, (h == -1) ? "PASS" : "FAIL");

    // Test 2: Insert and find operations
    printf("\nTest 2: Insert and find operations...\n");
    insert_bst(&tree, 50);

    // Test height of single node tree
    h = height_bst(&tree);
    printf("Height of single node tree: %d (Expected: 0) %s\n", h, (h == 0) ? "PASS" : "FAIL");

    insert_bst(&tree, 30);
    insert_bst(&tree, 70);

    // Test height of balanced tree with height 1
    h = height_bst(&tree);
    printf("Height of balanced tree with 3 nodes: %d (Expected: 1) %s\n", h, (h == 1) ? "PASS" : "FAIL");

    insert_bst(&tree, 20);
    insert_bst(&tree, 40);
    insert_bst(&tree, 60);
    insert_bst(&tree, 80);

    // Test height of balanced tree with height 2
    h = height_bst(&tree);
    printf("Height of balanced tree with 7 nodes: %d (Expected: 2) %s\n", h, (h == 2) ? "PASS" : "FAIL");

    printf("Tree after insertions: ");
    print_in_order_bst(&tree);
    printf("\n");

    // Test find function - existing values
    BSTNodePtr found = find_bst(&tree, 50);
    printf("Finding 50: %s\n", found != NULL ? "PASS" : "FAIL");

    found = find_bst(&tree, 30);
    printf("Finding 30: %s\n", found != NULL ? "PASS" : "FAIL");

    found = find_bst(&tree, 80);
    printf("Finding 80: %s\n", found != NULL ? "PASS" : "FAIL");

    // Test find function - non-existing value
    found = find_bst(&tree, 55);
    printf("Finding 55 (should not exist): %s\n", found == NULL ? "PASS" : "FAIL");

    // Test 3: Testing traversal methods
    printf("\nTest 3: Tree traversal methods...\n");
    printf("In-order traversal: ");
    print_in_order_bst(&tree);
    printf("\n");

    printf("Pre-order traversal: ");
    print_pre_order_bst(&tree);
    printf("\n");

    printf("Post-order traversal: ");
    print_post_order_bst(&tree);
    printf("\n");

    // Test 4: Deletion - single cases
    printf("\nTest 4: Testing deletion operations...\n");

    // Case 1: Deleting a leaf node (20)
    printf("Deleting leaf node (20)...\n");
    delete_bst(&tree, 20);
    printf("After deleting 20: ");
    print_in_order_bst(&tree);
    printf("\n");
    found = find_bst(&tree, 20);
    printf("Node 20 deleted: %s\n", found == NULL ? "PASS" : "FAIL");

    // Test height after deletion
    h = height_bst(&tree);
    printf("Height after deleting leaf node: %d (Expected: 2) %s\n", h, (h == 2) ? "PASS" : "FAIL");

    // Case 2: Deleting a node with one child (60)
    printf("Inserting 65 to test one-child deletion case...\n");
    insert_bst(&tree, 65);
    printf("After inserting 65: ");
    print_in_order_bst(&tree);
    printf("\n");

    printf("Deleting node with one child (60)...\n");
    delete_bst(&tree, 60);
    printf("After deleting 60: ");
    print_in_order_bst(&tree);
    printf("\n");
    found = find_bst(&tree, 60);
    printf("Node 60 deleted: %s\n", found == NULL ? "PASS" : "FAIL");

    // Test 8: Testing unbalanced trees for height function
    printf("\nTest 8: Testing height with unbalanced trees...\n");
    BST unbalanced_tree = new_bst();

    // Create right-skewed tree
    insert_bst(&unbalanced_tree, 10);
    insert_bst(&unbalanced_tree, 20);
    insert_bst(&unbalanced_tree, 30);
    insert_bst(&unbalanced_tree, 40);

    printf("Right-skewed tree: ");
    print_in_order_bst(&unbalanced_tree);
    printf("\n");

    h = height_bst(&unbalanced_tree);
    printf("Height of right-skewed tree with 4 nodes: %d (Expected: 3) %s\n", h, (h == 3) ? "PASS" : "FAIL");

    // Create left-skewed tree
    BST left_skewed_tree = new_bst();
    insert_bst(&left_skewed_tree, 40);
    insert_bst(&left_skewed_tree, 30);
    insert_bst(&left_skewed_tree, 20);
    insert_bst(&left_skewed_tree, 10);

    printf("Left-skewed tree: ");
    print_in_order_bst(&left_skewed_tree);
    printf("\n");

    h = height_bst(&left_skewed_tree);
    printf("Height of left-skewed tree with 4 nodes: %d (Expected: 3) %s\n", h, (h == 3) ? "PASS" : "FAIL");

    // Clean up the unbalanced trees
    destroy_bst(&unbalanced_tree);
    destroy_bst(&left_skewed_tree);

    // Continue with rest of original test function...
    // Case 3: Deleting a node with two children (50 - root)
    printf("Deleting node with two children (50 - root)...\n");
    delete_bst(&tree, 50);
    printf("After deleting 50: ");
    print_in_order_bst(&tree);
    printf("\n");
    found = find_bst(&tree, 50);
    printf("Node 50 deleted: %s\n", found == NULL ? "PASS" : "FAIL");

    // Test 5: Edge cases
    printf("\nTest 5: Testing edge cases...\n");

    // Delete non-existent node
    printf("Deleting non-existent node (100)...\n");
    delete_bst(&tree, 100);
    printf("After attempting to delete 100: ");
    print_in_order_bst(&tree);
    printf("\n");

    // Find in empty tree
    printf("Testing find in empty tree...\n");
    BST empty_tree = new_bst();
    found = find_bst(&empty_tree, 10);
    printf("Finding in empty tree returns NULL: %s\n", found == NULL ? "PASS" : "FAIL");

    // Insert duplicate value
    printf("Testing insertion of duplicate value...\n");
    printf("Before inserting duplicate 30: ");
    print_in_order_bst(&tree);
    printf("\n");
    insert_bst(&tree, 30);
    printf("After inserting duplicate 30: ");
    print_in_order_bst(&tree);
    printf("\n");

    // Test 6: Destroying trees
    printf("\nTest 6: Destroying trees...\n");
    destroy_bst(&tree);
    if (tree.root == NULL) {
        printf("PASS: Tree destroyed successfully\n");
    }
    else {
        printf("FAIL: Tree root should be NULL after destruction\n");
    }

    destroy_bst(&empty_tree);
    if (empty_tree.root == NULL) {
        printf("PASS: Empty tree destroyed successfully\n");
    }
    else {
        printf("FAIL: Empty tree root should be NULL after destruction\n");
    }

    // Test 7: Create and test another tree
    printf("\nTest 7: Creating a final test tree...\n");
    BST final_tree = new_bst();
    insert_bst(&final_tree, 100);
    insert_bst(&final_tree, 50);
    insert_bst(&final_tree, 150);
    insert_bst(&final_tree, 25);
    insert_bst(&final_tree, 75);
    insert_bst(&final_tree, 125);
    insert_bst(&final_tree, 175);

    printf("Final test tree: ");
    print_in_order_bst(&final_tree);
    printf("\n");

    // Test height of final test tree (should be 2 - perfectly balanced)
    h = height_bst(&final_tree);
    printf("Height of final test tree: %d (Expected: 2) %s\n", h, (h == 2) ? "PASS" : "FAIL");

    // Delete all nodes in various orders to test all deletion cases
    delete_bst(&final_tree, 25);   // Delete leaf
    delete_bst(&final_tree, 150);  // Delete with one child
    delete_bst(&final_tree, 50);   // Delete with two children
    delete_bst(&final_tree, 100);  // Delete root
    delete_bst(&final_tree, 75);
    delete_bst(&final_tree, 125);
    delete_bst(&final_tree, 175);

    if (final_tree.root == NULL) {
        printf("PASS: All nodes deleted successfully\n");
    }
    else {
        printf("FAIL: Tree should be empty after all deletions\n");
    }

    destroy_bst(&final_tree);

    printf("\n========== BST UNIT TEST COMPLETE ==========\n");
}


// recursive function to calculate the height of a tree
int height_bst_node(BSTNodePtr self) {
    if (self == NULL) {
        return -1;  // height of empty tree is -1
    }
    else {
        // calculate height of left and right subtrees
        int left_height = height_bst_node(self->left);
        int right_height = height_bst_node(self->right);

        // height is the maximum of left and right subtree heights plus 1
        return (left_height > right_height ? left_height : right_height) + 1;
    }
}

// calculate the height of the tree
int height_bst(BST* self) {
    return height_bst_node(self->root);
}