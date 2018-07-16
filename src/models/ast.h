#ifndef SANITY_AST_H
#define SANITY_AST_H

#include <string>
#include <memory>
#include <vector>
#include "token.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

// Forward declare Generator class to avoid a circular-dependency.
class Generator;

/**
 * Holds all the different class representing the different elements of an abstract syntax tree representing Sanity.
 */
namespace AST {
    class Element {
    public:
        virtual void print(llvm::raw_ostream& stream) const = 0;
    };

    class Expression : public Element {
    public:
        virtual llvm::Value* generate(Generator& generator) const = 0;
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

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class SubOpExpression : public BinaryOpExpression {
    public:
        SubOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class MulOpExpression : public BinaryOpExpression {
    public:
        MulOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class DivOpExpression : public BinaryOpExpression {
    public:
        DivOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
                std::shared_ptr<const AST::Expression> rightExpr);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Type : public Element {
    public:
        virtual llvm::Type* generate(Generator& generator) const = 0;
    };

    class IntegerType : public Type {
    public:
        IntegerType() = default;

        llvm::IntegerType* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class StringType : public Type {
    public:
        StringType() = default;

        llvm::PointerType* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class FunctionPrototype : public Type {
    public:
        const std::vector<std::shared_ptr<const Type>> parameters;
        const std::shared_ptr<const Type> returnType;

        FunctionPrototype(const std::vector<std::shared_ptr<const Type>>& parameters, std::shared_ptr<const Type> returnType);

        llvm::FunctionType* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Function : public Element {
    public:
        const std::string name;
        std::shared_ptr<const FunctionPrototype> type;

        Function(const std::string& name, std::shared_ptr<const FunctionPrototype> type);

        llvm::Function* generate(Generator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class Statement : public Element {
    public:
        std::shared_ptr<const Expression> expr;

        explicit Statement(std::shared_ptr<const Expression> expr);

        llvm::Value* generate(Generator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class File : public Element {
    public:
        const std::vector<std::shared_ptr<const Function>> funcs;
        const std::vector<std::shared_ptr<const Statement>> statements;

        File(std::vector<std::shared_ptr<const Function>> funcs,
            std::vector<std::shared_ptr<const Statement>> statements);

        llvm::Function* generate(Generator& generator) const;

        void print(llvm::raw_ostream& stream) const override;
    };

    class CharLiteral : public Expression {
    public:
        const char value;

        explicit CharLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class IntegerLiteral : public Expression {
    public:
        const int32_t value;

        explicit IntegerLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class StringLiteral : public Expression {
    public:
        const std::string value;

        explicit StringLiteral(std::shared_ptr<const Token> value);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };

    class FunctionCall : public Expression {
    public:
        const std::string callee;
        const std::vector<std::shared_ptr<const Expression>> arguments;

        FunctionCall(std::shared_ptr<const Token> callee,
            std::vector<std::shared_ptr<const Expression>> arguments);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };
};

#endif //SANITY_AST_H