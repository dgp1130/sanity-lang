@file:JvmName("Main")

package com.sanity.compiler

import com.sanity.compiler.lexer.Lexer
import com.sanity.compiler.utils.FileUtils
import com.xenomachina.argparser.ArgParser
import com.xenomachina.argparser.mainBody
import java.io.File

/**
 * Main function for the compiler binary.
 */
fun main(arguments: Array<String>) = mainBody {
    val args : CompilerArgs = ArgParser(arguments).parseInto(::CompilerArgs)

    // Parse the input file into a sequence of characters
    val charGenerator = FileUtils.generateFileChars(File(args.inputFile))

    // Tokenize the input characters
    val tokenGenerator = Lexer.tokenize(charGenerator)

    // Print each token
    tokenGenerator.forEach {
        println(it)
    }
}