#include "EditorApplicaiton.h"

Application* application;

int main()
{
	application = new EditorApplication();
	application->Run();
	delete application;

	return 0;
}