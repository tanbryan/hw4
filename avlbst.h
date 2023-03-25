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
* An overridden function for getting the parent since a static_cast is necessary to make sure
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
    void leftRotate(AVLNode<Key, Value>* node);
    void rightRotate(AVLNode<Key, Value>* node);
    //void nBalance (AVLNode<Key, Value>* q, AVLNode<Key, Value>* a, AVLNode<Key, Value>* b);
    void removeFix(AVLNode<Key, Value>* n, int diff);
    void insertFix(AVLNode<Key, Value>* a, AVLNode<Key, Value>* b);
};


template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* node){

    AVLNode<Key,Value>* right = node->getRight();
    AVLNode<Key,Value>* parent = node->getParent();
    right->setParent(parent);

    if(parent == NULL){
        this->root_ = right;
    }
    else if(parent->getLeft() == node){
        parent->setLeft(right);
    }
    else if(parent->getRight() == node){
        parent->setRight(right);
    }

    AVLNode<Key,Value>* child = right->getLeft();
    node->setParent(right);
    node->setRight(child);
    right->setLeft(node);
    if(child != NULL){
        child->setParent(node);	
    }
} 

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* node){

    AVLNode<Key,Value>* left = node->getLeft();
    AVLNode<Key,Value>* parent = node->getParent();
    left->setParent(parent);

    if(parent == NULL){
        this->root_ = left;
    }
    else if(parent->getLeft() == node){
        parent->setLeft(left);
    }
    else if(parent->getRight() == node){
        parent->setRight(left);
    }

    AVLNode<Key,Value>* child = left->getRight();
    node->setParent(left);
    node->setLeft(child);
    left->setRight(node);
    if(child != NULL){
        child->setParent(node);	
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key, Value>* a, AVLNode<Key, Value>* b)
{
    AVLNode<Key, Value>* q = a->getParent();

    // base case
    if(a == NULL || a->getParent() == NULL){
      return;
    }
    
    // if there is left child 
    if(a == q->getLeft()){ 
        q->setBalance(q->getBalance() - 1);
        // case1
        if(q->getBalance() == 0){
            return;
        }
        // case2
        else if(q->getBalance() == -1){
            insertFix(q, a);
        }
        // case3
        else if(q->getBalance() == -2){
          // zigzag case
          if(b == a->getLeft()){
              rightRotate(q);
              a->setBalance(0);
              q->setBalance(0);
          }
          // zigzag case
          else{ 
              leftRotate(a);
              rightRotate(q);
              // case A
              if(b->getBalance() == -1) {
                  a->setBalance(0);
                  q->setBalance(1);
              } 
              // case B
              else if(b->getBalance() == 0) {
                  a->setBalance(0);
                  q->setBalance(0);
              } 
              // case C
              else{
                  a->setBalance(-1);
                  q->setBalance(0);
              }
              b->setBalance(0);
          }
        }
    }

    // if there is right child
    else{ 
        q->setBalance(q->getBalance() + 1);
        // case1
        if (q->getBalance() == 0) {
            return;
        }
        // case 2
        else if (q->getBalance() == 1) {
            insertFix(q, a);
        }
        // case3
        else if (q->getBalance() == 2) {
          // zigzag case 
            if (b == a->getRight()) { 
              leftRotate(q);
              a->setBalance(0);
              q->setBalance(0);
            } 
          // zigzag case
            else { 
              rightRotate(a);
              leftRotate(q);
              // case A
              if (b->getBalance() == 1) {
                a->setBalance(0);
                q->setBalance(-1);
              } 
              // case B
              else if (b->getBalance() == 0) {
                a->setBalance(0);
                q->setBalance(0);
              } 
              // case C
              else {
                a->setBalance(1);
                q->setBalance(0);
              }
            b->setBalance(0);
            } 
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int diff)
{

  //base case
	if(n == NULL){
    return;
  }
  
  AVLNode<Key,Value>* p = n->getParent();
  int ndiff = -1;
	if(p!=NULL){
    if(n==p->getLeft()){
      ndiff = 1;
    }
  }
	
  int nb = n->getBalance();
	// for diff = -1
	if(diff == -1){
		//case1: nb + diff = -2
		if(nb + diff == -2){
			AVLNode<Key,Value>* c = n->getLeft();
      int cb = c->getBalance();
			//case 1a(zigzag): cb = -1
			if(cb == -1){
				rightRotate(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
			//case 1b(zigzag): cb = 0
			else if(cb == 0){
				rightRotate(n);
				n->setBalance(-1);
				c->setBalance(1);
				return;
			}
      //case 1c(zigzag): cb = 1
			else if(cb == 1){
				AVLNode<Key,Value>* g = c->getRight();
        int gb = g->getBalance();
				leftRotate(c);
				rightRotate(n);
				if(gb == 1){
					n->setBalance(0);
					c->setBalance(-1);
				}
				else if(gb == 0){
					n->setBalance(0);
					c->setBalance(0);
				}
				else if(gb == -1){
					n->setBalance(1);
					c->setBalance(0);
				}
				g->setBalance(0);
				removeFix(p,ndiff);
			}
		}
    //case2: nb + diff = 0
		else if(nb+ diff ==0){
				n->setBalance(0);
				removeFix(p, ndiff);
		}
	//case3: nb + diff = -1
	  else if (nb+ diff == -1){
			n->setBalance(-1);
			return;
		}
  
	}
  // for diff = 1
	else if(diff == 1){
		// case1: nb + diff = 2
		if(nb + diff == 2){
			AVLNode<Key,Value>* c = n->getRight();
      int cb = c->getBalance();
			//case 1a(zigzag): cb = -1
      if(cb == -1){
				AVLNode<Key,Value>* g = c->getLeft();
        int gb = g->getBalance();
				rightRotate(c);
				leftRotate(n);
				if(gb == -1){
					n->setBalance(0);
					c->setBalance(1);
				}
				else if(gb == 0){
					n->setBalance(0);
					c->setBalance(0);
				}
				else if(gb == 1){
					n->setBalance(-1);
					c->setBalance(0);
				}
				g->setBalance(0);
				removeFix(p,ndiff);
			}
      //case 1b(zigzag): cb = 0
      else if(cb == 0){
				leftRotate(n);
				n->setBalance(1);
				c->setBalance(-1);
				return;
			}
      //case 1c(zigzag): cb = 1
			else if(cb == 1){
				leftRotate(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
		}
    //case2: nb + diff = 0
		else if(nb+ diff ==0){
			n->setBalance(0);
			removeFix(p, ndiff);
		}
		//case3: nb + diff = 1
		else if (nb+ diff == 1){
			n->setBalance(1);
			return;
		}
	}
}
    

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    
    // case1: if already exists in the tree, simply return
    if(this->internalFind(new_item.first) != NULL){
			this->internalFind(new_item.first)->setValue(new_item.second);
			return;
		}
    // case2: if the root is null, set the new node as the root
    if(this->root_ == NULL){
			this->root_ = new_node;
			return;
		}
    
    // traverse the tree to find the parent node for the new node
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (true) {
      // if the new key is less than current, goes to left child
        if (new_node->getKey() < parent->getKey()) {
            if (parent->getLeft() != NULL) {
                parent = parent->getLeft();
            } 
            else {
                parent->setLeft(new_node);
                new_node->setParent(parent);
                break;
            }
        } 
        //otherwise, it goes to the right child.
        else {
            if (parent->getRight() != NULL) {
                parent = parent->getRight();
            } else {
                parent->setRight(new_node);
                new_node->setParent(parent);
                break;
            }
        }
    }
    
    // if balance was already -1 or 1, balance the tree to sets it to 0 
    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
        return;
    } 
    // otherwise, adjust it and fix imbalance
    else {
        int new_balance = (parent->getLeft() == new_node) ? -1 : 1;
        parent->setBalance(new_balance);
        insertFix(parent, new_node);
    }
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    // if no node is found, return
    if (temp == NULL) {
        return;
    }

    // if two children, swap it with its predecessor
    if (temp->getLeft() != NULL && temp->getRight() != NULL) {
        AVLNode<Key, Value>* predecessor = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
        nodeSwap(temp, predecessor);
    }

    AVLNode<Key, Value> *child = temp->getLeft() != NULL ? temp->getLeft() : temp->getRight();
    AVLNode<Key, Value>* parent = temp->getParent();
    if (child != NULL) {
        child->setParent(temp->getParent());
    }

    int diff = 0;
    // if temp is the root
    if (temp->getParent() == NULL) {
        this->root_ = child;
    }
    else {
        // if temp is the left child
        if (temp == parent->getLeft()) {
            parent->setLeft(child);
            diff = 1;
        }
        // if temp is the right child
        else {
            parent->setRight(child);
            diff = -1;
        }
    }
    // delete the node
    delete temp;
    removeFix(parent, diff);
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
