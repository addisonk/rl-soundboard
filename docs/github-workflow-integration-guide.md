# 🚀 GitHub Workflow Integration Guide

## Complete Cross-Platform Development System for BakkesMod Plugin

This guide explains the comprehensive GitHub workflow integration system that enables seamless Mac → GitHub Actions → Windows development for BakkesMod plugins.

---

## 🎯 **System Overview**

```
Mac Development → GitHub Actions → Windows Build → Plugin Download → BakkesMod Installation
```

### **What This System Provides:**

✅ **Complete Mac Development Freedom** - No Windows VM or dual-boot needed  
✅ **Automated Windows Builds** - GitHub Actions handles all C++ compilation  
✅ **Intelligent Caching** - 50%+ faster builds with SDK and dependency caching  
✅ **Comprehensive Error Investigation** - Real log extraction with specific error messages  
✅ **Automatic Version Management** - ⚠️ **FULLY AUTOMATED** - DO NOT MANUALLY INCREMENT  
✅ **Multi-Level Build Monitoring** - Real-time status tracking with detailed analysis  
✅ **Professional CI/CD Pipeline** - Enterprise-grade automation and error handling  

---

## 🔧 **Quick Setup (5 Minutes)**

### **1. Prerequisites**
```bash
# Install GitHub CLI (Mac)
brew install gh

# Authenticate with GitHub
gh auth login

# Verify authentication
gh auth status
```

### **2. Repository Setup**
```bash
# Clone your repository
git clone https://github.com/addisonkowalski/CustomPlayerAnthems.git
cd CustomPlayerAnthems

# Verify workflow files exist
ls -la .github/workflows/
```

### **3. First Build Test**
```bash
# Make a small change to trigger build
echo "// Test comment" >> MyBakkesModPlugin/MyBakkesModPlugin.cpp

# Commit and push
git add .
git commit -m "Test: Trigger first build"
git push origin main

# Monitor the build in real-time (no manual waiting!)
sleep 15 && RUN_ID=$(gh run list --limit 1 --json databaseId --jq '.[0].databaseId') && gh run watch $RUN_ID
```

---

## 📋 **Complete Workflow Integration Features**

### **🚀 Optimized GitHub Actions Pipeline**

#### **Smart Build Triggers**
```yaml
# Only builds when relevant files change
paths: 
  - 'MyBakkesModPlugin/**'
  - '.github/workflows/**'
  - '.github/templates/**'
```

#### **Advanced Caching System**
- **BakkesMod SDK Caching**: Saves 2-3 minutes per build
- **Build Dependencies**: Incremental compilation speedup
- **Hash-based Cache Keys**: Automatic cache invalidation

#### **Multi-Job Architecture**
1. **Build Job**: Main compilation and artifact generation
2. **Error Investigation Job**: Automatic failure analysis 
3. **Success Notification Job**: Build completion celebration

### **🔍 Multi-Level Build Monitoring**

#### **Level 1: Real-Time Monitoring (Recommended)**
```bash
# Keep connection open and watch build progress live
RUN_ID=$(gh run list --limit 1 --json databaseId --jq '.[0].databaseId')
gh run watch $RUN_ID
```
**Provides:**
- ✅ **Live progress updates** as each step completes
- ✅ **Automatic completion detection** (no manual checking)
- ✅ **Real-time status changes** (queued → in_progress → completed)
- ✅ **Final result notification** (success/failure)

#### **Level 2: Quick Status Check**
```bash
gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq '.workflow_runs[0] | {status: .status, conclusion: .conclusion, message: .head_commit.message}'
```
**Provides:**
- Build status (success/failure/in-progress) 
- Commit message and SHA
- Quick pass/fail determination

#### **Level 3: Detailed Job Analysis**
```bash
RUN_ID=$(gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq -r '.workflow_runs[0].id')
gh api "/repos/addisonkowalski/CustomPlayerAnthems/actions/runs/$RUN_ID/jobs" | jq '.jobs[] | select(.conclusion == "failure") | {name: .name, html_url: .html_url}'
```
**Provides:**
- Individual job status breakdown
- Failed step identification  
- Job-specific error analysis

