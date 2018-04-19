package com.sanity.compiler.lexer

data class Token(val content: String)

/**
 * Helper class used to tokenize a sequence of characters. This is a language-independent tokenizer, which can work with
 * any hypothetical lexical struture.
 */
class Tokenizer(private var generator: Sequence<Char>) {
    private data class Sequencer(val generator: Sequence<Char>, val buffer: String = "")

    private var buffer: String = ""
    private var result: String? = null

    private fun contentToParse(): Boolean {
        return result == null && generator.count() >= 1
    }

    /**
     * If there is content left to analyze, invoke the given function and update local state with its results.
     */
    private fun ifContent(cb: (Sequence<Char>) -> Sequencer): Tokenizer {
        if (contentToParse()) {
            val sequencer = cb(generator)
            generator = sequencer.generator
            buffer += sequencer.buffer
        }

        return this
    }

    /**
     * While there is content left to analyze and the given predicate passes, then invoke the given function and update
     * local state with its results.
     */
    private fun whileContent(condition: (Sequence<Char>) -> Boolean, cb: (Sequence<Char>) -> Sequencer): Tokenizer {
        while (contentToParse() && condition(generator)) {
            val sequencer = cb(generator)
            generator = sequencer.generator
            buffer += sequencer.buffer
        }

        return this
    }

    /**
     * If the current character matches the given Regex, invoke the provided callback.
     */
    fun match(matcher: Regex, cb: (Tokenizer) -> Unit): Tokenizer {
        if (contentToParse() && matcher.matches(generator.first().toString())) {
            cb(this)
        }

        return this
    }

    /**
     * While the current character matches the given Regex, invoke the provided callback.
     */
    fun repeat(matcher: Regex, cb: (Tokenizer) -> Unit): Tokenizer {
        while (contentToParse() && matcher.matches(generator.first().toString())) {
            cb(this)
        }

        return this
    }

    /**
     * Consume the current character by including it in the next token and move on to the next character.
     */
    fun consume(): Tokenizer = ifContent {
        Sequencer(it.drop(1), it.first().toString())
    }

    /**
     * As long as the current character matches the provided regex, consume it by including it in the next token and
     * move on to the next character.
     */
    fun consumeWhile(match: Regex): Tokenizer = whileContent({ match.matches(it.first().toString()) }) {
        Sequencer(it.drop(1), it.first().toString())
    }

    /**
     * Skip the current character by NOT including it in the next token and move on to the next character.
     */
    fun ignore(): Tokenizer = ifContent {
        Sequencer(it.drop(1))
    }

    /**
     * Signifies that the tokenizer has completed its next token. Save the current value to be returned when it is
     * requested and block all further processing until it is extracted.
     */
    fun done(): Tokenizer {
        if (result == null && buffer != "") {
            result = buffer
            buffer = ""
        }

        return this
    }

    /**
     * Return the token parsed and reset the Tokenizer for the next iteration.
     */
    fun tokenize(): Token? {
        if (result != null) {
            val token = Token(result!!)
            result = null
            return token
        } else {
            if (generator.count() == 0) return null
            else throw AssertionError("Attempted to tokenize with nothing in the buffer.")
        }
    }
}