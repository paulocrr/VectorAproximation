#include "pch.h"
#include "node.h"


node::node(point *topLeftPoint, point *botRightPoint)
{
	this->topLeftPoint = topLeftPoint;
	this->botRightPoint = botRightPoint;
}


node::~node()
{
}
