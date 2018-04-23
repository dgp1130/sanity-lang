package com.sanity.compiler.testing

import assertk.assert
import assertk.assertions.isEqualTo
import assertk.tableOf
import com.sanity.compiler.lexer.Lexer
import com.sanity.compiler.lexer.Token
import com.sanity.compiler.utils.Generator
import com.sanity.compiler.utils.GeneratorStep
import org.jetbrains.spek.api.Spek
import org.jetbrains.spek.api.dsl.describe
import org.jetbrains.spek.api.dsl.it

/**
 * Helper function to convert a string into a sequence of its character so it can be easily loaded into the Lexer
 * module.
 */
private fun generate(input: String): Generator<Char> {
    var index = 0
    return Generator {
        return@Generator if (index >= input.length) GeneratorStep.Done<Char>()
        else GeneratorStep.Value(input[index++])
    }
}

object LexerTest : Spek({
    describe("The Lexer module") {
        it("should tokenize identifiers") {
            val tokens = Lexer.tokenize(generate("function test hello")).toList()
            assert(tokens).isEqualTo(listOf(Token("function"), Token("test"), Token("hello")))
        }

        it("should tokenize non-alphanumeric characters") {
            val tokens = Lexer.tokenize(generate("() {} -")).toList()
            assert(tokens).isEqualTo(listOf(Token("("), Token(")"), Token("{"), Token("}"), Token("-")))
        }

        it("should tokenize string literals") {
            val tokens = Lexer.tokenize(generate("\"Hello\" \"This is a string!\"")).toList()
            assert(tokens).isEqualTo(listOf(
                Token("Hello", isStringLiteral = true),
                Token("This is a string!", isStringLiteral = true)
            ))
        }
    }
})