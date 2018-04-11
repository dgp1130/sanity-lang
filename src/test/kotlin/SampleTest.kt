package com.sanity.compiler.testing

import assertk.assert
import assertk.assertions.isEqualTo
import org.jetbrains.spek.api.Spek
import org.jetbrains.spek.api.dsl.describe
import org.jetbrains.spek.api.dsl.it

object SampleTest : Spek({
    describe("A sample test") {
        it("should add integers") {
            assert(1 + 2).isEqualTo(3)
        }
    }
})