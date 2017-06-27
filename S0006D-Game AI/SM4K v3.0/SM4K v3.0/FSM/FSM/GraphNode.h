#pragma once
///Node base class
class GraphNode
{
private:
	///Node index
	int index;

public:
	GraphNode(){};
	GraphNode(int i) :index(i){}
	virtual ~GraphNode(){};

	int getIndex() const { return index; }
	void setIndex(int val) { index = val; }
};

