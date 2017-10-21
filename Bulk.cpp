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
			if ((*testItr)->ifConnectTop()) {
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


Bulk::Bulk() :nodeNum(0), connectTopCount(0){};

Bulk::Bulk(BubbleType type) {
	nodeNum = 0;
	this->type = type;
	connectTopCount = 0;
}
void Bulk::attach(BubbleNode* bubble) {
	bubbles.push_back(bubble);
	nodeNum++;
	connectTopCount = 0;
}
void Bulk::detach(BubbleNode* bubble) {
	for (auto it = bubbles.begin(); it != bubbles.end(); ) {
		if ((*it) == bubble) {
			it = bubbles.erase(it);
			break;
		}
		else {
			++it;
		}
	}
	nodeNum--;
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
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->select();
		++start;
	}

	
}
void Bulk::unselect() {
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->unselect();
		++start;
	}
}
void Bulk::detachAll() {
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->setBulk(nullptr);
		++start;
	}
}
void Bulk::absorb(Bulk* bulk_delete) {
	nodeNum += bulk_delete->nodeNum;
	connectTopCount += bulk_delete->connectTopCount;
	std::for_each(bulk_delete->bubbles.begin(), bulk_delete->bubbles.end(), [this](BubbleNode* node) {
		node->setBulk(this);
	});
	bubbles.splice(bubbles.begin(),bulk_delete->bubbles);
	
	auto list1 = bulk_delete->getConnectBulkList();
	auto bulk_itr = list1.begin();
	while(bulk_itr!=list1.end()){
		if (std::find(list.begin(), list.end(), *bulk_itr) == list.end()) {
			list.emplace_back(*bulk_itr);
		}
		(*bulk_itr)->replaceConnectBulk(bulk_delete, this);
		++bulk_itr;
	}
	list1.clear();
	
	//delete bulk_delete;
}

void Bulk::addConnectBulk(Bulk* bulk) {
	auto find_itr = std::find(list.begin(), list.end(), bulk);
	if (find_itr == list.end()) {
		list.emplace_back(bulk);
	}
}
void Bulk::removeConnectBulk(Bulk* bulk) {
	auto find_itr = std::find(list.begin(), list.end(), bulk);
	if (find_itr != list.end()) {
		list.erase(find_itr);
	}
	if (list.size() == 0) {
		destory();
	}
}


BubbleNode* Bulk::getFirstConnectOuterNode() {
	connectOutItr = bubbles.begin();
	while (connectOutItr != bubbles.end()) {
		BubbleNode* node = (*connectOutItr);
		for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
			auto neighbour = node->connect[(int)node->connectType[i]];
			if (neighbour != nullptr) {
				if (neighbour->isBubble() || neighbour->isTopBoundry()) {
					return node;
				}
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
			auto neighbour = node->connect[(int)node->connectType[i]];
			if (neighbour != nullptr) {
				if (neighbour->isBubble() || neighbour->isTopBoundry()) {
					return true;
				}
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

	std::for_each(list.begin(), list.end(), [this](Bulk* bulk) {
		bulk->removeConnectBulk(this);
	});
	list.clear();
	
}

std::list<Bulk*> Bulk::getConnectBulkList(){ 
	return list;
}

void Bulk::replaceConnectBulk(Bulk* from, Bulk* to) {
	addConnectBulk(to);
	removeConnectBulk(from);
	
}