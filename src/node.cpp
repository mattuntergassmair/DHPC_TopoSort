#include "node.hpp"

typedef Node::value_type value_type;
typedef Node::nodecontainer_type nodecontainer_type;


void Node::addChild(Node& child) {
	childnodes_.push_back(&child); // add child to container
	child.setValue(0); // set value to 0 - child is not a root anymore
} // ;
