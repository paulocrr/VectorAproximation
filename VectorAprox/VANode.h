#pragma once
#include <vector>
#include <utility> 
using namespace std;
class VANode
{
public:
	float x;
	float y;
	vector<float> data;
	string aproximation;
	vector<pair<float, float>> intervals;
	vector<int> regions;
	VANode();
	~VANode();
};

