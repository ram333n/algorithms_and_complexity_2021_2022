#pragma once
#include "data_types.h"

namespace Treap {
	template<typename T>
	class Node {
	private:
		T value;
		int priority;

		Node<T>* left  = nullptr;
		Node<T>* right = nullptr;

		template<typename Type>
		friend class Treap;
	public:
		Node() : value(T()), priority() {}
		Node(const T& value_,
			 Node* left_  = nullptr,
			 Node* right_ = nullptr
		    ) : value(value_), priority(rand() % 20000), left(left_), right(right_) {}

		T Value() const {
			return value;
		}

		int Priority() const {
			return priority;
		}
	};

	template <typename T>
	class Treap {
	public:
		using NodePtr = Node<T>*;

		Treap() = default;

		const NodePtr Search(const T& val) const {
			NodePtr current = root;

			while (current && current->value != val) {
				current = current->value < val ? current->right : current->left;
			}
			return current;
		}

		void Insert(const T& val) {
			root = InsertImpl(root, val);
			++size;
		}

		void Remove(const T& val) {
			root = RemoveImpl(root, val);
		}

		bool Empty() const {
			return root == nullptr;
		}

		size_t Size() const {
			return size;
		}

		void InorderPrint() const {
			InorderPrintImpl(root);
			std::cout << std::endl;
		}

		int Height() const {
			return HeightImpl(root);
		}

		~Treap() {
			while (root) {
				Remove(root->value);
			}
		}
	private:
		NodePtr root = nullptr;
		size_t size = 0;
	private:
		NodePtr RightRotation(NodePtr to_rotate) {
			NodePtr new_root = to_rotate->left;

			to_rotate->left = new_root->right;
			new_root->right = to_rotate;
			return new_root;
		}

		NodePtr LeftRotation(NodePtr to_rotate) {
			NodePtr new_root = to_rotate->right;

			to_rotate->right = new_root->left;
			new_root->left = to_rotate;
			return new_root;
		}

		NodePtr InsertImpl(NodePtr root, const T& val) {
			if (!root) {
				return new Node<T>(val);
			}

			if (val <= root->value) {
				root->left = InsertImpl(root->left, val);
				
				if (root->left->priority > root->priority) {
					root = RightRotation(root);
				}
			}
			else {
				root->right = InsertImpl(root->right, val);

				if (root->right->priority > root->priority) {
					root = LeftRotation(root);
				}
			}

			return root;
		}

		NodePtr RemoveImpl(NodePtr root, const T& val) {
			if (!root) {
				return nullptr;
			}

			if (val < root->value) {
				root->left = RemoveImpl(root->left, val);
			}
			else if (val > root->value) {
				root->right = RemoveImpl(root->right, val);
			}
			else if (!root->left) {
				--this->size;
				NodePtr new_root = root->right;
				delete root;
				root = new_root;
			}
			else if (!root->right) {
				--this->size;
				NodePtr new_root = root->left;
				delete root;
				root = new_root;
			}
			else if (root->left->priority < root->right->priority) {
				root = LeftRotation(root);
				root->left = RemoveImpl(root->left, val);
			}
			else {
				root = RightRotation(root);
				root->right = RemoveImpl(root->right, val);
			}

			return root;
		}

		void InorderPrintImpl(NodePtr current) const {
			if (!current) {
				return;
			}

			InorderPrintImpl(current->left);
			if (current == this->root) {
				std::cout << "Root";
			}

			std::cout << "(" << current->value << ", " << current->priority << ") ";
			InorderPrintImpl(current->right);
		}

		int HeightImpl(NodePtr current) const {
			if (!current) {
				return 0;
			}

			return std::max(HeightImpl(current->left), HeightImpl(current->right)) + 1;
		}
	
	};
}