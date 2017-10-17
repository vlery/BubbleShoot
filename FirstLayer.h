#ifndef __FIRST_LAYER_H
#define __FIRST_LAYER_H
#include "cocos2d.h"


class FirstLayer : public cocos2d::Layer {
public :
	static cocos2d::Layer* createLayer();
	virtual bool init();
	int test();
	CREATE_FUNC(FirstLayer);

};
#endif // !__FIRST_LAYER_H#pragma once
