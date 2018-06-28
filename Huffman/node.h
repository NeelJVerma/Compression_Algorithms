/** 
  * Name: Neel Verma
  * Current File: node.h
  * Description: Abstracts a node in the huffman tree.
  */

#ifndef _NODE_H_
#define _NODE_H_

#include <cstdlib>

struct Node {
  char data;
  unsigned frequency;
  Node* left;
  Node* right;

  Node(const char& data, const unsigned& frequency) {
    left = right = NULL;
    this->data = data;
    this->frequency = frequency;
  }
};

struct Compare {
  bool operator()(const Node* left, const Node* right) {
    return left->frequency > right->frequency;
  }
};

#endif