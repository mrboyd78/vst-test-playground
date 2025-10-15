@echo off
REM Git Commit and Push Script
REM Commits all code review implementation changes

echo =====================================
echo Git Commit and Push
echo =====================================
echo.

REM Check if git is available
where git >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Git not found!
    echo Please install Git from: https://git-scm.com/
    pause
    exit /b 1
)

echo [1/5] Checking git status...
git status
echo.

echo [2/5] Adding all changes...
git add .
echo.

echo [3/5] Reviewing staged changes...
git status
echo.

echo The following commit will be made:
echo.
echo Title: "Implement code review recommendations - WebView2, APVTS, optimization, tests"
echo.
echo Description:
echo - Fixed WebView2 configuration with static linking
echo - Implemented proper parameter synchronization with WebSliderParameterAttachment
echo - Added WebView2 backend initialization with process isolation
echo - Optimized processBlock for real-time safety
echo - Added UndoManager for undo/redo support
echo - Fixed parameter range to industry standard (-60dB to +12dB)
echo - Added comprehensive error handling
echo - Expanded test suite from 1 to 6 tests
echo - Automated WebGUI build integration
echo - Removed non-existent onOff parameter from frontend
echo - Removed React StrictMode for production
echo - Created comprehensive documentation (70KB)
echo - Created build automation scripts
echo.
echo Files changed: 21 files (9 modified, 12 created)
echo.

set /p confirm="Proceed with commit? (Y/N): "
if /i not "%confirm%"=="Y" (
    echo Commit cancelled.
    pause
    exit /b 0
)

echo.
echo [4/5] Creating commit...
git commit -m "Implement code review recommendations - WebView2, APVTS, optimization, tests" -m "Major improvements addressing 12 critical issues:" -m "- Fixed WebView2 configuration with static linking and proper compile definitions" -m "- Implemented WebSliderParameterAttachment for proper DAW automation support" -m "- Added WebView2 backend initialization with process-isolated user data folders" -m "- Optimized processBlock for real-time safety (cached parameters, conditional updates)" -m "- Added UndoManager for full undo/redo support" -m "- Fixed parameter range from -100/+100dB to industry standard -60/+12dB" -m "- Added comprehensive error handling with fallback UI" -m "- Expanded test suite from 1 to 6 comprehensive tests (500%% increase)" -m "- Automated WebGUI build integration in CMake" -m "- Removed non-existent onOff parameter from React frontend" -m "- Removed React StrictMode to prevent double-rendering issues" -m "- Created 70KB of comprehensive documentation" -m "" -m "Modified files (9):" -m "- CMakeLists.txt" -m "- Source/PluginProcessor.h/cpp" -m "- Source/PluginEditor.h/cpp" -m "- Source/WebView.cpp" -m "- WebGUI/ui/src/App.jsx" -m "- WebGUI/ui/src/main.jsx" -m "- Tests/ParameterTests.cpp" -m "" -m "New files (12):" -m "- docs/CODE_REVIEW.md (27KB)" -m "- docs/IMPLEMENTATION_PLAN.md" -m "- docs/IMPLEMENTATION_SUMMARY.md (12KB)" -m "- docs/IMPLEMENTATION_STATUS.md (12KB)" -m "- docs/VALIDATION_STEPS.md (8KB)" -m "- docs/QUICKSTART_TESTING.md (7KB)" -m "- docs/README.md (13KB)" -m "- IMPLEMENTATION_COMPLETE.md (8KB)" -m "- build_and_test.bat" -m "- build_and_test.py" -m "- validate_build.ps1" -m "- BUILD_INSTRUCTIONS.txt" -m "" -m "All changes validated against official JUCE 8 documentation, Microsoft WebView2 best practices, and audio industry standards." -m "" -m "Closes: #code-review-implementation"

if %errorlevel% neq 0 (
    echo ERROR: Commit failed!
    pause
    exit /b 1
)
echo   Commit successful!
echo.

echo [5/5] Pushing to remote...
git push

if %errorlevel% neq 0 (
    echo ERROR: Push failed!
    echo.
    echo Common issues:
    echo 1. No remote configured: git remote add origin [URL]
    echo 2. Not authenticated: Set up SSH keys or HTTPS credentials
    echo 3. Branch not set: git push --set-upstream origin main
    echo.
    echo Try: git push --set-upstream origin main
    pause
    exit /b 1
)
echo   Push successful!
echo.

echo =====================================
echo SUCCESS!
echo =====================================
echo.
echo All changes have been committed and pushed.
echo.
echo Commit details:
git log -1 --stat
echo.
pause
