package com.sanity.compiler.lexer

import com.sanity.compiler.utils.Generator
import com.sanity.compiler.utils.GeneratorStep

data class Token(val content: String, val isStringLiteral: Boolean = false)

/**
 * Helper class used to tokenize a Generator of characters. This is a language-independent tokenizer, which can work
 * with any hypothetical lexical structure.
 */
class Tokenizer(private val generator: Generator<Char>) {
    private var currentStep: GeneratorStep<out Char> = generator.next()
    private var buffer = ""
    private var result: Token? = null

    /**
     * Gets the next value from the generator and store it in this.currentStep.
     */
    private fun next() {
        this.currentStep = generator.next()
    }

    /**
     * Executes the given lambda if there is still content to analyze and the current token is not yet complete.
     */
    private fun ifNotDone(cb: (Char) -> Unit): Tokenizer {
        if (this.currentStep !is GeneratorStep.Done && this.result == null) {
            cb((this.currentStep as GeneratorStep.Value).value)
        }

        return this
    }

    /**
     * Executes the given lambda as long as there is still content to analyze, the current token is not yet complete,
     * the callback has not returned that it is done.
     */
    private fun whileNotDone(cb: (Char) -> Boolean): Tokenizer {
        var stop = false
        while (this.currentStep !is GeneratorStep.Done && !stop && this.result == null) {
            stop = cb((this.currentStep as GeneratorStep.Value).value)
        }

        return this
    }

    /**
     * As long as the generator's first character matches the given Regex, invoke the callback.
     */
    fun repeat(regex: Regex, cb: (Tokenizer) -> Unit) = whileNotDone {
        if (regex.matches(it.toString())) {
            cb(this)
            return@whileNotDone false /* stop */
        } else {
            return@whileNotDone true /* stop */
        }
    }

    /**
     * If the generator's first character matches the given Regex, invoke the callback.
     */
    fun match(regex: Regex, cb: (Tokenizer) -> Unit) = ifNotDone {
        if (regex.matches(it.toString())) {
            cb(this)
            return@ifNotDone
        }
    }

    /**
     * Ignore the given number of characters and do not add them to the next token.
     */
    fun ignore(count: Int = 1): Tokenizer {
        if (count <= 0) return this

        ifNotDone {
            next()
        }

        this.ignore(count - 1)
        return this
    }

    /**
     * Consume the given number of characters by adding them to the next token.
     */
    fun consume(count: Int = 1): Tokenizer {
        if (count <= 0) return this

        ifNotDone {
            this.buffer += it
            next()
        }

        this.consume(count - 1)
        return this
    }

    /**
     * Stop parsing charaters on this iteration and save the current state for use as the next retrieved token.
     * After #tokenize() is called, this state is cleared and processing continues.
     */
    fun done(isStringLiteral: Boolean = false): Tokenizer {
        if (this.result != null) return this // Already stored a token previously

        if (this.buffer == "") this.result = null
        else {
            this.result = Token(this.buffer, isStringLiteral)
            this.buffer = ""
        }

        return this
    }

    /**
     * Extract the currently captured token and reset the state of the tokenizer to read the next token.
     */
    fun tokenize(): GeneratorStep<Token> {
        return if (this.result == null) GeneratorStep.Done()
        else {
            val result = this.result
            this.result = null
            GeneratorStep.Value(result!!)
        }
    }
}