#pragma once
#include<TestB.h>
#include <iostream>

class TestA
{

private:
	TestB* other;

public:
	TestA(TestB* _other) : other(_other) { other->OnSomething += std::bind(&TestA::Handle_ThangSomething, this, std::placeholders::_1); }

	void Handle_ThangSomething(int in)
	{
		std::cout << in << std::endl;
		other->OnSomething -= std::bind(&TestA::Handle_ThangSomething, this, std::placeholders::_1);
	}

	void StaticFunction()
	{
		std::cout << "6" << std::endl;
	}


};
