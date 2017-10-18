#include "BubbleNode.h";
#include "Bulk.h"
#include <algorithm>
#include <exception>
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
 void BubbleNode:: setPositions(Vec2 position) {
	this->position.x = position.x;
	this->position.y = position.y;
	bubble->setPosition(position.x,position.y);
}

bool BubbleNode::init() {
	
	
	int i =(int) type;
	if (i < 5) {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(i*RESOURCE_BUBBLE_SIZE, 0, RESOURCE_BUBBLE_SIZE, RESOURCE_BUBBLE_SIZE));
		bubble->setContentSize(size);
		bubble->setPosition(position);
		bubble->retain();
	}
	
	else {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(0, 0, 200,200));
		bubble->setContentSize(size);
		bubble->setVisible(false);
		bubble->setPosition(position);
		bubble->retain();
	}
	
	return true;
}


void BubbleNode::connectBubble(ConnectType type,BubbleNode* node){
	connectBulk(type, node);

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
bool BubbleNode::isBubble() {
	if (type <= BubbleType::Bubble_4) {
		return true;
	}
	return false;
}
void BubbleNode::registerBulk() {
	bulk = new Bulk(getType());
	bulk->attach(this);
}

void BubbleNode::setBulk(Bulk* bulk) {
	 this->bulk = bulk;
}
void BubbleNode::connectBulk(ConnectType type, BubbleNode* node) {
	if (!isBubble()) {
		return;
	}
	if(node->getType() == BubbleType::Boundry_Attach) {
		getBulk()->addConnection();
	}

	if (node->isBubble()) {
		if (node->getType() == getType()) {
			if (node->getBulk() != getBulk()) {
				getBulk()->absorb(node->getBulk());
			}
		}else {
			getBulk()->addConnection();
			node->getBulk()->addConnection();
		}
	}

	
}
Bulk* BubbleNode::getBulk() {
	return bulk;
}

bool rowSort(BubbleNode* a,  BubbleNode* b) {
	
	return a->getPosition().x < b->getPosition().x;
}


void BubbleNode::select() {
	//bubble->runAction(TintBy::create(0.2, 10, 10, 10));
	bubble->setOpacity(30);
}
void BubbleNode::unselect() {
	bubble->setOpacity(255);
///	bubble->runAction(TintBy::create(0.2, -10, -10, -10));
}
BubbleNode* BubbleNode::getNeighbour(ConnectType type) {
	switch (type) {
	case ConnectType::Left:return this->Left;
	case ConnectType::Right:return this->Right;
	case ConnectType::LeftTop:return this->LeftTop;
	case ConnectType::RightTop:return this->RightTop;
	case ConnectType::LeftBottom:return this->LeftBottom;
	case ConnectType::RightBottom:return this->RightBottom;
	default:return this;
	}
}
void BubbleNode::extendAllConnectionFrom(BubbleNode* fromNode) {
	
	
	if (fromNode->Left!= nullptr) {
		connectBubble(ConnectType::Left, fromNode->Left);
		fromNode->Left = nullptr;
	}
	if (fromNode->Right != nullptr) {
		connectBubble(ConnectType::Right, fromNode->Right);
		fromNode->Right = nullptr;
	}
	if (fromNode->LeftTop != nullptr) {
		connectBubble(ConnectType::LeftTop, fromNode->LeftTop);
		fromNode->LeftTop = nullptr;
	}
	if (fromNode->LeftBottom != nullptr) {
		connectBubble(ConnectType::LeftBottom, fromNode->LeftBottom);
		fromNode->LeftBottom = nullptr;
	}
	if (fromNode->RightTop != nullptr) {
		connectBubble(ConnectType::RightTop, fromNode->RightTop);
		fromNode->RightTop = nullptr;
	}
	if (fromNode->RightBottom != nullptr) {
		connectBubble(ConnectType::RightBottom, fromNode->RightBottom);
		fromNode->RightBottom = nullptr;
	}
}
void BubbleNode::disConnectBubble(ConnectType type, BubbleNode* node) {
	switch (type) {
	case ConnectType::Left:
		this->Left = node->Right=nullptr;
		return;
	case ConnectType::Right:
		this->Right = node->Left = nullptr;
		return;
	case ConnectType::LeftBottom:
		this->LeftBottom = node->RightTop = nullptr;
		return;
	case ConnectType::LeftTop:
		this->LeftTop =node->RightBottom = nullptr;
		return;
	case ConnectType::RightTop:
		this->RightTop = node->LeftBottom = nullptr;
		return;
	case ConnectType::RightBottom:
		this->RightBottom =node->LeftTop = nullptr;
		return;
	default:return;
	}
}
void BubbleNode::removeAllConnection() {
	if (this->Left != nullptr) {
		this->disConnectBubble(ConnectType::Left, this->Left);
	}
	if (this->Right != nullptr) {
		this->disConnectBubble(ConnectType::Right, this->Right);
	}
	if (this->LeftTop != nullptr) {
		this->disConnectBubble(ConnectType::LeftTop, this->LeftTop);
	}
	if (this->LeftBottom != nullptr) {
		this->disConnectBubble(ConnectType::LeftBottom, this->LeftBottom);
	}
	if (this->RightTop != nullptr) {
		this->disConnectBubble(ConnectType::RightTop, this->RightTop);
	}
	if (this->RightBottom != nullptr) {
		this->disConnectBubble(ConnectType::RightBottom, this->RightBottom);
	}

}





void 	BubbleNode::moveForward() {
	setPositions(position += velocity);
}


void BubbleNode::attachTo(Point position) {
		
		auto callback= CallFunc::create([this]() {
			this->position.x = bubble->getPosition().x;
			this->position.y = bubble->getPosition().y;
			this->nextState();
			this->select();
		});
		Vec2 velocityDir = velocity;
		velocityDir.normalize();
	
		ccBezierConfig bezier;
		bezier.controlPoint_1 = 2*velocityDir;
		
		bezier.endPosition = position-this->position;
		bezier.controlPoint_2 =bezier.endPosition;
		auto bezierForward = BezierBy::create(2/velocity.length() , bezier);

		bubble->runAction(Sequence::create(bezierForward, callback, nullptr));
		//bubble->runAction(Sequence::create(MoveTo::create(2 / velocity.length(),position), correctPos, nullptr));

}



void BubbleNode::setBubbleState(BubbleState state) {
	this->bubbleState = state;
}
void BubbleNode::nextState() {
	bubbleState =static_cast<BubbleState>( static_cast<int>(bubbleState)+1);
}