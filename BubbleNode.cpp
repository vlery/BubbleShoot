#include "BubbleNode.h";
USING_NS_CC;
ConnectType reflectY(ConnectType type) {
	
	switch (type) {
	case ConnectType::LeftTop:
		return ConnectType::RightTop;
	case ConnectType::RightTop:
		return ConnectType::LeftTop;
	case ConnectType::LeftBottom:
		return ConnectType::RightBottom;
	case ConnectType::RightBottom:
		return ConnectType::LeftBottom;
	default:
		return type;
	}
}


BubbleNode::BubbleNode(BubbleType type) {
	this->type = type;
	if (!init()) {
		CCLOG("create Type %d Bubble failure ",type);
	}
}

BubbleNode::BubbleNode(BubbleType type, Point position,Size size) {
	this->type = type;
	this->position = position;
	this->size = size;
	if (!init()) {
		CCLOG("create Type %d Bubble failure ", type);
	}

}
int BubbleNode::tagS = 0;

bool BubbleNode::init() {
	tag = tagS;
	tagS++;
	int i =(int) type;
	if (i < 5) {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(i*RESOURCE_BUBBLE_SIZE, 0, RESOURCE_BUBBLE_SIZE, RESOURCE_BUBBLE_SIZE));
		bubble->setContentSize(size);
		bubble->setPosition(position);
		bubble->retain();
	}
	else if (i==6) {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(0, 0, 200,200));
		bubble->setContentSize(size);
		bubble->setPosition(position);
		bubble->retain();
	}
	else if (i == 7) {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(200, 0, 200, 200));
		bubble->setContentSize(size);
		bubble->setPosition(position);
		bubble->retain();
	}

	return true;
}


void BubbleNode::connectBubble(ConnectType type,BubbleNode* node){
	switch (type) {
	case ConnectType::Left:
		this->Left = node;
		node->Right = this;
		return;
	case ConnectType::Right:
		this->Right = node; 
		node->Left = this; 
		return;
	case ConnectType::LeftBottom:
		this->LeftBottom = node; 
		node->RightTop = this;
		return;
	case ConnectType::LeftTop:
		this->LeftTop=node;
		node->RightBottom = this;
		return;
	case ConnectType::RightTop:
		this->RightTop = node; 
		node->LeftBottom = this;
		return;
	case ConnectType::RightBottom:
		this->RightBottom = node;
		node->LeftTop = this;
		return;
	default:return;
	}
	
}


bool rowSort(BubbleNode* a,  BubbleNode* b) {
	
	return a->getPosition().x < b->getPosition().x;
}
