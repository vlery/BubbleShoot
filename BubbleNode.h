#ifndef BUBBLE_NODE_H
#define BUBBLE_NODE_H
#include "cocos2d.h"
#include <string>

const unsigned int RESOURCE_BUBBLE_SIZE=200;
enum class BubbleType {
	Bubble_0=0,Bubble_1,Bubble_2,Bubble_3,Bubble_4,Boundry_Side,Boundry_Top, Boundry_Attach
};
enum class ConnectType{
	LeftTop, RightTop, Left, Right, LeftBottom, RightBottom

};
ConnectType reflectY(ConnectType type);


class BubbleNode{
public:
	static int tagS ;
	//BubbleNode();
	BubbleNode(BubbleType type);
	BubbleNode(BubbleType, cocos2d::Point pos, cocos2d::Size size);
	cocos2d::Sprite* getBubble() { return bubble; }
	void connectBubble(ConnectType type, BubbleNode* bubble);
	BubbleNode* LeftTop;
	BubbleNode* RightTop;
	BubbleNode* Left;
	BubbleNode* Right; 
	BubbleNode* LeftBottom;
	BubbleNode* RightBottom;
	template <typename T> void setPosition(T position) {
		this->position.x = position.x;
		this->position.y = position.y;
		bubble->setPosition(position);
	}
	cocos2d::Point getPosition() { return position; };
	
private:
	cocos2d::Point position;
	cocos2d::Sprite* bubble;
	BubbleType type;
	bool init();
	cocos2d::Size size;
	int tag;
    
};

bool rowSort( BubbleNode* a, BubbleNode* b);
#endif // !BUBBLE_NODE_H#pragma once
