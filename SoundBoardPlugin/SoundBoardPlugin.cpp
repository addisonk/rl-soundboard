#include "pch.h"
#include "SoundBoardPlugin.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <string>
#include <Lmcons.h>


BAKKESMOD_PLUGIN(SoundBoardPlugin, "A soundboard plugin who plays custom sounds when game events", "1.2.0", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Load function
void SoundBoardPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("Plugin loaded oui!");
	this->LoadHooks();
}

// Hooks listenner
void SoundBoardPlugin::LoadHooks()
{
	gameWrapper->HookEvent("Function TAGame.Ball_TA.EventHitWorld", std::bind(&SoundBoardPlugin::CrossBarHit, this, std::placeholders::_1));
    gameWrapper->HookEvent("Function TAGame.PlayerController_TA.NotifyGoalScored", std::bind(&SoundBoardPlugin::GoalScored, this, std::placeholders::_1));
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
                    

                    std::stringstream converter;
                    converter << std::boolalpha << isBlueGoal;
                    std::stringstream converterorange;
                    converterorange << std::boolalpha << isOrangeGoal;
                    LOG(converter.str());
                    LOG(converterorange.str());

                    if (isBlueGoal || isOrangeGoal) {
                        this->PlayASound("crossbar.wav");
                    }
                }
            }
        }
    );
}

// GoalScored detection
void SoundBoardPlugin::GoalScored(std::string name)
{
    LOG("Goal Scored!");
    this->PlayASound("goal.wav");
}

// SoundBoard feature
void SoundBoardPlugin::PlayASound(std::string name)
{
    wchar_t username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    if (GetUserNameW(username, &username_len)) {

        // Convert the username to a string
        std::wstring username_wstr(username);
        std::string username_str(username_wstr.begin(), username_wstr.end());


        wchar_t soundFilePath[MAX_PATH];
        std::wstring wName(name.begin(), name.end());
        swprintf(soundFilePath, MAX_PATH, L"C:/Users/%s/AppData/Roaming/bakkesmod/bakkesmod/plugins/sounds/%s", username, wName.c_str());

        PlaySound(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);
    }
}