#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SoundBoardPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
	// Core plugin functionality
	void onLoad() override;
	void LoadHooks();
	void CrossBarHit(std::string name);
	void PlayASound(std::string name);
	void OnStatTickerMessage(void* params);
};