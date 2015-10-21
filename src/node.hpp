#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <list>
#include <vector>

#include "node.hpp"

class Node {

	public:

		typedef unsigned index_type;
		typedef unsigned value_type;
		typedef std::vector<Node*> nodecontainer_type;

		Node(index_type id)
			: id_(id)
			, v_(1) // value = 1 by default
			, childnodes_(nodecontainer_type(0))
		{
			// NOTE: value = 1 indicates that a node is a source node
			// (no other nodes pointing to it)
			// by default every node that is constructed is a source node
		}

		void addChild(Node& child);

		inline value_type getValue() const {
			return v_;
		}
		
		inline value_type getID() const {
			return id_;
		}

		inline void setValue(value_type r) {
			v_ = r;
		}

		inline bool getChild() const;


	private:

		const index_type id_;
		value_type v_;
		nodecontainer_type childnodes_;

};


#endif // NODE_HPP
