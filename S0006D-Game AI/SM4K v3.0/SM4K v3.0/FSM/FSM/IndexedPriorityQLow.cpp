#include "IndexedPriorityQLow.h"
#include <assert.h>




void IndexedPriorityQLow::Swap(int a, int b)
{
	int temp = heap[a]; 
	heap[a] = heap[b];
	heap[b] = temp;

	invHeap[heap[a]] = a;
	invHeap[heap[b]] = b;
}

void IndexedPriorityQLow::ReorderUpwards(int nd)
{
	while ((nd > 1)&&(heap[nd/2] > heap[nd]))
	{
		Swap(nd / 2, nd);
		nd /= 2;
	}
}

void IndexedPriorityQLow::ReorderDownwards(int nd, int heapSize)
{
	while (2 * nd <= heapSize)
	{
		int child = 2 * nd;

		if (child < heapSize && heap[child] > heap[child + 1])
		{
			++child;
		}

		if (heap[nd] > heap[child])
		{
			Swap(child, nd);
			nd = child;
		}
		else
		{
			break;
		}
	}
}

void IndexedPriorityQLow::insert(const int i)
{
	assert(size + 1 <= maxSize);

	size++;

	heap[size] = i;

	invHeap[i] = size;
}

int IndexedPriorityQLow::Pop()
{
	Swap(1, size);

	ReorderDownwards(1, size - 1);

	return heap[size--];
}

void IndexedPriorityQLow::ChangePriority(const int i)
{
	ReorderUpwards(invHeap[i]);
}
