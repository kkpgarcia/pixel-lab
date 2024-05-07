#pragma once
#include "PixelEngine.h"
#include "EditorLayer.h"
//#include "Assets/ProjectDatabase.h"

class EditorApplication : public Application
{
public:
    EditorApplication() : Application("PixelLab", 1024, 720)
    {
        //ProjectDatabase::GetInstance()->InitializeDatabase();

        AddLayer(new EditorLayer());
    }

    ~EditorApplication()
    {
        //ProjectDatabase::GetInstance()->SerializeDatabase();
    }
};
