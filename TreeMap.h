// TreeMap header file

#ifndef TREE_MAP
#define TREE_MAP
#include <vector>
#include <stack>
#include <utility>

template<typename A> class optional {
	bool _isValid;
	A _value;
	public:
	optional(): _isValid(false) {}
	optional(A e) : _isValid(true), _value(e) {}
	optional(A e, bool v) : _isValid(v), _value(e) {}
	A value() const { return _value; }
	bool validation() const {return _isValid;}
};


template <typename K, typename V, typename Compare = std::less<K>>
class TreeMap {
	
	private:
		class Node {	
			public:
				K key;
				V val;
				Node * left;
				Node * right;
				Node * parent;
				int balanceFactor;
			bool hasLeftChild() {
				return this->left != nullptr;
			}

			bool hasRightChild() {
				return this->right != nullptr;
			}

			bool isLeftChild() {
				return (this->parent != nullptr) && (this->parent->left == this);
			}

			bool isRightChild() {
				return (this->parent != nullptr) && (this->parent->right == this);
			}

			bool isRoot() {
				return this->parent == nullptr;
			}

			bool isLeaf() {
				return !(this->right != nullptr || this->left != nullptr);
			}

			bool hasAnyChildren() {
				return (this->right != nullptr || this->left != nullptr);
			}

			bool hasBothChildren() {
				return (this->right != nullptr && this.left != nullptr);
			}
			
			void replaceNodeData(K _key, V _val, Node _left, Node _right){
					this->key = _key;
					this->val = _val;
					this->left = _left;
					this->right = _right;
					if (this->left)
						this->left->parent = this;
					if (this->right)
						this->right->parent = this;
			}
			Node * findSuccessor() {
				Node * succ = nullptr;
				if (this->hasRightChild()) {
					succ = this->right->findMin();
				}
				else {
					if (this->parent != nullptr) {
						if (this->isLeftChild()) {
							succ = this->parent;
						} 
						else {
							this->parent->right = nullptr;
							succ = this->parent->findSuccessor();
							this->parent->right = this;
						}
					}
                }
			return succ;
            }
			
			Node * findMin() {
				Node * current = this;
				while (current->hasLeftChild())
					current = current->left;
				return current;
			}
			
			void spliceOut(){
				if (this->isLeaf()) {
					if (this->isLeftChild())
						this->parent->left = nullptr;
					else
						this->parent->right = nullptr;
				} else if (this->hasAnyChildren()) {
					if (this->hasLeftChild()) {
						if (this->isLeftChild())
							this->parent->left = this->left;
						else
							this->parent->right = this->left;
						this->left->parent = this->parent;
					} else {
						if (this->isLeftChild())
							this->parent->left = this->right;
						else
							this->parent->right = this->right;
						this->right->parent = this->parent;
					}
				}
			}
			
	public: 
			explicit Node(K _key, V _val, Node * _left, Node * _right, Node * _parent) {
				this->key = _key;
				this->val = _val;
				this->left = _left;
				this->right = _right;
				this->parent = _parent;
			}
			~Node() {
				delete left;
				delete right;
			}
			inline bool operator==(const Node& rhs) {
					bool k = this->key == rhs->key;
					bool v = this->val == rhs->val;
					bool l = this->left == rhs->left;
					bool r = this->right == rhs->right;
					bool p = this->parent == rhs->parent;
					return (k && v && l && r && p);
				}
			K getKey() {
				return this->key;
			}
			
