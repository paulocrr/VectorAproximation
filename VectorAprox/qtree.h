#pragma once
#include"node.h"
#include <queue>
#include <iostream>
using namespace std;
class qtree
{
private:
	node* root;
	bool isInBox(float x, float y, node* r);
public:
	int maxPointPerRegion;
	qtree(node * root,int maxPointPerRegion);
	node *search(float x, float y);
	node * searchByLevel(float x, float y, int level);
	void insert(pair<point *, vector<float>> data);
	~qtree();
};

