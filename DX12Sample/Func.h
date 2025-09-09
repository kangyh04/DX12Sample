#pragma once

#include <functional>

using namespace std;

template<typename RETURN, typename... ARGS>
class Func
{
public:
	Func(function<RETURN(ARGS...)> function)
	{
		func = function;
	}

	RETURN operator()(ARGS... args)
	{
		return func(args...);
	}

private:
	function<RETURN(ARGS...)> func;
};

template<typename... T>
using Action = Func<void, T...>;
