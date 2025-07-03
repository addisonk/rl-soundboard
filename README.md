# RocketLeague_SoundBoard_BakkesModPlugin 🎧🔊

RocketLeague_SoundBoard_BakkesModPlugin is a soundboard plugin that plays custom sounds during game events in Rocket League. 🎮

## Requirements 🛠️

- Visual Studio 💻
- C++ Compiler 🔧

## Installation ⚙️

1. **Clone or download the repository**. 🔅
2. **Open the project in Visual Studio**. 🗂
3. **Build the project** to generate the plugin files. 🏗️
4. **Move the compiled plugin** to the BakkesMod plugin folder:
   - Copy the `sounds` folder to your BakkesMod directory: `bakkesmod\\bakkesmod\\data\\sound`. 🗂
     - `crossbar.wav` ⚽
     - `goal.wav` 🏊
     - `aerial_goal.wav` 🏀
     - `demolition.wav` 💥
     - `epic_save.wav` 🛡️
     - `save.wav` 🫄
     - `mvp.wav` ⭐
5. **Launch Rocket League** with BakkesMod enabled. 🚗✨

## 🎵 Configurable Sound Paths (NEW!)

You can now customize which sound files play for each game event! The plugin defaults to the original sound files but allows you to specify custom paths.

### Console Commands

#### Setting Custom Sound Paths
```
soundboard_goal_path "my_custom_goal.wav"
soundboard_save_path "another_sounds/amazing_save.wav"
soundboard_demolition_path "destruction.wav"
soundboard_mvp_path "victory_sound.wav"
soundboard_aerial_goal_path "aerial_celebration.wav"
soundboard_epic_save_path "epic_moment.wav"
soundboard_crossbar_path "crossbar_hit.wav"
```

#### Utility Commands
```
soundboard_list_paths          # Show all current sound file paths
soundboard_reset_all_paths     # Reset all paths to defaults
```

### Usage Examples

**Using sounds from the main sounds folder:**
```
soundboard_goal_path "goal.wav"              # Default
soundboard_goal_path "fortnite_win.wav"      # Custom sound
```

**Using sounds from subfolders:**
```
soundboard_save_path "another_sounds/perfect_fart.wav"
soundboard_mvp_path "another_sounds/final_fantasy_win.wav"
soundboard_demolition_path "another_sounds/minecraft_anvil_loud.wav"
```

**Reset to defaults:**
```
soundboard_goal_path "goal.wav"
# Or reset everything at once:
soundboard_reset_all_paths
```

### How It Works
- All paths are relative to your BakkesMod `sounds` folder: `%APPDATA%/bakkesmod/bakkesmod/data/sounds/`
- Settings persist between sessions (saved automatically)
- Backward compatible - defaults to original filenames
- File validation - only plays if the file exists

### Sound Event Types
- **Goal** - When you score a goal
- **Save** - When you make a save
- **Demolition** - When you demolish someone
- **MVP** - When you get MVP
- **AerialGoal** - When you score an aerial goal
- **EpicSave** - When you make an epic save
- **Crossbar** - When the ball hits the crossbar

## Usage 🔊

1. **Load the plugin**: Open the BakkesMod console (`F6`) and type `plugin load soundboard`. 🔌
2. **Configure custom sounds** (optional): Use the console commands above to set custom sound paths
3. **Play!**: The plugin will automatically play the configured sounds when events occur in-game. 🎮

## Troubleshooting 🛠️

- **No sound playing**: Check that your sound files exist in the BakkesMod sounds folder
- **Wrong sound playing**: Use `soundboard_list_paths` to check your current settings
- **Want to reset**: Use `soundboard_reset_all_paths` to restore defaults

## Default Sound Files 🔊

The plugin comes with these default sound files in the `sounds/` folder:
- `aerial_goal.wav` - Aerial goal celebration
- `crossbar.wav` - Crossbar hit sound  
- `demolition.wav` - Demolition impact
- `epic_save.wav` - Epic save celebration
- `goal.wav` - Standard goal sound
- `mvp.wav` - MVP announcement
- `save.wav` - Standard save sound

Plus additional sounds in `sounds/another_sounds/` subfolder for variety!

## Build & Development 🔧

This project uses GitHub Actions for automated Windows builds. The plugin is built for x64 Release configuration and artifacts are automatically generated.

- **Latest build**: Check the [Actions tab](../../actions) for the most recent build
- **Version**: Auto-incremented with each successful build
- **Artifacts**: `SoundBoardPlugin.dll` and debug symbols

## Contributing 🤝

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License 📄

This project is licensed under the MIT License.
