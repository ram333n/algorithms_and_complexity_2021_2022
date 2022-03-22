#include "test_runner.h"
#include "binomial_heap.h"
#include "treap.h"

#include <vector>
#include <algorithm>

using namespace std;

namespace BinomialHeap {
    template<typename T>
    void HeapSort(vector<T>& to_sort) {
        BinomialHeap<T> heap;
        size_t current_size = 0;

        ASSERT(heap.Empty());
        ASSERT_EQUAL(current_size, heap.Size());

        for (auto val : to_sort) {
            heap.Insert(val);
            ASSERT_EQUAL(++current_size, heap.Size());
            ASSERT(!heap.Empty());
        }

        for (size_t i = 0; i < to_sort.size(); ++i) {
            to_sort[i] = heap.ExtractMin()->Value();
        }
    }

    void TestHeapSort() {
        vector<RationalNumber> v = {
            {-1,2}, {3,11}, {6,3}, {0,342}, {-4,8}, {8,25}, {1,3}, {7,99}
        };
        HeapSort(v);
        ASSERT(std::is_sorted(begin(v), end(v)));
    }


    void TestMerge() {
        vector<RationalNumber> v1 = {
            {-1,2}, {3,11}, {6,3}, {0,342}, {-4,8}, {8,25}, {1,3}, {7,99}
        };

        vector<RationalNumber> v2 = {
            {6,11}, {-7,21}, {1,8}, {-9,3}, {2,-1}, {54,26}, {10,2}, {-7,1}, {0,1}, {8,12}
        };

        BinomialHeap<RationalNumber> h1, h2;

        for (auto val : v1) {
            h1.Insert(val);
            ASSERT(!h1.Empty());
        }

        for (auto val : v2) {
            h2.Insert(val);
            ASSERT(!h2.Empty());
        }

        ASSERT_EQUAL(v1.size(), h1.Size());
        ASSERT_EQUAL(v2.size(), h2.Size());

        vector<RationalNumber> expected = v1;
        vector<RationalNumber> source(v1.size() + v2.size(), { 0,1 });

        std::copy(begin(v2), end(v2), back_inserter(expected));
        std::sort(begin(expected), end(expected));
        ASSERT(std::is_sorted(begin(expected), end(expected)));

        h1.Merge(h2);
        ASSERT_EQUAL(h1.Size(), v1.size() + v2.size());
        ASSERT_EQUAL(h2.Size(), 0);
        ASSERT(h2.Empty());
        ASSERT(!h1.Empty());

        for (size_t i = 0; i < source.size(); ++i) {
            auto min_val = h1.Min();
            source[i] = h1.ExtractMin()->Value();
            ASSERT_EQUAL(min_val, source[i]);
            ASSERT_EQUAL(h1.Size(), source.size() - 1 - i);
        }
        ASSERT(h1.Empty());
        ASSERT_EQUAL(h1.Size(), 0);
        ASSERT_EQUAL(source, expected);
    }

    void TestDestruction() {
        vector<RationalNumber> source(100000, { 0,1 });

        for (size_t i = 0; i < 50; ++i) {
            BinomialHeap<RationalNumber> heap;
            for (auto val : source) {
                heap.Insert(val);
            }
            ASSERT_EQUAL(heap.Size(), 100000);
        }
    }

    void RunTests() {
        TestRunner tr;
        cout << "-----------BinomialHeap-----------" << endl;
        RUN_TEST(tr, TestHeapSort);
        RUN_TEST(tr, TestMerge);
        RUN_TEST(tr, TestDestruction);
    }
}

namespace Treap {
    void TestBasics() {
        vector<RationalNumber> v = {
            {6,11}, {-7,21}, {1,8}, {-9,3}, {2,-1}, {54,26}, {10,2}, {-7,1}, {0,1}, {8,12},{-3,65}, {-10,2}, {-43,32}, {354,23}, {0,1}
        };

        Treap<RationalNumber> tree;
        ASSERT(tree.Empty());
        ASSERT_EQUAL(tree.Size(), 0);

        cout << "---------------------------------------------------------------------------\n";
        for (size_t i = 0; i < v.size(); ++i) {
            tree.Insert(v[i]);
            ASSERT_EQUAL(tree.Size(), i + 1);
            ASSERT(!tree.Empty());
            cout << "Height : " << tree.Height() << ", Tree : ";
            tree.InorderPrint();
        }
        cout << "---------------------------------------------------------------------------\n";

        size_t expected_size = tree.Size();

        //Search

        ASSERT_EQUAL(expected_size, v.size());

        ASSERT(tree.Search({ 1,8 }));
        ASSERT(!tree.Search({ 1,9 }));
        ASSERT(tree.Search({ 0,1 }));
        ASSERT(tree.Search({ -7,1 }));
        ASSERT(!tree.Search({ 6,6 }));
        ASSERT(!tree.Search({ -32332,323232 }));

        //Remove

        tree.Remove({ 65,66 });
        ASSERT_EQUAL(tree.Size(), expected_size);

        tree.Remove({ 1,8 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(!tree.Search({ 1,8 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();

        tree.Remove({ 0,1 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(tree.Search({ 0,1 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();

        tree.Remove({ 0,1 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(!tree.Search({ 0,1 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();

        tree.Remove({ 6,11 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(!tree.Search({ 6,11 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();

        tree.Remove({ -2,1 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(!tree.Search({ -2,1 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();

        tree.Remove({ -1,3 });
        ASSERT_EQUAL(tree.Size(), --expected_size);
        ASSERT(!tree.Search({ -1,3 }));
        cout << "Height : " << tree.Height() << ", Tree : ";
        tree.InorderPrint();
        cout << "---------------------------------------------------------------------------\n";
    }

    void TestDestruction() {
        vector<RationalNumber> source(100000, { 0,1 });

        for (size_t i = 0; i < 50; ++i) {
            Treap<RationalNumber> tree;
            for (auto val : source) {
                tree.Insert(val);
            }
            ASSERT_EQUAL(tree.Size(), 100000);
        }
    }

    void RunTests() {
        TestRunner tr;
        cout << "---------------Treap--------------" << endl;
        RUN_TEST(tr, TestBasics);
        RUN_TEST(tr, TestDestruction);
    }
}

int main() {
    BinomialHeap::RunTests();
    Treap::RunTests();
}