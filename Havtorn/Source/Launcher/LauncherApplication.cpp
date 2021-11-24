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