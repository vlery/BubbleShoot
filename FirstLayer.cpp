#include "FirstLayer.h"
USING_NS_CC;
Layer* FirstLayer::createLayer() {
	
	return FirstLayer::create();
	

}
bool FirstLayer::init() {

	if (!Layer::init()) {
		return false;
	}

	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(0,0));

	// add the label as a child to this layer
	this->addChild(label, 1);
	*/
	// add "HelloWorld" splash screen"
	this->setPosition(origin.x, origin.y);
	//this->ignoreAnchorPointForPosition(true);
	auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	sprite->setPosition(visibleSize.width/2,visibleSize.height/2);
	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	

	return true;
}

int FirstLayer::test() {
	int a = 1;
	int b = 2;
	int c = a + b;
	return c;
}