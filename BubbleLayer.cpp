#include "BubbleLayer.h"
#include "Bulk.h"
USING_NS_CC;
bool BubbleLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	this->scheduleUpdate();
	return true;
	
}
void BubbleLayer::update(float delta) {
	if (shootList.size() > 0) {
		auto begin = shootList.begin();
		auto end = shootList.end();
		std::for_each(begin, end,[=](BubbleNode* node) {
			if (node->getState() == BubbleState::SHOOT) {
				
			
					if (node->getPosition().x <= bubbleSize / 2) {
						node->velocity.x *= -1;
					}else if (node->getPosition().x >= width-bubbleSize / 2) {
						node->velocity.x *= -1;
					}
					node->moveForward();
					this->testAttach(node);
			}
		});
		this->cleanShootList();
	}
	if (++cleanTimer > CLEAN_INTERVAL) {
		BubbleFactory::getFactory().recycle();
		cleanTimer = 0;
	}
}


void BubbleLayer::setWidth(float w) {
	this->width = w;
	setContentSize(Size(width, 2));
}

void BubbleLayer::initBubbles(int bubbleSize,int layer) {
	this->bubbleSize = bubbleSize;
	hexmap = new HexMap(bubbleSize, width, layer);
	while (layer) {
		moveDown();
		layer--;
	}

}
void BubbleLayer::addBubbles() {
	removeAllChildren();
	auto list =BubbleFactory::getFactory().getBubblesList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead()) {
			addChild(node->getBubble());
		}
	});


	list = BubbleFactory::getFactory().getAttachList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead()) {
			addChild(node->getBubble());
		}
	}
	);

	list = BubbleFactory::getFactory().getTopList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead()) {
			addChild(node->getBubble());
		}
	}
	);

	std::for_each(shootList.begin(), shootList.end(), [=](BubbleNode* node) {
		if (node->getState() == BubbleState::SHOOT) {
			addChild(node->getBubble());
		}
	});
}


void BubbleLayer::addLayer() {
	hexmap->generateRow([this]() {
		this->addBubbles();
		moveDown();
	});
	
}

void BubbleLayer::moveDown() {
	this->runAction(
		MoveBy::create(ANM_TIME, Vec2(0, -SQRT_3 / 2* bubbleSize))
		);
}

void BubbleLayer::moveSelect(ConnectType type) {
	if (!BUBBLE_DEBUG) {
		return;
	}
	if (select == nullptr) {
		return;
	}
	BubbleNode* next = select->getNeighbour(type);
	if (next != nullptr) {
		select->unselect();
		select = next;
		select->select();
	}
	/*
	if (next != nullptr&&next->isBubble()) {
	

		if (next->getBulk() == select->getBulk()) {
			select->select();
			select = next;
			select->getBubble()->setOpacity(10);
		}
		else {
			
			select->getBulk()->unselect();
			select = next;
			select->getBulk()->select();
			select->getBubble()->setOpacity(10);
		}
	}
	*/
}

void BubbleLayer::setOriginSelect() {
	if (!BUBBLE_DEBUG) {
		return;
	}
	/*
	if (select != nullptr) {
		select->getBulk()->unselect();
	}
	*/
	auto list = BubbleFactory::getFactory().getBubblesList();
	auto itr = list.begin();
	while (itr != list.end()) {
		if ((*itr)->getState() != BubbleState::DEAD) {
			select = (*itr);
			select->select();
			break;
		}
		++itr;
	}
	//select = (*BubbleFactory::getFactory().getBubblesList().begin());
	//select= (*(list.begin()));

//	select->getBulk()->select();
	
}


void BubbleLayer::processShootBubble(BubbleNode* bubble) {
	addChild(bubble->getBubble());
	shootList.push_back(bubble);
}

void BubbleLayer::testAttach(BubbleNode* node) {
	float boundry_L = LARGE_DISTANCE_TEST*bubbleSize;
	float boundry_M= bubbleSize *SMALL_DISTANCE_TEST ;
	auto attacList = BubbleFactory::getFactory().getAttachList();
	auto itr = attacList.begin();
	while (itr!= attacList.end()) {

		if (!(*itr)->isDead()) {
			Point attachPos = (*itr)->getPosition();
			Point nodePos = node->getPosition();

			if (abs(attachPos.x - nodePos.x) + abs(attachPos.y - nodePos.y) > boundry_L) {
				++itr;
				continue;
			}
			float distance = attachPos.distance(nodePos);
			Vec2 connectDir = attachPos - nodePos;
			connectDir.normalize();
			Vec2 moveDir = node->velocity;
			moveDir.normalize();
			float angle = connectDir.dot(moveDir);
			if (distance < boundry_M) {
				if (angle > SMALL_DISTANCE_TEST_ANGLE_COS) {
					node->nextState();
					attachBubble(node, (*itr));
					if (BUBBLE_DEBUG) {
						CCLOG("AttachWay: small distance");
					}
					break;
				}
			}
			else {
				if (angle > LARGE_DISTANCE_TEST_ANGLE_COS) {
					node->nextState();
					attachBubble(node, (*itr));
					if (BUBBLE_DEBUG) {
						CCLOG("AttachWay: Large distance");
					}
					break;
				}
				//node->velocity.normalize();
				//node->velocity *= SHOOT_SPEED;
			}
		}
		itr++;
	}

}
void BubbleLayer::attachBubble(BubbleNode* node, BubbleNode* attachNode) {
	node->attachTo(attachNode->getPosition(), 
		[this](BubbleNode* node, BubbleNode* attachNode) {
				this->checkThreeMatch(node, attachNode);
		},
		node,attachNode);
}

void BubbleLayer::checkThreeMatch(BubbleNode* node, BubbleNode* attachNode) {
	node->registerBulk();
	BubbleFactory::getFactory().addInList(node);
	node->extendAllConnectionFrom(attachNode);
	attachNode->destorySelf();
	attachNode->getBulk()->removeAllConnection();
	
	bool ifShootBulk = (node->getBulk()->getNodeNum()>2);
	BubbleNode* neighbour;
	

	if (ifShootBulk) {
		combineUnboundBulkAround(node->getBulk());
		auto bulk = node->getBulk();
		
		auto outerConnect = bulk->getFirstConnectOuterNode();
		if (outerConnect != nullptr) {
			generateAttachReplace(outerConnect);

		}
		while (bulk->ifHasNextConnectOuterNode()) {
			outerConnect = bulk->getNextConnectOuterNode();
			generateAttachReplace(outerConnect);
		}
		bulk->destory();
	}
	else {
		hexmap->generateAttachNodeAround(node);
		addBubbles();
	}
}

void BubbleLayer::cleanShootList() {
	auto itr = shootList.begin();
	while (itr != shootList.end()) {
		if ((*itr)->isAttached()||((*itr)->isDead())) {
			itr=shootList.erase(itr);
		}
		else {
			++itr;
		}
	}

}

void BubbleLayer::generateAttachReplace(BubbleNode* outerConnect) {
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		auto temp = outerConnect->connect[(int)outerConnect->connectType[i]];
		if (temp != nullptr) {
			if (temp->isTopBoundry()) {
				addChild(hexmap->generateAttachReplace(outerConnect)->getBubble());
				break;
			}
			else {
				if (temp->isBubble() &&! temp->ifBelongToSameBulk( outerConnect)) {
					temp->getBulk()->removeConnectBulk(outerConnect->getBulk());
					addChild(hexmap->generateAttachReplace(outerConnect)->getBubble());
					break;
				}
			}
		}
	}
}