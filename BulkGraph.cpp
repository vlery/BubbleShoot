#include "BulkGraph.h"

void BulkGraph::addConnection(Bulk* b1, Bulk* b2) {
	auto itr = connectList.begin();
	while (itr != connectList.end()) {
		auto connect = *itr;
		if((connect.bulk1==b1&&connect.bulk2==b2)|| (connect.bulk1 == b2&&connect.bulk2 == b1)){
			return;
		}
		++itr;
	}
	connectList.emplace_back(BulkConnection(b1, b2));
}
void BulkGraph::removeConnection(Bulk* b1, Bulk* b2) {
	auto itr = connectList.begin();
	while (itr != connectList.end()) {
		auto connect = *itr;
		if ((connect.bulk1 == b1&&connect.bulk2 == b2) || (connect.bulk1 == b2&&connect.bulk2 == b1)) {
			connectList.erase(itr);
			return;
		}
		++itr;
	}


}