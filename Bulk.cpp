#include "Bulk.h"
#include "BubbleNode.h"

void combineUnboundBulkAround(Bulk* bulk) {
	std::list<Bulk*> baseConnect;
	auto connectList = bulk->getConnectBulkList();
	auto itr = connectList.begin();
	while (itr != connectList.end()) {
		baseConnect.push_back(*itr);
		++itr;
	}

	while (baseConnect.size() > 0) {
		auto bulkItr= baseConnect.begin();
		Bulk* firstBulk = *bulkItr;
		
		bool ifEnd = false;
		std::list<Bulk*> in;
		in.push_back(firstBulk);
		while (!ifEnd) {
			ifEnd = true;
			auto begin = in.begin();
			while (begin != in.end()) {
				Bulk* crtBulk = *begin;
				auto t = crtBulk->getType();
				if (t != BubbleType::Boundry_Top&&t != BubbleType::Boundry_Attach&&crtBulk!=bulk) {
					auto addList = crtBulk->getConnectBulkList();
					auto add_itr = addList.begin();
					while (add_itr != addList.end()) {
						if (std::find(in.begin(), in.end(), *add_itr) == in.end()) {
							if ((*add_itr) != bulk && (*add_itr)->getType() != BubbleType::Boundry_Top) {
								in.push_back(*add_itr);
								ifEnd = false;
							}
						}
						++add_itr;
					}
				}
				++begin;
			}
		}
		bool ifConnectToTop = false;
		auto testItr = in.begin();
		while(testItr!=in.end()){ 
			if ((*testItr)->getType()!=BubbleType::Boundry_Attach&&(*testItr)->ifConnectTop()) {
				ifConnectToTop = true;
				break;
			}
			++testItr;
		}
		if (!ifConnectToTop) {
			auto scan = in.begin();
			while (scan != in.end()) {
				auto node = (*scan);
				Bulk* disconnectBulk = *scan;
			
				bulk->absorb(disconnectBulk);
				
				++scan;
			}

		}

			auto scan = in.begin();
			while (scan != in.end()) {
				auto node = (*scan);
				auto index = std::find(baseConnect.begin(), baseConnect.end(), node);
				if (index != baseConnect.end()) {
					baseConnect.erase(index);
				}
				++scan;
			}
		
		

	}
	


}


Bulk::Bulk() :nodeNum(0), connectTopCount(0), connectAttachCount(0){};

Bulk::Bulk(BubbleType type) {
	nodeNum = 0;
	this->type = type;
	connectTopCount = 0;
	connectAttachCount = 0;
}
void Bulk::attach(BubbleNode* bubble) {
	bubbles.push_back(bubble);
	nodeNum++;
}
void Bulk::detach(BubbleNode* bubble) {
	auto find_itr = std::find(bubbles.begin(), bubbles.end(), bubble);
	if (find_itr != bubbles.end()) {
		bubbles.erase(find_itr);
		nodeNum--;
	}
}


void Bulk::destory() {
	std::_For_each(bubbles.begin(), bubbles.end(), [](BubbleNode* node) {
		node->destorySelf();
	});

}

bool Bulk::ifHasBubble(BubbleNode* bubble) {
	return bubbles.end() != std::find(bubbles.begin(), bubbles.end(), bubble);
}


void  Bulk::select() {
	std::for_each(bubbles.begin(), bubbles.end(), [](BubbleNode* node) {
		node->select();
	});	
}
void Bulk::unselect() {
	std::for_each(bubbles.begin(), bubbles.end(), [](BubbleNode* node) {
		node->unselect();
	});
}
void Bulk::detachAll() {
	std::for_each(bubbles.begin(), bubbles.end(), [](BubbleNode* node) {
		node->setBulk(nullptr);
	});
}
void Bulk::absorb(Bulk* bulk_delete) {
	nodeNum += bulk_delete->nodeNum;
	connectTopCount += bulk_delete->connectTopCount;
	connectAttachCount += bulk_delete->connectAttachCount;
	std::for_each(bulk_delete->bubbles.begin(), bulk_delete->bubbles.end(), [this](BubbleNode* node) {
		node->setBulk(this);
	});
	bubbles.splice(bubbles.begin(),bulk_delete->bubbles);
	
	auto list1 = bulk_delete->connectBulkList;
	
	for_each(list1.begin(), list1.end(), [this,bulk_delete](Bulk* b) {
		if (b != this) {
			if (std::find(connectBulkList.begin(), connectBulkList.end(), b) == connectBulkList.end()) {
				connectBulkList.emplace_back(b);
			}
			b->replaceConnectBulk(bulk_delete, this);
		}
		else {
			this->removeConnectBulk(bulk_delete);

		}
	}			);
	list1.clear();
	
	delete bulk_delete;
}

void Bulk::addConnectBulk(Bulk* bulk) {
	if (std::find(connectBulkList.begin(), connectBulkList.end(), bulk) == connectBulkList.end()) {
		connectBulkList.emplace_back(bulk);
	}
}
void Bulk::removeConnectBulk(Bulk* bulk) {
	auto find_itr = std::find(connectBulkList.begin(), connectBulkList.end(), bulk);
	if (find_itr != connectBulkList.end()) {
		connectBulkList.erase(find_itr);
	}
	/*
	if (list.size() == 0) {
		//destory();
	}
	*/
}


BubbleNode* Bulk::getFirstConnectOuterNode() {
	connectOutItr = bubbles.begin();
	while (connectOutItr != bubbles.end()) {
		BubbleNode* node = (*connectOutItr);
		for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
			auto neighbour = node->getNeighbour(node->connectType[i]);
			if (neighbour != nullptr&& (neighbour->isBubble() || neighbour->isTopBoundry())) {
				return node;
			}
		}
		++connectOutItr;
	}
	return nullptr;
}
bool Bulk::ifHasNextConnectOuterNode() {
	++connectOutItr;
	while (connectOutItr != bubbles.end()) {
		BubbleNode* node = (*connectOutItr);
		for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
			auto neighbour = node->getNeighbour(node->connectType[i]); 
			if (neighbour != nullptr &&(neighbour->isBubble() || neighbour->isTopBoundry())) {
					return true;
			}
		}
		++connectOutItr;
	}
	return false;
}
BubbleNode* Bulk::getNextConnectOuterNode() {
	return *connectOutItr;
}

void Bulk::removeAllConnection() {
	std::for_each(connectBulkList.begin(), connectBulkList.end(), [this](Bulk* bulk) {
		bulk->removeConnectBulk(this);
	});
	connectBulkList.clear();
	
}

std::list<Bulk*> Bulk::getConnectBulkList(){ 
	return connectBulkList;
}

void Bulk::replaceConnectBulk(Bulk* from, Bulk* to) {
	addConnectBulk(to);
	removeConnectBulk(from);
	
}