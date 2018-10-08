#ifndef SANITY_AST_H
#define SANITY_AST_H

#include <string>
#include <memory>
#include <vector>
#include "compiler/models/token.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

/**
 * Holds all the different class representing the different elements of an abstract syntax tree representing Sanity.
 */
namespace AST {
    // Forward declare all AST types for the visitor.
    class AddOpExpression;
    class SubOpExpression;
    class MulOpExpression;
    class DivOpExpression;
    class IntegerType;
    class StringType;
    class FunctionPrototype;
    class Function;
    class Statement;
    class File;
    class CharLiteral;
    class IntegerLiteral;
    class StringLiteral;
    class FunctionCall;

    // Declare a visitor interface.
    class IGenerator {
    public:
        virtual llvm::Value* generate(const AST::AddOpExpression& addition) = 0;
        virtual llvm::Value* generate(const AST::SubOpExpression& subtraction) = 0;
        virtual llvm::Value* generate(const AST::MulOpExpression& multiplication) = 0;
        virtual llvm::Value* generate(const AST::DivOpExpression& division) = 0;
        virtual llvm::IntegerType* generate(const AST::IntegerType& integer) = 0;
        virtual llvm::PointerType* generate(const AST::StringType& string) = 0;
        virtual llvm::FunctionType* generate(const AST::FunctionPrototype& prototype) = 0;
        virtual llvm::Function* generate(const AST::Function& func) = 0;
        virtual llvm::Value* generate(const AST::Statement& stmt) = 0;
        virtual llvm::Function* generate(const AST::File& file) = 0;
        virtual llvm::Value* generate(const AST::CharLiteral& literal) = 0;
        virtual llvm::Value* generate(const AST::IntegerLiteral& literal) = 0;
        virtual llvm::Value* generate(const AST::StringLiteral& literal) = 0;
        virtual llvm::Value* generate(const AST::FunctionCall& call) = 0;
    };

    class Element {
    public:
        virtual void print(llvm::raw_ostream& stream) const = 0;
    };

    class Expression : public Element {
    public:
        virtual llvm::Value* generate(IGenerator& generator) const = 0;
    };

    class BinaryOpExpression : public Expression {
    public:
        std::shared_ptr<const AST::Expression> leftExpr;
        std::shared_ptr<const AST::Expression> rightExpr;

        BinaryOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);
    };

    class AddOpExpression : public BinaryOpExpression {
    public:
        AddOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class SubOpExpression : public BinaryOpExpression {
    public:
        SubOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class MulOpExpression : public BinaryOpExpression {
    public:
        MulOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class DivOpExpression : public BinaryOpExpression {
    public:
        DivOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Type : public Element {
    public:
        virtual llvm::Type* generate(IGenerator& generator) const = 0;
    };

    class IntegerType : public Type {
    public:
        IntegerType() = default;

        llvm::IntegerType* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class StringType : public Type {
    public:
        StringType() = default;

        llvm::PointerType* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class FunctionPrototype : public Type {
    public:
        const std::vector<std::shared_ptr<const Type>> parameters;
        const std::shared_ptr<const Type> returnType;

        FunctionPrototype(const std::vector<std::shared_ptr<const Type>>& parameters, std::shared_ptr<const Type> returnType);

        llvm::FunctionType* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Function : public Element {
    public:
        const std::string name;
        std::shared_ptr<const FunctionPrototype> type;

        Function(const std::string& name, std::shared_ptr<const FunctionPrototype> type);

        llvm::Function* generate(IGenerator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Statement : public Element {
    public:
        std::shared_ptr<const Expression> expr;

        explicit Statement(std::shared_ptr<const Expression> expr);

        llvm::Value* generate(IGenerator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class File : public Element {
    public:
        const std::vector<std::shared_ptr<const Function>> funcs;
        const std::vector<std::shared_ptr<const Statement>> statements;

        File(std::vector<std::shared_ptr<const Function>> funcs,
            std::vector<std::shared_ptr<const Statement>> statements);

        llvm::Function* generate(IGenerator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class CharLiteral : public Expression {
    public:
        const char value;

        explicit CharLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class IntegerLiteral : public Expression {
    public:
        const int32_t value;

        explicit IntegerLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class StringLiteral : public Expression {
    public:
        const std::string value;

        explicit StringLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class FunctionCall : public Expression {
    public:
        const std::string callee;
        const std::vector<std::shared_ptr<const Expression>> arguments;

        FunctionCall(std::shared_ptr<const Token> callee,
            std::vector<std::shared_ptr<const Expression>> arguments);

        llvm::Value* generate(IGenerator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };
};

#endif //SANITY_AST_H