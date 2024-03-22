#pragma once
#include <Event.h>


class TestB
{

public:
	AG::Event<int> OnSomething;

	void BroadcastOnSomething(int arg1)
	{
		OnSomething(arg1);
	}
};
