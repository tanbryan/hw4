#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here


int getHeight(Node* root) {
  // base case: tree is empty, so its height is 0
    if (root == NULL) {
        return 0;
    } 
    else {
      // compute the height of the left and right subtrees
        int leftHeight = getHeight(root->left);
        int rightHeight = getHeight(root->right);
        // get the height of left and right subtrees
        return 1 + max(leftHeight, rightHeight);
    }
}


bool equalPaths(Node* root) {
    // Add your code below

    // base case: all paths of equal length of 0
    if (root == NULL)
        return true;

    // compute the height of the left and right subtrees
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    //case1: the heights of the left and right are equal and have equal path lengths
    if (leftHeight == rightHeight) {
      if (equalPaths(root->left) && equalPaths(root->right)){
        return true;
      }
    }
    //case2: the heights of the right = 0 and left = 0 as well
    if (rightHeight ==0 ){
      if(equalPaths(root->left)){
        return true;
      }
    }
    //case3: the heights of the left = 0 and right = 0 as well
    if (leftHeight ==0 ){ 
      if(equalPaths(root->right)){
        return true;
      }
    }

    //if tree has paths of different lengths, return false
    return false;
}



