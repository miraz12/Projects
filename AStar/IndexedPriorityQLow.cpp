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
	while ((nd > 1) && (vecKeys[heap[nd / 2]] > vecKeys[heap[nd]]))
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

		if ((child < heapSize) && (vecKeys[heap[child]] > vecKeys[heap[child + 1]]))
		{
			++child;
		}

		if (vecKeys[heap[nd]] > vecKeys[heap[child]])
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

	++size;

	heap[size] = i;

	invHeap[i] = size;

	ReorderUpwards(size);
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
