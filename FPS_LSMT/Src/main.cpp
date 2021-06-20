#include <stdlib.h>
#include <crtdbg.h>
#include <Application.h>
#include "Time.h"
#include "Debug.h"

using namespace Core::API;
using namespace Core::Debug;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();

	Log* log(Log::getInstance());
	log->configureLogFiles();

	Application* api(Application::getInstance());

	int apiDebug = api->init();
	Assertion::assertApi(apiDebug);

	api->update();

	delete api;

	return 0;
}