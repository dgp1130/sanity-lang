package com.sanity.compiler.testing

import assertk.assert
import assertk.assertions.isEqualTo
import assertk.tableOf
import com.sanity.compiler.lexer.Lexer
import com.sanity.compiler.lexer.Token
import org.jetbrains.spek.api.Spek
import org.jetbrains.spek.api.dsl.describe
import org.jetbrains.spek.api.dsl.it
import kotlin.coroutines.experimental.buildSequence

/**
 * Helper function to convert a string into a sequence of its character so it can be easily loaded into the Lexer
 * module.
 */
private fun sequenize(input: String): Sequence<Char> {
    return buildSequence {
        input.forEach {
            yield(it)
        }
    }
}

object LexerTest : Spek({
    describe("The Lexer module") {
        it("should tokenize valid programs") {
            tableOf("input", "output")
                .row("function test() {}", listOf("function", "test", "(", ")", "{", "}"))
                .row("foo bar 123 456 !!", listOf("foo", "bar", "123", "456", "!", "!"))
                .forAll { input, output ->
                    val expected = output.map { Token(it) }
                    val actual = Lexer.tokenize(sequenize(input)).toList()

                    assert(actual).isEqualTo(expected)
                }
        }
    }
})