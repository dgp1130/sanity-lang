package com.sanity.compiler.utils

import java.io.File
import kotlin.coroutines.experimental.buildSequence

/**
 * General file utilities for the compiler.
 */
object FileUtils {
    private const val EOF = -1

    /**
     * Converts a file into a sequence of its characters.
     */
    fun generateFileChars(file: File): Sequence<Char> {
        return buildSequence {
            file.reader().use {
                while (true) {
                    val i: Int = it.read()
                    if (i == EOF) return@buildSequence
                    yield(i.toChar())
                }
            }
        }
    }
}