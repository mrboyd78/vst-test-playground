#include <JuceHeader.h>

class TestRunnerApplication : public juce::JUCEApplication
{
public:
    TestRunnerApplication() {}

    const juce::String getApplicationName() override       { return "VstTestPlayground_Tests"; }
    const juce::String getApplicationVersion() override    { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise (const juce::String& commandLine) override
    {
        juce::UnitTestRunner runner;
        runner.runAllTests();

        int numFailures = 0;
        for (int i = 0; i < runner.getNumResults(); ++i)
        {
            const auto* result = runner.getResult(i);
            if (result->failures > 0)
                numFailures++;
        }

        setApplicationReturnValue (numFailures);
        quit();
    }

    void shutdown() override {}

    void systemRequestedQuit() override
    {
        quit();
    }
};

START_JUCE_APPLICATION (TestRunnerApplication)
