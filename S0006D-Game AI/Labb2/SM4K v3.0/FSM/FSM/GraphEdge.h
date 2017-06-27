#pragma once
///Edges base class
class GraphEdge
{
private:
	///Edge cost
	double eCost;
	///from node index
	int indexFrom;
	///To node index
	int indexTo;


public:
	GraphEdge(){};
	GraphEdge(int f, int t, int c) :indexFrom(f), indexTo(t), eCost(c){};
	//GraphEdge(int f, int t) :indexFrom(f), indexTo(t), eCost(0.0){};


	virtual ~GraphEdge(){};

	int getIndexFrom() const { return indexFrom; }
	void setIndexFrom(int val) { indexFrom = val; }

	int getIndexTo() const { return indexTo; }
	void setIndexTo(int val) { indexTo = val; }

	double& getCost() { return eCost; }
	void setCost(double val) { eCost = val; }
};

