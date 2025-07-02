# Custom Player Anthems - Auto-Update Scripts

Automated tools for downloading and installing the latest plugin builds during development.

## 📋 Prerequisites

1. **GitHub CLI** installed and authenticated
   - Download: https://cli.github.com/
   - Login: `gh auth login`

2. **PowerShell** (Windows only)
   - Built into Windows 10/11

3. **BakkesMod** installed with plugin directory accessible

## 🚀 Quick Start

### Command Line Usage

```powershell
# Basic update (download and install)
.\update-plugin.ps1

# Update with reload instructions
.\update-plugin.ps1 -AutoReload

# Verbose output for debugging
.\update-plugin.ps1 -AutoReload -Verbose
```

### In-Game Usage

1. **Settings Window**: BakkesMod Settings > Plugins > Custom Player Anthems
   - Click "🔍 Check for Updates" to check latest build
   - Click "⬇️ Download Latest" to download and install

2. **F-Key Window**: Press your bound F-key (e.g., F5)
   - Click "🔍 Check for Updates" to check latest build  
   - Click "⬇️ Download & Reload" to download, install, and get reload instructions

## 🔄 Development Workflow

### Super Fast Iteration
```
Mac: Make changes → Push to GitHub → GitHub Actions builds
PC: .\update-plugin.ps1 -AutoReload → F6 in RL → plugin_reload customplayer
```

**Time: 30 seconds instead of 5+ minutes!**

### Plugin Reload Commands (BakkesMod Console)

Press **F6** in Rocket League, then run:

```
plugin_unload customplayer
plugin_load customplayer

# OR the shortcut:
plugin_reload customplayer
```

## 📁 What the Script Does

1. **Checks GitHub**: Uses GitHub CLI to find latest successful build
2. **Downloads Artifacts**: Downloads CustomPlayerAnthems.dll from GitHub Actions
3. **Backs Up**: Creates backup of existing plugin (with timestamp)
4. **Installs**: Copies new plugin to `%APPDATA%\bakkesmod\bakkesmod\plugins\`
5. **Instructions**: Provides reload commands for hot-swapping

## 🛠️ Troubleshooting

### "GitHub CLI not found"
- Install from: https://cli.github.com/
- Make sure `gh` command is in your PATH

### "No successful builds found"
- Check that GitHub Actions is building successfully
- Make sure repository name is correct: `addisonk/CustomPlayerAnthems`

### "BakkesMod plugins directory not found"
- Make sure BakkesMod is installed
- Directory should be: `%APPDATA%\bakkesmod\bakkesmod\plugins\`

### Plugin not reloading
- Try: `plugin_unload customplayer` then `plugin_load customplayer`
- Alternative: Restart Rocket League
- Check BakkesMod console (F6) for error messages

## 📋 Script Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| `-AutoReload` | Show reload instructions | `.\update-plugin.ps1 -AutoReload` |
| `-Verbose` | Show detailed output | `.\update-plugin.ps1 -Verbose` |
| `-repo` | Different repository | `.\update-plugin.ps1 -repo "user/repo"` |

## 🎯 Perfect for Development

This system is designed for rapid development iteration:

- ✅ **No manual downloading** from GitHub web interface
- ✅ **No manual unzipping** and file copying
- ✅ **Hot-reload support** without restarting Rocket League
- ✅ **Automatic backup** of previous versions
- ✅ **Version tracking** with commit SHAs
- ✅ **Plugin UI integration** for even easier updates

**From 5+ minutes of manual work down to 30 seconds of automation!** 🚀 

# SoundBoard Plugin Scripts

This directory contains automation scripts for managing and updating the SoundBoard BakkesMod plugin.

## 🔄 Auto-Update Scripts

### Windows (PowerShell) - `update-plugin.ps1`

**Automatically downloads and installs the latest GitHub Actions build to BakkesMod:**

```powershell
# Basic update
.\update-plugin.ps1

# With auto-reload instructions  
.\update-plugin.ps1 -AutoReload

# With verbose output
.\update-plugin.ps1 -Verbose -AutoReload
```

**What it does:**
✅ Downloads latest successful build from GitHub Actions  
✅ Backs up existing plugin  
✅ Installs `SoundBoardPlugin.dll` to BakkesMod plugins directory  
✅ Provides reload instructions for Rocket League  

**Requirements:**
- GitHub CLI (`gh`) installed: https://cli.github.com/
- BakkesMod installed in standard location

---

### macOS/Linux (Bash) - `update-plugin.sh`

Cross-platform version using curl and GitHub API:

```bash
# Basic update (downloads to current directory)
./update-plugin.sh

# With verbose output
./update-plugin.sh -v
```

---

## 🔧 Development Scripts

### `version-manager.sh`
Advanced version management and build monitoring.

### `monitor-build.sh` 
Real-time GitHub Actions build monitoring and status checking.

---

## 📦 Usage Examples

### Windows Users (Recommended):
```powershell
# One-command update from PowerShell
cd path\to\rl-soundboard\scripts
.\update-plugin.ps1 -AutoReload
```

### Mac/Linux Users:
```bash
# Download latest build for transfer to Windows PC
./update-plugin.sh
# Then transfer SoundBoardPlugin.dll to Windows BakkesMod
```

---

## 🚀 Integration with GitHub Actions

The auto-update scripts work seamlessly with our CI/CD pipeline:

1. **Code changes** → Push to main branch
2. **GitHub Actions** → Builds `SoundBoardPlugin.dll` automatically  
3. **Version increment** → Auto-bumps version (e.g., v1.0.0.106 → v1.0.0.107)
4. **Artifact upload** → `SoundBoardPlugin-Release-{SHA}`
5. **Update script** → Downloads latest build and installs

---

## 🎮 BakkesMod Integration

After updating, reload the plugin in Rocket League:

1. Press **F6** to open BakkesMod console
2. Run: `plugin_unload soundboard`
3. Run: `plugin_load soundboard`
4. Or try: `plugin_reload soundboard`

---

## 🔗 Repository Information

- **Repository**: `addisonk/rl-soundboard`
- **Artifacts**: `SoundBoardPlugin-Release-*`
- **Latest Builds**: https://github.com/addisonk/rl-soundboard/actions 