#### **Level 4: Comprehensive Error Investigation**
```bash
# Download and extract GitHub Actions logs
RUN_ID=$(gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq -r '.workflow_runs[0].id')
gh api "/repos/addisonkowalski/CustomPlayerAnthems/actions/runs/$RUN_ID/logs" > build_logs.zip
unzip -o -q build_logs.zip
grep -A 5 -B 5 -i "error" 0_build.txt | head -30
```
**Provides:**
- **BREAKTHROUGH METHOD**: Real GitHub Actions log extraction
- Specific C++ compilation error messages with line numbers
- Complete error context with surrounding code

### **🔢 Automatic Version Management**

#### **⚠️ CRITICAL: VERSION MANAGEMENT IS FULLY AUTOMATED**

**✅ What GitHub Actions Does Automatically:**
- Auto-increments VERSION_BUILD after every successful build
- Creates unique artifact names with commit SHA
- Manages version history automatically
- Pushes version updates back to repository

**❌ What You Should NEVER Do Manually:**
- ~~Run `./scripts/version-manager.sh auto`~~ - **CAUSES CONFLICTS**
- ~~Manually increment VERSION_BUILD~~ - **CAUSES CONFLICTS** 
- ~~Version commits after successful builds~~ - **CAUSES CONFLICTS**

#### **Version Strategy**
- **VERSION_BUILD**: ⚠️ **AUTOMATED ONLY** - GitHub Actions handles this
- **VERSION_PATCH**: Manual increment for bugfix releases (when needed)
- **VERSION_MINOR**: Manual increment for new feature releases  
- **VERSION_MAJOR**: Manual increment for breaking changes

#### **Manual Version Commands (Use Sparingly)**
```bash
# Show current version only
grep "VERSION_BUILD" MyBakkesModPlugin/version.h

# Manual increments for RELEASES only (not regular development)
# Only use these for official releases, not routine development
./scripts/version-manager.sh patch   # 0.0.X.0 - for release bugfixes
./scripts/version-manager.sh minor   # 0.X.0.0 - for release features
./scripts/version-manager.sh major   # X.0.0.0 - for breaking changes
```

---

## 🛠️ **Development Workflows**

### **🚀 RECOMMENDED: Branch-Based Development (ELIMINATES CONFLICTS)**

#### **🎯 Why Feature Branches:**
- ✅ **ZERO version conflicts** between development and GitHub Actions
- ✅ **Clean main branch** with only stable, tested code  
- ✅ **Professional Git workflow** with review opportunities
- ✅ **Eliminates dual-commit issues** permanently

#### **1. Create Feature Branch (Mac)**
```bash
# Start each new feature/fix on a branch
git checkout main
git pull origin main  # Get latest stable state
git checkout -b feature/your-feature-name  # Create development branch

# Examples:
git checkout -b feature/auto-updater
git checkout -b fix/metronome-crash
git checkout -b feature/new-sound-system
```

#### **2. Develop on Feature Branch**
```bash
# Use any Mac editor/IDE
code MyBakkesModPlugin/MyBakkesModPlugin.cpp
vim MyBakkesModPlugin/MyBakkesModPlugin.h
# ... make your changes

# Commit to feature branch (NO conflicts possible!)
git add .
git commit -m "feat: add plugin auto-update system"
git push origin feature/your-feature-name
```

#### **3. Monitor Feature Branch Build**
```bash
# Monitor build in real-time (feature branch builds, no auto-increment)
sleep 15 && RUN_ID=$(gh run list --limit 1 --json databaseId --jq '.[0].databaseId') && gh run watch $RUN_ID

# Feature branch builds test code without auto-incrementing main!
```

#### **4. Merge When Ready**
```bash
# Option A: Direct merge (when confident)
git checkout main
git pull origin main  # Get any new changes
git merge feature/your-feature-name --no-ff  # Merge with history
git push origin main  # This triggers auto-increment on main
git branch -d feature/your-feature-name  # Clean up

# Option B: Create Pull Request (for review)
# Push feature branch → Create PR on GitHub → Merge via web interface
gh pr create --title "Add auto-update system" --body "Description"
```

