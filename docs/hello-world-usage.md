# Hello World Plugin Usage Guide

## Overview

The Hello World Plugin is a simple demonstration of how to create an ImGui window that works everywhere in Rocket League. This plugin showcases the basic functionality needed to create interactive overlays that can be accessed during any game mode.

## Getting Started

### 1. Installation
- Build the plugin using Visual Studio
- The compiled DLL should be placed in your BakkesMod plugins folder
- BakkesMod will automatically load the plugin on startup

### 2. Basic Usage
Once the plugin is loaded, you can interact with it in several ways:

#### Console Commands
Open the BakkesMod console (default: F6) and use these commands:

```
helloworld_toggle    # Toggle the Hello World window on/off
helloworld_show      # Show the Hello World window
helloworld_hide     # Hide the Hello World window
```

#### Plugin Settings
1. Open BakkesMod settings (F2)
2. Navigate to "Plugins" tab
3. Find "Hello World Plugin" in the list
4. Use the settings panel to control the plugin

## Features Demonstrated

### 1. Universal Access
- The plugin works in all game modes:
  - Freeplay
  - Custom Training
  - Spectator mode
  - Replay viewing
  - Online matches (as a spectator)

### 2. Interactive Elements
- **Goal Counter**: Automatically increments when goals are scored
- **Reset Button**: Resets the counter to zero
- **Increment Button**: Manually increases the counter
- **Status Display**: Shows current plugin status and messages

### 3. Event Handling
- Listens for ball hit events
- Tracks goal scoring events
- Updates status messages in real-time

## Technical Implementation

### Plugin Types Used
```cpp
PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING | PLUGINTYPE_SPECTATOR | PLUGINTYPE_REPLAY
```

### Permission Level
```cpp
PERMISSION_ALL  // Allows commands to work in all contexts
```

### Interface Inheritance
```cpp
public BakkesMod::Plugin::PluginSettingsWindow  // Settings integration
public BakkesMod::Plugin::PluginWindow          // Standalone window
```

## Customization Ideas

This Hello World plugin serves as a foundation for more complex plugins. You could extend it by:

1. **Adding more game event hooks**
2. **Creating multiple windows**
3. **Implementing data persistence**
4. **Adding keyboard shortcuts**
5. **Integrating with external APIs**
6. **Creating custom visualizations**

## Troubleshooting

### Window Not Appearing
- Check if the plugin is loaded: look for "Hello World Plugin loaded successfully!" in the console
- Try the `helloworld_show` command
- Verify the plugin is enabled in settings

### Commands Not Working
- Make sure you're typing the exact command name
- Check that BakkesMod console is open (F6)
- Verify the plugin loaded without errors

### Performance Issues
- The plugin is designed to be lightweight
- The window only renders when visible
- Input blocking is disabled to maintain game performance

## Development Notes

This plugin demonstrates best practices for BakkesMod plugin development:

- **Safe memory management** using the wrapper system
- **Event-driven architecture** for game interaction
- **Proper ImGui integration** for user interfaces
- **Console command registration** for user control
- **Settings panel integration** for configuration

Use this as a starting point for your own BakkesMod plugins! 