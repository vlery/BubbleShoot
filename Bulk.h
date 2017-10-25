
#ifndef BULK_H
#define BULK_H
#include "BubbleNode.h"
#include<algorithm>
#include <list>

void combineUnboundBulkAround(Bulk* bulk);
bool ifOuterConnection(BubbleNode* neighbour,Bulk* bulk);
class Bulk {
public:


	Bulk(BubbleType type);

	void attach(BubbleNode* bubble);
	void detach(BubbleNode* bubble);
	void detachAll();
	void destory();
	bool ifHasBubble(BubbleNode* bubble);

	void select();
	void unselect();
	void absorb(Bulk* bulk);
	
	int getNodeNum() {
		return nodeNum;
	}
	std::list<BubbleNode*> getAttachedBubble() {
		return bubbles;
	}

	BubbleNode* getFirstConnectOuterNode();
	bool ifHasNextConnectOuterNode();
	BubbleNode* getNextConnectOuterNode();
	void addConnectBulk(Bulk* bulk);

	void removeConnectBulk(Bulk* bulk);
	void replaceConnectBulk(Bulk* from, Bulk* to);
	std::list<Bulk*> getConnectBulkList() ;
	void removeAllConnection();
	BubbleType getType() { return type; }
	void connectToTop() {++connectTopCount;}
	void disConnectToTop() { --connectTopCount; }
	bool ifConnectTop() { return (connectTopCount>0); }
	bool ifConnectAttach() { return (connectAttachCount>0); }
	void connectToAttach() { ++connectAttachCount; }
	void disConnectToAttach() { --connectAttachCount; }
	
private:
	Bulk();
	std::list<BubbleNode*> bubbles;
	int connectTopCount=0;
	int connectAttachCount=0;
	BubbleType type;
	int nodeNum=0;
	std::list<BubbleNode*>::iterator connectOutItr;
	std::list<Bulk*> connectBulkList;
	void extendCountParameters(Bulk* bulk);
};




#endif // !BULK_H#pragma once