#### **5. Results on Main Branch**
```bash
# ✅ Main branch auto-increments cleanly (no conflicts!)
echo "✅ Feature merged! GitHub Actions auto-increments version on main."

# Check final result
gh api '/repos/addisonkowalski/CustomPlayerAnthems/commits?per_page=1' | jq '.[0] | {sha: .sha, message: .commit.message}'
```

### **⚠️ Legacy: Direct Main Development (CONFLICT-PRONE)**

*Only use for tiny hotfixes. Feature branches are much better!*

#### **Direct Main Workflow (Not Recommended)**
```bash
# ⚠️ CAUSES CONFLICTS if GitHub Actions completes while you're working
git add .
git commit -m "changes"
git push origin main  # Can conflict with auto-increment!
```

**❌ Build Failure:**
```bash
# Investigate errors using Level 3 monitoring
RUN_ID=$(gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq -r '.workflow_runs[0].id')
gh api "/repos/addisonkowalski/CustomPlayerAnthems/actions/runs/$RUN_ID/logs" > build_logs.zip
unzip -o -q build_logs.zip
grep -A 10 -B 5 -E "(error C|undefined|cannot)" 0_build.txt | head -40

# Fix identified issues based on specific error messages
# ... edit source files based on error analysis

# Re-push and monitor (simple cycle)
git add .
git commit -m "fix: resolve compilation error"
git push origin main
```

### **🚨 Conflict Resolution (Mostly Eliminated with Feature Branches!)**

#### **🎯 BEST SOLUTION: Use Feature Branches to Prevent Conflicts**
```bash
# ✅ RECOMMENDED: Work on feature branches (NO conflicts possible!)
git checkout -b feature/my-changes
git add . && git commit -m "changes" && git push origin feature/my-changes
# Feature branches never conflict with main's auto-increments!
```

#### **Legacy: Direct Main Conflicts (Rare with Branch Workflow)**

**Symptoms (only when working directly on main):**
```bash
# You'll see this error:
error: failed to push some refs to 'https://github.com/addisonkowalski/CustomPlayerAnthems.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
```

**Root Cause:** GitHub Actions auto-incremented version while you were working on main directly.

**Quick Resolution:**
```bash
# 1. Pull remote changes (let GitHub Actions version win)
git pull origin main --no-rebase

# 2. If merge conflict on version.h, accept GitHub Actions version:
git checkout --theirs MyBakkesModPlugin/version.h
git add MyBakkesModPlugin/version.h

# 3. Complete merge commit
git commit --no-edit

# 4. Push merged result
git push origin main

# 5. Verify everything is in sync
gh api '/repos/addisonkowalski/CustomPlayerAnthems/commits?per_page=1' | jq '.[0] | {sha: .sha, message: .commit.message}'
```

#### **Prevention Strategy:**
```bash
# ✅ BEST: Use feature branches (eliminates conflicts)
git checkout -b feature/your-changes

# ⚠️ LEGACY: For direct main development
git pull origin main  # Always start with latest
# ... make changes quickly
git pull origin main  # Check again before pushing
git push origin main   # Push immediately
```

### **Advanced Workflows**

#### **Release Preparation (Official Releases Only)**
```bash
# For official releases, manually increment version BEFORE development
./scripts/version-manager.sh patch  # OR minor/major
git add MyBakkesModPlugin/version.h
git commit -m "bump: version for v0.1.1 release"
git push origin main

# Then do development work normally
# GitHub Actions will auto-increment BUILD numbers from new base
```

#### **Continuous Development (Recommended)**
```bash
# Simple monitoring without version interference
gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq '.workflow_runs[0] | {status: .status, conclusion: .conclusion}'

# Download latest successful build when needed
gh run list --limit 1
```

---

## 🚨 **Error Investigation System**

### **The BREAKTHROUGH METHOD: Real Log Extraction**

When builds fail, use this proven method to get actual error messages:

