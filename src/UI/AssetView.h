#pragma once

#include "PixelEngine.h"
#include "UI.h"

class AssetView : public UI
{
public:
	AssetView() : UI("Asset View") {}
	~AssetView() {}

	void OnGUI() override;
};