#include <NewEngine.h>

class CLauncherApplication : public NewEngine::CApplication
{
public:
	CLauncherApplication() {}
	~CLauncherApplication() {}
};

NewEngine::CApplication* NewEngine::CreateApplication()
{
	return new CLauncherApplication();
}