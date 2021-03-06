#include "BubbleNode.h";
#include "Bulk.h"

USING_NS_CC;
ConnectType  BubbleNode::connectType[] = { ConnectType::LeftTop,ConnectType::RightTop,
																	ConnectType::Left, ConnectType::Right,
																ConnectType::LeftBottom, ConnectType::RightBottom };
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

bool testAroundBubble(BubbleNode* node, std::function<bool(BubbleNode* node)> test) {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		if (test(node->getNeighbour(node->connectType[i]))) {
			return true;
		}
	}
	return false;
}


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
		bubble = Sprite::create(BUBBLE_IMG_PATH, CCRectMake(i*RESOURCE_BUBBLE_SIZE, 0, RESOURCE_BUBBLE_SIZE, RESOURCE_BUBBLE_SIZE));
	}else {
		bubble = Sprite::create(BUBBLE_DEFAULT_IMG_PATH, CCRectMake(0, 0, RESOURCE_BUBBLE_SIZE, RESOURCE_BUBBLE_SIZE));
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
	if (node->isTopBoundry() &&! node->isSameType(this)) {
		if (isPotentialAttach()) {
			node->bulk->disConnectToAttach();
		}
		bulk->disConnectToTop();
	}
	
	if (isTopBoundry() &&!node->isSameType(this)) {
		if (node->isPotentialAttach()) {
			bulk->disConnectToAttach();
		}
		node->getBulk()->disConnectToTop();
	}

	if (node->isPotentialAttach() && isBubble()) {
		bulk->disConnectToAttach();
	}
	if (isPotentialAttach() &&node-> isBubble()) {
		node->bulk->disConnectToAttach();
	}
	node->connect[(int)opposite(ctype)] = nullptr;
	connect[(int)ctype] = nullptr;
}




void BubbleNode::registerBulk() {
	bulk = new Bulk(getType());
	bulk->attach(this);
}

void BubbleNode::connectBulk( BubbleNode* node) {

	if (node->isTopBoundry()) {
		if(!node->isSameType(this)){
			bulk->connectToTop();
		}
		if (isPotentialAttach()) {
			node->bulk->connectToAttach();
		}
		return;
	}
	if (isTopBoundry()) {
		if (!node->isSameType(this)) {
			node->getBulk()->connectToTop();
		}
		if (node->isPotentialAttach()) {
			bulk->connectToAttach();
		}
		return;
	}



	if (!node->isSameType(this)) {
		bulk->addConnectBulk(node->getBulk());
		node->getBulk()->addConnectBulk(bulk);
		
		if (node->isPotentialAttach()) {
			bulk->connectToAttach();
		}
		if (isPotentialAttach()) {
			node->bulk->connectToAttach();
		}
	}
	else {
		if (isBubble()&&!node->ifBelongToSameBulk(this)) {
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
		if (BUBBLE_DEBUG) {
			CCLOG("currentNode, bulk nodeNum:%d", getBulk()->getNodeNum());
			CCLOG("currentNode, type:%d", getType());
			//CCLOG("currentNode, Top connect:%d", (int)getBulk()->connectTopCount);
			//CCLOG("currentNode, Attach connect:%d", (int)getBulk()->connectAttachCount);

		}
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
		auto connectTo = fromNode->getNeighbour(connectType[i]);
		if (connectTo != nullptr) {
			fromNode->disConnectBubble(connectType[i], connectTo);
			connectBubble(connectType[i], connectTo);
			if (!connectTo->isTopBoundry()) {
				connectTo->getBulk()->removeConnectBulk(fromNode->getBulk());
			}
		}
	}


}

void BubbleNode::extendOuterConnectionFrom(BubbleNode* fromNode) {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		auto connectTo = fromNode->getNeighbour(connectType[i]);
		if (connectTo != nullptr&&!connectTo->ifBelongToSameBulk(fromNode)) {
				fromNode->disConnectBubble(connectType[i], connectTo);
				connectBubble(connectType[i], connectTo);
				if (!connectTo->isTopBoundry()) {
					connectTo->getBulk()->removeConnectBulk(fromNode->getBulk());
				}
		}

	}
}

void BubbleNode::removeAllConnection() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		auto connectTo =getNeighbour(connectType[i]);
		if (connectTo != nullptr) {
			this->disConnectBubble(connectType[i], connectTo);
		}
	}
}


void 	BubbleNode::moveForward() {
	setPositions(position += velocity);
}


void BubbleNode::attachTo(Point position, std::function<void(BubbleNode* node,BubbleNode* attach)> detect, BubbleNode* node, BubbleNode* attach) {
		
		auto callback= CallFunc::create([this,detect,node,attach]() {
			this->position.x = bubble->getPosition().x;
			this->position.y = bubble->getPosition().y;
			this->nextState();
			detect(node,attach);
		});
		/*		
		Vec2 velocityDir = velocity;
		velocityDir.normalize();
	
		ccBezierConfig bezier;
		bezier.controlPoint_1 = 2*velocityDir;
		bezier.endPosition = position-this->position;
		bezier.controlPoint_2 =bezier.endPosition;
		auto bezierForward = BezierBy::create(2/velocity.length() , bezier);
		bubble->runAction(Sequence::create(bezierForward, callback, nullptr));
		*/
		bubble->runAction(Sequence::create(MoveTo::create(2 / velocity.length(),position), callback, nullptr));

}



void BubbleNode::setBubbleState(BubbleState state) {
	this->bubbleState = state;
}
void BubbleNode::nextState() {
	if (bubbleState >= BubbleState::DEAD) {
		auto a = bubbleState;
	}
	bubbleState =static_cast<BubbleState>( static_cast<int>(bubbleState)+1);
	
}

void BubbleNode::destorySelf() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		auto connectTo = getNeighbour(connectType[i]);
		
		if (connectTo != nullptr) {
			disConnectBubble(connectType[i], connectTo);
		}
	}

	auto callback = CallFunc::create([=]() {
		bubble->removeFromParent();
		this->nextState();
		
	});
	if (isBubble()) {
		this->nextState();
		bubble->runAction(Sequence::create(MoveBy::create(1, Vec2(0, -500)), callback, nullptr));
	}
	else {
		auto debug_temp = bubbleState;
		this->getBubble()->removeFromParent();
		this->nextState();
		this->nextState();
		//bubbleState = BubbleState::DEAD;
	}

}

void BubbleNode::reset() {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		connect[i] = nullptr;
	}
	bulk = nullptr;	
	if (bubble->getParent() != nullptr) {
		bubble->removeFromParent();
	}
	attachTarget = nullptr;
}


BubbleNode* BubbleNode::getNeighbourByConnectType(ConnectType type) {
	return connect[static_cast<int>(type)];
}




bool BubbleNode::isDead() {
	return bubbleState == BubbleState::DEAD;
}

bool BubbleNode::isAttached() {
	return bubbleState== BubbleState::ATTACH;
}

bool BubbleNode::isSameType(BubbleNode* node) {
	return type == node->type;
}

bool BubbleNode::isTopBoundry() {
	return type == BubbleType::Boundry_Top;
}

bool BubbleNode::ifBelongToSameBulk(BubbleNode* node) {
	return bulk == node->bulk;
}

bool BubbleNode::isPotentialAttach() {
	return type == BubbleType::Boundry_Attach;
}

bool BubbleNode::isBubble() {
	return type <= BubbleType::Bubble_PINK;

}

bool BubbleNode::isInShoot() {
	return bubbleState==BubbleState::SHOOT;
}



