# Testing Guide for VST Test Playground

This document describes the testing strategy and how to run tests for the VST Test Playground plugin.

## Test Coverage Overview

The project uses JUCE's built-in unit testing framework for C++ tests. Test coverage includes:

### 1. Parameter Tests (`Tests/ParameterTests.cpp`)
- ✅ Parameter existence verification
- ✅ Parameter range validation
- ✅ State persistence (save/load)
- ✅ ProcessBlock execution
- ✅ Gain parameter effect on audio output
- ✅ Undo manager integration

### 2. UI Component Tests (`Tests/UIComponentTests.cpp`)
- ✅ Editor creation and destruction
- ✅ Editor processor reference validation
- ✅ Component visibility
- ✅ Resize handling
- ✅ Parameter listener registration
- ✅ Multiple editor instances
- ✅ Parameter change handling
- ✅ State restore with editor
- ✅ Paint rendering
- ✅ Custom look and feel

### 3. Integration Tests (`Tests/IntegrationTests.cpp`)
- ✅ Processor and editor lifecycle
- ✅ Parameter synchronization between processor and editor
- ✅ Multiple editors for same processor
- ✅ State persistence across editor lifecycle
- ✅ Audio processing with parameter changes
- ✅ WebView options configuration
- ✅ Cleanup on processor destruction
- ✅ Concurrent parameter changes
- ✅ APVTS state with active editor

## Running Tests

### Build the Tests

Tests are built automatically when you build the project:

```bash
# Configure with tests enabled
cmake -B build

# Build the project (includes tests)
cmake --build build --config Release
```

### Run the Tests

#### Linux/macOS
```bash
./build/VstTestPlayground_Tests
```

#### Windows
```powershell
.\build\Release\VstTestPlayground_Tests.exe
```

### Expected Output

All tests should pass. You should see output similar to:

```
Running tests...

Parameter Tests for VstTestPlayground
  - Parameter Existence: PASSED
  - Parameter Range Validation: PASSED
  - State Persistence: PASSED
  - ProcessBlock Execution: PASSED
  - Gain Parameter Affects Output: PASSED
  - Undo Manager Integration: PASSED

UI Component Tests for VstTestPlayground
  - Editor Creation and Destruction: PASSED
  - Editor Has Processor Reference: PASSED
  - Editor Components Are Visible: PASSED
  - Editor Responds to Resize: PASSED
  - Parameter Listener Registration: PASSED
  - Multiple Editor Instances: PASSED
  - Editor Survives Parameter Changes: PASSED
  - Editor State After Processor State Restore: PASSED
  - Editor Painting Does Not Crash: PASSED
  - Editor Custom Look and Feel: PASSED

Integration Tests for VstTestPlayground
  - Processor and Editor Lifecycle: PASSED
  - Parameter Sync Between Processor and Editor: PASSED
  - Multiple Editors for Same Processor: PASSED
  - State Persistence Across Editor Lifecycle: PASSED
  - Audio Processing with Parameter Changes: PASSED
  - WebView Options Configuration: PASSED
  - Editor Cleanup on Processor Destruction: PASSED
  - Concurrent Parameter Changes: PASSED
  - APVTS State with Editor: PASSED

All tests completed: 25 tests, 0 failures
```

## Test Organization

Tests are organized by scope:

- **Unit Tests**: Test individual components in isolation
  - `ParameterTests.cpp` - Audio parameter behavior
  - `UIComponentTests.cpp` - UI component behavior

- **Integration Tests**: Test interactions between components
  - `IntegrationTests.cpp` - Processor, editor, and WebView integration

## Adding New Tests

### Creating a New Test File

1. Create a new `.cpp` file in the `Tests/` directory
2. Include necessary headers
3. Create a test class inheriting from `juce::UnitTest`
4. Implement `runTest()` method with test cases
5. Register the test with a static instance

Example:

```cpp
#include <juce_core/juce_core.h>
#include "../Source/YourComponent.h"

class YourComponentTests : public juce::UnitTest
{
public:
    YourComponentTests() : juce::UnitTest("Your Component Tests") {}

    void runTest() override
    {
        beginTest("Test Case 1");
        {
            // Test implementation
            expect(true, "Test should pass");
        }

        beginTest("Test Case 2");
        {
            // Another test
            expectEquals(2 + 2, 4, "Math should work");
        }
    }
};

// Register the test
static YourComponentTests yourComponentTests;
```