#### **Step-by-Step Error Investigation:**
```bash
# 1. Get the failed run ID
RUN_ID=$(gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq -r '.workflow_runs[0].id')
echo "Investigating run: $RUN_ID"

# 2. Download compressed logs
gh api "/repos/addisonkowalski/CustomPlayerAnthems/actions/runs/$RUN_ID/logs" > build_logs.zip

# 3. Extract using non-interactive method (prevents hanging)
unzip -o -q build_logs.zip

# 4. Search for specific C++ compilation errors
echo "=== SEARCHING FOR COMPILATION ERRORS ==="
grep -A 10 -B 5 -E "(error C[0-9]|undefined|cannot|not found)" 0_build.txt | head -40

# 5. Get error context with line numbers
grep -n -E "(error C[0-9])" 0_build.txt
```

#### **Common Error Patterns:**
- **`error C2664`**: Type conversion issues (add `.c_str()` for string conversions)
- **`error C2440`**: Cannot convert types (check variable types)
- **`undefined reference`**: Missing library links or function definitions
- **`not found`**: Missing include files or dependencies

### **Error Resolution Protocol**

1. **Extract Specific Error**: Use the method above to get exact error with line numbers
2. **Fix Source Code**: Make targeted changes based on error analysis
3. **Test Fix**: Commit and push fix using standard workflow
4. **Verify Resolution**: Monitor new build for success
5. **Repeat if Needed**: Continue until build succeeds

---

## 📦 **Artifact Management**

### **Download Build Results**
```bash
# List recent runs and artifacts
gh run list --limit 5

# Download latest successful artifact
gh run download --name "CustomPlayerAnthems-Release-*"
```

### **Artifact Contents**
Each successful build generates:
- **CustomPlayerAnthems.dll** - Main plugin file for BakkesMod
- **plugin.cfg** - Plugin configuration file
- **version.h** - Version information for tracking

### **Installation on PC**
1. Download artifacts using GitHub CLI or web interface
2. Extract files from downloaded zip
3. Copy `.dll` file to BakkesMod plugins folder
4. Load plugin in Rocket League via BakkesMod console

---

## 🔧 **Troubleshooting**

### **Common Issues and Solutions**

#### **Recurring Git Conflicts**
```bash
# Problem: Constant version conflicts between local and remote
# Root Cause: Manually incrementing versions when GitHub Actions also does it
# Solution: NEVER manually increment VERSION_BUILD

# ✅ Correct approach:
git pull origin main  # Get latest auto-incremented version
# Make your changes
git add . && git commit -m "changes" && git push origin main

# ❌ Incorrect approach that causes conflicts:
# Don't run: ./scripts/version-manager.sh auto
```

#### **Build Monitoring Hangs**
```bash
# Problem: unzip commands hang waiting for user input
# Solution: Always use -o flag to force overwrite
unzip -o -q build_logs.zip  # ✅ Correct (non-interactive)
# Not: unzip -q build_logs.zip  # ❌ Can hang on file replace prompts
```

#### **GitHub CLI Authentication**
```bash
# Problem: gh commands fail with auth errors
# Solution:
gh auth logout
gh auth login
gh auth status
```

#### **Version File Conflicts**
```bash
# Problem: Merge conflicts on version.h
# Solution: Always accept GitHub Actions version (--theirs)
git checkout --theirs MyBakkesModPlugin/version.h
git add MyBakkesModPlugin/version.h
git commit --no-edit
```

### **Debug Commands**

#### **Repository Status**
```bash
# Check latest commit matches what you expect
gh api '/repos/addisonkowalski/CustomPlayerAnthems/commits?per_page=1' | jq '.[0] | {sha: .sha, message: .commit.message}'

# Check if builds are triggering
gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=3' | jq '.workflow_runs[] | {status: .status, conclusion: .conclusion, created_at: .created_at}'

# Verify authentication
gh auth status
```

#### **Local Git Status**
```bash
# Check for uncommitted changes
git status

# Check if local is behind remote
git fetch origin && git status

# See recent local commits
git log --oneline -5
```

---

## 🚀 **Performance Optimizations**

### **Build Speed Improvements**

#### **Achieved Optimizations:**
- **50%+ faster builds** with intelligent caching
- **SDK reuse** saves 2-3 minutes per build
- **Path filtering** prevents unnecessary builds
- **Conflict-free workflows** eliminate resolution delays

