#pragma once
#ifndef BUBBLE_NODE_H
#define BUBBLE_NODE_H
#include "cocos2d.h"
#include "BubbleType.h"
#include <string>
const static int SHOOT_SPEED = 10;
class Bulk;
const unsigned int RESOURCE_BUBBLE_SIZE=200;

enum class ConnectType{
	LeftTop, RightTop, Left, Right, LeftBottom, RightBottom
};
enum class BubbleState {
	READY, SHOOT,CONNECT,ATTACH,DEAD
};
ConnectType reflectY(ConnectType type);


class BubbleNode{
public:
	BubbleNode* LeftTop = nullptr;
	BubbleNode* RightTop = nullptr;
	BubbleNode* Left = nullptr;
	BubbleNode* Right = nullptr;
	BubbleNode* LeftBottom = nullptr;
	BubbleNode* RightBottom = nullptr;
	cocos2d::Vec2 velocity;

	//BubbleNode();
	BubbleNode(BubbleType type);
	BubbleNode(BubbleType, cocos2d::Point pos, cocos2d::Size size);
	cocos2d::Sprite* getBubble() { return bubble; }
	void connectBubble(ConnectType type, BubbleNode* bubble);
	
	void registerBulk();
	void connectBulk(ConnectType type, BubbleNode* node);
	Bulk* getBulk();
	void setBulk(Bulk* bulk);
    void setPositions(cocos2d::Vec2 position);
	cocos2d::Point getPosition() { return position; };
	BubbleType getType() {return type;}
	void select();
	void unselect();
	BubbleNode* getNeighbour(ConnectType type);
	void extendAllConnectionFrom(BubbleNode* fromNode);
	void removeAllConnection();
	void disConnectBubble(ConnectType type,BubbleNode* node);
	bool isBubble();
	void moveForward();
	BubbleState getState() { return bubbleState; }
	void setBubbleState(BubbleState state);
	void nextState();

	void attachTo(cocos2d::Point position);
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
    
};

bool rowSort( BubbleNode* a, BubbleNode* b);
#endif // !BUBBLE_NODE_H
