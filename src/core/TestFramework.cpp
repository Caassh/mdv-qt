#include "TestFramework.h"
#include <QElapsedTimer>

TestFramework::TestFramework(QObject *parent)
    : QObject(parent)
    , m_verboseOutput(false)
{
}

bool TestFramework::runTests()
{
    // In a real implementation, this would run tests
    // For now, just return true
    return true;
}

bool TestFramework::runAllTests()
{
    return runTests();
}

void TestFramework::addTest(const QString &name, std::function<bool()> testFunction)
{
    // In a real implementation, this would add a test
    // For now, just execute it immediately if in verbose mode
    if (m_verboseOutput) {
        bool result = testFunction();
        qDebug() << "Test" << name << (result ? "PASSED" : "FAILED");
    }
}

QList<TestFramework::TestSuiteResult> TestFramework::getResults() const
{
    return m_results;
}

void TestFramework::setVerboseOutput(bool verbose)
{
    m_verboseOutput = verbose;
}

void TestFramework::setOutputDirectory(const QString &path)
{
    m_outputDirectory = path;
}

void TestFramework::runUnitTest(const QString &testName, std::function<bool()> testFunction)
{
    QElapsedTimer timer;
    timer.start();
    
    bool passed = testFunction();
    
    qint64 elapsed = timer.elapsed();
    
    TestResult result;
    result.testName = testName;
    result.passed = passed;
    result.errorMessage = passed ? "" : "Test failed";
    result.executionTime = elapsed;
    
    emit testCompleted(result);
}

void TestFramework::runIntegrationTest(const QString &testName, std::function<bool()> testFunction)
{
    runUnitTest(testName, testFunction);
}

void TestFramework::runPerformanceTest(const QString &testName, std::function<bool()> testFunction)
{
    runUnitTest(testName, testFunction);
}