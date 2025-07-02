#!/bin/bash
# 🚀 Hello World Plugin - Comprehensive Build Monitor
# Implements the GitHub Workflow Integration monitoring protocols

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Repository configuration
REPO_OWNER="addisonk"
REPO_NAME="CustomPlayerAnthems"
REPO_FULL="$REPO_OWNER/$REPO_NAME"

echo -e "${BLUE}🚀 Hello World Plugin Build Monitor${NC}"
echo -e "${BLUE}======================================${NC}"
echo "Repository: $REPO_FULL"
echo "Timestamp: $(date)"
echo ""

# Function: Check GitHub CLI authentication
check_auth() {
    if ! gh auth status &>/dev/null; then
        echo -e "${RED}❌ GitHub CLI not authenticated${NC}"
        echo "Run: gh auth login"
        exit 1
    fi
    echo -e "${GREEN}✅ GitHub CLI authenticated${NC}"
}

# Function: Level 1 - Basic Status Check
level1_basic_status() {
    echo -e "\n${YELLOW}🔍 LEVEL 1: Basic Status Check${NC}"
    echo "================================================="
    
    # Get latest workflow run
    LATEST_RUN=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0]')
    
    if [ "$LATEST_RUN" = "null" ]; then
        echo -e "${RED}❌ No workflow runs found${NC}"
        return 1
    fi
    
    STATUS=$(echo "$LATEST_RUN" | jq -r '.status')
    CONCLUSION=$(echo "$LATEST_RUN" | jq -r '.conclusion')
    COMMIT_MESSAGE=$(echo "$LATEST_RUN" | jq -r '.head_commit.message')
    RUN_ID=$(echo "$LATEST_RUN" | jq -r '.id')
    HTML_URL=$(echo "$LATEST_RUN" | jq -r '.html_url')
    
    echo "Run ID: $RUN_ID"
    echo "Status: $STATUS"
    echo "Conclusion: $CONCLUSION"
    echo "Commit: $COMMIT_MESSAGE"
    echo "URL: $HTML_URL"
    
    # Status evaluation
    if [ "$STATUS" = "completed" ] && [ "$CONCLUSION" = "success" ]; then
        echo -e "${GREEN}🎉 BUILD SUCCESS!${NC}"
        return 0
    elif [ "$STATUS" = "completed" ] && [ "$CONCLUSION" = "failure" ]; then
        echo -e "${RED}❌ BUILD FAILED${NC}"
        return 2
    elif [ "$STATUS" = "in_progress" ]; then
        echo -e "${YELLOW}🔄 BUILD IN PROGRESS${NC}"
        return 3
    else
        echo -e "${YELLOW}⚠️  UNKNOWN STATUS: $STATUS/$CONCLUSION${NC}"
        return 4
    fi
}

# Function: Level 2 - Detailed Job Analysis
level2_job_analysis() {
    echo -e "\n${YELLOW}🔍 LEVEL 2: Detailed Job Analysis${NC}"
    echo "================================================="
    
    # Get latest run ID
    RUN_ID=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0].id')
    
    # Get job details
    JOBS=$(gh api "/repos/$REPO_FULL/actions/runs/$RUN_ID/jobs")
    
    echo "Jobs for run $RUN_ID:"
    echo "$JOBS" | jq -r '.jobs[] | "Job: \(.name) - Status: \(.conclusion // "running") - URL: \(.html_url)"'
    
    # Check for failed jobs
    FAILED_JOBS=$(echo "$JOBS" | jq '.jobs[] | select(.conclusion == "failure")')
    
    if [ -n "$FAILED_JOBS" ] && [ "$FAILED_JOBS" != "null" ]; then
        echo -e "\n${RED}❌ FAILED JOBS DETECTED${NC}"
        echo "$FAILED_JOBS" | jq -r '{name: .name, conclusion: .conclusion, html_url: .html_url, failed_steps: [.steps[] | select(.conclusion != "success") | {name: .name, conclusion: .conclusion}]}'
        return 1
    else
        echo -e "${GREEN}✅ All jobs completed successfully or running${NC}"
        return 0
    fi
}

