#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SoundBoardPlugin : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase, public PluginWindowBase
{
	// Core plugin functionality
	void onLoad() override;
	void LoadHooks();
	void CrossBarHit(std::string name);
	void PlayASound(std::string name);
	void OnStatTickerMessage(void* params);

	// Settings window (F2 menu)
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

	// Plugin window (F6 togglemenu)
	void RenderWindow() override;
	std::string GetMenuName() override;
	std::string GetMenuTitle() override;

private:
	// Update management
	struct UpdateInfo {
		std::string latestVersion = "";
		std::string currentVersion = plugin_version;
		std::string downloadUrl = "";
		bool updateAvailable = false;
		bool checkingForUpdates = false;
		bool downloadingUpdate = false;
		std::string statusMessage = "";
		float downloadProgress = 0.0f;
	};
	
	UpdateInfo updateInfo_;
	
	// Update methods
	void CheckForUpdates();
	void DownloadAndInstallUpdate();
	void ShowUpdateStatus();
	std::string GetCurrentVersion();
	
	// GUI state
	bool showUpdateWindow_ = false;
	bool enableAutoCheck_ = true;
	int autoCheckInterval_ = 24; // hours
};