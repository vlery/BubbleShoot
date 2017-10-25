#include "Bulk.h"


void combineUnboundBulkAround(Bulk* bulk) {
	std::list<Bulk*> baseConnect;
	auto b_connectList = bulk->getConnectBulkList();
	auto b_itr = b_connectList.begin();
	while (b_itr != b_connectList.end()) {
		baseConnect.push_back(*b_itr);
		++b_itr;
	}

	while (baseConnect.size() > 0) {
		auto baseConnectItr= baseConnect.begin();
		Bulk* firstBulk = *baseConnectItr;
		bool ifEnd = false;
		std::list<Bulk*> in;
		in.push_back(firstBulk);
		//start expand
		while (!ifEnd) {
			ifEnd = true;
			auto begin = in.begin();
			while (begin != in.end()) {
				Bulk* itr = *begin;
				auto t = itr->getType();
				//if the crt bubble (in list)is not top and attach type 
				if (t != BubbleType::Boundry_Top&&t != BubbleType::Boundry_Attach&&itr !=bulk) {
					auto addinList = itr->getConnectBulkList();
				
					auto addin_itr = addinList.begin();
					while (addin_itr != addinList.end()) {
						if (std::find(in.begin(), in.end(), *addin_itr) == in.end()) {
							//add the neighbour is not top and basebulk 
							if ((*addin_itr) != bulk && (*addin_itr)->getType() != BubbleType::Boundry_Top) {
								in.push_back(*addin_itr);
								//has update
								ifEnd = false;
							}
						}
						++addin_itr;
					}
					
				}
				++begin;
			}
		}
		//finish expand
		bool ifConnectToTop = false;
		auto testItr = in.begin();
		while(testItr!=in.end()){ 
			//attach type won't connect isolate bulk to the top
			if ((*testItr)->getType()!=BubbleType::Boundry_Attach&&(*testItr)->ifConnectTop()) {
				ifConnectToTop = true;
				break;
			}
			++testItr;
		}

		//destory all disConnectBulk
		if (!ifConnectToTop) {
			std::for_each(in.begin(), in.end(), [bulk](Bulk* disConnectBulk) {
				bulk->absorb(disConnectBulk);
			});
		}
		//erase the neighbours have been checked 
		auto erase_itr = in.begin();
		while (erase_itr != in.end()) {
				auto node = (*erase_itr);
				auto find_erase_itr = std::find(baseConnect.begin(), baseConnect.end(), node);
				if (find_erase_itr != baseConnect.end()) {
					baseConnect.erase(find_erase_itr);
				}
				++erase_itr;
			}	
	}
}

bool ifOuterConnection(BubbleNode* neighbour,Bulk* bulk) {
	if (neighbour != nullptr) {
		if (neighbour->isBubble() && neighbour->getBulk() != bulk) {
			return true;
		}
		if (neighbour->isTopBoundry()) {
			return true;
		}
	}
	return false;
}


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
	this->removeAllConnection();
	bubbles.clear();
	delete this;
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
void Bulk::extendCountParameters(Bulk* bulk) {
	nodeNum += bulk->nodeNum;
	connectTopCount += bulk->connectTopCount;
	connectAttachCount += bulk->connectAttachCount;
}
void Bulk::absorb(Bulk* bulk_delete) {
	extendCountParameters(bulk_delete);
	std::for_each(bulk_delete->bubbles.begin(), bulk_delete->bubbles.end(), [this](BubbleNode* node) {
		node->setBulk(this);
	});
	bubbles.splice(bubbles.begin(),bulk_delete->bubbles);
	
	auto d_bulk_neighbour = bulk_delete->connectBulkList;
	for_each(d_bulk_neighbour.begin(), d_bulk_neighbour.end(), [this,bulk_delete](Bulk* b) {
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
	d_bulk_neighbour.clear();
	
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
}


BubbleNode* Bulk::getFirstConnectOuterNode() {
	BubbleNode*  n1=nullptr;
	BubbleNode* n2= nullptr;
	
	connectOutItr = bubbles.begin();
	
	while (connectOutItr != bubbles.end()) {
		BubbleNode* node = (*connectOutItr);
		
		bool ifConnectOut= testAroundBubble(node, [this](BubbleNode* neighbour) {
			return  ifOuterConnection(neighbour, this);
		});
		if (ifConnectOut) {
			return node;
		}
		++connectOutItr;
	}

	return nullptr;
	
}
bool Bulk::ifHasNextConnectOuterNode() {
	++connectOutItr;
	while (connectOutItr != bubbles.end()) {
		
		bool ifConnectOut = testAroundBubble((*connectOutItr), [this](BubbleNode* neighbour) {
			return ifOuterConnection(neighbour, this);
		});
		if (ifConnectOut) {
			return true;
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

