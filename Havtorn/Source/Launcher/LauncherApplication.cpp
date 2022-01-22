// Copyright 2022 Team Havtorn. All Rights Reserved.

#include <hvpch.h>

#include <Havtorn.h>

class CLauncherApplication : public Havtorn::CApplication
{
public:
	CLauncherApplication() {}
	~CLauncherApplication() {}
};

Havtorn::CApplication* Havtorn::CreateApplication()
{
	return new CLauncherApplication();
}