# Sanity

A general purpose programming language for the purpose of evaluating some
random ideas I had about ways to improve existing languages.

Every programming language has its faults and as generations of languages progress
they slowly improve. Throughout my work I've come across a few things I think
could be done better. This language serves the purpose of trying out many of these
ideas to discover their practicality and usefullness. Most are probably trash, but
maybe one or two will prove to be an improvement on modern programming paradigms.
If anything works out particularly well, maybe it could be included in the next
big programming language!

## Overview

Sanity is a compiled strongly-typed language somewhere between object-oriented and
functional. Current plans are to compile it with LLVM as a backend. This would
allow it to be run on just about any machine without requiring me to generate
assembly for every assembler on the market and is the standard tool for this kind
of project.

## Feature List

This is the list of random features I'd like to try out. Note that because the
language syntax and how the features work together is not completely finalized, the
code snippets I use to solve each particular issue do not have a consistent syntax.
Each is simply meant to illustrate a particular problem and how it can be addressed.

TODO: Elaborate and add.

### Object-oriented vs Functional

### Constructors

As they exist in modern programming languages, constructors make surprisingly little
sense. See [my rant about constructors](doc/constructors.md) for the full details on
why existing constructor models are terrible and how they can be done better.

### Proxies

### Mixins

### Type System

#### Typesafe Anonymous Objects

#### First-Class Generics

#### No Nulls

### Context

### Compile-Time Execution

### Elm-style Streams

### Testing Support

### Properties

### Events

#### Deep Notify

### Miscellaneous

#### Variables Read-Only by Default

#### this Always Lexically Bound

