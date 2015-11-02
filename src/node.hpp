#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <list>
#include <vector>
#include <cassert>

#include "node.hpp"

class Node {

	public:

		typedef unsigned index_type;
		typedef unsigned value_type;
		typedef std::vector<Node*> nodecontainer_type;

		Node(index_type id)
			: id_(id)
			, v_(1) // value = 1 by default
			, childcount_(0)
			, parcount_(0)
			, childnodes_(nodecontainer_type(childcount_))
		{
			// NOTE: value = 1 indicates that a node is a source node
			// (no other nodes pointing to it)
			// by default every node that is constructed is a source node
		}

		void addChild(Node& child);
		bool hasChild(Node& childCandidate);

		inline value_type getValue() const {
			return v_;
		}
		
		inline value_type getID() const {
			return id_;
		}

		inline void setValue(value_type r) {
			v_ = r;
		}

		inline Node* getChild(index_type i) {
			assert(i<childcount_);
			return childnodes_[i];
		}

		inline unsigned getChildCount() const {
			return childcount_;
		}

		inline bool requestValueUpdate() {
			--parcount_;
			assert(parcount_>=0);
			return (parcount_ == 0);
		}


	private:

		const index_type id_;
		unsigned childcount_;
		value_type v_;
		nodecontainer_type childnodes_;
		
	protected:
		unsigned parcount_;

};


#endif // NODE_HPP