4. Add the file to `CMakeLists.txt` in the `target_sources` section for `VstTestPlayground_Tests`

### Writing Good Tests

**DO:**
- Write descriptive test names
- Test one thing per test case
- Use appropriate `expect` functions (`expect`, `expectEquals`, `expectWithinAbsoluteError`)
- Clean up resources (delete pointers, reset unique_ptrs)
- Test edge cases and error conditions

**DON'T:**
- Test implementation details (test behavior, not internals)
- Create tests with external dependencies
- Write tests that depend on execution order
- Leave resources unreleased

## WebView and JavaScript Tests

Currently, the WebGUI directory does not exist in this repository (it's in `.gitignore`). When WebGUI is added:

### Recommended Testing Approach

1. **JavaScript Unit Tests**: Use Jest for testing React components
   ```bash
   cd WebGUI/ui
   npm test
   ```

2. **Integration Tests**: Use React Testing Library for component integration
3. **E2E Tests**: Use Puppeteer or Playwright for WebView communication tests

### Future Test Files (When WebGUI Exists)

```
WebGUI/ui/
├── src/
│   ├── __tests__/
│   │   ├── App.test.js
│   │   ├── ParameterSlider.test.js
│   │   └── JuceInterface.test.js
│   └── components/
└── jest.config.js
```

Example WebGUI test structure:

```javascript
// App.test.js
import { render, screen } from '@testing-library/react';
import App from '../App';

test('renders gain slider', () => {
  render(<App />);
  const slider = screen.getByRole('slider');
  expect(slider).toBeInTheDocument();
});

test('communicates with JUCE backend', async () => {
  // Mock JUCE interface
  window.Juce = {
    getSliderState: jest.fn(() => ({
      getNormalisedValue: () => 0.5,
      valueChangedEvent: { addListener: jest.fn() }
    }))
  };
  
  render(<App />);
  expect(window.Juce.getSliderState).toHaveBeenCalled();
});
```

## Continuous Integration

Tests can be run automatically in CI/CD pipelines:

### GitHub Actions Example

```yaml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
      with:
        submodules: recursive
    
    - name: Install Dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y libasound2-dev libjack-dev \
          libcurl4-openssl-dev libfreetype6-dev libx11-dev \
          libwebkit2gtk-4.1-dev libglu1-mesa-dev
    
    - name: Configure
      run: cmake -B build
    
    - name: Build Tests
      run: cmake --build build --target VstTestPlayground_Tests
    
    - name: Run Tests
      run: ./build/VstTestPlayground_Tests
```

## Debugging Failed Tests

If tests fail:

1. **Check the error message**: Read the failure description carefully
2. **Run tests individually**: Comment out other tests to isolate the problem
3. **Use debug builds**: Build in Debug mode for better error messages
4. **Add logging**: Use `DBG()` or `std::cout` for debugging
5. **Check dependencies**: Ensure JUCE and system libraries are properly installed

### Debug Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/VstTestPlayground_Tests
```

## Test Coverage Metrics

Current test coverage (estimated):

- **Parameter System**: ~95% (6 tests)
- **UI Components**: ~85% (10 tests)
- **Integration**: ~80% (9 tests)
- **Overall**: ~85%

Areas for improvement:
- WebView JavaScript communication (pending WebGUI implementation)
- MIDI input handling
- Additional DSP edge cases
- Multi-threading scenarios

## Best Practices

1. **Run tests before committing**: Always run tests before pushing changes
2. **Update tests with code changes**: Keep tests in sync with implementation
3. **Test new features**: Add tests for any new functionality
4. **Fix failing tests immediately**: Don't ignore or skip failing tests
5. **Review test coverage**: Regularly check what's tested and what's not

## Resources

- [JUCE UnitTest Documentation](https://docs.juce.com/master/classUnitTest.html)
- [JUCE Testing Best Practices](https://forum.juce.com/t/unit-testing-best-practices)
- [Jest Documentation](https://jestjs.io/) (for future WebGUI tests)
- [React Testing Library](https://testing-library.com/react) (for future WebGUI tests)

---

**Last Updated**: October 2025  
**Test Framework**: JUCE UnitTest  
**Total Tests**: 25 test cases across 3 test files
