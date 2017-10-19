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
	case ConnectType::Left:
		return ConnectType::Right;
	case ConnectType::Right:
		return ConnectType::Left;
	default:
		return type;
	}
}
ConnectType opposite(ConnectType type) {
	switch (type) {
	case ConnectType::LeftTop:
		return ConnectType::RightBottom;
	case ConnectType::RightTop:
		return ConnectType::LeftBottom;
	case ConnectType::LeftBottom:
		return ConnectType::RightTop;
	case ConnectType::RightBottom:
		return ConnectType::LeftTop;
	case ConnectType::Left:
		return ConnectType::Right;
	case ConnectType::Right:
		return ConnectType::Left;
	default:
		return type;
	}
}


ConnectType  BubbleNode::connectType[] = { ConnectType::LeftTop,ConnectType::RightTop,
																		ConnectType::Left, ConnectType::Right,
																		ConnectType::LeftBottom, ConnectType::RightBottom }; 
BubbleNode::BubbleNode(BubbleType type) {
	this->type = type;
	init();
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		connect[i] = nullptr;

	}
}

BubbleNode::BubbleNode(BubbleType type, Point position,Size size) {
	this->type = type;
	this->position = position;
	this->size = size;
	init();
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		connect[i] = nullptr;
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
	}else {
		bubble = Sprite::create("asset/bubble.png", CCRectMake(0, 0, 200,200));
		bubble->setVisible(BUBBLE_DEBUG);
	}
	bubble->setContentSize(size);
	bubble->setPosition(position);
	bubble->retain();
	return true;
}


void BubbleNode::connectBubble(ConnectType type,BubbleNode* node){
	connectBulk( node);
	
	connect[(int)type] = node;
	node->connect[(int)opposite(type)] = this;
}

void BubbleNode::disConnectBubble(ConnectType ctype, BubbleNode* node, bool ifCheck) {
	disConnectBulk(node, ifCheck);
	node->connect[(int)opposite(ctype)] = nullptr;
	connect[(int)ctype] = nullptr;

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

void BubbleNode::connectBulk( BubbleNode* node) {
	if (!isBubble()||!node->isBubble()) {
		if (this->type == BubbleType::Boundry_Attach||isBubble()) {
			if (node->getType() == BubbleType::Boundry_Top||node->isBubble()) {
				this->getBulk()->addConnection();
			}
		}
	   if(node->getType()== BubbleType::Boundry_Attach|| node->isBubble()){
			if (this->type == BubbleType::Boundry_Top||isBubble()) {
				node->getBulk()->addConnection();
			}
		}
	 
		return;
	}
	
	if (node->getType() == getType()) {
			if (node->getBulk() != getBulk()) {
				getBulk()->absorb(node->getBulk());
			}
	}else {
			getBulk()->addConnection();
			node->getBulk()->addConnection();
	}
	

	
}

void BubbleNode::disConnectBulk(BubbleNode* node,bool ifCheck) {
	if (!isBubble() || !node->isBubble()) {
		if (this->type == BubbleType::Boundry_Attach || isBubble()) {
			if (node->getType() == BubbleType::Boundry_Top||node->isBubble()) {
				this->getBulk()->removeConnection();
				if (ifCheck) {
					
					if (this->getBulk()->getConnection() ==0) {
						this->getBulk()->destoryBubbles();
					}
				}
			}
		}
 	   if (node->getType() == BubbleType::Boundry_Attach||node->isBubble()) {
			if (this->type == BubbleType::Boundry_Top||isBubble()) {
				node->getBulk()->removeConnection();
			}
			if (ifCheck) {
				if (node->getBulk()->getConnection() == 0) {
					node->getBulk()->destoryBubbles();
				}
			}
		}
		return;
	}

		if (node->getType() != getType()) {
	 
			getBulk()->removeConnection();
			node->getBulk()->removeConnection();
			if (ifCheck) {
				if (node->getBulk()->getConnection() == 0) {
					node->getBulk()->destoryBubbles();
				}
			}
		}
	


}
void BubbleNode::setBulk(Bulk* bulk) {
	this->bulk = bulk;
}
Bulk* BubbleNode::getBulk() {
	return bulk;
}
//sort by x position
bool rowSort(BubbleNode* a,  BubbleNode* b) {
	return a->getPosition().x < b->getPosition().x;
}

//for Debug
void BubbleNode::select() {
	//bubble->runAction(TintBy::create(0.2, 10, 10, 10));
	auto z = this;
	bubble->setOpacity(30);
	if (getBulk() != nullptr) {
		CCLOG("currentNode, connection:%d", getBulk()->getConnection());
		CCLOG("currentNode, bulk nodeNum:%d", getBulk()->getNodeNum());
	}
	
}
void BubbleNode::unselect() {
	bubble->setOpacity(255);
///	bubble->runAction(TintBy::create(0.2, -10, -10, -10));
}



BubbleNode* BubbleNode::getNeighbour(ConnectType type) {
	return connect[(int)type];
}
void BubbleNode::extendAllConnectionFrom(BubbleNode* fromNode) {
	
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType  type = connectType[i];
		int type_i = (int)type;
		if (fromNode->connect[type_i] != nullptr) {
			auto connectTo = fromNode->connect[type_i];
			fromNode->disConnectBubble(type, connectTo,false);
			connectBubble(type, connectTo);
		}

	}
}


void BubbleNode::extendStrongConnectionFrom(BubbleNode* fromNode) {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType  type = connectType[i];
		int type_i = (int)type;
		if (fromNode->connect[type_i] != nullptr) {
			auto neighbour = fromNode->connect[type_i];
			if (neighbour->isBubble() || neighbour->getType() == BubbleType::Boundry_Top) {
				auto connectTo = fromNode->connect[type_i];
				fromNode->disConnectBubble(type, connectTo, true);
				connectBubble(type, connectTo);
			}
		}

	}
}
void BubbleNode::removeAllConnection() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType  type = connectType[i];
		int type_i = (int)type;
		if (connect[type_i] != nullptr) {
			this->disConnectBubble(type, connect[(int)type], true);
		}
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

void BubbleNode::destorySelf() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType type = connectType[i];
		int type_i = (int)type;
		if (connect[type_i] != nullptr) {
			disConnectBubble(type, connect[type_i], true);
		}
	}

	auto callback = CallFunc::create([this]() {
		this->getBubble()->removeFromParent();
		this->nextState();
	});
	bubble->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -1000)),callback,nullptr));

}

void BubbleNode::reset() {

}


BubbleNode* BubbleNode::getNeighbourByConnectType(ConnectType type) {
	return connect[static_cast<int>(type)];
}