# Function: Level 3 - Artifact and Error Investigation
level3_artifact_investigation() {
    echo -e "\n${YELLOW}🔍 LEVEL 3: Artifact and Error Investigation${NC}"
    echo "================================================="
    
    # Get latest run ID
    RUN_ID=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0].id')
    
    # Check for artifacts
    echo "Checking for build artifacts..."
    ARTIFACTS=$(gh api "/repos/$REPO_FULL/actions/runs/$RUN_ID/artifacts")
    
    if [ "$(echo "$ARTIFACTS" | jq '.total_count')" -gt 0 ]; then
        echo -e "${GREEN}📦 Artifacts found:${NC}"
        echo "$ARTIFACTS" | jq -r '.artifacts[] | "- \(.name) (\(.size_in_bytes) bytes) - Expires: \(.expires_at)"'
        
        # Show download commands
        echo -e "\n${BLUE}💾 Download commands:${NC}"
        echo "$ARTIFACTS" | jq -r --arg run_id "$RUN_ID" '.artifacts[] | "gh run download " + $run_id + " --name " + .name'
    else
        echo -e "${YELLOW}⚠️  No artifacts found${NC}"
    fi
}

# Function: Build Error Log Extraction (BREAKTHROUGH METHOD)
extract_build_logs() {
    echo -e "\n${RED}🚨 EXTRACTING BUILD ERROR LOGS${NC}"
    echo "================================================="
    
    # Get latest run ID
    RUN_ID=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0].id')
    
    echo "Downloading logs for run $RUN_ID..."
    
    # Clean up any existing files
    rm -f build_logs.zip *.txt 2>/dev/null || true
    
    # Download build logs
    if gh api "/repos/$REPO_FULL/actions/runs/$RUN_ID/logs" > build_logs.zip; then
        echo "✅ Logs downloaded successfully"
        
        # Extract logs using non-interactive method
        echo "Extracting logs..."
        if unzip -o -q build_logs.zip 2>/dev/null; then
            echo "✅ Logs extracted successfully"
            
            # List extracted files
            echo -e "\n📄 Extracted log files:"
            ls -la *.txt 2>/dev/null || echo "No .txt files found"
            
            # Search for build errors
            echo -e "\n${RED}🔍 SEARCHING FOR BUILD ERRORS:${NC}"
            echo "================================================="
            
            # Search in all log files for errors
            for log_file in *.txt; do
                if [ -f "$log_file" ]; then
                    echo -e "\n📋 Checking $log_file for errors:"
                    grep -A 5 -B 5 -i "error" "$log_file" | head -20 || echo "No errors found in $log_file"
                fi
            done
            
            # Search for specific C++ compilation errors
            echo -e "\n${RED}🔍 SEARCHING FOR C++ COMPILATION ERRORS:${NC}"
            echo "================================================="
            grep -r -A 3 -B 3 -E "(error C[0-9]+|undefined reference|cannot convert)" *.txt 2>/dev/null | head -20 || echo "No C++ compilation errors found"
            
        else
            echo -e "${RED}❌ Failed to extract logs${NC}"
            return 1
        fi
    else
        echo -e "${RED}❌ Failed to download logs${NC}"
        return 1
    fi
}

# Function: Complete Build Investigation
investigate_build_failure() {
    echo -e "\n${RED}🚨 COMPREHENSIVE BUILD FAILURE INVESTIGATION${NC}"
    echo "================================================="
    
    level2_job_analysis
    level3_artifact_investigation
    extract_build_logs
    
    echo -e "\n${BLUE}🔗 INVESTIGATION COMMANDS FOR DEVELOPERS:${NC}"
    echo "================================================="
    RUN_ID=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0].id')
    echo "# View detailed build logs:"
    echo "gh run view $RUN_ID --log"
    echo ""
    echo "# Get specific job logs:"
    echo "gh run view $RUN_ID --log --job build"
    echo ""
    echo "# Download artifacts:"
    echo "gh run download $RUN_ID"
    echo ""
    echo "# Check latest commit details:"
    LATEST_SHA=$(gh api "/repos/$REPO_FULL/commits?per_page=1" | jq -r '.[0].sha')
    echo "gh api '/repos/$REPO_FULL/commits/$LATEST_SHA'"
}

