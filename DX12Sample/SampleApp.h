#pragma once

#include "BaseApp.h"

class SampleApp : public BaseApp
{
public:
	SampleApp(HINSTANCE hInstance);
	SampleApp(const SampleApp&) = delete;
	SampleApp& operator=(const SampleApp&) = delete;
	~SampleApp();
};
