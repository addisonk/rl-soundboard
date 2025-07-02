#!/bin/bash
# 🔢 Hello World Plugin - Version Manager
# Implements automatic version incrementing for successful builds

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
VERSION_FILE="MyBakkesModPlugin/version.h"
REPO_OWNER="addisonk"
REPO_NAME="CustomPlayerAnthems"
REPO_FULL="$REPO_OWNER/$REPO_NAME"

echo -e "${BLUE}🔢 Hello World Plugin Version Manager${NC}"
echo -e "${BLUE}======================================${NC}"

# Function: Check if version.h exists
check_version_file() {
    if [ ! -f "$VERSION_FILE" ]; then
        echo -e "${RED}❌ Version file not found: $VERSION_FILE${NC}"
        echo "Creating version.h file..."
        create_version_file
        return 1
    fi
    echo -e "${GREEN}✅ Version file found: $VERSION_FILE${NC}"
}

# Function: Create version.h file if it doesn't exist
create_version_file() {
    mkdir -p "$(dirname "$VERSION_FILE")"
    cat > "$VERSION_FILE" << 'EOF'
#pragma once

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 0
#define VERSION_BUILD 1

#define stringify(a) stringify_(a)
#define stringify_(a) #a
EOF
    echo -e "${GREEN}✅ Created version.h with initial version 0.0.0.1${NC}"
}

