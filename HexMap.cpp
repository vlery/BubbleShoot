#include "HexMap.h"
#include "cocos2d.h"
USING_NS_CC;
HexMap::HexMap(float size, float width, int layer) {
	this->size = size;
	this->width = width;
	this->layer = layer;
	ifOffset = true;
	initBoundry();
	while (layer > 0) {
		generateLayer();
		
		
		layer--;
	}





}


void HexMap::initBoundry() {

	int rowNum = ifOffset ? (width - size / 2 * SQRT_3) / size : width / size;
	Vec2 layerOffset_rd = Vec2(1, -SQRT_3)*size / 2;
	Vec2 layerOffset_ld = Vec2(-1, -SQRT_3)*size / 2;
	float offset_x = ifOffset ? size  : 0;
	ConnectType connectDown = ConnectType::RightBottom;
	/*
	*    generate head
	*		X 
	*	  X  X  
	*
	*/
	
	BubbleNode* node00 = new BubbleNode(BubbleType::Boundry_Top, Point(offset_x, size/2),Size(size,size));
	//bubbles.push_back(node00);
	topBoundries.push_back(node00);
	BubbleNode* node01;
	if (ifOffset) {
		node01= new BubbleNode(BubbleType::Boundry_Attach, node00->getPosition() + layerOffset_ld, Size(size, size));
	//	bubbles.push_back(node01);
		potentialAttachPositions.push_back(node01);
		node00->connectBubble(reflectY(connectDown), node01);
	}		
		node01 = new BubbleNode(BubbleType::Boundry_Attach, node00->getPosition() + layerOffset_rd, Size(size, size));
	//	bubbles.push_back(node01);
		potentialAttachPositions.push_back(node01);
		node00->connectBubble(connectDown, node01);


	/*
	*    generate right_botton position      
	*		X  O .. 
	*	  X  X  O..
	*
	*/	
	for (int i = 1; i < rowNum - 1; ++i) {
		BubbleNode* node10 = new BubbleNode(BubbleType::Boundry_Top, node00->getPosition() + Vec2(size, 0), Size(size, size));
		BubbleNode* node11 = new BubbleNode(BubbleType::Boundry_Attach, node01->getPosition() + Vec2(size, 0), Size(size, size));
		node10->connectBubble(connectDown, node11);
		node10->connectBubble(ConnectType::Left, node00);
		node11->connectBubble(ConnectType::Left, node01);
		node10->connectBubble(reflectY(connectDown), node01);
	//	bubbles.push_back(node10);
	//	bubbles.push_back(node11);
		topBoundries.push_back(node10);
		potentialAttachPositions.push_back(node11);
		node00 = node10;
		node01 = node11;
	}
	/*
	*    generate last bubble
	*		X  O O O
	*	  X  X  O O  ? 
	*
	*/
	BubbleNode* node10 = new BubbleNode(BubbleType::Boundry_Top, node00->getPosition() + Vec2(size, 0),  Size(size, size));
	node10->connectBubble(ConnectType::Left, node00);
	node10->connectBubble(reflectY(connectDown), node01);
//	bubbles.push_back(node10);
	topBoundries.push_back(node10);

	if (node01->getPosition().x + size * 3 / 2 <= width) {
		BubbleNode* node11 = new BubbleNode(BubbleType::Boundry_Attach, node01->getPosition() + Vec2(size, 0),  Size(size, size));
		potentialAttachPositions.push_back(node11);
//		bubbles.push_back(node11);
		node10->connectBubble(connectDown, node11);
		node11->connectBubble(ConnectType::Left, node01);
	}


}

void HexMap::generateLayer() {
	//up-right,up-left
	Vec2 layerOffset_ur = Vec2(1, SQRT_3)*size / 2;
	Vec2 layerOffset_ul = Vec2(-1, SQRT_3)*size / 2;
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
	auto itr = topBoundries.begin();
	float height_crt_save = (*itr)->getPosition().y;
	auto ul_pos = (*itr)->getPosition() + layerOffset_ul;
	if (ul_pos.x >= size / 2) {
		BubbleNode* b00 = new BubbleNode(BubbleType::Boundry_Top, ul_pos, Size(size, size));
		topBoundries.push_back(b00);
		
	}
	itr = topBoundries.begin();
	while (abs((*itr)->getPosition().y - height_crt_save)<0.1) {
		BubbleNode* b01 = *(itr);
		topBoundries.pop_front();
		BubbleNode* b00 = new BubbleNode(static_cast<BubbleType>(rand()%5), b01->getPosition(), Size(size, size));
		bubbles.push_back(b00);
		
		auto ur_pos = b01->getPosition() + layerOffset_ur;
		if (ur_pos.x <= width - size / 2) {
			b01->setPosition(ur_pos);
			topBoundries.push_back(b01);
		}
		else {
			delete b01;
		}

		itr	= topBoundries.begin();
	}


	
}






