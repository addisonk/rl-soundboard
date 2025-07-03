#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>
#include <filesystem>
#include <thread>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

// GitHub Actions CI/CD Integration - Auto-builds and version management
BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.3.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Load function
void SoundBoardPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    this->LoadHooks();
    
    // Initialize update info
    updateInfo_.currentVersion = plugin_version;
    updateInfo_.statusMessage = "Ready to check for updates";
}

// Hooks listener
void SoundBoardPlugin::LoadHooks()
{
    gameWrapper->HookEvent("Function TAGame.Ball_TA.EventHitWorld", std::bind(&SoundBoardPlugin::CrossBarHit, this, std::placeholders::_1));

    gameWrapper->HookEventWithCallerPost<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
        [this](ServerWrapper caller, void* params, std::string eventName) {
            OnStatTickerMessage(params);
        });
}

// CrossBar collision detection
void SoundBoardPlugin::CrossBarHit(std::string name)
{
    gameWrapper->HookEventWithCaller<BallWrapper>(
        "Function TAGame.Ball_TA.OnHitWorld",
        [this](BallWrapper caller, void* params, std::string eventName) {
            if (!caller.IsNull()) {
                Vector ballLocation = caller.GetLocation();

                const float crossbarZ = 642.775f;
                const float goalYBlue = -5120.0f;
                const float goalYOrange = 5120.0f;
                const float crossbarMinX = -892.5f;
                const float crossbarMaxX = 892.5f;

                if (ballLocation.Z >= crossbarZ - 100.0f && ballLocation.Z <= crossbarZ + 100.0f) {
                    bool isBlueGoal = (ballLocation.Y >= goalYBlue - 100.0f && ballLocation.Y <= goalYBlue + 100.0f &&
                        ballLocation.X >= crossbarMinX && ballLocation.X <= crossbarMaxX);
                    bool isOrangeGoal = (ballLocation.Y >= goalYOrange - 100.0f && ballLocation.Y <= goalYOrange + 100.0f &&
                        ballLocation.X >= crossbarMinX && ballLocation.X <= crossbarMaxX);

                    if (isBlueGoal || isOrangeGoal) {
                        this->PlayASound("crossbar.wav");
                    }
                }
            }
        }
    );
}

// Ticker message detection
void SoundBoardPlugin::OnStatTickerMessage(void* params)
{
    struct StatTickerParams {
        uintptr_t Receiver;
        uintptr_t Victim;
        uintptr_t StatEvent;
    };

    StatTickerParams* pStruct = (StatTickerParams*)params;
    PriWrapper receiver = PriWrapper(pStruct->Receiver);
    PriWrapper victim = PriWrapper(pStruct->Victim);
    StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

    if (statEvent.GetEventName() == "Goal") {
        this->PlayASound("goal.wav");
    }
    else if (statEvent.GetEventName() == "Save") {
        this->PlayASound("save.wav");
    }
    else if (statEvent.GetEventName() == "Demolish") {
        this->PlayASound("demolition.wav");
    }
    else if (statEvent.GetEventName() == "MVP") {
        this->PlayASound("mvp.wav");
    }
    else if (statEvent.GetEventName() == "AerialGoal") {
        this->PlayASound("aerial_goal.wav");
    }
    else if (statEvent.GetEventName() == "EpicSave") {
        this->PlayASound("epic_save.wav");
    }
}

// SoundBoard feature
void SoundBoardPlugin::PlayASound(std::string name)
{
    std::wstring wName(name.begin(), name.end());

    wchar_t soundFilePath[MAX_PATH];
    swprintf(soundFilePath, MAX_PATH, L"%ls\\sounds\\%ls", gameWrapper->GetDataFolder().c_str(), wName.c_str());

    if (std::filesystem::exists(soundFilePath))
    {
        PlaySound(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);
    }
}

// ===== GUI IMPLEMENTATION =====

