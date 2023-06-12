#include <vector>

using namespace std;


template<typename T, typename _Pr = greater <T>>
class PriorityQueue
{
protected:
	_Pr mycompare;
	vector<T> contain;
private:
	void VecPush(T _data)
	{
		contain.push_back(_data);

		for (int i = 1; i < contain.size(); i++)
		{
			int child = i;
			do
			{
				int root = (child - 1) / 2;
				if (mycompare(contain[root], contain[child]))
				{
					T temp = contain[root];
					contain[root] = contain[child];
					contain[child] = temp;
				}
				child = root;
			} while (child != 0);
		}
	}
	void heapify(int i)
	{
		int parent = i;
		int child = i * 2+1;
		int size = contain.size() - 1;
		if (child > size)
			return;
		if (child + 1 <= size && this->mycompare(contain[child], contain[child + 1])) child++;

		if (this->mycompare(contain[parent], contain[child]))
		{
			T temp = contain[parent];
			contain[parent] = contain[child];
			contain[child] = temp;

			if (child <= size / 2)
			{
				heapify(child);
			}
		}
	}
	void VecSort()
	{
		int size = contain.size();
		if (size == 1)
		{
			return;
		}
		for (int i = size / 2-1; i >= 0; i--)
		{
			heapify(i);
		}
	}
public:
	PriorityQueue(int capacity = 40)
	{
		contain.reserve(capacity);
	}
	void Push(T _data)
	{
		VecPush(_data);
		//VecSort();
	}
	void Pop()
	{
		int size = contain.size() - 1;
		T data;

		data = contain[size];

		for (auto itr = contain.begin(); itr != contain.end(); itr++)
		{
			if (data == *itr)
			{
				contain.erase(itr);
				break;
			}
		}
	}
	// 나중에 참조로 변경해야 함.
	T Front()
	{
		T data;
		T temp = contain[0];
		int size = contain.size() - 1;
		if (this->mycompare(contain[size], contain[0]) == false) // 우선순위 조건이 동일한 경우 들어온 순서대로 나간다.
		{
			contain[0] = contain[size];
			contain[size] = temp;
			return temp;
		}
		contain[0] = contain[size];
		contain[size] = temp;
		int root = 0;
		int child = 1;
		do
		{
			child = root * 2 + 1;
			if (child < size - 1 && this->mycompare(contain[child], contain[child + 1]))
			{
				child++;
			}
			if (child < size && this->mycompare(contain[root], contain[child]))
			{
				temp = contain[root];
				contain[root] = contain[child];
				contain[child] = temp;
			}
			root = child;
		} while (child < size);

		data = contain[size];
		return data;
	}
	size_t Size()
	{
		return contain.size();
	}
	bool Empty()
	{
		return contain.size() == 0;
	}
	void Sort()
	{
		VecSort();
	}
	const T& operator[](int _index)
	{
		return contain[_index];
	}
};





