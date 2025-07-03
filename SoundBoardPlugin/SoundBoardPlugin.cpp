#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>
#include <filesystem>

// GitHub Actions CI/CD Integration - Auto-builds and version management
BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.3.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Load function
void SoundBoardPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    this->RegisterCVars();
    this->LoadHooks();
}

// Register CVars for configurable sound paths
void SoundBoardPlugin::RegisterCVars()
{
    // Register CVars for each sound event with default values
    soundGoalPath = cvarManager->registerCvar("soundboard_goal_path", "goal.wav", "Path to goal sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundSavePath = cvarManager->registerCvar("soundboard_save_path", "save.wav", "Path to save sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundDemolitionPath = cvarManager->registerCvar("soundboard_demolition_path", "demolition.wav", "Path to demolition sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundMVPPath = cvarManager->registerCvar("soundboard_mvp_path", "mvp.wav", "Path to MVP sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundAerialGoalPath = cvarManager->registerCvar("soundboard_aerial_goal_path", "aerial_goal.wav", "Path to aerial goal sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundEpicSavePath = cvarManager->registerCvar("soundboard_epic_save_path", "epic_save.wav", "Path to epic save sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundCrossbarPath = cvarManager->registerCvar("soundboard_crossbar_path", "crossbar.wav", "Path to crossbar hit sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);

    // Register console commands for easier management
    cvarManager->registerNotifier("soundboard_reset_all_paths", [this](std::vector<std::string> args) {
        this->ResetAllSoundPaths();
        cvarManager->log("All sound paths reset to defaults");
    }, "Reset all sound paths to default values", PERMISSION_ALL);

    cvarManager->registerNotifier("soundboard_list_paths", [this](std::vector<std::string> args) {
        cvarManager->log("Current sound paths:");
        cvarManager->log("Goal: " + this->GetSoundPath("goal"));
        cvarManager->log("Save: " + this->GetSoundPath("save"));
        cvarManager->log("Demolition: " + this->GetSoundPath("demolition"));
        cvarManager->log("MVP: " + this->GetSoundPath("mvp"));
        cvarManager->log("AerialGoal: " + this->GetSoundPath("aerial_goal"));
        cvarManager->log("EpicSave: " + this->GetSoundPath("epic_save"));
        cvarManager->log("Crossbar: " + this->GetSoundPath("crossbar"));
    }, "List all current sound file paths", PERMISSION_ALL);
}

// Get the configured sound path for a specific event type
std::string SoundBoardPlugin::GetSoundPath(const std::string& eventType)
{
    if (eventType == "goal" && soundGoalPath) {
        return soundGoalPath->getStringValue();
    }
    else if (eventType == "save" && soundSavePath) {
        return soundSavePath->getStringValue();
    }
    else if (eventType == "demolition" && soundDemolitionPath) {
        return soundDemolitionPath->getStringValue();
    }
    else if (eventType == "mvp" && soundMVPPath) {
        return soundMVPPath->getStringValue();
    }
    else if (eventType == "aerial_goal" && soundAerialGoalPath) {
        return soundAerialGoalPath->getStringValue();
    }
    else if (eventType == "epic_save" && soundEpicSavePath) {
        return soundEpicSavePath->getStringValue();
    }
    else if (eventType == "crossbar" && soundCrossbarPath) {
        return soundCrossbarPath->getStringValue();
    }
    
    // Fallback to default filename if CVar not found
    return eventType + ".wav";
}

// Set a custom sound path for a specific event type
void SoundBoardPlugin::SetSoundPath(const std::string& eventType, const std::string& path)
{
    if (eventType == "goal" && soundGoalPath) {
        soundGoalPath->setValue(path);
    }
    else if (eventType == "save" && soundSavePath) {
        soundSavePath->setValue(path);
    }
    else if (eventType == "demolition" && soundDemolitionPath) {
        soundDemolitionPath->setValue(path);
    }
    else if (eventType == "mvp" && soundMVPPath) {
        soundMVPPath->setValue(path);
    }
    else if (eventType == "aerial_goal" && soundAerialGoalPath) {
        soundAerialGoalPath->setValue(path);
    }
    else if (eventType == "epic_save" && soundEpicSavePath) {
        soundEpicSavePath->setValue(path);
    }
    else if (eventType == "crossbar" && soundCrossbarPath) {
        soundCrossbarPath->setValue(path);
    }
}

// Reset a specific sound path to default
void SoundBoardPlugin::ResetSoundPath(const std::string& eventType)
{
    std::string defaultPath = eventType + ".wav";
    this->SetSoundPath(eventType, defaultPath);
}

// Reset all sound paths to defaults
void SoundBoardPlugin::ResetAllSoundPaths()
{
    if (soundGoalPath) soundGoalPath->setValue("goal.wav");
    if (soundSavePath) soundSavePath->setValue("save.wav");
    if (soundDemolitionPath) soundDemolitionPath->setValue("demolition.wav");
    if (soundMVPPath) soundMVPPath->setValue("mvp.wav");
    if (soundAerialGoalPath) soundAerialGoalPath->setValue("aerial_goal.wav");
    if (soundEpicSavePath) soundEpicSavePath->setValue("epic_save.wav");
    if (soundCrossbarPath) soundCrossbarPath->setValue("crossbar.wav");
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
                        std::string soundFile = this->GetSoundPath("crossbar");
                        this->PlayASound(soundFile);
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

    std::string eventName = statEvent.GetEventName();
    std::string soundFile;

    if (eventName == "Goal") {
        soundFile = this->GetSoundPath("goal");
    }
    else if (eventName == "Save") {
        soundFile = this->GetSoundPath("save");
    }
    else if (eventName == "Demolish") {
        soundFile = this->GetSoundPath("demolition");
    }
    else if (eventName == "MVP") {
        soundFile = this->GetSoundPath("mvp");
    }
    else if (eventName == "AerialGoal") {
        soundFile = this->GetSoundPath("aerial_goal");
    }
    else if (eventName == "EpicSave") {
        soundFile = this->GetSoundPath("epic_save");
    }

    if (!soundFile.empty()) {
        this->PlayASound(soundFile);
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
