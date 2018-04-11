package com.sanity.compiler.lexer

import kotlin.coroutines.experimental.buildSequence

/**
 * The lexer module responsible for perform lexical analysis on a Sanity program.
 */
object Lexer {
    /**
     * Given a sequence of characters written in Sanity, perform lexical analysis to convert them into a sequence of
     * Tokens.
     */
    fun tokenize(charGenerator: Sequence<Char>): Sequence<Token> {
        val tokenizer = Tokenizer(charGenerator)
        return yieldWhileNotNull {
            tokenizer
                // Ignore all whitespace
                .repeat(Regex("[ \n]"), { it.ignore() })
                // Match any alphanumeric whole words
                .match(Regex("[a-zA-Z0-9]"), { it.consumeWhile(Regex("[a-zA-Z0-9]")).done() })
                // Match any non-alphanumeric characters as individual tokens
                .consume().done()
            .tokenize()
        }
    }

    /**
     * Invoke the given callback and yield the result into a sequence as long as that result is not null.
     */
    private fun <T> yieldWhileNotNull(cb: () -> T?): Sequence<T> {
        return buildSequence {
            while (true) {
                val value = cb()
                if (value === null) return@buildSequence
                yield(value!!)
            }
        }
    }
}