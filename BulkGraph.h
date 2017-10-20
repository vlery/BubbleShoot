#ifndef BULK_GRAPH_H
#define BULK_GRAPH_H
#include "Bulk.h"
#include <list>
struct BulkConnection {
	Bulk*  bulk1;
	Bulk* bulk2;
	BulkConnection(Bulk* b1, Bulk* b2) {
		bulk1 = b1;
		bulk2 = b2;
	}
};

class BulkGraph {
public :
	static BulkGraph& getGraph() {
		if (graph == nullptr) {
			graph = new BulkGraph();
		}
		return *graph;
	}
	void addConnection(Bulk* b1, Bulk* b2);
	void removeConnection(Bulk* b1, Bulk* b2);
private:
	BulkGraph() {};
	std::list<BulkConnection> connectList;
	static BulkGraph* graph;
	BulkGraph(BulkGraph const&) {};
	void operator=(const BulkGraph&);
};
#endif // !BULK_GRAPH_H