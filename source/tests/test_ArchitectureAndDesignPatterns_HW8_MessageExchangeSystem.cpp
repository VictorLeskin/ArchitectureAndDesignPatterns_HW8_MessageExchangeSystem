///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

// gTest grouping class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem
  {
  public:
    // add here members for free access.
    using ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem; // delegate constructors
  };

};
 
TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_ctor )
{
  Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem t;
}


// for convenience
using json = nlohmann::json;

TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_cCreatingJSONObject)
{
    // Create a JSON object
    json j;
    j["name"] = "John Doe";
    j["age"] = 30;
    j["is_student"] = false;
    j["courses"] = { "Math", "Physics", "Chemistry" };
    j["address"] = 
    {
        {"street", "123 Main St"},
        {"city", "New York"},
        {"zip", "10001"}
    };

    // Serialize to string
    std::string json_str = j.dump(4); // 4 spaces for pretty printing
    //EXPECT_EQ("", json_str);
    std::cout << "JSON output:\n" << json_str << "\n\n";

    // Parse from string
    json j2 = json::parse(json_str);

    // Access values
    std::cout << "Name: " << j2["name"] << "\n";
    std::cout << "Age: " << j2["age"] << "\n";
    std::cout << "First course: " << j2["courses"][0] << "\n";
    std::cout << "City: " << j2["address"]["city"] << "\n";

    // Modify values
    j2["age"] = 31;
    j2["courses"].push_back("Biology");

    // Check if key exists
    if (j2.contains("is_student")) 
    {
        std::cout << "Is student: " << j2["is_student"] << "\n";
    }
}