#include "GameScene.h"

USING_NS_CC;

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}


	initLayout();
	initKeyboardListener();

	return true;

}
void GameScene::initLayout() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255), visibleSize.width, visibleSize.height);

	addChild(background);


	auto layer = Layer::create();
	layer->setContentSize(Size(360, visibleSize.height - 50));

	layer->setAnchorPoint(Point(0.5, 0.5));
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(layer);


	DrawNode* drawNode = DrawNode::create();
	Vec2 vertices[] = { Vec2(-1,-1), Vec2(361,-1), Vec2(361,visibleSize.height - 50), Vec2(-1,visibleSize.height - 50) };
	drawNode->setLineWidth(4);
	drawNode->drawPoly(vertices, 4, true, Color4F(BORDER_COLOR[0], BORDER_COLOR[1], BORDER_COLOR[2], BORDER_COLOR[3]));
	layer->addChild(drawNode, 1);

	LayerColor* gameBackground = LayerColor::create(Color4B(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], BG_COLOR[3]), layer->getContentSize().width, layer->getContentSize().height);

	layer->addChild(gameBackground);

	bubbleLayer = BubbleLayer::create();
	bubbleLayer->setPosition(0, layer->getContentSize().height);
	bubbleLayer->setWidth(layer->getContentSize().width);
	bubbleLayer->initBubbles(INIT_BUBBLE_SIZE, INIT_LAYER);
	bubbleLayer->addBubbles();



	layer->addChild(bubbleLayer, 1);


	shootLayer= ShootLayer::create();
	shootLayer->setContentSize(layer->getContentSize());
	shootLayer->initLayout(INIT_BUBBLE_SIZE);
	shootLayer->loadBubble();
	layer->addChild(shootLayer);

}
void GameScene::initKeyboardListener() {
	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_SPACE:
			bubbleLayer->addLayer();
			break;
		case EventKeyboard::KeyCode::KEY_O:
			bubbleLayer->setOriginSelect();
			break;
		case EventKeyboard::KeyCode::KEY_Y:
			bubbleLayer->moveSelect(ConnectType::LeftTop);
			break;
		case EventKeyboard::KeyCode::KEY_G:
			bubbleLayer->moveSelect(ConnectType::Left);
			break;
		case EventKeyboard::KeyCode::KEY_B:
			bubbleLayer->moveSelect(ConnectType::LeftBottom);
			break;
		case EventKeyboard::KeyCode::KEY_N:
			bubbleLayer->moveSelect(ConnectType::RightBottom);
			break;
		case EventKeyboard::KeyCode::KEY_J:
			bubbleLayer->moveSelect(ConnectType::Right);
			break;
		case EventKeyboard::KeyCode::KEY_U:
			bubbleLayer->moveSelect(ConnectType::RightTop);
			break;

		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			shootLayer->changeDirection(false);
			break;

		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			shootLayer->changeDirection(true);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			shootBubble();
			break;

			/*
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
			event->getCurrentTarget()->setPosition(--loc.x, loc.y);
			break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
			event->getCurrentTarget()->setPosition(++loc.x, loc.y);
			break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			case EventKeyboard::KeyCode::KEY_W:
			event->getCurrentTarget()->setPosition(loc.x, ++loc.y);
			break;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			case EventKeyboard::KeyCode::KEY_S:
			event->getCurrentTarget()->setPosition(loc.x, --loc.y);
			break;
			*/
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void GameScene::shootBubble() {
	BubbleNode* bubble = shootLayer->shoot();
	shootLayer->loadBubble();
//	shootLayer->resetControlPanel();
	bubble->setPositions(correctPosCrt2BB(bubble->getPosition()));
	bubbleLayer->processShootBubble(bubble);
}

Point GameScene::correctPosCrt2BB(Point position) {
	Point posCrt2World = position + shootLayer->getPosition();
	Point posWorld2BB = posCrt2World - bubbleLayer->getPosition();
	return posWorld2BB;
}