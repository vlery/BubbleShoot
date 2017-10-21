#pragma once
#ifndef BUBBLE_NODE_H
#define BUBBLE_NODE_H
#include "cocos2d.h"
#include "BubbleType.h"
#include <string>
#include "Config.h"
class Bulk;
class BubbleFactory;

enum class ConnectType{
	LeftTop=0, RightTop, Left, Right, LeftBottom, RightBottom
};
enum class BubbleState {
	READY, SHOOT,CONNECT,ATTACH,DEAD
};

ConnectType reflectY(ConnectType type);
ConnectType opposite(ConnectType type);

class BubbleNode{
public:
	friend class BubbleFactory;
	BubbleNode* connect[6];
	static ConnectType  connectType[6];

	cocos2d::Vec2 velocity;

	//BubbleNode();
	BubbleNode(BubbleType type);
	BubbleNode(BubbleType, cocos2d::Point pos, cocos2d::Size size);
	cocos2d::Sprite* getBubble() { return bubble; }
	void connectBubble(ConnectType type, BubbleNode* bubble);
	void disConnectBubble(ConnectType type, BubbleNode* bubble);
	void removeAllBulkConnection();
	void registerBulk();
	void connectBulk(BubbleNode* node);
	
	Bulk* getBulk();
	void setBulk(Bulk* bulk);
    void setPositions(cocos2d::Vec2 position);
	cocos2d::Point getPosition() { return position; };

	void select();
	void unselect();
	BubbleNode* getNeighbour(ConnectType type);
	void extendAllConnectionFrom(BubbleNode* fromNode);
	void extendOuterConnectionFrom(BubbleNode* fromNode);
	void removeAllConnection();

	bool isBubble();
	void moveForward();
	BubbleState getState() { return bubbleState; }
	void setBubbleState(BubbleState state);
	void nextState();

	void attachTo(cocos2d::Point position, std::function<void(BubbleNode* node, BubbleNode* attach)> detect,BubbleNode* param1,BubbleNode* param2);

	void destorySelf();


	void reset();
	bool isDead();
	bool isAttached();
	bool isSameType(BubbleNode* node);
	bool isTopBoundry();
	bool ifBelongToSameBulk(BubbleNode* node);
	bool isPotentialAttach();
private:
	cocos2d::Point position;
	cocos2d::Sprite* bubble;
	BubbleType type;
	BubbleState bubbleState= BubbleState::ATTACH;
	bool init();
	cocos2d::Size size;
	int tag;
	Bulk* bulk=nullptr;
	void bubbleAttachUpdate(float time);
	BubbleNode* getNeighbourByConnectType(ConnectType type);
	BubbleType getType() { return type; }
    
};

bool rowSort( BubbleNode* a, BubbleNode* b);
#endif // !BUBBLE_NODE_H
