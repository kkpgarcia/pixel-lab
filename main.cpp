#include "EditorApplicationSlim.h"

Application* application;

int main()
{
	application = new EditorApplicationSlim();
	application->Init();
	application->Run();
	application->Shutdown();
	delete application;

	return 0;
}