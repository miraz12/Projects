#pragma once
#include <vector>


class IndexedPriorityQLow
{

private:


	std::vector<double>& vecKeys;

	std::vector<int> heap;

	std::vector<int> invHeap;

	int size, maxSize;
	///Swap a and b
	void Swap(int a, int b);

	///move up the heap swapping the elements until the heap is ordered
	void ReorderUpwards(int nd);

	///move down the heap from node nd swapping the elements until the heap is reordered
	void ReorderDownwards(int nd, int HeapSize);

public:
	IndexedPriorityQLow(std::vector<double>& keys, int maxSize) :vecKeys(keys), maxSize(maxSize), size(0)
	{
		heap.assign(maxSize + 1, 0);
		invHeap.assign(maxSize + 1, 0);

	};

	bool empty(){ return (size == 0); };

	//to insert an item into the queue it gets added to the end of the heap
	void insert(const int idx);

	//to get the min item the first element is exchanged with the lowest
	int Pop();

	void ChangePriority(const int idx);

};