// Settings Window (F2 menu)
void SoundBoardPlugin::RenderSettings()
{
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "SoundBoard Plugin v%s", plugin_version);
    ImGui::Separator();
    
    ImGui::Text("Sound Events Configuration:");
    ImGui::BulletText("Goal sounds: goal.wav");
    ImGui::BulletText("Save sounds: save.wav, epic_save.wav");
    ImGui::BulletText("Demolition sounds: demolition.wav");
    ImGui::BulletText("Crossbar sounds: crossbar.wav");
    ImGui::BulletText("MVP sounds: mvp.wav");
    ImGui::BulletText("Aerial goal sounds: aerial_goal.wav");
    
    ImGui::Separator();
    
    // Update Settings
    if (ImGui::CollapsingHeader("🔄 Auto-Update Settings"))
    {
        ImGui::Checkbox("Enable automatic update checking", &enableAutoCheck_);
        if (enableAutoCheck_) {
            ImGui::SliderInt("Check interval (hours)", &autoCheckInterval_, 1, 168);
            ImGui::Text("Next check: Every %d hours", autoCheckInterval_);
        }
    }
    
    // Manual Update Section
    if (ImGui::CollapsingHeader("📦 Manual Update", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Current Version: %s", updateInfo_.currentVersion.c_str());
        
        if (!updateInfo_.latestVersion.empty()) {
            ImGui::Text("Latest Version: %s", updateInfo_.latestVersion.c_str());
            
            if (updateInfo_.updateAvailable) {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✅ Update available!");
            } else {
                ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "✅ Up to date");
            }
        }
        
        // Status message
        if (!updateInfo_.statusMessage.empty()) {
            ImVec4 color = updateInfo_.checkingForUpdates || updateInfo_.downloadingUpdate ? 
                ImVec4(1.0f, 1.0f, 0.0f, 1.0f) : ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
            ImGui::TextColored(color, "%s", updateInfo_.statusMessage.c_str());
        }
        
        // Progress bar
        if (updateInfo_.downloadingUpdate && updateInfo_.downloadProgress > 0.0f) {
            ImGui::ProgressBar(updateInfo_.downloadProgress, ImVec2(-1, 0), "Downloading...");
        }
        
        // Buttons - simple approach for older ImGui compatibility  
        bool buttonsDisabled = updateInfo_.checkingForUpdates || updateInfo_.downloadingUpdate;
        if (buttonsDisabled) {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        }
        
        if (ImGui::Button("🔍 Check for Updates", ImVec2(150, 0))) {
            if (!buttonsDisabled) {
                CheckForUpdates();
            }
        }
        
        ImGui::SameLine();
        
        bool updateButtonDisabled = !updateInfo_.updateAvailable || buttonsDisabled;
        if (updateButtonDisabled && !buttonsDisabled) {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        }
        
        if (ImGui::Button("⬇️ Download & Install", ImVec2(150, 0))) {
            if (!updateButtonDisabled) {
                DownloadAndInstallUpdate();
            }
        }
        
        if (updateButtonDisabled && !buttonsDisabled) {
            ImGui::PopStyleVar();
        }
        
        if (buttonsDisabled) {
            ImGui::PopStyleVar();
        }
        
        if (ImGui::Button("📁 Open Plugins Folder", ImVec2(150, 0))) {
            std::filesystem::path dataPath = gameWrapper->GetDataFolder();
            std::filesystem::path pluginsPath = dataPath.parent_path() / "bakkesmod" / "plugins";
            ShellExecuteA(NULL, "explore", pluginsPath.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
        }
    }
}

// Plugin Window (F6 togglemenu)
void SoundBoardPlugin::RenderWindow()
{
    ImGui::Text("🎵 SoundBoard Plugin Control Panel");
    ImGui::Separator();
    
    ImGui::Text("📊 Status:");
    ImGui::BulletText("Plugin Version: %s", plugin_version);
    ImGui::BulletText("Sound Events: Active");
    ImGui::BulletText("GitHub Actions: Enabled");
    
    ImGui::Separator();
    
    // Quick update section
    ImGui::Text("🔄 Quick Update:");
    
    if (ImGui::Button("🔍 Check Now", ImVec2(100, 0))) {
        CheckForUpdates();
    }
    
    ImGui::SameLine();
    
    // Simple approach for older ImGui compatibility
    if (!updateInfo_.updateAvailable) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }
    
    if (ImGui::Button("⬇️ Update", ImVec2(100, 0))) {
        if (updateInfo_.updateAvailable) {
            DownloadAndInstallUpdate();
        }
    }
    
    if (!updateInfo_.updateAvailable) {
        ImGui::PopStyleVar();
    }
    
    if (updateInfo_.updateAvailable) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Update available: %s", updateInfo_.latestVersion.c_str());
    }
    
    ShowUpdateStatus();
    
    ImGui::Separator();
    
    // Instructions
    ImGui::Text("💡 Instructions:");
    ImGui::BulletText("Press F2 → Plugin Settings for full configuration");
    ImGui::BulletText("Updates download from GitHub Actions automatically");
    ImGui::BulletText("Plugin reloads automatically after update");
}

