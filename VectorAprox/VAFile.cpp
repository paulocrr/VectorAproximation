#include "pch.h"
#include "VAFile.h"


VAFile::VAFile(int b,int d)
{
	node * root = new node(new point(0.0, 1.0), new point(1.0, 0));
	this->geo = new qtree(root,50);
	string nameFile = "dataSet.txt";
	this->BITS = b;
	for (int j = 1; j <= d; ++j) {
		int b_j = pow(2, (floor(b / d) + (j <= (b%d))));
		vector<float> p;
		double partitionSize = (1.2/(b_j));
		double pointPartition = 0;
		for (int i = 1; i < b_j + 1; ++i) {
			p.push_back(pointPartition);
			pointPartition += partitionSize;
		}
		p.push_back(pointPartition);
		this->p.push_back(p);
	}
	string line;
	ifstream myfile(nameFile);
	vector<float> data;
	int numLines = 0;
	int numElement = 0;
	float posX, posY;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			float number;
			string temp;
			for (int i = 0; i < line.size(); ++i) {
				if (line[i] == '	') {
					if (numElement == 0 ) {
						posX = stod(temp);
					}
					else if (numElement == 1) {
						posY = stod(temp);
					}else{
						data.push_back(stod(temp));
					}
					temp = "";
					++numElement;
					continue;
				}
				temp += line[i];
			}
			if (temp != "") {
				data.push_back(stod(temp));
				temp = "";
			}
			numElement = 0;
			numLines++;
			//cout << "Line " << numLines << endl;
			VANode* node = new VANode;
			node->x = posX;
			node->y = posY;
			node->data = data;
			generateVectorAproximarion(data,node);
			this->aproximation.push_back(node);
			pair<point *, vector<float>>element(new point(posX, posY), data);
			geo->insert(element);
			data.clear();
		}
		myfile.close();
		cout << "Tamaño Vector: " << this->aproximation.size() << endl;;
	}
	else
	{
		cout << "Error al abrir el archivo" << endl;
	}
}

vector<pair<float, VANode*>> VAFile::knnSearch(vector<float> data,int k)
{
	vector<pair<float,VANode*>> ans(k);
	for (auto &p : ans) {
		p.first = numeric_limits<float>::infinity();
	}
	float d = numeric_limits<float>::infinity();
	for (auto &a : this->aproximation) {
		pair<vector<float>, float> li = this->getLowerBound(data, a);
		if (li.second<d)
		{
			d = this->candidate(this->distanceBetweenTwoVectors(data, a->data), ans, a);

		}
	}
	return ans;
}

vector<pair<pair<point*, vector<float>>, vector<pair<float, VANode*>>>> VAFile::knnRegionSearch(point * p, int level, int k)
{
	node * r = this->geo->searchByLevel(p->x, p->y, level);
	//cout << "region encontrada" << r->data.size() << endl;
	vector<pair<pair<point*, vector<float>>, vector<pair<float, VANode*>>>>ans;
	for (auto &d: r->data)
	{
		pair<pair<point*, vector<float>>, vector<pair<float, VANode*>>> t;
		t.first = d;
		vector<pair<float, VANode*>> knn = this->knnSearch(d.second, k);
		t.second = knn;
		ans.push_back(t);
	}
	return ans;
}

void VAFile::generateVectorAproximarion(vector<float> data, VANode *n)
{
	vector<int> regionNumber;
	int d = 0;
	string signature;
	pair<float, float> t;
	for (auto &point : data)
	{
		for (size_t i = 0; i < this->p[d].size() - 1;++i)
		{
			if (pointIsInRegion(point, d, i))
			{
				t.first = this->p[d][i];
				t.second = this->p[d][i+1];
				n->intervals.push_back(t);
				regionNumber.push_back(i);
				++d;
				break;
			}
		}
	}
	n->regions = regionNumber;
	signature = generateSignature(regionNumber);
	n->aproximation=signature;
}



bool VAFile::pointIsInRegion(float point,int d, int r)
{
	return ( point >= this->p[d][r] && point < this->p[d][r+1]);
}

string VAFile::generateSignature(vector<int> regionNumber)
{
	string res;
	string temp;
	for (auto &r : regionNumber)
	{
		bitset<64> bits(fromDecimaltoBinary(r));	
		temp = bits.to_string();
		res += temp.substr(temp.size()-3,this->BITS);
	}
	
	

	return res;
}

string VAFile::fromDecimaltoBinary(int n)
{
	if (n == 0)
	{
		return "0";
	}
	string r;
	int i = 0;
	int binaryNum[32];
	while (n > 0)
	{
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}
	for (int j = i - 1; j >= 0; j--)
		r+=to_string(binaryNum[j]);
	return r;
}

pair<vector<float>,float> VAFile::getLowerBound(vector<float> data, VANode * a)
{
	vector<float>li;
	float sum = 0.0;
	for (size_t j = 0; j < data.size(); ++j)
	{
		for (size_t i = 0; i < this->p[j].size() - 1; ++i)
		{
			if (pointIsInRegion(data[j], j, i))
			{
				if (a->regions[j]<i) {
					li.push_back(data[j] - this->p[j][i+1]);
					sum += (data[j] - this->p[j][i + 1]);
				}
				else if (i == a->regions[j]) {
					li.push_back(0.0);
				}
				else {
					li.push_back(this->p[j][i + 1] - data[j]);
					sum += (this->p[j][i] - data[j]);
				}
				break;
			}
		}
	}
	return pair<vector<float>,float>(li,sum);
}

float VAFile::distanceBetweenTwoVectors(vector<float> q, vector<float> v)
{
	float d = 0.0;
	for (size_t i = 0; i < v.size(); ++i) {
		d += powf((q[i] - v[i]),2.0);
	}
	return d;
}

float VAFile::candidate(float d, vector<pair<float, VANode*>> &ans, VANode *a)
{
	int n = ans.size() - 1;
	if (d < ans[n].first) {
		ans[n].first = d;
		ans[n].second = a;
		sort(ans.begin(), ans.end());
	}
	return ans[n].first;
}


VAFile::~VAFile()
{
}
