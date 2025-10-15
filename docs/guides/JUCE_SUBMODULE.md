# JUCE Submodule Management

This project uses JUCE as a git submodule to manage the dependency efficiently. This document explains how to work with the JUCE submodule.

## Why Use a Submodule?

Using a git submodule for JUCE provides several benefits:

- **Smaller Repository**: The main repository doesn't include JUCE's source code, keeping it lightweight
- **Version Control**: Easy to see which version of JUCE is being used
- **Easy Updates**: Updating JUCE is a simple git command
- **Clean History**: JUCE's commit history is separate from the project's history

## Initial Setup

When cloning the repository for the first time, use the `--recursive` flag:

```bash
git clone --recursive https://github.com/mrboyd78/vst-test-playground.git
cd vst-test-playground
```

If you already cloned without the `--recursive` flag:

```bash
git submodule update --init --recursive
```

## Current JUCE Version

The project currently uses **JUCE 8.0.10**.

To verify the current version:

```bash
cd JUCE
git describe --tags
```

## Updating JUCE Version

To update to a newer version of JUCE:

1. Navigate to the JUCE submodule directory:
   ```bash
   cd JUCE
   ```

2. Fetch the latest tags from JUCE repository:
   ```bash
   git fetch --tags
   ```

3. Checkout the desired version:
   ```bash
   git checkout 8.0.11  # Replace with desired version
   ```

4. Return to the main project directory:
   ```bash
   cd ..
   ```

5. Commit the submodule update:
   ```bash
   git add JUCE
   git commit -m "Update JUCE to version 8.0.11"
   git push
   ```

## Common Issues and Solutions

### Submodule is Empty After Clone

If the JUCE folder is empty after cloning:

```bash
git submodule update --init --recursive
```

### Detached HEAD Warning

When working with submodules, you'll often see "detached HEAD" warnings. This is normal - submodules are pinned to specific commits. You don't need to create a branch unless you're modifying JUCE itself.

### Submodule Changes Not Showing

If you've made changes in the JUCE directory but they're not showing up:

```bash
git status
# Should show "modified: JUCE (new commits)"
git add JUCE
git commit -m "Update JUCE submodule"
```

### Pull Request Conflicts on Submodule

If you encounter conflicts on the JUCE submodule during a pull:

```bash
# Update to the version specified in the main branch
git submodule update --init --recursive
```

## Best Practices

1. **Don't Modify JUCE**: Unless you're contributing to JUCE itself, don't modify files in the JUCE directory
2. **Coordinate Updates**: When updating JUCE, test thoroughly and coordinate with the team
3. **Document Changes**: When updating JUCE, document any breaking changes or new features in your commit message
4. **Check Compatibility**: Before updating JUCE, check the release notes for breaking changes

## Building Without Submodule (Not Recommended)

If you need to use a different JUCE installation:

1. Remove the submodule:
   ```bash
   git submodule deinit -f JUCE
   rm -rf .git/modules/JUCE
   rm -rf JUCE
   ```

2. Add JUCE to .gitignore:
   ```bash
   echo "JUCE/" >> .gitignore
   ```

3. Manually place JUCE in the project directory or update CMakeLists.txt to point to your JUCE installation

This approach is not recommended as it defeats the purpose of the submodule.

## Additional Resources

- [Git Submodules Documentation](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
- [JUCE GitHub Repository](https://github.com/juce-framework/JUCE)
- [JUCE Documentation](https://docs.juce.com/)
