//
// Created by kkpga on 5/6/2024.
//

#ifndef PIXEL_LAB_EDITORLAYER_H
#define PIXEL_LAB_EDITORLAYER_H

#include "PixelEngine.h"

class EditorLayer : public Layer
{
public:
    EditorLayer();
    ~EditorLayer() {
//        delete _camera;
//        delete _geometryPass;
//        for (auto& ui : _uiElements)
//            delete ui;
    }

    void OnEnable() override;
    void OnDisable() override;
    void OnGUI() override;
    void OnUpdate() override;

    void OnEvent(Event& e) override;

private:
//    std::vector<UI*> _uiElements;
//    EditorCamera* _camera;
//
//    GeometryPass* _geometryPass;

    void OnViewportSizeChanged(const ImVec2& size);
    void OnWindowClose(WindowCloseEvent& e);
//    UI* GetUIElement(const std::string& name);

//    ProjectWindow* _projectWindow;
};


#endif //PIXEL_LAB_EDITORLAYER_H
