#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SoundBoardPlugin : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase
{
private:
	// CVars for configurable sound file paths
	std::shared_ptr<CVarWrapper> soundGoalPath;
	std::shared_ptr<CVarWrapper> soundSavePath;
	std::shared_ptr<CVarWrapper> soundDemolitionPath;
	std::shared_ptr<CVarWrapper> soundMVPPath;
	std::shared_ptr<CVarWrapper> soundAerialGoalPath;
	std::shared_ptr<CVarWrapper> soundEpicSavePath;
	std::shared_ptr<CVarWrapper> soundCrossbarPath;

	// GUI input buffers
	char goalPathBuffer[256] = "";
	char savePathBuffer[256] = "";
	char demolitionPathBuffer[256] = "";
	char mvpPathBuffer[256] = "";
	char aerialGoalPathBuffer[256] = "";
	char epicSavePathBuffer[256] = "";
	char crossbarPathBuffer[256] = "";

public:
	// Core plugin functionality
	void onLoad() override;
	void LoadHooks();
	void CrossBarHit(std::string name);
	void PlayASound(std::string name);
	void OnStatTickerMessage(void* params);
	
	// Configuration functionality
	void RegisterCVars();
	std::string GetSoundPath(const std::string& eventType);
	void SetSoundPath(const std::string& eventType, const std::string& path);
	void ResetSoundPath(const std::string& eventType);
	void ResetAllSoundPaths();

	// GUI functionality
	void RenderSettings() override;
	void UpdateInputBuffers();
	void ApplyPathFromBuffer(const std::string& eventType, const char* buffer);
	void BrowseForSound(const std::string& eventType);
	void ResetSoundGUI(const std::string& eventType);
};