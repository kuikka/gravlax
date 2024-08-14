#include <fmt/core.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gravlax/parser.h>
#include <gravlax/scanner.h>

using gravlax::Token;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::MockFunction;

class ParserTest : public ::testing::Test
{
  public:
    gravlax::Scanner scanner;
    gravlax::Parser<std::string> parser;
};

TEST_F(ParserTest, test_parser)
{
    auto code = "var foo = 10;";
    std::unique_ptr<std::vector<Token>> tokens = scanner.scanString(code);
    parser.parse(std::move(tokens));
}
