package com.sanity.compiler

import com.xenomachina.argparser.ArgParser

/**
 * Command line argument parser for the compiler binary.
 */
class CompilerArgs(parser: ArgParser) {
    val inputFile by parser.positional("INPUT_FILE", help = "The input file to compile.")

    val outputFile by parser.positional("OUTPUT_FILE", help = "The output location to write the compiled binary to")
}