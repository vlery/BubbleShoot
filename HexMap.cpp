#include "HexMap.h"

USING_NS_CC;

bool isSamePos(Point p1, Point p2) {

	return (abs(p1.x - p2.x)<3 && abs(p1.y - p2.y)<3);
}
HexMap::HexMap(float size, float width, int layer) {
	this->size = size;
	this->width = width;
	this->layer = layer;
	ifOffset = false;
	initBoundry();
	while (layer > 0) {
		generateBubbleLayer();
		layer--;
	}
	
}

void HexMap::initBoundry() {
	
	int rowNum = ifOffset ? (width - size / 2 * SQRT_3) / size : width / size;

	float offset_x = ifOffset ? size  : size/2;
	ConnectType connectDown = ConnectType::RightBottom;
	/*
	*    generate head
	*		X 
	*	  X  X  
	*
	*/
	
	BubbleNode* node00 = createBubbleToList(BubbleType::Boundry_Top, Point(offset_x, size*(SQRT_3 - 1) / 2));
	BubbleNode* node01;
	if (ifOffset) {
		node01= createBubbleToList(BubbleType::Boundry_Attach, 
			node00->getPosition() + getOffsetByType(ConnectType::LeftBottom));
		node00->connectBubble(reflectY(connectDown), node01);
	}		
;
	node01 = createBubbleToList(BubbleType::Boundry_Attach, 
		node00->getPosition() + getOffsetByType(ConnectType::RightBottom));
	node00->connectBubble(connectDown, node01);
	if (ifOffset) {
		node00->connect[(int)ConnectType::RightBottom]->
			connectBubble(ConnectType::Left, node00->connect[(int)ConnectType::LeftBottom]);
	}


	/*
	*    generate right_botton position      
	*		X  O .. 
	*	  X  X  O..
	*
	*/	
	for (int i = 1; i < rowNum - 1; ++i) {
		BubbleNode* node10 = createBubbleToList(BubbleType::Boundry_Top, node00->getPosition() + Vec2(size, 0));
		BubbleNode* node11 = createBubbleToList(BubbleType::Boundry_Attach, node01->getPosition() + Vec2(size, 0));
		node10->connectBubble(connectDown, node11);
		node10->connectBubble(ConnectType::Left, node00);
		node11->connectBubble(ConnectType::Left, node01);
		node10->connectBubble(reflectY(connectDown), node01);
		node00 = node10;
		node01 = node11;
	}
	/*
	*    generate last bubble
	*		X  O O O
	*	  X  X  O O  ? 
	*
	*/
	BubbleNode* node10 = createBubbleToList(BubbleType::Boundry_Top, node00->getPosition() + Vec2(size, 0));
	node10->connectBubble(ConnectType::Left, node00);
	node10->connectBubble(reflectY(connectDown), node01);
	

	if (node10->getPosition().x + size  <= width) {
		BubbleNode* node11 = createBubbleToList(BubbleType::Boundry_Attach, node01->getPosition() + Vec2(size, 0));
		node10->connectBubble(connectDown, node11);
		node11->connectBubble(ConnectType::Left, node01);
	}
}

void HexMap::generateRow(std::function<void()> moveDown) {
	generateBubbleLayer();
	moveDown();
}

