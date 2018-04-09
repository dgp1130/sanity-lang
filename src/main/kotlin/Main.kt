@file:JvmName("Main")

package com.sanity.compiler

import com.xenomachina.argparser.ArgParser
import com.xenomachina.argparser.mainBody

/**
 * Main function for the compiler binary.
 */
fun main(arguments: Array<String>) = mainBody {
    val args : CompilerArgs = ArgParser(arguments).parseInto(::CompilerArgs)

}