#include "GameScene.h"
USING_NS_CC;

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	hexmap = new HexMap(60,360,2);
	auto layer =Layer:: create();
	layer->setContentSize(Size(360, visibleSize.height-50));
	addChild(layer);
	
	DrawNode* drawNode = DrawNode::create();
	Vec2 vertices[] = { Vec2(-1,-1), Vec2(361,-1), Vec2( 361,visibleSize.height-50), Vec2(-1,visibleSize.height-50) };
	drawNode->drawPoly(vertices, 4, true, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	
	layer->addChild(drawNode);
	layer->setAnchorPoint(Point(0.5, 0.5));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(origin.x + visibleSize.width / 2, origin.y+visibleSize.height / 2);
	
	std::list<BubbleNode*> list = hexmap->getBubblesList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		layer->addChild(node->getBubble());
	}
	);
	 list = hexmap->getAttachList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		layer->addChild(node->getBubble());
	}
	);
	list = hexmap->getTopList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		layer->addChild(node->getBubble());
	}
	);


	
	return true;

}

