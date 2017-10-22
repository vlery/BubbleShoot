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
						node->attachTarget = nullptr;
					}else if (node->getPosition().x >= width-bubbleSize / 2) {
						node->velocity.x *= -1;
						node->attachTarget = nullptr;
					}
					node->moveForward();
					this->testAttach_ByBubble(node);
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

	auto list =BubbleFactory::getFactory().getBubblesList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead()&&node->getBubble()->getParent()==nullptr) {
			addChild(node->getBubble());
		}
	});


	list = BubbleFactory::getFactory().getAttachList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead() && node->getBubble()->getParent() == nullptr) {
			addChild(node->getBubble());
		}
	}
	);

	list = BubbleFactory::getFactory().getTopList();
	std::for_each(list.begin(), list.end(), [=](BubbleNode* node) {
		if (!node->isDead() && node->getBubble()->getParent() == nullptr) {
			addChild(node->getBubble());
		}
	}
	);
	

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
	auto list = BubbleFactory::getFactory().getAttachList();
	auto itr = list.begin();
	while (itr != list.end()) {
		if (!(*itr)->isDead()) {
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
/*
*testAttach and testAttach1 use potential attach set
*/
void BubbleLayer::testAttach(BubbleNode* node) {
	float boundry_L = LARGE_DISTANCE_TEST*bubbleSize;
	float boundry_M= bubbleSize *MEDIUM_DISTANCE_TEST ;
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
				if (angle > MEDIUM_DISTANCE_TEST_ANGLE_COS) {
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



void BubbleLayer::testAttach1(BubbleNode* node) {
	float boundry_L = LARGE_DISTANCE_TEST*bubbleSize;
	float boundry_M = bubbleSize *MEDIUM_DISTANCE_TEST;
	float boundry_S = bubbleSize*0.51;
	if (node->attachTarget == nullptr) {
		float max_angle = 0.95;
		auto attacList = BubbleFactory::getFactory().getAttachList();
		auto itr = attacList.begin();
		while (itr != attacList.end()) {
			if (!(*itr)->isDead()) {
				Point attachPos = (*itr)->getPosition();
				Point nodePos = node->getPosition();
				Vec2 connectDir = attachPos - nodePos;
				connectDir.normalize();
				Vec2 moveDir = node->velocity;
				moveDir.normalize();
				float angle = connectDir.dot(moveDir);
				if (angle > max_angle) {
					node->attachTarget = (*itr);
					max_angle = angle;
				}
			}
			itr++;
		}
		if (node->attachTarget == nullptr) {
			node->attachTarget = node;
		}
	}
	else {
	
		if (node->attachTarget != node) {
			auto attacList = BubbleFactory::getFactory().getAttachList();
			auto itr = attacList.begin();
			while (itr != attacList.end()) {

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
					if (distance < boundry_S) {
					
							node->nextState();
							attachBubble(node, (*itr));
							if (BUBBLE_DEBUG) {
								CCLOG("AttachWay: small distance");
							}
							break;
						
					}
					else {
						if (angle > 0.85) {
							node->nextState();
							attachBubble(node, (*itr));
							if (BUBBLE_DEBUG) {
								CCLOG("AttachWay: medium distance");
							}
							break;
						}
					}
				}
				itr++;
			}
		}
		else {
			auto attacList = BubbleFactory::getFactory().getAttachList();
			auto itr = attacList.begin();
			while (itr != attacList.end()) {
				if (!(*itr)->isDead()) {
					Point attachPos = (*itr)->getPosition();
					Point nodePos = node->getPosition();
					if (abs(attachPos.x - nodePos.x) + abs(attachPos.y - nodePos.y) > boundry_L) {
						++itr;
						if (abs(attachPos.x - nodePos.x) + abs(attachPos.y - nodePos.y) > 1000) {
							node->nextState();
							node->nextState();
							BubbleFactory::getFactory().recycle(node);
						}
						continue;
					}
					float distance = attachPos.distance(nodePos);
					Vec2 connectDir = attachPos - nodePos;
					connectDir.normalize();
					Vec2 moveDir = node->velocity;
					moveDir.normalize();
					if (distance < boundry_M) {
						node->nextState();
						attachBubble(node, (*itr));
						if (BUBBLE_DEBUG) {
							CCLOG("AttachWay: SMALL distance");
						}
						break;
					}
					
				}
				itr++;
			}
		}


	}




}



void BubbleLayer::testAttach_ByBubble(BubbleNode* node) {
	float boundry_L = LARGE_DISTANCE_TEST*bubbleSize;
	auto bubbleList = BubbleFactory::getFactory().getBubblesList();
	auto bb_itr = bubbleList.begin();
	Point nodePos = node->getPosition();
	while (bb_itr != bubbleList.end()) {
		auto bb = (*bb_itr);
		Point bbPos = bb->getPosition();

		if (!bb->getBulk()->ifConnectAttach()) {
			++bb_itr;
			continue;
		}
		if (abs(bbPos.x - nodePos.x) + abs(bbPos.y - nodePos.y) > boundry_L) {
			++bb_itr;
			continue;
		}

		float distance = bbPos.distance(nodePos);
		float dot = -1;
		
		if (distance < bubbleSize) {
			float attach2node = 3 * bubbleSize;
			BubbleNode* nearestAttach = nullptr;
			for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
				auto neighbour = bb->getNeighbour(bb->connectType[i]);
				if (neighbour!=nullptr&&neighbour->isPotentialAttach()) {
					float a2n = nodePos.getDistance(neighbour->getPosition());
					if (a2n < attach2node) {
						nearestAttach = neighbour;
						attach2node = a2n;
					}
				}
				
			}
			node->nextState();
			attachBubble(node,nearestAttach);
			if (BUBBLE_DEBUG) {
				CCLOG("AttachWay: nearest Attach of collision Bubble");
			}
			break;
		
		}



		++bb_itr;
	}

	 bubbleList = BubbleFactory::getFactory().getTopList();
	 bb_itr = bubbleList.begin();

	while (bb_itr != bubbleList.end()) {
		auto bb = (*bb_itr);
		Point bbPos = bb->getPosition();

		if (!bb->getBulk()->ifConnectAttach()) {
			++bb_itr;
			continue;
		}
		if (abs(bbPos.x - nodePos.x) + abs(bbPos.y - nodePos.y) > boundry_L) {
			++bb_itr;
			continue;
		}

		float distance = bbPos.distance(nodePos);
		float dot = -1;

		if (distance < bubbleSize + 5) {
			float attach2node = 3 * bubbleSize;
			BubbleNode* nearestAttach = nullptr;
			for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
				auto neighbour = bb->getNeighbour(bb->connectType[i]);
				if (neighbour != nullptr&&neighbour->isPotentialAttach()) {
					float a2n = nodePos.getDistance(neighbour->getPosition());
					if (a2n < attach2node) {
						nearestAttach = neighbour;
						attach2node = a2n;
					}
				}

			}
			node->nextState();
			attachBubble(node, nearestAttach);
			if (BUBBLE_DEBUG) {
				CCLOG("AttachWay: nearest Attach of collision Bubble");
			}
			break;

		}



		++bb_itr;
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