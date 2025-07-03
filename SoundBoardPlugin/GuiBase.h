#pragma once
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/plugin/pluginwindow.h"

class SettingsWindowBase : public BakkesMod::Plugin::PluginSettingsWindow
{
public:
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	
	// Pure virtual method for rendering settings content
	virtual void RenderSettings() = 0;
	
	// Override the main render method to call our custom RenderSettings
	void Render() override {
		RenderSettings();
	}
};

class PluginWindowBase : public BakkesMod::Plugin::PluginWindow
{
public:
	virtual ~PluginWindowBase() = default;

	bool isWindowOpen_ = false;
	std::string menuTitle_ = "SoundBoardPlugin";

	std::string GetMenuName() override;
	std::string GetMenuTitle() override;
	void SetImGuiContext(uintptr_t ctx) override;
	bool ShouldBlockInput() override;
	bool IsActiveOverlay() override;
	void OnOpen() override;
	void OnClose() override;
	void Render() override;

	virtual void RenderWindow() = 0;
};