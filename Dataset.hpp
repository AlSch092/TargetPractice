#pragma once
#include <Windows.h>
#include <list>
using namespace std;

template<typename T>
class Dataset
{
public:

	list<T> GetDataset() { return this->Data; }

	void AddData(T data) { Data.push_back(data); }

private:
	list<T> Data;
};
