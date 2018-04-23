package com.sanity.compiler.utils

import java.io.File

/**
 * General file utilities for the compiler.
 */
object FileUtils {
    private const val EOF = -1

    /**
     * Converts a file into a generator of its characters.
     */
    fun generateFileChars(file: File): Generator<Char> {
        val reader = file.reader()
        return Generator {
            val i: Int = reader.read()
            if (i == EOF) {
                reader.close()
                return@Generator GeneratorStep.Done<Char>()
            }
            return@Generator GeneratorStep.Value(i.toChar())
        }
    }
}