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
   - Copy the `sounds` folder to your BakkesMod directory: `bakkesmod\bakkesmod\data\sound`. 🗂
     - `crossbar.wav` ⚽
     - `goal.wav` 🏊
     - `aerial_goal.wav` 🏀
     - `demolition.wav` 💥
     - `epic_save.wav` 🛡️
     - `save.wav` 🫄
     - `mvp.wav` ⭐
5. **Launch Rocket League** with BakkesMod enabled. 🚗✨

## Changing Sounds 🎶

You can replace the default sounds with your own. To do so:

- **Keep the same file names** as the originals (e.g., `goal.wav`, `save.wav`).
- All sound files **must be in `.wav` format**.
- Additional sounds can be found in the `another-sounds` folder or on websites like [MyInstants](https://www.myinstants.com/en/index/fr/).

### Notes:

- There is currently **no functionality to change sound volume in the plugin**. You will need to adjust the volume using an external audio or video editor before importing the sound files.
- To disable a sound, simply **delete the corresponding `.wav` file** from the folder.

## Usage 🎶

Once the plugin is installed and the sound files are in place, the sounds will trigger automatically during specific in-game events (e.g., hitting the crossbar, scoring a goal). No further setup is required once the plugin is loaded. 🎉

## Contributing 🤝

Feel free to fork the repository, submit issues, and contribute to the project via pull requests. 🔄

## License 📜

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
