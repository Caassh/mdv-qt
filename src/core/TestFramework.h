// TestFramework.h
#ifndef TESTFRAMEWORK_H
#define TESTFRAMEWORK_H

#include <QObject>
#include <QStringList>
#include <functional>
#include <QDateTime>
#include <QElapsedTimer>
#include <QDebug>

class TestFramework : public QObject
{
    Q_OBJECT

public:
    explicit TestFramework(QObject *parent = nullptr);
    
    struct TestResult {
        QString testName;
        bool passed;
        QString errorMessage;
        qint64 executionTime; // in milliseconds
    };
    
    struct TestSuiteResult {
        QString suiteName;
        QList<TestResult> results;
        QDateTime startTime;
        QDateTime endTime;
        int totalTests;
        int passedTests;
        int failedTests;
    };
    
    bool runTests();
    bool runAllTests();
    void addTest(const QString &name, std::function<bool()> testFunction);
    
    QList<TestSuiteResult> getResults() const;
    
    void setVerboseOutput(bool verbose);
    void setOutputDirectory(const QString &path);
    
signals:
    void testStarted(const QString &testName);
    void testCompleted(const TestFramework::TestResult &result);
    void testSuiteCompleted(const TestFramework::TestSuiteResult &result);
    void testingProgress(int percentage);
    void testingCompleted();
    
private:
    bool m_verboseOutput;
    QString m_outputDirectory;
    QList<TestSuiteResult> m_results;
    QElapsedTimer m_timer;
    
    void runUnitTest(const QString &testName, std::function<bool()> testFunction);
    void runIntegrationTest(const QString &testName, std::function<bool()> testFunction);
    void runPerformanceTest(const QString &testName, std::function<bool()> testFunction);
};

#endif // TESTFRAMEWORK_H