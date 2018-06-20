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

    class FunctionCall : public Expression {
    public:
        const std::string callee;
        std::shared_ptr<const Expression> argument;

        FunctionCall(std::shared_ptr<const Token> callee, std::shared_ptr<const Expression> argument);

        llvm::Value* generate(Generator& generator) const override;

        void print(llvm::raw_ostream& stream) const override;
    };
};

#endif //SANITY_AST_H