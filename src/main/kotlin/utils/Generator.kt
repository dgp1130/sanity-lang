package com.sanity.compiler.utils

/**
 * An algebraic type representing whether this iteration of the generator yields a value or the completion of the
 * generator.
 */
sealed class GeneratorStep<T> {
    data class Value<T>(val value: T): GeneratorStep<T>()
    class Done<T>: GeneratorStep<T>()
}

/**
 * A class representing a JavaScript-like generator. It is provided a function which when called multiple times, will
 * return a GeneratorStep.Value<T> with the value generated, or a GeneratorStep.Done which signals that no more values
 * will be returned.
 *
 * Call #next() to get the values in order as returned from the generator function. Each value can only be retrieved
 * once, this class does not cache any results.
 */
class Generator<out T> constructor(private val generator: () -> GeneratorStep<T>) {
    private var lastStep: GeneratorStep<T>? = null

    /**
     * Get the next GeneratorStep from the generator and return it.
     */
    fun next(): GeneratorStep<out T> {
        // Assert that the caller has not called the Generator one too many times
        if (this.lastStep != null && this.lastStep is GeneratorStep.Done) {
            throw AssertionError("Tried to read a generator after it finished.")
        }

        this.lastStep = generator()
        return this.lastStep!!
    }

    /**
     * Iterate over the entire Generator and call the given function.
     */
    fun forEach(cb: (T) -> Unit) {
        do {
            val step = next()
            if (step is GeneratorStep.Value) {
                cb(step.value)
            }
        } while (step !is GeneratorStep.Done)
    }

    fun toList(): List<T> {
        val list = mutableListOf<T>()
        this.forEach {
            list.add(it)
        }
        return list
    }
}