			V getVal(){
				return this->val;
			}
			Node * getLeft() {
				return left;
			}
			Node * getRight() {
				return right;
			}
		};
	// End of class Node
		Node *root;
		size_t N;
		public:
		TreeMap() {
			this->N = 0;
			this->root = nullptr;
		}
		TreeMap(const TreeMap<K, V>& t) {
			this->N = 0;
			this->root = nullptr;
			std::stack<Node*> st;
			Node * current = t.root;
			while (!st.empty() || nullptr != current) {
				while (nullptr != current) {
					st.push(current);
					current = current->left;
				}
				current = st.top();
				
				this->put(current->key, current->val);
				st.pop();
				current =current->right;
			}
		}
		TreeMap<K, V> & operator=(const TreeMap<K, V>& t) {
			
				this->N = 0;
				this->root = nullptr;
				std::stack<Node*> st;
				Node * current = t.root;
				while (!st.empty() || nullptr != current) {
					while (nullptr != current) {
						st.push(current);
						current = current->left;
					}
					current = st.top();
				
					this->put(current->key, current->val);
					st.pop();
					current =current->right;
				}
				delete &t;
			
			return *this;
		}
		
		TreeMap (TreeMap<K, V>&& t) : root{t.root}, N{t.length()} {
			t.root = nullptr;
			t.N = 0;
		}
		TreeMap<K, V> & operator=(const TreeMap<K, V>&& t) {
			
				delete root;
				std::stack<Node*> st;
				Node * current = t.root;
				while (!st.empty() || nullptr != current) {
					while (nullptr != current) {
						st.push(current);
						current = current->left;
					}
					current = st.top();
				
					this->put(current->key, current->val);
					st.pop();
					current =current->right;
				}
				t.root = nullptr;
				t.N = 0;
			
			return *this;
		}
		~TreeMap() {
			delete root;
		}
		size_t length() {return this->N;}
		bool isEmpty()  {return this->root == nullptr;}
		optional<V> get(K _key) {
			Node * res = root;
			while (nullptr != res) {
				if (_key < res->key)
					res = res->left;
				else if (res->key < _key)
					res = res->right;
				else return optional<V>(res->val);
			}
			return optional<V>{};
		}
		void put(K _key, V _val) {
			if (nullptr == this->root) {
				Node * tmp = new Node(_key, _val, nullptr, nullptr, nullptr);
				this->root = tmp;
			}
			else {
				this->__put(_key, _val, this->root);
			}
			++this->N;
		}
		
		// returns vector of pairs K, V in sorted order
		std::vector<std::pair<K, V>> sorted() {
			Node * current = this->root;
			std::vector<std::pair<K, V>> v;
			std::stack<Node*> st;
			if (nullptr == current) return v;
			while (!st.empty() || nullptr != current) {
				while (nullptr != current) {
					st.push(current);
					current = current->left;
				}
				current = st.top();
				v.push_back(std::make_pair(current->key, current->val));
				st.pop();
				current = current->right;
			}
			return v;
		}
	
			
		 

		
		private: 
		void __put(K _key, V _val, Node * x) {
			if (_key < x->key) {
				if (x->hasLeftChild())
					this->__put(_key, _val, x->left);
				else {
					Node * tmp = new Node(_key, _val, nullptr, nullptr, x);
					x->left = tmp;
					this->__updateBalance(x->left);
				}
			}
			else if (_key > x->key) {
				if (x->hasRightChild())
					this->__put(_key, _val, x->right);
				else {
					Node * tmp = new Node(_key, _val, nullptr, nullptr, x);
					x->right = tmp;
					this->__updateBalance(x->right);
				}
			}
			else {
				x->val = _val;
				--this->N;
			}
		}
		// rebalancing tree private methods
		void __rotateLeft(Node * rotRoot) {
			Node * newRoot = rotRoot->right;
			rotRoot->right = newRoot->left;
			if (nullptr != newRoot->left)
				newRoot->left->parent = rotRoot;
        newRoot->parent = rotRoot->parent;
        if (rotRoot->isRoot()) {
            this->root = newRoot;
        } else {
            if (rotRoot->isLeftChild())
                rotRoot->parent->left = newRoot;
            else
                rotRoot->parent->right = newRoot;
        }
        newRoot->left = rotRoot;
        rotRoot->parent = newRoot;
        rotRoot->balanceFactor = rotRoot->balanceFactor + 1 - std::min(newRoot->balanceFactor, 0);
        newRoot->balanceFactor = newRoot->balanceFactor + 1 + std::max(rotRoot->balanceFactor, 0);
		}
		void __rotateRight(Node * rotRoot) {
			Node * newRoot = rotRoot->left;
			rotRoot->left = newRoot->right;
			if (nullptr != newRoot->right)
				newRoot->right->parent = rotRoot;
			newRoot->parent = rotRoot->parent;
			if (rotRoot->isRoot()) {
				this->root = newRoot;
			} else {
            if (rotRoot->isLeftChild())
                rotRoot->parent->left = newRoot;
            else
                rotRoot->parent->right = newRoot;
			}
			newRoot->right = rotRoot;
			rotRoot->parent = newRoot;
			rotRoot->balanceFactor = rotRoot->balanceFactor - 1 - std::max(newRoot->balanceFactor, 0);
			newRoot->balanceFactor = newRoot->balanceFactor - 1 + std::min(rotRoot->balanceFactor, 0);
		}
		void __updateBalance(Node * node) {
			if ((node->balanceFactor > 1) || (node->balanceFactor < -1)) {
				this->__rebalance(node);
				return;
			}
			if (nullptr != node->parent) {
				if (node->isLeftChild())
					++node->parent->balanceFactor;
				else if (node->isRightChild())
					--node->parent->balanceFactor;
				if (node->parent->balanceFactor != 0)
					this->__updateBalance(node->parent);
			}
		}
		void __rebalance(Node * node){
			if (node->balanceFactor < 0) {
				if (node->right->balanceFactor > 0) {
                 this->__rotateRight(node->right);
                 this->__rotateLeft(node);
				}
				else {
					this->__rotateLeft(node);
				}
			}
			else if (node->balanceFactor > 0) {
				if (node->left->balanceFactor < 0) {
					this->__rotateLeft(node->left);
					this->__rotateRight(node);
            } 
				else {
					this->__rotateRight(node);
				}
			}
		}
		// debug methods and print
		public:
		
