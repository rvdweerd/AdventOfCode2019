#pragma once
#include <string>
#include <queue>
#include <iostream>

struct Person
{
	std::string name;
	int age;
};
bool operator>(const Person& lhs, const Person& rhs)
{
	return lhs.age > rhs.age;
}
bool operator<(const Person& lhs, const Person& rhs)
{
	return lhs.age < rhs.age;
}
struct GreaterThanByAge
{
	bool operator()(const Person& lhs, const Person& rhs) const
	{
		return lhs.age > rhs.age;
	}
};
void TestPQ()
{
	Person R = Person({ "rogier", 45 });
	Person J = Person({ "julia", 8 });
	Person S = Person({ "sanne", 41 });
	Person M = Person({ "merijn", 4 });


	//std::priority_queue<Person, std::vector<Person>, std::less<Person>> pq0;
	std::priority_queue<Person, std::vector<Person>, GreaterThanByAge> pq0;
	pq0.push(R);
	pq0.push(S);
	pq0.push(J);
	pq0.push(M);
	while (pq0.size() > 0)
	{
		std::cout << pq0.top().name << "-" << pq0.top().age << ",";
		pq0.pop();
	}

	std::priority_queue<int, std::vector<int>, std::greater<int> > pq;
	pq.push(3);
	pq.push(4);
	pq.push(5);
	pq.push(2);
	while (pq.size() > 0)
	{
		std::cout << pq.top() << ",";
		pq.pop();
	}
}