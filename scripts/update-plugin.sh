#!/bin/bash

# SoundBoard Plugin - Auto-Update Script (Cross-Platform)
# Downloads latest GitHub Actions build 
# Usage: ./update-plugin.sh [-v|--verbose]

set -e

# Configuration
REPO="addisonk/rl-soundboard"
ARTIFACT_PATTERN="SoundBoardPlugin-Release"
VERBOSE=false

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [-v|--verbose] [-h|--help]"
            echo ""
            echo "Downloads latest SoundBoard plugin build from GitHub Actions"
            echo ""
            echo "Options:"
            echo "  -v, --verbose    Show detailed output"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
GRAY='\033[0;37m'
NC='\033[0m' # No Color

log() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

verbose_log() {
    if [ "$VERBOSE" = true ]; then
        log $GRAY "$1"
    fi
}

check_dependencies() {
    log $CYAN "🔍 Checking dependencies..."
    
    if ! command -v curl &> /dev/null; then
        log $RED "❌ curl not found. Please install curl."
        exit 1
    fi
    
    if ! command -v jq &> /dev/null; then
        log $RED "❌ jq not found. Please install jq for JSON parsing."
        echo "Install with: brew install jq (macOS) or apt-get install jq (Linux)"
        exit 1
    fi
    
    if ! command -v unzip &> /dev/null; then
        log $RED "❌ unzip not found. Please install unzip."
        exit 1
    fi
    
    verbose_log "✅ All dependencies found"
}

get_latest_run() {
    log $CYAN "📡 Getting latest successful build..."
    
    local api_url="https://api.github.com/repos/${REPO}/actions/runs?status=success&per_page=1"
    verbose_log "API URL: $api_url"
    
    local response=$(curl -s "$api_url")
    local run_id=$(echo "$response" | jq -r '.workflow_runs[0].id')
    local commit_sha=$(echo "$response" | jq -r '.workflow_runs[0].head_sha' | cut -c1-7)
    local created_at=$(echo "$response" | jq -r '.workflow_runs[0].created_at')
    
    if [ "$run_id" = "null" ] || [ -z "$run_id" ]; then
        log $RED "❌ No successful builds found"
        exit 1
    fi
    
    verbose_log "Run ID: $run_id"
    verbose_log "Commit: $commit_sha"
    verbose_log "Created: $created_at"
    
    echo "$run_id|$commit_sha|$created_at"
}

download_artifacts() {
    local run_info=$1
    IFS='|' read -r run_id commit_sha created_at <<< "$run_info"
    
    log $GREEN "📦 Downloading build $run_id (commit: $commit_sha)..."
    
    # Create temp directory
    local temp_dir="./soundboard-update-$(date +%Y%m%d-%H%M%S)"
    mkdir -p "$temp_dir"
    verbose_log "Temp directory: $temp_dir"
    
    # Get artifacts list
    local artifacts_url="https://api.github.com/repos/${REPO}/actions/runs/${run_id}/artifacts"
    local artifacts_response=$(curl -s "$artifacts_url")
    
    # Find the correct artifact
    local artifact_download_url=$(echo "$artifacts_response" | jq -r ".artifacts[] | select(.name | contains(\"$ARTIFACT_PATTERN\")) | .archive_download_url")
    
    if [ "$artifact_download_url" = "null" ] || [ -z "$artifact_download_url" ]; then
        log $RED "❌ SoundBoard plugin artifact not found!"
        rm -rf "$temp_dir"
        exit 1
    fi
    
    verbose_log "Artifact URL: $artifact_download_url"
    
    # Download artifact (requires GitHub token for private repos, but this is public)
    local zip_file="$temp_dir/artifacts.zip"
    log $BLUE "⬇️  Downloading artifact..."
    
    if curl -L -s -o "$zip_file" "$artifact_download_url"; then
        verbose_log "✅ Downloaded to: $zip_file"
    else
        log $RED "❌ Failed to download artifact"
        rm -rf "$temp_dir"
        exit 1
    fi
    
    # Extract artifact
    log $BLUE "📂 Extracting artifact..."
    unzip -q "$zip_file" -d "$temp_dir"
    rm "$zip_file"
    
    # Find the DLL file
    local dll_file=$(find "$temp_dir" -name "SoundBoardPlugin.dll" -type f | head -n1)
    
    if [ -z "$dll_file" ]; then
        log $RED "❌ SoundBoardPlugin.dll not found in artifacts!"
        rm -rf "$temp_dir"
        exit 1
    fi
    
    # Move DLL to current directory
    cp "$dll_file" "./SoundBoardPlugin.dll"
    rm -rf "$temp_dir"
    
    log $GREEN "✅ SoundBoard Plugin downloaded successfully!"
    log $GRAY "📋 Build: $commit_sha | Created: $created_at"
    log $YELLOW "💡 Plugin saved as: ./SoundBoardPlugin.dll"
    
    # Get file size
    local file_size=$(ls -lh SoundBoardPlugin.dll | awk '{print $5}')
    log $GRAY "📦 Size: $file_size"
}

show_windows_instructions() {
    log $YELLOW ""
    log $YELLOW "🎮 TO INSTALL ON WINDOWS PC WITH BAKKESMOD:"
    log $NC "   1. Transfer SoundBoardPlugin.dll to your Windows PC"
    log $NC "   2. Copy to: %APPDATA%\\bakkesmod\\bakkesmod\\plugins\\"
    log $NC "   3. In Rocket League, press F6 and run:"
    log $CYAN "      plugin_unload soundboard"
    log $CYAN "      plugin_load soundboard"
    log $YELLOW ""
    log $YELLOW "💡 Or use the PowerShell script on Windows for automatic installation!"
}

# Main execution
log $CYAN "🔄 SoundBoard Plugin Auto-Updater (Cross-Platform)"
log $GRAY "Repository: $REPO"

check_dependencies
run_info=$(get_latest_run)
download_artifacts "$run_info"
show_windows_instructions

log $GREEN "🚀 Download complete!" 