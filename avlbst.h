#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
ting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void rotateLeft(AVLNode<Key, Value>* node) {
			// TODO
			if (node == NULL || node->getRight() == NULL) {
				return;
			}

			AVLNode<Key, Value>* parent = node->getParent();
			AVLNode<Key, Value>* rightChild = node->getRight();
			AVLNode<Key, Value>* rightChildLeft = rightChild->getLeft();

			int8_t rightBalance = rightChild->getBalance();
			int8_t nodeBalance = node->getBalance();

			rightChild->setParent(parent);

			// if working with root node, update pointer
			if (parent == NULL) {
				this->root_ = rightChild;
			} else {
				if (parent->getLeft() == node) {
					parent->setLeft(rightChild);
				} else {
					parent->setRight(rightChild);
					}
				}

			// move left subtree of rightChild to the right subtree of node
			node->setRight(rightChildLeft);
			if (rightChildLeft != NULL) {
				rightChildLeft->setParent(node);
			}

			// make original node to left child of right child
			rightChild->setLeft(node);
			node->setParent(rightChild);

			// set balance to each modified node in subtree
			int8_t newNodeBalance = nodeBalance - 1 - std::max(rightBalance, (int8_t)0);
			int8_t newRightChildBalance = rightBalance - 1 + std::min(newNodeBalance, (int8_t)0);

			node->setBalance(newNodeBalance);
			rightChild->setBalance(newRightChildBalance);
		}

		void rotateRight(AVLNode<Key, Value>* node) {
			// base case or don't need a right rotation
			if (node == NULL || node->getLeft() == NULL) {
				return;
			}

			AVLNode<Key, Value>* parent = node->getParent();
			AVLNode<Key, Value>* leftChild = node->getLeft();
			AVLNode<Key, Value>* leftRightChild = leftChild->getRight();

			int8_t leftBalance = leftChild->getBalance();
			int8_t nodeBalance = node->getBalance();

			leftChild->setParent(parent);

			// if working with root node, update pointer
			if (parent == NULL) {
				this->root_ = leftChild;
			} else {
				if (parent->getLeft() == node) {
					parent->setLeft(leftChild);
				} else {
					parent->setRight(leftChild);
					}
				}

			// move left subtree of rightChild to the right subtree of node
			node->setLeft(leftRightChild);
			if (leftRightChild != NULL) {
				leftRightChild->setParent(node);
			}

			// make original node to left child of right child
			leftChild->setRight(node);
			node->setParent(leftChild);

			// set balance to each modified node in subtree
			int8_t newNodeBalance = nodeBalance + 1 - std::min(leftBalance, (int8_t)0);
			int8_t newLeftChildBalance = leftBalance + 1 + std::max(newNodeBalance, (int8_t)0);

			node->setBalance(newNodeBalance);
			leftChild->setBalance(newLeftChildBalance);
		}

		void insertHelper(AVLNode<Key, Value>* node) {
			AVLNode<Key, Value>* parent = node;

			while (parent != NULL) {
				int8_t parent_balance = parent->getBalance();
				if (parent_balance == 0) {
					break;
				}

				if (parent_balance == 1 || parent_balance == -1) {
					AVLNode<Key, Value>* gp = parent->getParent();
					if (gp != NULL) {
						if (gp->getLeft() == parent) {
							gp->updateBalance(-1);
						} else {
							gp->updateBalance(1);
						}

						parent = gp;
					} else {
						break;
					}
				}

				else {
					if (parent_balance == 2) {
						AVLNode<Key, Value>* rightChild = parent->getRight();
						// zig-zag
						if (rightChild != NULL && rightChild->getBalance() == -1) {
							rotateRight(rightChild);
						}
						// zig-zig
						rotateLeft(parent);
						break;
					} else {
						AVLNode<Key, Value>* leftChild = parent->getLeft();
						// zig-zag
						if (leftChild != NULL && leftChild->getBalance() == 1) {
							rotateLeft(leftChild);
						}
						// zig-zig
						rotateRight(parent);
						break;
					}
				}
			}
		}

		void removeHelper(AVLNode<Key, Value>* node, bool isLeftChild) {

			while (node != NULL) {
				// update initial balance
				if (isLeftChild) {
					node->updateBalance(1);
				} else {
					node->updateBalance(-1);
				}

			int balance = node->getBalance();
			AVLNode<Key, Value>* gp = node->getParent();

			if (balance == 1 || balance == -1) {
				break;
			}
	
			// right heavy
			if (balance == 2) {
				AVLNode<Key, Value>* rightChild = node->getRight();
				if (rightChild && rightChild->getBalance() < 0) {
					rotateRight(rightChild);
				} 
					
				rotateLeft(node);
			}

			// left heavy
			else if (balance == -2) {
				AVLNode<Key, Value>* leftChild = node->getLeft();
				if (leftChild && leftChild->getBalance() > 0) {
					rotateLeft(leftChild);
				}
				
				rotateRight(node);
			}

			// if after balancing balance is not 0, continue
			if (node->getBalance() != 0) {
				break;
			}

			AVLNode<Key, Value>* parent = node->getParent();
			if (parent == NULL) {
				break;
			}

			isLeftChild = (parent && parent->getLeft() == node);
			node = parent;
			}
		}
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
	if (this->root_ == NULL) {
		this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
		return;
	}

	AVLNode<Key, Value>* curr = static_cast<AVLNode<Key,Value>*>(this->root_);
	AVLNode<Key, Value>* parent = NULL;

	while (curr != NULL) {
		parent = curr;
		if (new_item.first < curr->getKey()) {
			curr = curr->getLeft();
		} else if (new_item.first > curr->getKey()) {
			curr = curr->getRight();
		} else {
			curr->setValue(new_item.second);
			return;
		}
	}

	AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
	if (new_item.first < parent->getKey()) {
		parent->setLeft(newNode);
		parent->updateBalance(-1);
	} else {
		parent->setRight(newNode);
		parent->updateBalance(1);
	}

	if (parent->getBalance() != 0) {
		insertHelper(parent);
	}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
	
	if (nodeToRemove == NULL) {
		return;
	}

	// swap with predecessor and remove if 2 children
	if (nodeToRemove->getLeft() != NULL && nodeToRemove->getRight() != NULL) {
		AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
		nodeSwap(nodeToRemove, pred);
	}

	AVLNode<Key, Value>* parent = nodeToRemove->getParent();
	bool isLeft = false;
	if (parent != NULL) {
		isLeft = (parent->getLeft() == nodeToRemove);
	}

	BinarySearchTree<Key, Value>::remove(nodeToRemove->getKey());

	if (parent != NULL) {
		removeHelper(parent, isLeft);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
