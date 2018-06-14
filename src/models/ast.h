#ifndef SANITY_AST_H
#define SANITY_AST_H

#include <string>
#include <memory>
#include "token.h"

/**
 * Holds all the different class representing the different elements of an abstract syntax tree representing Sanity.
 */
namespace AST {
    class Element {
    public:
        virtual void print(std::ostream& stream) const = 0;
    };

    class Expression : public Element { };

    class Statement : public Element {
    public:
        std::shared_ptr<const Expression> expr;

        explicit Statement(std::shared_ptr<const Expression> expr);

        void print(std::ostream& stream) const override;
    };

    class Block : public Element {
    public:
        const std::vector<std::shared_ptr<const Statement>> statements;

        explicit Block(std::vector<std::shared_ptr<const Statement>> statements);

        void print(std::ostream& stream) const override;
    };

    class Identifier : public Expression {
    public:
        const std::string name;

        explicit Identifier(std::shared_ptr<const Token> name);

        void print(std::ostream& stream) const override;
    };

    class CharLiteral : public Expression {
    public:
        const char value;

        explicit CharLiteral(std::shared_ptr<const Token> value);

        void print(std::ostream& stream) const override;
    };

    class FunctionCall : public Expression {
    public:
        std::shared_ptr<const Identifier> callee;
        std::shared_ptr<const Expression> argument;

        FunctionCall(std::shared_ptr<const Identifier> callee, std::shared_ptr<const Expression> argument);

        void print(std::ostream& stream) const override;
    };
};

#endif //SANITY_AST_H