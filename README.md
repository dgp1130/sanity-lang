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
functional. Current plans are to compile it to the Java Virtual Machine (JVM) and
execute it on that platform. This would allow it to be run on just about any
machine without requiring me to generate assembly for every assembler on the
market.

## Feature List

This is the list of random features I'd like to try out. Note that because the
language syntax and how the features work together is not completely finalized, the
code snippets I use to solve each particular issue do not have a consistent syntax.
Each is simply meant to illustrate a particular problem and how it can be addressed.

TODO: Elaborate and add.

### Simplified Constructors

Constructors are weird. Most object-oriented programming languages turn themselves
inside out in order to get something that is semi-reasonable. Consider the
following Java example:

```java
public class SuperModel {
    private final int foo;
    
    public SuperModel(final int foo) {
        this.foo = foo;
    }
}

public class Model extends SuperModel {
    private final int bar;
    
    public Model(final int halfFoo, final int bar) {
        super(halfFoo * 2);
        this.bar = bar;
    }
}

public class Elsewhere {
    public static void doSomething() {
        final Model model = new Model(1, 2);
    }
}
```

For starters, the keyword `new` must be used to instantiate a class, which means
that the impelementation detail of constructing a class has leaked out of the
class. If `Model` or `SuperModel` wanted to implement a cache which reuses
previously created objects, they can't do it without breaking `Elsewhere`.

There are also a number of things a contructor can't do:

* Constructors cannot return `null`.
* Constructors cannot be asynchronous.
* Constructors cannot return a subclass.
* Constructors cannot return an already existing object.

Factories solve a lot of these problems, but factories have their own issues. They
don't play well with subclassing and inheritence in general, because subclasses
cannot extend an object returned by another method. Factories also don't work well
with frameworks which often need hooks into constructors or need to own them entirely
and have to implement their own lifecycle methods instead.

Lastly, consider all the syntactical exceptions programming languages make to support
this system. Looking specifically at the following example. I'm picking on Java here,
but many of these apply to most general purpose object-oriented languages.

```java
public class Model extends SuperModel {
    private final int bar;
    
    public Model(final int halfFoo, final int bar) {
        super(halfFoo * 2);
        this.bar = bar;
    }
}
```

* Constructors do not have a declared return type, it is implied to be the same class.
* Constructors *must* have the same name as their class. When else does the name of
something affect its behavior?
* `super()` *must* be the first line executed in the block. When else does a particular
statement have to be the first item of a block of code?
* No code can come before `super()`, but you can execute inlined expressions. That
means that the following is not ok, despite the fact that it compiles to the exact same
thing!

```java
public Model(final int halfFoo, final int bar) {
    final int foo = halfFoo * 2;
    super(foo);
}
```

* Normally you cannot assign to a `final` variable, but in constructors you can. However
this can only be done if the compiler is sure that the variable is only assigned once.
`if` statements often need to be turned into ternary operators to get things to compile.
* Constructors do not require a `return;` statement, because a `return this;` is implied.
However, if you wish to return early, you should use `return;` without `this`. Wat?

Many languages fix specific parts of these issues. Dart has named constructors. JavaScript
allows other statements before a `super()` call provided they don't access `this`. C# uses
a `: base()` syntax to pull the `super()` call out of the body of the function, which is
more intuitive. Kotlin omits the `new` operator. These all improves things slightly, but
don't change the underlying functionality of constructors and the inheritence hierarchy
model, particularly in the direct coupling of a subclass' construction to that of its
superclass. I am asserting that the this core concept of constructors is flawed and can be
done better. **Constructors are a fundamentally broken mechanism for instantiating objects.**
I believe this can be done better in a far simpler and more flexible fashion.

My solution to this problem is to decouple subclass construction from superclass
construction. With this one change the overall model of constructors can be completely
reworked to solve a lot of these issues. Lets start off simple without worry about inheritence.
Consider the following Java-like snippet:

```java
public class Model {
    int foo;
    public static Model create() {
        return new Model();
    }
}
```

In this example, there are no user-defined constructors (a method named `Model` would be
just that, a method which happened to have the name `Model`). The `new` keyword is still
used to indicate that object creation is happening, but it is actually closer to a C-style
`malloc` call. It really just allocates the memory necessary for `Model` and initializes
its instance variables to their defaults (`foo` is `0`). We can also restrict the `new`
keyword to **only** apply to the class it is used in. Hence the following would **not**
compile:

```java
public class Elsewhere {
    public static Model createModel() {
        return new Model(); // Err: `new Model` cannot be used outside the `Model` class.
    }
}
```

All the logic that traditionally goes inside constructors can be handled by this factory
instead. What about `final` variables how might they look?

```java
public class Model {
    final int myFoo;
    final int myBar;
    public static Model create(final int foo, final int bar) {
        return new Model(
            myFoo = foo,
            myBar = bar,
        );
    }
}
```

Here, `final` variables are included as named parameters to the `new` call and cannot be
set after this. This is a much cleaner implementation of the system Java presents, because
if-statements can be handled at the factory level. Consider the following snippet which
creates a server with the cache enabled unless development mode is enabled or the cache
is explicitly requested as disabled:

```java
public class Server {
    final boolean cacheEnabled;
    public static Server create(final boolean devModeEnabled, final boolean cacheDisabled) {
        if (cacheDisabled) {
            return new Server(cacheEnabled = false);
        } else {
            if (devModelEnabled) {
                return new Server(cacheEnabled = false);
            } else {
                return new Server(cacheEnabled = true);
            }
        }
    }
}
```

Notice how nested if statements work just fine with `final` variables. Running this same kind
of logic in a constructor would require nested ternary operations which can complicated and
difficult to read.

### Proxies

### First Class Mixins

### Variables Read-Only by Default

### Context

### Compile-Time Execution

### Elm-style Streams

### Testing Support