		void printTree() {
			std::cout << "Printing tree, balance Factor: ";
			std::cout << this->root->balanceFactor;
			std::cout << "\n";
			std::cout << "Height: ";
			std::cout << this->height();
			std::cout <<"\n";
			std:: cout <<"{";
			this->__printHelp(this->root);
			std::cout <<"}\n";
		}
		
		void printPostOrder() {
				std::cout << "Printing tree, balance Factor: ";
			std::cout << this->root->balanceFactor;
			std::cout << "\n";
			std::cout << "Height: ";
			std::cout << this->height();
			std::cout <<"\n";
			std:: cout <<"{";
			this->__printHelpPostOrder(this->root);
			std::cout <<"}\n";
		}
		
		int height() {return __height(this->root);}
		int getBalance() {
			return this->root->balanceFactor;
		}
		private:
		int __height(Node * x) {
			if (nullptr == x) 
				return 0;
			else {
				int l_height = __height(x->left);
				int r_height = __height(x->right);
				return 1 + std::max(l_height, r_height);
			}
		}
		void __printHelp(Node * n) {
			if (n == nullptr)
				return;
			this->__printHelp(n->getLeft());
			std::cout <<"(key: "<< n->getKey()<<" val: "<<n->getVal()<<"), ";
			this->__printHelp(n->getRight());
		}
		void __printHelpPostOrder(Node * n) {
			if (n == nullptr)
				return;
			this->__printHelp(n->getLeft());
			this->__printHelp(n->getRight());
			std::cout <<"(key: "<< n->getKey()<<" val: "<<n->getVal()<<"), ";
			
		}
};

template <typename K, typename V>

class TreeMapIterator {
	std::vector<std::pair<K, V>> nodes;
	int ind;
	public: 
	TreeMapIterator (TreeMap<K, V> & tree) {
		this->ind = -1;
		nodes = tree.sorted();
	}
	std::pair<K, V>  next() {
		return this->nodes[++this->ind];
	}
	
	bool hasNext() {
		return this->ind + 1 < nodes.size();
	}
	
};



#endif