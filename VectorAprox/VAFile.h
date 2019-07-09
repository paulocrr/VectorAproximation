#pragma once
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include "VANode.h"
#include <iostream>
#include <limits>
#include <algorithm> 
#include "qtree.h"
using namespace std;
class VAFile
{
private:
	int BITS;
	vector<vector<float>> p;
	vector<VANode*> aproximation;
	bool pointIsInRegion(float point, int d, int r);
	string generateSignature(vector<int> regionNumber);
	void generateVectorAproximarion(vector<float> data,VANode *n);
	string fromDecimaltoBinary(int r);
	pair<vector<float>, float> getLowerBound(vector<float> data, VANode *a);
	float distanceBetweenTwoVectors(vector<float>q, vector<float>v);
	float candidate(float d,vector<pair<float,VANode*>> &ans, VANode *a);
	qtree *geo;
public:
	/**
	* d: Numero de Dimensiones
	* b: Numero de Bits por dimension
	**/
	VAFile(int b,int d);
	vector<pair<float, VANode*>> knnSearch(vector<float> data,int k);
	vector<
		pair<
			pair<point *, vector<float>>,
			vector<pair<float, VANode*>>
		>
	> knnRegionSearch(point *p,int level, int k);
	~VAFile();
};

