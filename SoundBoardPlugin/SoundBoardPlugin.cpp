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
    // Register CVars for each sound event with default values and wrap in shared_ptr
    auto goalCvar = cvarManager->registerCvar("soundboard_goal_path", "goal.wav", "Path to goal sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundGoalPath = std::make_shared<CVarWrapper>(goalCvar);
    
    auto saveCvar = cvarManager->registerCvar("soundboard_save_path", "save.wav", "Path to save sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundSavePath = std::make_shared<CVarWrapper>(saveCvar);
    
    auto demoCvar = cvarManager->registerCvar("soundboard_demolition_path", "demolition.wav", "Path to demolition sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundDemolitionPath = std::make_shared<CVarWrapper>(demoCvar);
    
    auto mvpCvar = cvarManager->registerCvar("soundboard_mvp_path", "mvp.wav", "Path to MVP sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundMVPPath = std::make_shared<CVarWrapper>(mvpCvar);
    
    auto aerialCvar = cvarManager->registerCvar("soundboard_aerial_goal_path", "aerial_goal.wav", "Path to aerial goal sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundAerialGoalPath = std::make_shared<CVarWrapper>(aerialCvar);
    
    auto epicCvar = cvarManager->registerCvar("soundboard_epic_save_path", "epic_save.wav", "Path to epic save sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundEpicSavePath = std::make_shared<CVarWrapper>(epicCvar);
    
    auto crossbarCvar = cvarManager->registerCvar("soundboard_crossbar_path", "crossbar.wav", "Path to crossbar hit sound file (relative to sounds folder)", true, true, 0.0, true, 1.0);
    soundCrossbarPath = std::make_shared<CVarWrapper>(crossbarCvar);

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
    if (eventType == "goal" && soundGoalPath && !soundGoalPath->IsNull()) {
        return soundGoalPath->getStringValue();
    }
    else if (eventType == "save" && soundSavePath && !soundSavePath->IsNull()) {
        return soundSavePath->getStringValue();
    }
    else if (eventType == "demolition" && soundDemolitionPath && !soundDemolitionPath->IsNull()) {
        return soundDemolitionPath->getStringValue();
    }
    else if (eventType == "mvp" && soundMVPPath && !soundMVPPath->IsNull()) {
        return soundMVPPath->getStringValue();
    }
    else if (eventType == "aerial_goal" && soundAerialGoalPath && !soundAerialGoalPath->IsNull()) {
        return soundAerialGoalPath->getStringValue();
    }
    else if (eventType == "epic_save" && soundEpicSavePath && !soundEpicSavePath->IsNull()) {
        return soundEpicSavePath->getStringValue();
    }
    else if (eventType == "crossbar" && soundCrossbarPath && !soundCrossbarPath->IsNull()) {
        return soundCrossbarPath->getStringValue();
    }
    
    // Fallback to default filename if CVar not found
    return eventType + ".wav";
}

// Set a custom sound path for a specific event type
void SoundBoardPlugin::SetSoundPath(const std::string& eventType, const std::string& path)
{
    if (eventType == "goal" && soundGoalPath && !soundGoalPath->IsNull()) {
        soundGoalPath->setValue(path);
    }
    else if (eventType == "save" && soundSavePath && !soundSavePath->IsNull()) {
        soundSavePath->setValue(path);
    }
    else if (eventType == "demolition" && soundDemolitionPath && !soundDemolitionPath->IsNull()) {
        soundDemolitionPath->setValue(path);
    }
    else if (eventType == "mvp" && soundMVPPath && !soundMVPPath->IsNull()) {
        soundMVPPath->setValue(path);
    }
    else if (eventType == "aerial_goal" && soundAerialGoalPath && !soundAerialGoalPath->IsNull()) {
        soundAerialGoalPath->setValue(path);
    }
    else if (eventType == "epic_save" && soundEpicSavePath && !soundEpicSavePath->IsNull()) {
        soundEpicSavePath->setValue(path);
    }
    else if (eventType == "crossbar" && soundCrossbarPath && !soundCrossbarPath->IsNull()) {
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
    if (soundGoalPath && !soundGoalPath->IsNull()) soundGoalPath->setValue("goal.wav");
    if (soundSavePath && !soundSavePath->IsNull()) soundSavePath->setValue("save.wav");
    if (soundDemolitionPath && !soundDemolitionPath->IsNull()) soundDemolitionPath->setValue("demolition.wav");
    if (soundMVPPath && !soundMVPPath->IsNull()) soundMVPPath->setValue("mvp.wav");
    if (soundAerialGoalPath && !soundAerialGoalPath->IsNull()) soundAerialGoalPath->setValue("aerial_goal.wav");
    if (soundEpicSavePath && !soundEpicSavePath->IsNull()) soundEpicSavePath->setValue("epic_save.wav");
    if (soundCrossbarPath && !soundCrossbarPath->IsNull()) soundCrossbarPath->setValue("crossbar.wav");
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

// GUI functionality
void SoundBoardPlugin::RenderSettings()
{
    ImGui::TextUnformatted("Sound File Configuration");
    ImGui::Separator();
    ImGui::Spacing();

    // Update input buffers with current CVar values
    UpdateInputBuffers();

    float inputWidth = 400.0f;
    float buttonWidth = 80.0f;
    float spacing = 10.0f;

    // Goal Sound
    ImGui::Text("Goal Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##goal_path", goalPathBuffer, sizeof(goalPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("goal", goalPathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##goal", ImVec2(buttonWidth, 0))) {
        BrowseForSound("goal");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##goal", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("goal");
    }
    ImGui::Spacing();

    // Save Sound
    ImGui::Text("Save Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##save_path", savePathBuffer, sizeof(savePathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("save", savePathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##save", ImVec2(buttonWidth, 0))) {
        BrowseForSound("save");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##save", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("save");
    }
    ImGui::Spacing();

    // Demolition Sound
    ImGui::Text("Demolition Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##demolition_path", demolitionPathBuffer, sizeof(demolitionPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("demolition", demolitionPathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##demolition", ImVec2(buttonWidth, 0))) {
        BrowseForSound("demolition");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##demolition", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("demolition");
    }
    ImGui::Spacing();

    // MVP Sound
    ImGui::Text("MVP Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##mvp_path", mvpPathBuffer, sizeof(mvpPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("mvp", mvpPathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##mvp", ImVec2(buttonWidth, 0))) {
        BrowseForSound("mvp");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##mvp", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("mvp");
    }
    ImGui::Spacing();

    // Aerial Goal Sound
    ImGui::Text("Aerial Goal Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##aerial_goal_path", aerialGoalPathBuffer, sizeof(aerialGoalPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("aerial_goal", aerialGoalPathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##aerial_goal", ImVec2(buttonWidth, 0))) {
        BrowseForSound("aerial_goal");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##aerial_goal", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("aerial_goal");
    }
    ImGui::Spacing();

    // Epic Save Sound
    ImGui::Text("Epic Save Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##epic_save_path", epicSavePathBuffer, sizeof(epicSavePathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("epic_save", epicSavePathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##epic_save", ImVec2(buttonWidth, 0))) {
        BrowseForSound("epic_save");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##epic_save", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("epic_save");
    }
    ImGui::Spacing();

    // Crossbar Sound
    ImGui::Text("Crossbar Hit Sound:");
    ImGui::SetNextItemWidth(inputWidth);
    if (ImGui::InputText("##crossbar_path", crossbarPathBuffer, sizeof(crossbarPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        ApplyPathFromBuffer("crossbar", crossbarPathBuffer);
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Browse##crossbar", ImVec2(buttonWidth, 0))) {
        BrowseForSound("crossbar");
    }
    ImGui::SameLine(0, spacing);
    if (ImGui::Button("Reset##crossbar", ImVec2(buttonWidth, 0))) {
        ResetSoundGUI("crossbar");
    }
    ImGui::Spacing();
    ImGui::Spacing();

    // Global Reset Button
    if (ImGui::Button("Reset All Sounds", ImVec2(200, 0))) {
        ResetAllSoundPaths();
        UpdateInputBuffers(); // Refresh GUI after reset
    }

    ImGui::Spacing();
    ImGui::TextUnformatted("Note: All paths are relative to the BakkesMod sounds folder:");
    ImGui::TextUnformatted("%APPDATA%/bakkesmod/bakkesmod/data/sounds/");
    ImGui::TextUnformatted("Example: 'goal.wav' or 'another_sounds/custom_goal.wav'");
}

// Update GUI input buffers with current CVar values
void SoundBoardPlugin::UpdateInputBuffers()
{
    std::string currentPath;
    
    currentPath = GetSoundPath("goal");
    strncpy_s(goalPathBuffer, currentPath.c_str(), sizeof(goalPathBuffer) - 1);
    
    currentPath = GetSoundPath("save");
    strncpy_s(savePathBuffer, currentPath.c_str(), sizeof(savePathBuffer) - 1);
    
    currentPath = GetSoundPath("demolition");
    strncpy_s(demolitionPathBuffer, currentPath.c_str(), sizeof(demolitionPathBuffer) - 1);
    
    currentPath = GetSoundPath("mvp");
    strncpy_s(mvpPathBuffer, currentPath.c_str(), sizeof(mvpPathBuffer) - 1);
    
    currentPath = GetSoundPath("aerial_goal");
    strncpy_s(aerialGoalPathBuffer, currentPath.c_str(), sizeof(aerialGoalPathBuffer) - 1);
    
    currentPath = GetSoundPath("epic_save");
    strncpy_s(epicSavePathBuffer, currentPath.c_str(), sizeof(epicSavePathBuffer) - 1);
    
    currentPath = GetSoundPath("crossbar");
    strncpy_s(crossbarPathBuffer, currentPath.c_str(), sizeof(crossbarPathBuffer) - 1);
}

// Apply path from GUI input buffer to CVar
void SoundBoardPlugin::ApplyPathFromBuffer(const std::string& eventType, const char* buffer)
{
    if (buffer && strlen(buffer) > 0) {
        SetSoundPath(eventType, std::string(buffer));
        cvarManager->log("Updated " + eventType + " sound path to: " + std::string(buffer));
    }
}

// Browse for sound file (placeholder - opens file dialog simulation)
void SoundBoardPlugin::BrowseForSound(const std::string& eventType)
{
    // For now, show a message about how to use the browse feature
    // In a full implementation, this would open a file dialog
    cvarManager->log("Browse for " + eventType + " sound file:");
    cvarManager->log("1. Navigate to your BakkesMod sounds folder:");
    cvarManager->log("   %APPDATA%/bakkesmod/bakkesmod/data/sounds/");
    cvarManager->log("2. Copy your .wav file there");
    cvarManager->log("3. Enter the filename (or subfolder/filename) in the input field");
    cvarManager->log("4. Press Enter or click outside the field to apply");
    
    // Set focus back to the input field for this event type
    // This helps user experience by highlighting the relevant field
    if (eventType == "goal") {
        ImGui::SetKeyboardFocusHere(-1); // Focus previous item (the input field)
    }
}

// Reset specific sound path via GUI
void SoundBoardPlugin::ResetSoundGUI(const std::string& eventType)
{
    ResetSoundPath(eventType);
    UpdateInputBuffers(); // Refresh GUI to show the reset value
    cvarManager->log("Reset " + eventType + " sound to default");
}
