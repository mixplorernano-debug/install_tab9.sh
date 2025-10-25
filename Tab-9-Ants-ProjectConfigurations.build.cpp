#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <string>
#include "Tab-9-Ants-ProjectConfigurations.cpp"

class TabnineConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(TabnineConfigTest, LoadConfigWithDisableTeamLearningTrue) {
    // Create a temporary .tabnine file with disableTeamLearning set to true
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "disableTeamLearning": true\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Check if disableTeamLearning is set to true
    EXPECT_TRUE(config.disableTeamLearning);

    // Check if teamLearningIgnore is empty
    EXPECT_TRUE(config.teamLearningIgnore.empty());

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, LoadConfigWithEmptyFile) {
    // Create an empty .tabnine file
    std::ofstream tempFile(".tabnine");
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Check if default configuration is used
    EXPECT_FALSE(config.disableTeamLearning);
    EXPECT_TRUE(config.teamLearningIgnore.empty());

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, MatchFilesCatchAllMask) {
    // Create a temporary .tabnine file with catch-all mask
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["*"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test various file paths
    EXPECT_TRUE(config.isIgnored("main.cpp"));
    EXPECT_TRUE(config.isIgnored("src/tests/unit_test.cpp"));
    EXPECT_TRUE(config.isIgnored("build/logs/error.log"));
    EXPECT_TRUE(config.isIgnored("secrets/prod.key"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, HandleLeadingWildcardPattern) {
    // Create a temporary .tabnine file with a leading wildcard pattern
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["*.key"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test files that should match the wildcard pattern
    EXPECT_TRUE(config.isIgnored("secrets/prod.key"));
    EXPECT_TRUE(config.isIgnored("config.key"));
    EXPECT_TRUE(config.isIgnored("/root/system.key"));

    // Test files that should not match the wildcard pattern
    EXPECT_FALSE(config.isIgnored("keyfile.txt"));
    EXPECT_FALSE(config.isIgnored("my.key.config"));
    EXPECT_FALSE(config.isIgnored("key"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, HandleTrailingWildcardPattern) {
    // Create a temporary .tabnine file with a trailing wildcard pattern
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["secrets/*"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test files that should match the wildcard pattern
    EXPECT_TRUE(config.isIgnored("secrets/prod.key"));
    EXPECT_TRUE(config.isIgnored("secrets/config.json"));
    EXPECT_TRUE(config.isIgnored("secrets/subfolder/file.txt"));

    // Test files that should not match the wildcard pattern
    EXPECT_FALSE(config.isIgnored("src/secrets.cpp"));
    EXPECT_FALSE(config.isIgnored("secrets"));
    EXPECT_FALSE(config.isIgnored("not_secrets/file.txt"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, IdentifyIgnoredFilesWithMultiplePatterns) {
    // Create a temporary .tabnine file with multiple ignore patterns
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["*.log", "src/tests/*", "config.json"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test various file paths
    EXPECT_TRUE(config.isIgnored("error.log"));
    EXPECT_TRUE(config.isIgnored("src/tests/unit_test.cpp"));
    EXPECT_TRUE(config.isIgnored("src/tests/integration/test_suite.hpp"));
    EXPECT_TRUE(config.isIgnored("config.json"));
    
    EXPECT_FALSE(config.isIgnored("main.cpp"));
    EXPECT_FALSE(config.isIgnored("src/main/app.cpp"));
    EXPECT_FALSE(config.isIgnored("docs/readme.md"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, HandleNonExistentTabnineFile) {
    // Ensure the .tabnine file doesn't exist
    std::remove(".tabnine");

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Check if default configuration is used
    EXPECT_FALSE(config.disableTeamLearning);
    EXPECT_TRUE(config.teamLearningIgnore.empty());

    // Verify console output
    testing::internal::CaptureStdout();
    config.print();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_THAT(output, testing::HasSubstr("Disable Team Learning: false"));
    EXPECT_THAT(output, testing::HasSubstr("Team Learning Ignore Masks (0):"));
}

TEST_F(TabnineConfigTest, MatchFilesInNestedSubdirectories) {
    // Create a temporary .tabnine file with ignore patterns for nested directories
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["src/tests", "build/output"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test files in nested subdirectories
    EXPECT_TRUE(config.isIgnored("src/tests/unit/core_test.cpp"));
    EXPECT_TRUE(config.isIgnored("src/tests/integration/api/test_suite.hpp"));
    EXPECT_TRUE(config.isIgnored("build/output/logs/error.log"));
    EXPECT_TRUE(config.isIgnored("build/output/temp/cache/data.bin"));

    // Test files that should not be ignored
    EXPECT_FALSE(config.isIgnored("src/main/app.cpp"));
    EXPECT_FALSE(config.isIgnored("build/artifacts/release.zip"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, DoNotIgnoreFilesNotMatchingPatterns) {
    // Create a temporary .tabnine file with specific ignore patterns
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["*.log", "src/tests/*", "config.json"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test files that should not be ignored
    EXPECT_FALSE(config.isIgnored("main.cpp"));
    EXPECT_FALSE(config.isIgnored("src/main/app.cpp"));
    EXPECT_FALSE(config.isIgnored("docs/readme.md"));
    EXPECT_FALSE(config.isIgnored("src/config.yaml"));

    // Clean up the temporary file
    std::remove(".tabnine");
}

TEST_F(TabnineConfigTest, HandleEdgeCasesInTeamLearningIgnore) {
    // Create a temporary .tabnine file with edge case patterns
    std::ofstream tempFile(".tabnine");
    tempFile << "{\n";
    tempFile << "    "teamLearningIgnore": ["", " ", "  ", "\\t\\n"]\n";
    tempFile << "}\n";
    tempFile.close();

    // Load the configuration
    TabnineConfig config = loadConfig(".tabnine");

    // Test various file paths
    EXPECT_FALSE(config.isIgnored("main.cpp"));
    EXPECT_FALSE(config.isIgnored("src/tests/unit_test.cpp"));
    EXPECT_FALSE(config.isIgnored("build/logs/error.log"));

    // Verify that the empty and whitespace-only patterns were not added
    EXPECT_TRUE(config.teamLearningIgnore.empty());

    // Clean up the temporary file
    std::remove(".tabnine");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