void HexMap::generateBubbleLayer() {
	//up-right,up-left

	/*
	*Better to sort;  Push_back form a sorted list
	*/
	//topBoundries.sort(rowSort);

	/*
	*    generate first bubble
	*	?
	*	  X  X  X  X  X
	*
	*/
	auto list = BubbleFactory::getFactory().getTopList();
	auto itr = list.begin();
	float height_crt_save = (*itr)->getPosition().y;
	auto ul_pos = (*itr)->getPosition() + getOffsetByType(ConnectType::LeftTop);
	BubbleNode* b00=nullptr;
	if (ul_pos.x >= size / 2) {
		b00 = createBubbleToList(BubbleType::Boundry_Top, ul_pos);
	}
	BubbleNode*  b01=nullptr;
	BubbleNode* b10=nullptr;
	BubbleNode* b11=nullptr;
	while (abs((*itr)->getPosition().y - height_crt_save)<0.1) {
		b10 = *(itr);
		BubbleFactory::getFactory().topListPopFront();
		b11 = createBubbleToList(static_cast<BubbleType>(rand()% 5), b10->getPosition());
		b11->extendAllConnectionFrom(b10);
		if (b00 != nullptr) {
			b11->connectBubble(ConnectType::LeftTop, b00);
		}
	
		auto ur_pos = b10->getPosition() + getOffsetByType(ConnectType::RightTop);
		if (ur_pos.x <= width - size / 2) {
			b10->setPositions(ur_pos);
			BubbleFactory::getFactory().addInList(b10);
			b11->connectBubble(ConnectType::RightTop, b10);
			
			if (b00 != nullptr) {
				b10->connectBubble(ConnectType::Left, b00);
			}
		}
		else {
			b10->removeAllConnection();
			BubbleFactory::getFactory().recycle(b10);
			break;
		}
		b01 = b11;
		b00 = b10;
		list = BubbleFactory::getFactory().getTopList();
		itr	= list.begin();
	}


	
}

BubbleNode* HexMap::createBubbleToList(BubbleType type, Point pos) {
	auto node=BubbleFactory::getFactory().generateBubble(type, pos, Size(size, size));
	if (node->getBubble()->getParent() != nullptr) {
		auto z = node;
	}
	node->setBubbleState(BubbleState::ATTACH);
	BubbleFactory::getFactory().addInList(node);

	node->registerBulk();
	
	return node;
}


BubbleNode* HexMap::generateAttachReplace(BubbleNode* replace) {
	BubbleNode* node = createBubbleToList(BubbleType::Boundry_Attach, replace->getPosition());
	node->extendOuterConnectionFrom(replace);
	return node;
}

void 	 HexMap::generateAttachNodeAround(BubbleNode* node) {
	
	
	for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
		ConnectType type = node->connectType[i];
		int type_i = (int)type;
		if (node->getNeighbour(node->connectType[i])== nullptr) {
			Point pos = node->getPosition() + getOffsetByType(type);
			if (pos.x<size / 2 || pos.x>width - size / 2) {
				continue;
			}
			bool ifConnect = false;
			
			auto attach_list = BubbleFactory::getFactory().getAttachList();
			auto find_bb=getFirstMatchBubble(&attach_list, [pos](BubbleNode* node) {
				if (!node->isAttached()) {
					return false;
				}
				if (isSamePos(node->getPosition(), pos)) {
					return true;
				}
				return false;
			});

			if (find_bb != nullptr) {
				ifConnect = true;
				node->connectBubble(type, find_bb);
			}
			
			if (!ifConnect) {
				auto bubble_list = BubbleFactory::getFactory().getBubblesList();

				auto find_bb = getFirstMatchBubble(&bubble_list, [pos](BubbleNode* node) {
					if (!node->isAttached()) {
						return false;
					}
					if (isSamePos(node->getPosition(), pos)) {
						return true;
					}
					return false;
				});
				if (find_bb != nullptr) {
				ifConnect = true;
				node->connectBubble(type, find_bb);
			}
	
			}
		
			if (!ifConnect) {
				BubbleNode* attach=createBubbleToList(BubbleType::Boundry_Attach, pos);
				node->connectBubble(type, attach);
			}
		
		}
	}

}


Vec2 HexMap::getOffsetByType(ConnectType type) {
	Vec2 offset = Vec2::ZERO;
	switch (type) {
	case  ConnectType::Left:
		offset = Vec2(-1, 0);
		break;
	case ConnectType::Right:
		offset = Vec2(1, 0);
		break;
	case ConnectType::LeftBottom:
		offset = Vec2(-0.5f, -SQRT_3 / 2);
		break;
	case ConnectType::LeftTop:
		offset = Vec2(-0.5f, SQRT_3 / 2);
		break;
	case ConnectType::RightBottom:
		offset = Vec2(0.5f, -SQRT_3 / 2);
		break;
	case ConnectType::RightTop:
		offset = Vec2(0.5f, SQRT_3 / 2);
		break;
	Default:;
	}
	return offset*size;
}