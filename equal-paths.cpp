#ifndef RECCHECK
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int initialDepth(Node* root) {
	if (root == NULL) {
		return 0;
	}

	if (root->right) {
		return 1 + initialDepth(root->right);
	}
	
	if (root->left) {
		return 1 + initialDepth(root->left);
	}

	else {
		return 1;
	}
}

bool getMaxDepth(Node* root, int levels, int path_depth) {
	// assume all subtrees are true at first
	bool leftSubtree = true;
	bool rightSubtree = true;
	
	if (root->left == NULL && root->right == NULL) {
		return levels == path_depth;
	}

	if (root->left) {
		leftSubtree = getMaxDepth(root->left, levels + 1, path_depth);
		}
		
		if (root->right) {
		rightSubtree = getMaxDepth(root->right, levels + 1, path_depth);
		}

		return leftSubtree == rightSubtree;
}

// you can do a depth first search and hold the max depth and ensure
// all other depths are that max depth
bool equalPaths(Node * root)
{
	if (root == NULL) {
		return true;
	}
	
	// get path_depth (just a random path in the root. doesnt need to be max/min)
	int path_depth = initialDepth(root);

	// if any path has a different depth than the given path,
	// return false
	return getMaxDepth(root, 1, path_depth);
}

