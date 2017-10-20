#include "ShootLayer.h"

USING_NS_CC;
bool ShootLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	return true;
}


void ShootLayer::initLayout(float bubbleSize) {
	this->bubbleSize = bubbleSize;
	controlPanel = Layer::create();
	controlPanel->setAnchorPoint(Point(0.5, 0));
	controlPanel->ignoreAnchorPointForPosition(false);
	controlPanel->setContentSize(Size(20, 20));
	controlPanel->setPosition(this->getContentSize().width / 2, bubbleSize/2);

	
	auto targetArrow = Sprite::create("asset/up-arrow.png");
	targetArrow->setPosition(controlPanel->getContentSize().width/2, 60);
	targetArrow->setContentSize(Size(50, 50));


	controlPanel->addChild(targetArrow);
	addChild(controlPanel);
	

}

void ShootLayer::changeDirection(bool ifClockwise) {
	if (ifClockwise&&controlPanel->getRotation() > 60) {
		return;
	}
	if (!ifClockwise && controlPanel->getRotation() < -60) {
		return;
	}
	float rotateDegree = ifClockwise ? 1 : -1;
	rotateDegree *= 5;
	controlPanel->runAction(RotateBy::create(0.1, rotateDegree));
}


void ShootLayer::loadBubble() {
	currentBubble = BubbleFactory::getFactory().generateBubble(static_cast<BubbleType>(rand() % 5), Point(this->getContentSize().width / 2, bubbleSize / 2), Size(bubbleSize, bubbleSize));
//		new BubbleNode(static_cast<BubbleType>(rand() % 5),Point(this->getContentSize().width/2,bubbleSize/2),Size(bubbleSize,bubbleSize));
	currentBubble->setBubbleState(BubbleState::READY);
	this->addChild(currentBubble->getBubble());
}
void ShootLayer::resetControlPanel() {
	controlPanel->runAction(RotateTo::create(0.5, 0));
}

BubbleNode* ShootLayer::shoot() {
	this->removeChild(currentBubble->getBubble(),true);
	shootBubble = currentBubble;
	float rotate = (90.0f-controlPanel->getRotation())*PI/180.0f;
	shootBubble->velocity.x = cos(rotate)*SHOOT_SPEED;
	shootBubble->velocity.y = sin(rotate)*SHOOT_SPEED;
	shootBubble->nextState();
	return shootBubble;
}


void ShootLayer::abandon() {
	currentBubble->getBubble()->removeFromParent();
	this->removeChild(currentBubble->getBubble(), true);
	BubbleFactory::getFactory().recycle(currentBubble);
	
	 loadBubble();
	
	
}