# Function: Monitor build progress
monitor_progress() {
    echo -e "\n${YELLOW}🔄 MONITORING BUILD PROGRESS${NC}"
    echo "================================================="
    
    while true; do
        STATUS_RESULT=$(level1_basic_status)
        STATUS_CODE=$?
        
        case $STATUS_CODE in
            0)
                echo -e "${GREEN}🎉 Build completed successfully!${NC}"
                level3_artifact_investigation
                break
                ;;
            2)
                echo -e "${RED}❌ Build failed. Starting investigation...${NC}"
                investigate_build_failure
                break
                ;;
            3)
                echo -e "${YELLOW}🔄 Build still in progress. Waiting 30 seconds...${NC}"
                sleep 30
                ;;
            *)
                echo -e "${YELLOW}⚠️  Unknown status. Waiting 30 seconds...${NC}"
                sleep 30
                ;;
        esac
    done
}

# Function: Quick status check
quick_status() {
    check_auth
    level1_basic_status
    STATUS_CODE=$?
    
    if [ $STATUS_CODE -eq 2 ]; then
        echo -e "\n${RED}Build failed. Run with --investigate for detailed analysis.${NC}"
    elif [ $STATUS_CODE -eq 0 ]; then
        echo -e "\n${GREEN}Build successful! Run with --artifacts to see available downloads.${NC}"
    fi
}

# Function: Show available artifacts
show_artifacts() {
    check_auth
    level3_artifact_investigation
}

# Function: Push and monitor workflow
push_and_monitor() {
    echo -e "${BLUE}🚀 PUSH AND MONITOR WORKFLOW${NC}"
    echo "================================================="
    
    # Check if there are changes to commit
    if ! git diff --staged --quiet; then
        echo "📝 Committing staged changes..."
        git commit -m "Auto-commit: $(date)"
    elif ! git diff --quiet; then
        echo "📝 Staging and committing all changes..."
        git add .
        git commit -m "Auto-commit: $(date)"
    else
        echo "ℹ️  No changes to commit"
    fi
    
    # Push changes
    echo "📤 Pushing to GitHub..."
    git push origin main
    
    # Wait a moment for GitHub Actions to start
    echo "⏱️  Waiting 60 seconds for GitHub Actions to start..."
    sleep 60
    
    # Start monitoring
    monitor_progress
}

# Main script execution
main() {
    case "${1:-quick}" in
        "quick"|"status")
            quick_status
            ;;
        "monitor"|"watch")
            check_auth
            monitor_progress
            ;;
        "investigate"|"logs")
            check_auth
            investigate_build_failure
            ;;
        "artifacts"|"download")
            show_artifacts
            ;;
        "push")
            check_auth
            push_and_monitor
            ;;
        "help"|"--help"|"-h")
            echo "Hello World Plugin Build Monitor"
            echo ""
            echo "Usage: $0 [command]"
            echo ""
            echo "Commands:"
            echo "  quick, status     - Quick build status check (default)"
            echo "  monitor, watch    - Monitor build progress in real-time"
            echo "  investigate, logs - Detailed failure investigation with log extraction"
            echo "  artifacts, download - Show available build artifacts"
            echo "  push              - Commit, push, and monitor build"
            echo "  help              - Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                # Quick status check"
            echo "  $0 monitor        # Monitor current build"
            echo "  $0 investigate    # Investigate failed build"
            echo "  $0 push           # Push changes and monitor"
            ;;
        *)
            echo "Unknown command: $1"
            echo "Run '$0 help' for usage information"
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@" 