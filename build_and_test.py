#!/usr/bin/env python3
"""
Alternative build script using Python
This can run without PowerShell 6+
"""
import subprocess
import sys
import os
from pathlib import Path

def run_command(cmd, cwd=None, shell=True):
    """Run a command and return success status"""
    print(f"\n→ Running: {cmd}")
    try:
        result = subprocess.run(
            cmd,
            cwd=cwd,
            shell=shell,
            capture_output=True,
            text=True,
            timeout=300
        )
        if result.stdout:
            print(result.stdout)
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        return result.returncode == 0
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return False

def main():
    print("=" * 50)
    print("VST Plugin Build and Test (Python)")
    print("=" * 50)
    
    # Get project root
    project_root = Path(__file__).parent
    os.chdir(project_root)
    
    # Step 1: Check Node.js
    print("\n[1/6] Checking Node.js...")
    if not run_command("node --version"):
        print("ERROR: Node.js not found!")
        print("Install from: https://nodejs.org/")
        return 1
    
    # Step 2: Check npm
    print("\n[2/6] Checking npm...")
    if not run_command("npm --version"):
        print("ERROR: npm not found!")
        return 1
    
    # Step 3: Build WebGUI
    print("\n[3/6] Building WebGUI...")
    webgui_dir = project_root / "WebGUI" / "ui"
    
    print("  Installing npm dependencies...")
    if not run_command("npm install --silent", cwd=webgui_dir):
        print("ERROR: npm install failed!")
        return 1
    
    print("  Building with Vite...")
    if not run_command("npm run build", cwd=webgui_dir):
        print("ERROR: npm build failed!")
        return 1
    
    # Verify dist exists
    dist_dir = webgui_dir / "dist"
    if not dist_dir.exists():
        print(f"ERROR: Build output not found at {dist_dir}")
        return 1
    print("  ✓ WebGUI built successfully")
    
    # Step 4: Configure CMake
    print("\n[4/6] Configuring CMake...")
    build_dir = project_root / "build"
    
    # Clean build directory if exists
    if build_dir.exists():
        print("  Cleaning existing build directory...")
        import shutil
        shutil.rmtree(build_dir)
    
    cmake_cmd = 'cmake -G "Visual Studio 17 2022" -A x64 -B build'
    if not run_command(cmake_cmd):
        print("ERROR: CMake configuration failed!")
        print("Try: cmake -G \"Visual Studio 16 2019\" -A x64 -B build")
        return 1
    print("  ✓ CMake configured successfully")
    
    # Step 5: Build Plugin
    print("\n[5/6] Building plugin (this may take a few minutes)...")
    if not run_command("cmake --build build --config Release"):
        print("ERROR: Build failed!")
        return 1
    print("  ✓ Plugin built successfully")
    
    # Step 6: Run Tests
    print("\n[6/6] Running tests...")
    test_exe = build_dir / "Release" / "VstTestPlayground_Tests.exe"
    if test_exe.exists():
        if not run_command(str(test_exe)):
            print("ERROR: Some tests failed!")
            return 1
        print("  ✓ All tests passed!")
    else:
        print(f"WARNING: Test executable not found at {test_exe}")
    
    # Success!
    print("\n" + "=" * 50)
    print("BUILD SUCCESSFUL!")
    print("=" * 50)
    print("\nPlugin location:")
    print("  C:\\Program Files\\Common Files\\VST3\\VstTestPlayground.vst3")
    print("\nNext steps:")
    print("  1. Load the plugin in your DAW")
    print("  2. Test parameter automation")
    print("  3. Test multiple instances")
    print("  4. See docs/QUICKSTART_TESTING.md for details")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
