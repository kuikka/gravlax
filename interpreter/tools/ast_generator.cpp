#include <fstream>
#include <numeric>
#include <string>
#include <vector>

#include <fmt/core.h>

#include <gravlax/utils/string_join.h>
#include <gravlax/utils/string_tokenizer.h>

using gravlax::utils::to_lowercase;
using gravlax::utils::tokenize_string;
using gravlax::utils::trim_string;

std::vector<std::string> expressionData = {
    "Binary   : Expr left, Token oper, Expr right",
    "Grouping : Expr expression", "Literal : Token::Literal value",
    "Unary    : Token oper, Expr right"};

struct ExpressionData {
    std::string name;
    std::vector<std::pair<std::string, std::string>> fields;
};

struct AstGenerator {
    std::string outputDir;
    std::string baseClassName;

    AstGenerator(std::string_view outputDir, std::string_view baseClassName)
        : outputDir(outputDir), baseClassName(baseClassName)
    {
    }

    void generate(const std::vector<std::string> &expressionData)
    {
        std::vector<ExpressionData> types = parseExpressionData(expressionData);

        generateVisitorBase(types);
        for (auto &type : types) {
            // generateType(std::cout, type);
            generateType(type);
        }
    }

    std::vector<ExpressionData>
    parseExpressionData(std::vector<std::string> data)
    {
        std::vector<ExpressionData> ret;

        for (auto &type : data) {
            ExpressionData exp;

            auto tokens = tokenize_string(type, ":", 1);
            exp.name = trim_string(tokens[0]);
            auto fields = tokenize_string(tokens[1], ",");

            for (auto &field : fields) {
                // Expect type name, i.e. "Expression left"
                auto split = tokenize_string(field, " ");
                exp.fields.push_back(
                    {trim_string(split[0]), trim_string(split[1])});
            }
            ret.push_back(exp);
        }
        return ret;
    }

    void generateVisitorBase(const std::vector<ExpressionData> &types)
    {
        // std::ostream &out = std::cout;

        std::ofstream out;
        std::string path = fmt::format("{}/{}", outputDir, "visitor_base.h");
        out.open(path);

        out << "#pragma once\n";

        // out << "#include <gravlax/expression.h>\n\n";
        out << "namespace gravlax::generated {\n";

        // Forward declerations
        for (auto &type : types) {
            out << fmt::format("template <typename> struct {};\n", type.name);
        }

        out << "template <typename R>\n";
        out << fmt::format("class {}VisitorBase {{\n", baseClassName);
        out << "public:\n";

        for (auto &type : types) {
            out << fmt::format("virtual R visit{}{}({}<R>& visitor) = 0;\n",
                               type.name, baseClassName, type.name);
        }

        out << "};\n";
        out << "}; // namespace gravlax::generated\n";
    }

    void generateType(ExpressionData &type)
    {
        std::ofstream out;
        std::string path =
            fmt::format("{}/{}.h", outputDir, to_lowercase(type.name));
        out.open(path);

        generateType(out, type);

        out.close();
    }

    void generateType(std::ostream &out, ExpressionData &type)
    {
        writeHeader(out);
        writeType(out, type);
    }

    void writeHeader(std::ostream &out)
    {
        out << "#pragma once\n\n";

        out << "#include <gravlax/expression.h>\n";
        out << "#include <gravlax/token.h>\n\n";
    }

    std::string field_to_string(std::pair<std::string, std::string> field)
    {
        if (field.first == "Token::Literal" || field.first == "Token") {
            return fmt::format("{} {}", templatize(field.first), field.second);
        } else {
            return fmt::format("std::shared_ptr<{}> {}",
                               templatize(field.first), field.second);
        }
    }

    std::string templatize(const std::string &type)
    {
        if (type == "Token" || type == "Token::Literal") {
            return type;
        }
        return fmt::format("{}<R>", type);
    }

    void writeType(std::ostream &out, ExpressionData &type)
    {
        out << "namespace gravlax::generated {\n\n";

        out << "template <typename R>\n";
        out << fmt::format("struct {} : public {}<R> {{\n", type.name,
                           baseClassName);

        // data members
        for (auto &field : type.fields) {
            out << fmt::format("{};\n", field_to_string(field));
        }
        out << "\n";

        // Constructor
        out << fmt::format("{}(", type.name);

        std::vector<std::string> f;
        for (auto &field : type.fields) {
            f.push_back(field_to_string(field));
        }

        out << string_join(f, ", ");

        out << ") : ";

        // Constructor Initializers
        f.clear();
        for (auto &field : type.fields) {
            f.push_back(fmt::format("{}({})", field.second, field.second));
        }
        out << string_join(f, ", ");

        out << "\n{}\n";

        out << "R accept(ExprVisitorBase<R> & visitor) override {\n";
        out << fmt::format("    return visitor.visit{}Expr(*this);\n",
                           type.name);
        out << "};\n";

        out << "};\n";
        out << "}; // namespace gravlax::generated\n";
    }
};

int main(int argc, char *argv[])
{
    AstGenerator gen(argv[1], "Expr");
    gen.generate(expressionData);
}