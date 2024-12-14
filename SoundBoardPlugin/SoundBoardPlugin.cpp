#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>
#include <filesystem>

BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.3.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Load function
void SoundBoardPlugin::onLoad()
{
    _globalCvarManager = cvarManager;
    this->LoadHooks();
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
