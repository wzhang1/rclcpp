// Copyright 2017 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <string>
#include <memory>
#include <vector>

#include "rclcpp/exceptions.hpp"
#include "rclcpp/node.hpp"
#include "rclcpp/rclcpp.hpp"

class TestNode : public ::testing::Test
{
protected:
  static void SetUpTestCase()
  {
    rclcpp::init(0, nullptr);
  }
};

/*
   Testing node construction and destruction.
 */
TEST_F(TestNode, construction_and_destruction) {
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
  }

  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("invalid_node?", "/ns");
    }, rclcpp::exceptions::InvalidNodeNameError);
  }

  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "/invalid_ns?");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
}

TEST_F(TestNode, get_name_and_namespace) {
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/ns", node->get_namespace());
    EXPECT_STREQ("/ns/my_node", node->get_fully_qualified_name());
  }
  {
    auto options = rclcpp::NodeOptions()
      .arguments({"__ns:=/another_ns"});
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns", options);
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/another_ns", node->get_namespace());
    EXPECT_STREQ("/another_ns/my_node", node->get_fully_qualified_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/ns", node->get_namespace());
    EXPECT_STREQ("/ns/my_node", node->get_fully_qualified_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/", node->get_namespace());
    EXPECT_STREQ("/my_node", node->get_fully_qualified_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/", node->get_namespace());
    EXPECT_STREQ("/my_node", node->get_fully_qualified_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/my/ns");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/my/ns", node->get_namespace());
    EXPECT_STREQ("/my/ns/my_node", node->get_fully_qualified_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "my/ns");
    EXPECT_STREQ("my_node", node->get_name());
    EXPECT_STREQ("/my/ns", node->get_namespace());
    EXPECT_STREQ("/my/ns/my_node", node->get_fully_qualified_name());
  }
}

TEST_F(TestNode, subnode_get_name_and_namespace) {
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
    auto subnode = node->create_sub_node("sub_ns");
    EXPECT_STREQ("my_node", subnode->get_name());
    EXPECT_STREQ("/ns", subnode->get_namespace());
    EXPECT_STREQ("sub_ns", subnode->get_sub_namespace().c_str());
    EXPECT_STREQ("/ns/sub_ns", subnode->get_effective_namespace().c_str());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
    auto subnode = node->create_sub_node("sub_ns");
    EXPECT_STREQ("my_node", subnode->get_name());
    EXPECT_STREQ("/ns", subnode->get_namespace());
    EXPECT_STREQ("sub_ns", subnode->get_sub_namespace().c_str());
    EXPECT_STREQ("/ns/sub_ns", subnode->get_effective_namespace().c_str());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node");
    auto subnode = node->create_sub_node("sub_ns");
    EXPECT_STREQ("my_node", subnode->get_name());
    EXPECT_STREQ("/", subnode->get_namespace());
    EXPECT_STREQ("sub_ns", subnode->get_sub_namespace().c_str());
    EXPECT_STREQ("/sub_ns", subnode->get_effective_namespace().c_str());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
    auto subnode = node->create_sub_node("sub_ns");
    EXPECT_STREQ("my_node", subnode->get_name());
    EXPECT_STREQ("/ns", subnode->get_namespace());
    EXPECT_STREQ("sub_ns", subnode->get_sub_namespace().c_str());
    EXPECT_STREQ("/ns/sub_ns", subnode->get_effective_namespace().c_str());
    auto subnode2 = subnode->create_sub_node("sub_ns2");
    EXPECT_STREQ("my_node", subnode2->get_name());
    EXPECT_STREQ("/ns", subnode2->get_namespace());
    EXPECT_STREQ("sub_ns/sub_ns2", subnode2->get_sub_namespace().c_str());
    EXPECT_STREQ("/ns/sub_ns/sub_ns2", subnode2->get_effective_namespace().c_str());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node");
    auto subnode = node->create_sub_node("sub_ns");
    EXPECT_STREQ("my_node", subnode->get_name());
    EXPECT_STREQ("/", subnode->get_namespace());
    EXPECT_STREQ("sub_ns", subnode->get_sub_namespace().c_str());
    EXPECT_STREQ("/sub_ns", subnode->get_effective_namespace().c_str());
    auto subnode2 = subnode->create_sub_node("sub_ns2");
    EXPECT_STREQ("my_node", subnode2->get_name());
    EXPECT_STREQ("/", subnode2->get_namespace());
    EXPECT_STREQ("sub_ns/sub_ns2", subnode2->get_sub_namespace().c_str());
    EXPECT_STREQ("/sub_ns/sub_ns2", subnode2->get_effective_namespace().c_str());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node");
    ASSERT_THROW({
      auto subnode = node->create_sub_node("/sub_ns");
    }, rclcpp::exceptions::NameValidationError);
  }
}
/*
   Testing node construction and destruction.
 */
TEST_F(TestNode, subnode_construction_and_destruction) {
  {
    ASSERT_NO_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
      auto subnode = node->create_sub_node("sub_ns");
    });
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
      auto subnode = node->create_sub_node("invalid_ns?");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns/");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns/");
      auto subnode = node->create_sub_node("/sub_ns");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
      auto subnode = node->create_sub_node("/sub_ns");
    }, rclcpp::exceptions::NameValidationError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
      auto subnode = node->create_sub_node("~sub_ns");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
      auto subnode = node->create_sub_node("invalid_ns?");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
  {
    ASSERT_NO_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
      auto subnode = node->create_sub_node("sub_ns");
    });
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
      auto subnode = node->create_sub_node("/sub_ns");
    }, rclcpp::exceptions::NameValidationError);
  }
  {
    ASSERT_THROW({
      auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
      auto subnode = node->create_sub_node("~sub_ns");
    }, rclcpp::exceptions::InvalidNamespaceError);
  }
}

TEST_F(TestNode, get_logger) {
  {
    auto node = std::make_shared<rclcpp::Node>("my_node");
    EXPECT_STREQ("my_node", node->get_logger().get_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
    EXPECT_STREQ("ns.my_node", node->get_logger().get_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "ns");
    EXPECT_STREQ("ns.my_node", node->get_logger().get_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "/my/ns");
    EXPECT_STREQ("my.ns.my_node", node->get_logger().get_name());
  }
  {
    auto node = std::make_shared<rclcpp::Node>("my_node", "my/ns");
    EXPECT_STREQ("my.ns.my_node", node->get_logger().get_name());
  }
}

TEST_F(TestNode, get_clock) {
  auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
  auto ros_clock = node->get_clock();
  EXPECT_TRUE(ros_clock != nullptr);
  EXPECT_EQ(ros_clock->get_clock_type(), RCL_ROS_TIME);
}

TEST_F(TestNode, now) {
  auto node = std::make_shared<rclcpp::Node>("my_node", "/ns");
  auto clock = node->get_clock();
  auto now_builtin = node->now().nanoseconds();
  auto now_external = clock->now().nanoseconds();
  EXPECT_GE(now_external, now_builtin);
  EXPECT_LT(now_external - now_builtin, 5000000L);
}