# Function: Get current version numbers
get_current_version() {
    if [ ! -f "$VERSION_FILE" ]; then
        echo "0.0.0.0"
        return
    fi
    
    MAJOR=$(grep "VERSION_MAJOR" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    MINOR=$(grep "VERSION_MINOR" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    PATCH=$(grep "VERSION_PATCH" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    BUILD=$(grep "VERSION_BUILD" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    
    echo "$MAJOR.$MINOR.$PATCH.$BUILD"
}

# Function: Display current version
show_current_version() {
    check_version_file
    CURRENT_VERSION=$(get_current_version)
    echo -e "\n${YELLOW}📋 Current Version Information${NC}"
    echo "================================================="
    echo "Version File: $VERSION_FILE"
    echo "Current Version: $CURRENT_VERSION"
    
    if [ -f "$VERSION_FILE" ]; then
        echo -e "\n📄 Version.h Contents:"
        grep "VERSION_" "$VERSION_FILE"
    fi
}

# Function: Increment build version
increment_build() {
    check_version_file
    
    CURRENT_BUILD=$(grep "VERSION_BUILD" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    NEW_BUILD=$((CURRENT_BUILD + 1))
    
    echo -e "\n${YELLOW}📈 Incrementing Build Version${NC}"
    echo "================================================="
    echo "Current BUILD: $CURRENT_BUILD"
    echo "New BUILD: $NEW_BUILD"
    
    # Create backup
    cp "$VERSION_FILE" "${VERSION_FILE}.bak"
    
    # Update VERSION_BUILD
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD $NEW_BUILD/" "$VERSION_FILE"
    else
        # Linux
        sed -i "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD $NEW_BUILD/" "$VERSION_FILE"
    fi
    
    echo -e "${GREEN}✅ Build version incremented to $NEW_BUILD${NC}"
    
    # Show updated version
    UPDATED_VERSION=$(get_current_version)
    echo "Updated Version: $UPDATED_VERSION"
    
    # Clean up backup
    rm -f "${VERSION_FILE}.bak"
}

# Function: Increment patch version
increment_patch() {
    check_version_file
    
    CURRENT_PATCH=$(grep "VERSION_PATCH" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    NEW_PATCH=$((CURRENT_PATCH + 1))
    
    echo -e "\n${YELLOW}📈 Incrementing Patch Version${NC}"
    echo "================================================="
    echo "Current PATCH: $CURRENT_PATCH"
    echo "New PATCH: $NEW_PATCH"
    
    # Create backup
    cp "$VERSION_FILE" "${VERSION_FILE}.bak"
    
    # Update VERSION_PATCH and reset VERSION_BUILD to 1
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH $NEW_PATCH/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    else
        # Linux
        sed -i "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH $NEW_PATCH/" "$VERSION_FILE"
        sed -i "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    fi
    
    echo -e "${GREEN}✅ Patch version incremented to $NEW_PATCH${NC}"
    echo -e "${GREEN}✅ Build version reset to 1${NC}"
    
    # Show updated version
    UPDATED_VERSION=$(get_current_version)
    echo "Updated Version: $UPDATED_VERSION"
    
    # Clean up backup
    rm -f "${VERSION_FILE}.bak"
}

# Function: Increment minor version
increment_minor() {
    check_version_file
    
    CURRENT_MINOR=$(grep "VERSION_MINOR" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    NEW_MINOR=$((CURRENT_MINOR + 1))
    
    echo -e "\n${YELLOW}📈 Incrementing Minor Version${NC}"
    echo "================================================="
    echo "Current MINOR: $CURRENT_MINOR"
    echo "New MINOR: $NEW_MINOR"
    
    # Create backup
    cp "$VERSION_FILE" "${VERSION_FILE}.bak"
    
    # Update VERSION_MINOR and reset VERSION_PATCH and VERSION_BUILD
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/#define VERSION_MINOR [0-9]*/#define VERSION_MINOR $NEW_MINOR/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH 0/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    else
        # Linux
        sed -i "s/#define VERSION_MINOR [0-9]*/#define VERSION_MINOR $NEW_MINOR/" "$VERSION_FILE"
        sed -i "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH 0/" "$VERSION_FILE"
        sed -i "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    fi
    
    echo -e "${GREEN}✅ Minor version incremented to $NEW_MINOR${NC}"
    echo -e "${GREEN}✅ Patch version reset to 0${NC}"
    echo -e "${GREEN}✅ Build version reset to 1${NC}"
    
    # Show updated version
    UPDATED_VERSION=$(get_current_version)
    echo "Updated Version: $UPDATED_VERSION"
    
    # Clean up backup
    rm -f "${VERSION_FILE}.bak"
}

# Function: Increment major version
increment_major() {
    check_version_file
    
    CURRENT_MAJOR=$(grep "VERSION_MAJOR" "$VERSION_FILE" | grep -o '[0-9]*' || echo "0")
    NEW_MAJOR=$((CURRENT_MAJOR + 1))
    
    echo -e "\n${YELLOW}📈 Incrementing Major Version${NC}"
    echo "================================================="
    echo "Current MAJOR: $CURRENT_MAJOR"
    echo "New MAJOR: $NEW_MAJOR"
    
    # Create backup
    cp "$VERSION_FILE" "${VERSION_FILE}.bak"
    
    # Update VERSION_MAJOR and reset all others
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        sed -i '' "s/#define VERSION_MAJOR [0-9]*/#define VERSION_MAJOR $NEW_MAJOR/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_MINOR [0-9]*/#define VERSION_MINOR 0/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH 0/" "$VERSION_FILE"
        sed -i '' "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    else
        # Linux
        sed -i "s/#define VERSION_MAJOR [0-9]*/#define VERSION_MAJOR $NEW_MAJOR/" "$VERSION_FILE"
        sed -i "s/#define VERSION_MINOR [0-9]*/#define VERSION_MINOR 0/" "$VERSION_FILE"
        sed -i "s/#define VERSION_PATCH [0-9]*/#define VERSION_PATCH 0/" "$VERSION_FILE"
        sed -i "s/#define VERSION_BUILD [0-9]*/#define VERSION_BUILD 1/" "$VERSION_FILE"
    fi
    
    echo -e "${GREEN}✅ Major version incremented to $NEW_MAJOR${NC}"
    echo -e "${GREEN}✅ Minor version reset to 0${NC}"
    echo -e "${GREEN}✅ Patch version reset to 0${NC}"
    echo -e "${GREEN}✅ Build version reset to 1${NC}"
    
    # Show updated version
    UPDATED_VERSION=$(get_current_version)
    echo "Updated Version: $UPDATED_VERSION"
    
    # Clean up backup
    rm -f "${VERSION_FILE}.bak"
}

# Function: Check if build was successful (for auto-increment)
check_build_success() {
    echo -e "\n${YELLOW}🔍 Checking Build Status${NC}"
    echo "================================================="
    
    # Check GitHub CLI authentication
    if ! gh auth status &>/dev/null; then
        echo -e "${RED}❌ GitHub CLI not authenticated${NC}"
        echo "Run: gh auth login"
        return 1
    fi
    
    # Get latest workflow run
    LATEST_RUN=$(gh api "/repos/$REPO_FULL/actions/runs?per_page=1" | jq -r '.workflow_runs[0]')
    
    if [ "$LATEST_RUN" = "null" ]; then
        echo -e "${RED}❌ No workflow runs found${NC}"
        return 1
    fi
    
    STATUS=$(echo "$LATEST_RUN" | jq -r '.status')
    CONCLUSION=$(echo "$LATEST_RUN" | jq -r '.conclusion')
    
    echo "Build Status: $STATUS"
    echo "Build Conclusion: $CONCLUSION"
    
    if [ "$STATUS" = "completed" ] && [ "$CONCLUSION" = "success" ]; then
        echo -e "${GREEN}✅ Build was successful!${NC}"
        return 0
    else
        echo -e "${RED}❌ Build was not successful${NC}"
        return 1
    fi
}

# Function: Auto-increment version after successful build
auto_increment() {
    echo -e "\n${BLUE}🚀 Auto-Increment Version After Successful Build${NC}"
    echo "================================================="
    
    if check_build_success; then
        echo -e "\n${GREEN}✅ Build successful - proceeding with version increment${NC}"
        increment_build
        
        # Create commit for version update
        UPDATED_VERSION=$(get_current_version)
        echo -e "\n${YELLOW}📝 Committing Version Update${NC}"
        echo "================================================="
        
        git add "$VERSION_FILE"
        git commit -m "Auto-increment version to $UPDATED_VERSION

- Successful build verified ✅
- Auto-incremented VERSION_BUILD
- Hello World Plugin ready for deployment
- Build artifacts available for download"
        
        echo -e "${GREEN}✅ Version update committed${NC}"
        
        # Optionally push the version update
        echo -e "\n${YELLOW}Do you want to push the version update? (y/n)${NC}"
        read -r PUSH_RESPONSE
        if [[ "$PUSH_RESPONSE" =~ ^[Yy]$ ]]; then
            git push origin main
            echo -e "${GREEN}✅ Version update pushed to GitHub${NC}"
        else
            echo -e "${YELLOW}⚠️  Version update committed locally but not pushed${NC}"
        fi
    else
        echo -e "\n${RED}❌ Build not successful - skipping version increment${NC}"
        echo "Fix build issues before incrementing version"
        return 1
    fi
}

# Function: Create version tag
create_tag() {
    CURRENT_VERSION=$(get_current_version)
    TAG_NAME="v$CURRENT_VERSION"
    
    echo -e "\n${YELLOW}🏷️  Creating Version Tag${NC}"
    echo "================================================="
    echo "Tag Name: $TAG_NAME"
    echo "Version: $CURRENT_VERSION"
    
    # Check if tag already exists
    if git tag -l | grep -q "^$TAG_NAME$"; then
        echo -e "${RED}❌ Tag $TAG_NAME already exists${NC}"
        return 1
    fi
    
    # Create tag
    git tag -a "$TAG_NAME" -m "Release $TAG_NAME

Hello World Plugin version $CURRENT_VERSION
- Compiled and tested BakkesMod plugin
- Compatible with all Rocket League game modes
- Ready for deployment"
    
    echo -e "${GREEN}✅ Tag $TAG_NAME created${NC}"
    
    # Optionally push the tag
    echo -e "\n${YELLOW}Do you want to push the tag to GitHub? (y/n)${NC}"
    read -r PUSH_TAG_RESPONSE
    if [[ "$PUSH_TAG_RESPONSE" =~ ^[Yy]$ ]]; then
        git push origin "$TAG_NAME"
        echo -e "${GREEN}✅ Tag $TAG_NAME pushed to GitHub${NC}"
    else
        echo -e "${YELLOW}⚠️  Tag created locally but not pushed${NC}"
    fi
}

# Function: Show version history
show_history() {
    echo -e "\n${YELLOW}📚 Version History${NC}"
    echo "================================================="
    
    # Show git tags
    echo -e "${BLUE}Git Tags:${NC}"
    git tag -l "v*" | sort -V | tail -10 || echo "No version tags found"
    
    echo -e "\n${BLUE}Recent Commits Affecting Version:${NC}"
    git log --oneline --grep="version" --grep="Auto-increment" -10 || echo "No version-related commits found"
}

# Main script execution
main() {
    case "${1:-show}" in
        "show"|"current"|"status")
            show_current_version
            ;;
        "increment"|"build")
            increment_build
            ;;
        "patch")
            increment_patch
            ;;
        "minor")
            increment_minor
            ;;
        "major")
            increment_major
            ;;
        "auto")
            auto_increment
            ;;
        "tag")
            create_tag
            ;;
        "history")
            show_history
            ;;
        "create")
            create_version_file
            ;;
        "help"|"--help"|"-h")
            echo "Hello World Plugin Version Manager"
            echo ""
            echo "Usage: $0 [command]"
            echo ""
            echo "Commands:"
            echo "  show, current, status - Show current version information (default)"
            echo "  increment, build      - Increment build version (0.0.0.X)"
            echo "  patch                 - Increment patch version (0.0.X.0)"
            echo "  minor                 - Increment minor version (0.X.0.0)"
            echo "  major                 - Increment major version (X.0.0.0)"
            echo "  auto                  - Auto-increment after successful build"
            echo "  tag                   - Create version tag from current version"
            echo "  history               - Show version history and tags"
            echo "  create                - Create initial version.h file"
            echo "  help                  - Show this help message"
            echo ""
            echo "Version Strategy:"
            echo "  BUILD   - Auto-incremented for each successful plugin build (testing versions)"
            echo "  PATCH   - Manual increment for bugfixes requiring new plugin release"
            echo "  MINOR   - Manual increment for new features requiring new plugin release"
            echo "  MAJOR   - Manual increment for breaking changes requiring new plugin release"
            echo ""
            echo "Examples:"
            echo "  $0                    # Show current version"
            echo "  $0 increment          # Increment build version"
            echo "  $0 auto               # Auto-increment if build was successful"
            echo "  $0 patch              # Increment patch version for bugfix release"
            echo "  $0 tag                # Create and optionally push version tag"
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