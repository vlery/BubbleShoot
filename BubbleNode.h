#pragma once
#ifndef BUBBLE_NODE_H
#define BUBBLE_NODE_H
#include "cocos2d.h"
#include "BubbleType.h"
#include <string>
class Bulk;
class BubbleFactory;
const static int SHOOT_SPEED = 10;
const unsigned int RESOURCE_BUBBLE_SIZE=200;
const unsigned int NEIGHBOUR_NUMBER = 6;
const static bool BUBBLE_DEBUG = false;
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
	BubbleNode* connect[6];
	static ConnectType  connectType[6];
/*
	BubbleNode* LeftTop = nullptr;
	BubbleNode* RightTop = nullptr;
	BubbleNode* Left = nullptr;
	BubbleNode* Right = nullptr;
	BubbleNode* LeftBottom = nullptr;
	BubbleNode* RightBottom = nullptr;
*/
	cocos2d::Vec2 velocity;

	//BubbleNode();
	BubbleNode(BubbleType type);
	BubbleNode(BubbleType, cocos2d::Point pos, cocos2d::Size size);
	cocos2d::Sprite* getBubble() { return bubble; }
	void connectBubble(ConnectType type, BubbleNode* bubble);
	void disConnectBubble(ConnectType type, BubbleNode* bubble,bool ifCheck);

	void registerBulk();
	void connectBulk(BubbleNode* node);
	void disConnectBulk( BubbleNode* node,bool ifCheak);
	Bulk* getBulk();
	void setBulk(Bulk* bulk);
    void setPositions(cocos2d::Vec2 position);
	cocos2d::Point getPosition() { return position; };
	BubbleType getType() {return type;}
	void select();
	void unselect();
	BubbleNode* getNeighbour(ConnectType type);
	void extendAllConnectionFrom(BubbleNode* fromNode);
	void extendStrongConnectionFrom(BubbleNode* fromNode);
	void removeAllConnection();

	bool isBubble();
	void moveForward();
	BubbleState getState() { return bubbleState; }
	void setBubbleState(BubbleState state);
	void nextState();

	void attachTo(cocos2d::Point position);

	void destorySelf();

	void reset();
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
    
};

bool rowSort( BubbleNode* a, BubbleNode* b);
#endif // !BUBBLE_NODE_H
