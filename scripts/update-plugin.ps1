# SoundBoard Plugin - Auto-Update Script
# Downloads latest GitHub Actions build and installs to BakkesMod
# Usage: .\update-plugin.ps1 [-AutoReload]

param(
    [string]$repo = "addisonk/rl-soundboard",
    [string]$artifactName = "SoundBoardPlugin-Release",
    [switch]$AutoReload = $false,
    [switch]$Verbose = $false
)

function Write-Status {
    param([string]$Message, [string]$Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Test-GitHubCLI {
    try {
        $null = gh --version
        return $true
    } catch {
        Write-Status "❌ GitHub CLI not found. Install from: https://cli.github.com/" "Red"
        return $false
    }
}

function Get-LatestBuild {
    Write-Status "📡 Checking for latest successful build..." "Cyan"
    
    try {
        $latestRun = gh run list --repo $repo --status success --limit 1 --json databaseId,headSha,conclusion,createdAt --jq ".[0]"
        $runData = $latestRun | ConvertFrom-Json
        
        if (!$runData) {
            Write-Status "❌ No successful builds found" "Red"
            return $null
        }
        
        return @{
            RunId = $runData.databaseId
            CommitSha = $runData.headSha.Substring(0,7)
            CreatedAt = $runData.createdAt
        }
    } catch {
        Write-Status "❌ Failed to get latest build: $($_.Exception.Message)" "Red"
        return $null
    }
}

function Download-Plugin {
    param($BuildInfo)
    
    Write-Status "📦 Downloading build $($BuildInfo.RunId) (commit: $($BuildInfo.CommitSha))..." "Green"
    
    # Create temp directory
    $tempDir = "$env:TEMP\SoundBoardPlugin-Update-$(Get-Date -Format 'yyyyMMdd-HHmmss')"
    if (Test-Path $tempDir) { Remove-Item $tempDir -Recurse -Force }
    New-Item -ItemType Directory -Path $tempDir | Out-Null
    
    try {
        # Download artifacts
        gh run download $BuildInfo.RunId --repo $repo --dir $tempDir
        
        # Find the plugin DLL
        $pluginFiles = Get-ChildItem -Path $tempDir -Name "*.dll" -Recurse | Where-Object { $_ -like "*SoundBoardPlugin*" }
        
        if (!$pluginFiles) {
            Write-Status "❌ SoundBoardPlugin.dll not found in artifacts!" "Red"
            return $null
        }
        
        # Get the first match
        $pluginFile = $pluginFiles[0]
        $fullPath = Get-ChildItem -Path $tempDir -Name $pluginFile -Recurse | Select-Object -First 1
        
        return @{
            TempDir = $tempDir
            PluginPath = (Get-ChildItem -Path $tempDir -Filter $pluginFile -Recurse | Select-Object -First 1).FullName
        }
    } catch {
        Write-Status "❌ Download failed: $($_.Exception.Message)" "Red"
        if (Test-Path $tempDir) { Remove-Item $tempDir -Recurse -Force }
        return $null
    }
}

function Install-Plugin {
    param($DownloadInfo, $BuildInfo)
    
    # Determine BakkesMod plugins directory
    $bakkesModPath = "$env:APPDATA\bakkesmod\bakkesmod\plugins"
    if (!(Test-Path $bakkesModPath)) {
        Write-Status "❌ BakkesMod plugins directory not found: $bakkesModPath" "Red"
        Write-Status "💡 Make sure BakkesMod is installed" "Yellow"
        return $false
    }
    
    $targetPath = "$bakkesModPath\SoundBoardPlugin.dll"
    
    try {
        # Backup existing plugin if it exists
        if (Test-Path $targetPath) {
            $backupPath = "$targetPath.backup-$(Get-Date -Format 'yyyyMMdd-HHmmss')"
            Copy-Item $targetPath $backupPath
            if ($Verbose) { Write-Status "📋 Backed up existing plugin to: $backupPath" "Gray" }
        }
        
        # Install new plugin
        Write-Status "📁 Installing to: $targetPath" "Cyan"
        Copy-Item $DownloadInfo.PluginPath $targetPath -Force
        
        # Cleanup temp files
        Remove-Item $DownloadInfo.TempDir -Recurse -Force
        
        Write-Status "✅ SoundBoard Plugin updated successfully!" "Green"
        Write-Status "📋 Build: $($BuildInfo.CommitSha) | Created: $($BuildInfo.CreatedAt)" "Gray"
        
        return $true
    } catch {
        Write-Status "❌ Installation failed: $($_.Exception.Message)" "Red"
        return $false
    }
}

function Trigger-PluginReload {
    Write-Status "🔄 Setting up plugin reload..." "Yellow"
    
    # Create reload instructions
    $reloadCommands = @(
        "cl_settings_reminders 0",
        "plugin_unload soundboard",
        "plugin_load soundboard"
    )
    
    Write-Status "" 
    Write-Status "🎮 TO RELOAD PLUGIN IN ROCKET LEAGUE:" "Yellow"
    Write-Status "   1. Press F6 to open BakkesMod console" "White"
    Write-Status "   2. Run these commands:" "White"
    foreach ($cmd in $reloadCommands) {
        Write-Status "      $cmd" "Cyan"
    }
    Write-Status "   OR try: plugin_reload soundboard" "Cyan"
    Write-Status ""
    
    # Try to write reload script for potential automation
    $reloadScript = "$env:TEMP\reload-soundboard-plugin.txt"
    $reloadCommands | Out-File $reloadScript -Encoding UTF8
    
    if ($Verbose) {
        Write-Status "📝 Reload commands saved to: $reloadScript" "Gray"
    }
}

# Main execution
Write-Status "🔄 SoundBoard Plugin Auto-Updater" "Cyan"
Write-Status "Repository: $repo" "Gray"

# Check prerequisites
if (!(Test-GitHubCLI)) { exit 1 }

# Get latest build
$buildInfo = Get-LatestBuild
if (!$buildInfo) { exit 1 }

# Download plugin
$downloadInfo = Download-Plugin $buildInfo
if (!$downloadInfo) { exit 1 }

# Install plugin
$success = Install-Plugin $downloadInfo $buildInfo
if (!$success) { exit 1 }

# Handle reload
if ($AutoReload) {
    Trigger-PluginReload
} else {
    Write-Status "💡 To reload plugin:" "Yellow"
    Write-Status "   Run with -AutoReload flag, or manually reload in BakkesMod console" "White"
}

Write-Status "🚀 Update complete!" "Green" 