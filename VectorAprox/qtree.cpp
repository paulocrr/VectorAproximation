#include "pch.h"
#include "qtree.h"


qtree::qtree(node *root, int maxPointPerRegion)
{
	this->root = root;
	this->maxPointPerRegion = maxPointPerRegion;
}

bool qtree::isInBox(float x, float y, node * r) {
	return (x >= r->topLeftPoint->x && x <= r->botRightPoint->x && y <= r->topLeftPoint->y && y >= r->botRightPoint->y);
}

node * qtree::search(float x, float y) {
	if (!isInBox(x, y, this->root)) {
		return nullptr;
	}
	queue<node *>path;
	node * guide = this->root;
	path.push(guide);
	while (!path.empty() && !guide->isLeaf) {
		for (auto &r : guide->regions) {
			if (isInBox(x, y, r)) {
				path.push(r);
				break;
			}
		}
		path.pop();
		guide = path.front();
	}
	return guide;
}

node * qtree::searchByLevel(float x, float y,int level) {
	if (!isInBox(x, y, this->root)) {
		return nullptr;
	}
	queue<node *>path;
	node * guide = this->root;
	path.push(guide);
	while (!path.empty() && !guide->isLeaf && guide->level<level) {
		for (auto &r : guide->regions) {
			if (isInBox(x, y, r)) {
				path.push(r);
				break;
			}
		}
		path.pop();
		guide = path.front();
	}
	return guide;
}

void qtree::insert(pair<point *, vector<float>> data) {
	node *n = this->search(data.first->x, data.first->y);
	n->data.push_back(data);
	if (n->data.size() < this->maxPointPerRegion) {
		return;
	}
	while (n->data.size() > this->maxPointPerRegion && n->level<10) {
		point *topLeftPoint;
		point *botLeftPoint;
		point *centerPoint = new point((n->topLeftPoint->x + n->botRightPoint->x) / 2, (n->topLeftPoint->y + n->botRightPoint->y) / 2);
		topLeftPoint = n->topLeftPoint;
		botLeftPoint = centerPoint;
		int actualLevel = n->level + 1;
		n->regions[0] = new node(topLeftPoint, botLeftPoint);
		n->regions[0]->level = actualLevel;
		n->regions[0]->isLeaf = true;
		topLeftPoint = new point(centerPoint->x, n->topLeftPoint->y);
		botLeftPoint = new point(n->botRightPoint->x, centerPoint->y);
		n->regions[1] = new node(topLeftPoint, botLeftPoint);
		n->regions[1]->level = actualLevel;
		n->regions[1]->isLeaf = true;
		topLeftPoint = new point(n->topLeftPoint->x, centerPoint->y);
		botLeftPoint = new point(centerPoint->x, n->botRightPoint->y);
		n->regions[2] = new node(topLeftPoint, botLeftPoint);
		n->regions[2]->level = actualLevel;
		n->regions[2]->isLeaf = true;
		n->regions[3] = new node(centerPoint, n->botRightPoint);
		n->regions[3]->level = actualLevel;
		n->regions[3]->isLeaf = true;
		for (auto &d: n->data)
		{
			if (this->isInBox(d.first->x, d.first->y, n->regions[0])) {
				n->regions[0]->data.push_back(d);
			}
			else if (this->isInBox(d.first->x, d.first->y, n->regions[1])) {
				n->regions[1]->data.push_back(d);
			}
			else if (this->isInBox(d.first->x, d.first->y, n->regions[2]))
			{
				n->regions[2]->data.push_back(d);
			}
			else {
				n->regions[3]->data.push_back(d);
			}
		}
		n->isLeaf = 0;
		if (n->regions[0]->data.size() > this->maxPointPerRegion) {
			n = n->regions[0];
		}
		else if (n->regions[1]->data.size() > this->maxPointPerRegion) {
			n = n->regions[1];
		}
		else if (n->regions[2]->data.size() > this->maxPointPerRegion) {
			n = n->regions[2];
		}
		else if(n->regions[3]->data.size() > this->maxPointPerRegion) {
			n = n->regions[3];
		}
		else {
			break;
		}
	}
}

qtree::~qtree()
{
}