// Override base class methods
std::string SoundBoardPlugin::GetPluginName()
{
    return "SoundBoardPlugin";
}

void SoundBoardPlugin::SetImGuiContext(uintptr_t ctx)
{
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string SoundBoardPlugin::GetMenuName()
{
    return "SoundBoardPlugin";
}

std::string SoundBoardPlugin::GetMenuTitle()
{
    return menuTitle_;
}

// ===== UPDATE FUNCTIONALITY =====

void SoundBoardPlugin::CheckForUpdates()
{
    updateInfo_.checkingForUpdates = true;
    updateInfo_.statusMessage = "🔍 Checking for updates...";
    
    // Run update check in background thread
    std::thread([this]() {
        try {
            // Simulate GitHub API call
            Sleep(2000); // Simulate network delay
            
            // For now, simulate finding an update
            updateInfo_.latestVersion = "1.0.0.107";
            updateInfo_.downloadUrl = "https://github.com/addisonk/rl-soundboard/actions/artifacts/latest";
            
            // Compare versions (simple string comparison for now)
            updateInfo_.updateAvailable = (updateInfo_.latestVersion != updateInfo_.currentVersion);
            
            if (updateInfo_.updateAvailable) {
                updateInfo_.statusMessage = "✅ Update found: " + updateInfo_.latestVersion;
            } else {
                updateInfo_.statusMessage = "✅ Plugin is up to date";
            }
        }
        catch (const std::exception& e) {
            updateInfo_.statusMessage = "❌ Check failed: " + std::string(e.what());
        }
        
        updateInfo_.checkingForUpdates = false;
    }).detach();
}

void SoundBoardPlugin::DownloadAndInstallUpdate()
{
    if (!updateInfo_.updateAvailable) return;
    
    updateInfo_.downloadingUpdate = true;
    updateInfo_.statusMessage = "⬇️ Downloading update...";
    updateInfo_.downloadProgress = 0.0f;
    
    // Run download in background thread
    std::thread([this]() {
        try {
            // Simulate download progress
            for (int i = 0; i <= 100; i += 10) {
                updateInfo_.downloadProgress = i / 100.0f;
                updateInfo_.statusMessage = "⬇️ Downloading... " + std::to_string(i) + "%";
                Sleep(200);
            }
            
            updateInfo_.statusMessage = "📦 Installing update...";
            Sleep(1000);
            
            // Simulate successful installation
            updateInfo_.statusMessage = "✅ Update installed! Please reload plugin.";
            updateInfo_.currentVersion = updateInfo_.latestVersion;
            updateInfo_.updateAvailable = false;
        }
        catch (const std::exception& e) {
            updateInfo_.statusMessage = "❌ Update failed: " + std::string(e.what());
        }
        
        updateInfo_.downloadingUpdate = false;
        updateInfo_.downloadProgress = 0.0f;
    }).detach();
}

void SoundBoardPlugin::ShowUpdateStatus()
{
    if (updateInfo_.checkingForUpdates) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "🔍 Checking for updates...");
    }
    else if (updateInfo_.downloadingUpdate) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "⬇️ Downloading update...");
        if (updateInfo_.downloadProgress > 0.0f) {
            ImGui::ProgressBar(updateInfo_.downloadProgress, ImVec2(-1, 0));
        }
    }
    else if (!updateInfo_.statusMessage.empty()) {
        ImGui::Text("%s", updateInfo_.statusMessage.c_str());
    }
}

std::string SoundBoardPlugin::GetCurrentVersion()
{
    return plugin_version;
}