#### **Monitoring Efficiency:**
- **Simple status checks** using GitHub CLI
- **Automatic error investigation** using proven log extraction
- **Conflict prevention** through proper workflow design

---

## 🎉 **Success Metrics**

### **When Everything is Working:**

✅ **Development Flow:**
- Code changes on Mac trigger builds automatically
- Build status visible within 2 minutes of push
- Successful builds complete in under 5 minutes
- ⚠️ **Version automatically increments via GitHub Actions (NO MANUAL ACTION NEEDED)**

✅ **Error Handling:**
- Failed builds investigated using log extraction  
- Specific error messages with line numbers extracted
- Fix guidance provided based on error patterns
- Re-builds triggered seamlessly after fixes

✅ **Conflict-Free Operations:**
- No version conflicts between local and remote
- Clean git history without merge commits
- Predictable workflow without manual version management

✅ **Professional CI/CD:**
- No manual Windows development needed
- Complete build transparency and control
- Fully automated version management
- Enterprise-grade error investigation

---

## 📚 **Quick Reference**

### **Essential Commands (Feature Branch Workflow - RECOMMENDED)**
```bash
# ✅ RECOMMENDED: Branch-based development (eliminates conflicts)
git checkout main && git pull origin main                    # Start with latest
git checkout -b feature/your-feature-name                    # Create feature branch
# ... make changes
git add . && git commit -m "changes" && git push origin feature/your-feature-name

# Monitor feature branch build
sleep 15 && RUN_ID=$(gh run list --limit 1 --json databaseId --jq '.[0].databaseId') && gh run watch $RUN_ID

# Merge to main when ready
git checkout main && git pull origin main                    # Get latest main
git merge feature/your-feature-name --no-ff                  # Merge with history
git push origin main                                          # Triggers auto-increment
git branch -d feature/your-feature-name                      # Clean up

# Or create PR instead of direct merge:
gh pr create --title "Feature title" --body "Description"
```

### **Legacy Commands (Direct Main - Conflict-Prone)**
```bash
# ⚠️ LEGACY: Direct main development (can cause conflicts)
git pull origin main                     # Start with latest
# ... make changes
git add . && git commit -m "changes" && git push origin main

# Conflict resolution (if needed)
git pull origin main --no-rebase        # Accept remote version changes
git checkout --theirs MyBakkesModPlugin/version.h  # If version conflict
git commit --no-edit && git push origin main
```

### **Universal Commands (Work with Any Workflow)**
```bash
# Real-time build monitoring (keeps connection open)
sleep 15 && RUN_ID=$(gh run list --limit 1 --json databaseId --jq '.[0].databaseId') && gh run watch $RUN_ID

# Error investigation
RUN_ID=$(gh api '/repos/addisonkowalski/CustomPlayerAnthems/actions/runs?per_page=1' | jq -r '.workflow_runs[0].id')
gh api "/repos/addisonkowalski/CustomPlayerAnthems/actions/runs/$RUN_ID/logs" > build_logs.zip
unzip -o -q build_logs.zip && grep -A 5 -B 5 -i "error" 0_build.txt | head -30

# Artifact download
gh run list --limit 1                   # See recent runs
gh run download --name "CustomPlayerAnthems-Release-*"  # Download latest
```

### **❌ Commands to AVOID (Cause Conflicts)**
```bash
# ❌ These cause version conflicts:
./scripts/version-manager.sh auto       # Conflicts with GitHub Actions
sed -i "s/VERSION_BUILD [0-9]*/VERSION_BUILD X/" MyBakkesModPlugin/version.h  # Manual version changes
git commit -m "Auto-increment version"  # Manual version commits
```

---

## 🎯 **Next Steps**

1. **Complete Setup**: Install GitHub CLI and test authentication
2. **Test Integration**: Make a small change and monitor build 
3. **Practice Conflict Resolution**: Familiarize with pull --no-rebase workflow
4. **Focus on Code**: Let automation handle versions, focus on plugin development
5. **Master Error Investigation**: Practice log extraction for faster debugging

---

**This GitHub workflow integration transforms BakkesMod plugin development from a Windows-only process into a truly cross-platform, professional development experience with conflict-free automation! 🚀** 