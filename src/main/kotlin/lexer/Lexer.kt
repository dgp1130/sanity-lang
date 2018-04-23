package com.sanity.compiler.lexer

import com.sanity.compiler.utils.Generator

/**
 * The lexer module responsible for perform lexical analysis on a Sanity program.
 */
object Lexer {
    /**
     * Given a sequence of characters written in Sanity, perform lexical analysis to convert them into a sequence of
     * Tokens.
     */
    fun tokenize(charGenerator: Generator<Char>): Generator<Token> {
        val tokenizer = Tokenizer(charGenerator)

        return Generator {
            return@Generator tokenizer
                // Ignore all whitespace
                .repeat(Regex("[ \n\t]"), { it.ignore() })
                // Match any alphanumeric whole words
                .match(Regex("[a-zA-Z_]"), { it.repeat(Regex("[a-zA-Z0-9_]"), { it.consume() }).done() })
                // Match a string literal
                .match(Regex("[\"]"), {
                    it.ignore(/* open quote */)
                    it.repeat(Regex("[^\"]"), { it.consume() })
                    it.ignore(/* closing quote */)
                    it.done(isStringLiteral = true)
                })
                // Match any non-alphanumeric characters as individual tokens
                .consume().done()
            .tokenize()
        }
    }
}