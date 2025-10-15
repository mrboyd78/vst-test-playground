# Remaining Technical Debt

This document outlines the remaining technical debt in the project. These items can be used to create GitHub issues to track their resolution.

## Issue 1: JUCE Dependency Management

**Title:** Improve JUCE Dependency Management

**Body:**

Currently, the entire JUCE library is committed directly to the repository. This is not ideal for several reasons:

*   **Repository size:** It bloats the repository with a large amount of third-party code.
*   **Updates:** Updating JUCE becomes a manual and error-prone process of replacing the files.
*   **Version tracking:** It's difficult to see which version of JUCE is being used.

The recommended approach is to use a git submodule to manage the JUCE dependency. This will keep the JUCE code separate from the project's code and make it easy to update.

**Acceptance Criteria:**

*   The JUCE library is added as a git submodule.
*   The project builds successfully using the JUCE submodule.
*   The `.gitmodules` file is configured correctly.

## Issue 2: Testing

**Title:** Improve Test Coverage

**Body:**

The current test suite only covers the audio processor parameters. There are no tests for the UI or the `WebView`. This makes it difficult to ensure the quality of the UI and to prevent regressions.

The following areas should be tested:

*   **UI:**
    *   Component rendering
    *   User interaction
    *   State changes
*   **WebView:**
    *   Communication between the `WebView` and the plugin
    *   UI updates
    *   JavaScript code

A UI testing framework, such as Jest and Puppeteer, should be used to write the tests.

**Acceptance Criteria:**

*   Unit tests are added for the UI components.
*   Integration tests are added for the `WebView`.
*   The test coverage is significantly increased.

## Issue 3: Documentation

**Title:** Update and Organize Documentation

**Body:**

The `docs` folder is disorganized and contains outdated information. This makes it difficult for new developers to get started with the project and for existing developers to find the information they need.

The following should be done to improve the documentation:

*   **Organize the `docs` folder:** The files in the `docs` folder should be organized into a logical structure.
*   **Update the documentation:** The documentation should be updated to reflect the current state of the project.
*   **Add missing documentation:** Documentation should be added for any areas that are not currently covered.

**Acceptance Criteria:**

*   The `docs` folder is organized and easy to navigate.
*   The documentation is up-to-date and accurate.
*   The documentation covers all aspects of the project.
