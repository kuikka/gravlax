#include <fmt/core.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gravlax/utils/string_join.h>
#include <gravlax/utils/string_tokenizer.h>

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::MockFunction;

using gravlax::utils::tokenize_string;
using gravlax::utils::trim_string;

class StringTest : public ::testing::Test
{
};

TEST_F(StringTest, test_tokenizer_1)
{
    auto tokens = tokenize_string("a,b,c", ",");
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("a", tokens[0]);
    EXPECT_EQ("b", tokens[1]);
    EXPECT_EQ("c", tokens[2]);
}

TEST_F(StringTest, test_tokenizer_2)
{
    auto tokens = tokenize_string("a , b,c ", ",");
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("a ", tokens[0]);
    EXPECT_EQ(" b", tokens[1]);
    EXPECT_EQ("c ", tokens[2]);
}

TEST_F(StringTest, test_tokenizer_max_split_1)
{
    auto tokens = tokenize_string("a,b,c", ",", 1);
    EXPECT_EQ(2, tokens.size());
    EXPECT_EQ("a", tokens[0]);
    EXPECT_EQ("b,c", tokens[1]);
}

TEST_F(StringTest, test_tokenizer_max_split_2)
{
    auto tokens = tokenize_string("a,b,c,d", ",", 2);
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("a", tokens[0]);
    EXPECT_EQ("b", tokens[1]);
    EXPECT_EQ("c,d", tokens[2]);
}

TEST_F(StringTest, test_tokenizer_max_split_3)
{
    auto tokens = tokenize_string("a,b,c", ",", 3);
    EXPECT_EQ(3, tokens.size());
    EXPECT_EQ("a", tokens[0]);
    EXPECT_EQ("b", tokens[1]);
    EXPECT_EQ("c", tokens[2]);
}

TEST_F(StringTest, test_trim_empty)
{
    EXPECT_EQ("", trim_string(""));
}

TEST_F(StringTest, test_trim_1)
{
    EXPECT_EQ("a", trim_string("a"));
}

TEST_F(StringTest, test_trim_2)
{
    EXPECT_EQ("a", trim_string(" a"));
}

TEST_F(StringTest, test_trim_3)
{
    EXPECT_EQ("a", trim_string("a "));
}

TEST_F(StringTest, test_trim_4)
{
    EXPECT_EQ("a", trim_string(" a "));
}

TEST_F(StringTest, test_trim_5)
{
    EXPECT_EQ("a a", trim_string(" a a "));
}
