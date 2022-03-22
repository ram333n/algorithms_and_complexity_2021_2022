#pragma once
#include "data_types.h"
#include <stdexcept>

const RationalNumber MINUS_INFINITY = { numeric_limits<int>::min(), 1 };
 
namespace BinomialHeap {
	template<typename T>
	class Node {
	private:
		T value;
		Node<T>* parent = nullptr;
		Node<T>* child = nullptr;
		Node<T>* sibling = nullptr;
		int degree = 0;

		template<typename Type>
		friend class BinomialHeap;
	public:
		Node() : value(T()) {}
		Node(const T& value_) : value(value_) {}

		T Value() const {
			return value;
		}

	private:
		void Link(Node<T>* to_link) {
			to_link->parent = this;
			to_link->sibling = child;
			child = to_link;
			degree++;
		}
	};

	template<typename T>
	class BinomialHeap {
	public:
		BinomialHeap() = default;

		T Min() const {
			if (!head) {
				throw std::runtime_error("Heap is empty");
			}

			T result = head->value;
			for (Node<T>* current = head->sibling; current; current = current->sibling) {
				if (current->value < result) {
					result = current->value;
				}
			}

			return result;
		}

		void Merge(BinomialHeap<T>& source) {
			MergeRoots(source);
			if (!head) {
				return;
			}

			Node<T>* prev = nullptr;
			Node<T>* current = head;
			Node<T>* next = current->sibling;

			while (next) {
				if (current->degree != next->degree ||
					(next->sibling && next->sibling->degree == current->degree))
				{
					prev = current;
					current = next;
				}
				else if (current->value <= next->value) {
					current->sibling = next->sibling;
					current->Link(next);
				}
				else {
					if (!prev) {
						head = next;
					}
					else {
						prev->sibling = next;
					}

					next->Link(current);
					current = next;
				}

				next = current->sibling;
			}
		}

		void Insert(const T& value) {
			BinomialHeap<T> temp;
			temp.head = new Node(value);
			++temp.size;
			Merge(temp);
		}

		Node<T>* ExtractMin() {
			if (!head) {
				return nullptr;
			}

			Node<T>* min_node = MinNode();
			Node<T>* prev_min = GetNodeBefore(min_node);

			if (prev_min) {
				prev_min->sibling = min_node->sibling;
			}
			else {
				head = min_node->sibling;
			}

			--size;

			if (!min_node->child) {
				return min_node;
			}

			Node<T>* current = min_node->child;
			Node<T>* prev = nullptr;
			Node<T>* next = nullptr;

			while (current) {
				current->parent = nullptr;

				next = current->sibling;
				current->sibling = prev;

				prev = current;
				current = next;
			}
			min_node->child = nullptr;

			BinomialHeap<T> temp;
			temp.head = prev;
			Merge(temp);
			return min_node;
		}

		void DecreaseValue(Node<T>* to_decrease, T value) {
			if (value > to_decrease->value) {
				return;
			}
			to_decrease->value = value;

			Node<T>* temp_node = to_decrease;
			Node<T>* parent = to_decrease->parent;

			while (parent && temp_node->value < parent->value) {
				std::swap(temp_node->value, parent->value);
				temp_node = parent;
				parent = parent->parent;
			}
		}

		void Erase(Node<T>* to_erase) {
			DecreaseValue(to_erase, MINUS_INFINITY);
			delete ExtractMin();
		}

		size_t Size() const {
			return size;
		}

		bool Empty() const {
			return head == nullptr;
		}

		~BinomialHeap() {
			auto to_delete = ExtractMin();
			while (to_delete) {
				delete to_delete;
				to_delete = ExtractMin();
			}
		}
	private:
		Node<T>* head = nullptr;
		size_t size = 0;
	private:
		Node<T>* MinNode() const {
			if (!head) {
				return nullptr;
			}

			Node<T>* result = head;
			for (Node<T>* current = head->sibling; current; current = current->sibling) {
				if (current->value < result->value) {
					result = current;
				}
			}

			return result;
		}

		void MergeRoots(BinomialHeap<T>& source) {
			if (!source.head) {
				return;
			}

			Node<T>* current = new Node<T>();
			Node<T>* new_head = current;
			Node<T>* first_current = head;
			Node<T>* second_current = source.head;

			while (first_current && second_current) {
				if (first_current->degree <= second_current->degree) {
					current->sibling = first_current;
					first_current = first_current->sibling;
				}
				else {
					current->sibling = second_current;
					second_current = second_current->sibling;
				}
				current = current->sibling;
			}

			current->sibling = first_current ? first_current : second_current;
			head = new_head->sibling;

			delete new_head;

			size += source.size;
			source.size = 0;
			source.head = nullptr;
		}

		Node<T>* GetNodeBefore(Node<T>* given) {
			if (!given || given == head) {
				return nullptr;
			}

			Node<T>* result = head;

			while (result->sibling != given) {
				result = result->sibling;
			}

			return result;
		}
	};
}