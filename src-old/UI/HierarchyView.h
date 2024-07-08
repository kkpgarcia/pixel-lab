#pragma once

#include <filesystem>
#include "UI.h"

class HierarchyView : public UI
{
public:
	HierarchyView() : UI("Hierarchy View") {
        ConstructIcons("assets/icons/hierarchy");
    }
	~HierarchyView() {}

	void OnGUI() override;
private:
    std::map<std::string, Texture*> _iconTextures;
    void ConstructIcons(std::string directory);

    std::string ResolveIcons(Entity& entity);
};