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
		bubble = Sprite::create("asset/default.png", CCRectMake(0, 0, 200,200));
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

void BubbleNode::disConnectBubble(ConnectType ctype, BubbleNode* node) {
	if (node->getType() == BubbleType::Boundry_Top&&node->getType()!=getType()) {
		bulk->disConnectToTop();
	}
	if (type == BubbleType::Boundry_Top&&node->getType() != getType()) {
		node->getBulk()->disConnectToTop();
	}
	node->connect[(int)opposite(ctype)] = nullptr;
	connect[(int)ctype] = nullptr;
}


bool BubbleNode::isBubble() {
	if (type <= BubbleType::Bubble_PINK) {
		return true;
	}
	return false;
}

void BubbleNode::registerBulk() {
	bulk = new Bulk(getType());
	bulk->attach(this);
}

void BubbleNode::connectBulk( BubbleNode* node) {

	if (node->getType() == BubbleType::Boundry_Top) {
		if (node->getType() != getType()) {
			bulk->connectToTop();
		}
		return;
	}
	if (type == BubbleType::Boundry_Top) {
		if (node->getType() != getType()) {
			node->getBulk()->connectToTop();
		}
		return;
	}

	if (type != node->getType()) {
		bulk->addConnectBulk(node->getBulk());
		node->getBulk()->addConnectBulk(bulk);
	}
	else {
		if (isBubble()&&bulk!=node->getBulk()) {
			bulk->absorb(node->getBulk());
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
		//CCLOG("currentNode, connection:%d", getBulk()->getConnection());
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
			fromNode->disConnectBubble(type, connectTo);
			connectBubble(type, connectTo);
		}

	}
}

void BubbleNode::extendOuterConnectionFrom(BubbleNode* fromNode) {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType  type = connectType[i];
		int type_i = (int)type;
		if (fromNode->connect[type_i] != nullptr) {
	
				auto connectTo = fromNode->connect[type_i];
				fromNode->disConnectBubble(type, connectTo);
				connectBubble(type, connectTo);
				if (connectTo->getType() != BubbleType::Boundry_Top) {
					connectTo->getBulk()->removeConnectBulk(fromNode->getBulk());
				}
		}

	}
}


	
	/*for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
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

	}*/

void BubbleNode::removeAllConnection() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType  type = connectType[i];
		int type_i = (int)type;
		if (connect[type_i] != nullptr) {
			this->disConnectBubble(type, connect[(int)type]);
		}
	}
}


void 	BubbleNode::moveForward() {
	setPositions(position += velocity);
}


void BubbleNode::attachTo(Point position, std::function<void(BubbleNode* node,BubbleNode* attach)> detect, BubbleNode* param1, BubbleNode* param2) {
		
		auto callback= CallFunc::create([this,detect,param1,param2]() {
			this->position.x = bubble->getPosition().x;
			this->position.y = bubble->getPosition().y;
			this->nextState();
			//this->select();
			detect(param1,param2);
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
			disConnectBubble(type, connect[type_i]);
		}
	}

	auto callback = CallFunc::create([this]() {
		this->getBubble()->removeFromParent();
		this->nextState();
	});
	if (isBubble()) {
		bubble->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -20)), callback, nullptr));
	}
	else {
		this->getBubble()->removeFromParent();
		this->nextState();
	}

}

void BubbleNode::reset() {

}


BubbleNode* BubbleNode::getNeighbourByConnectType(ConnectType type) {
	return connect[static_cast<int>(type)];
}


void BubbleNode::removeAllBulkConnection() {
	bulk->removeAllConnection();
}