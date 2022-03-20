// Copyright 2022 Team Havtorn. All Rights Reserved.

#include <hvpch.h>

#include <Havtorn.h>
#include <test.h>

class CLauncherApplication : public Havtorn::CApplication
{
public:
	CLauncherApplication() {}
	~CLauncherApplication() {}
};

Havtorn::CApplication* Havtorn::CreateApplication()
{
	Test::Do();
	return new CLauncherApplication();
}