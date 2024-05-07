#include <iostream>

#include "src/EditorApplication.h"

int main() {
    auto* app = new EditorApplication();
    app->Run();
    delete app;
    return 0;
}
