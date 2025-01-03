#pragma once
#include <vector>

using namespace std;

template<typename T>
class Dataset
{
public:

	vector<T> GetDataset() { return this->Data; }

	void AddData(T data) { Data.emplace_back(data); }

private:
	vector<T> Data;
};
