#include "src/PixelLab/EditorApplication.h"

Application* application;

int main()
{
	application = new EditorApplication();
	application->Init();
	application->Run();
	application->Shutdown();
	delete application;

	return 0;
}