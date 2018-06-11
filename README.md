# Sanity

A general purpose programming language for the purpose of evaluating some
random ideas I had about ways to improve existing languages.

Every programming language has its faults and as generations of languages progress
they slowly improve. Throughout my work I've come across a few things I think
could be done better. This language serves the purpose of trying out many of these
ideas to discover their practicality and usefulness. Most are probably trash, but
maybe one or two will prove to be an improvement on modern programming paradigms.
If anything works out particularly well, maybe it could be included in the next
big programming language!

## Overview

Sanity is a compiled strongly-typed language somewhere between object-oriented and
functional. Current plans are to compile it with LLVM as a backend. This would
allow it to be run on just about any machine without requiring me to generate
assembly for every assembler on the market and is the standard tool for this kind
of project.

## Philosophy

Sanity is a collage of random ideas I had about ways to improve modern programming
languages. As a result, it is somewhat lacking of a core philosophy which drives its
design. However, if it did have one, it would prioritize code maintenance and
intuitiveness above all else. Bugs tend to come from unintuitive or confusing code
when changes to one system have unexpected effects on other systems. Sanity
prioritizes features which enable code changes to be easy to reason about and
minimize the unexpected impact they can have. It also tries to be as intuitive and
straightforward as possible, following the principle of least surprise to avoid
potential bugs and issues from developers who may not be experts in the language.

This is not to detract from other important features of a programming language, such
as performance, it simply places a stronger emphasis on maintainability and
predictability over those other features. Sanity also does not attempt to make a
completely "safe" language, where it is impossible for one to shoot themselves in the
foot. Such a language is impossible and Sanity does not attempt to solve human
stupidity. Rather, it provides features which allow a competent yet non-genius
developer to write maintainable, stable code which can survive many unexpected changes
and modifications with minimal bugs. 

## Feature List

This is the list of random features I'd like to try out. Note that because the
language syntax and how the features work together is not completely finalized, the
code snippets I use to solve each particular issue do not have a consistent syntax.
Each is simply meant to illustrate a particular problem and how it can be addressed.

TODO: Elaborate and add.

### Object-oriented vs Functional

Whenever a new language comes up, the first question anyone asks is whether it is
object-oriented or functional. Many individuals feel very strongly that one is better
than the other. However, any general-purpose programming language such as Sanity needs
to support both object-oriented designs and functional designs. Certain problems are
better solved by one than the other, and neither is a solution to all of life's problems.
In more specific domains, supporting only one of these may be appropriate, but in a
general-purpose domain, both are necessary.

Object-oriented code can be very useful to encapsulate state in a logical abstraction
which is easy to reason about. Inheritance also allows behavior to be overridden in a
very useful fashion. However, it will not solve all problems so easily, as additional
state is often the cause of many bugs and unanticipated changes can lead to unintuitive
code.

Pure-functional code largely eliminates state to provide a system which can be easier to
reason about. It can work very well for transforming data through a pipeline of
computations. However, not every problem is well suited to this paradigm, as a complete
lack of state and side effects can also make many problems harder to solve.

As a result, a general-purpose language must support both object-oriented and functional
paradigms as both are useful design patterns to utilize when solving various different
problems. Many general-purpose languages started on one end of the spectrum but tend to
incorporate features from the other end as they have been found to be very useful. Java
for instance was very object-oriented, but later introduced lambdas, closures, and other
functional features. JavaScript was more on the functional side initially, but later
introduced proper classes because they are very useful for solving many problems.

Because of this, Sanity will incorporate features from both the object-oriented and
functional paradigms. It will have both classes and lambdas, and they will be
well-supported enough that either could be considered "first-class". Sanity may find
itself leaning towards one end of the spectrum or the other, but conceptually it can and
should support both paradigms because they can be equally useful in many domains.

### Constructors

As they exist in modern programming languages, constructors make surprisingly little
sense. See [my rant about constructors](doc/constructors.md) for the full details on
why existing constructor models are terrible and how they can be done better.

### Proxies

### Mixins

### Type System

Sanity is a strongly-typed compiled language. It's syntax is built on declaring the name
of a variable first, followed by the type delimited by a colon (similar to TypeScript or
Kotlin). I think there's a name for this kind of declaration syntax, but for the life of
me I can't remember it. This format is used not to solve any particular problem in existing
programming languages, since syntax is one of the least important aspects of a language.
Rather it is this way because of a theory I have about how humans think.

Whenever I find myself using a C-like language, I say to myself "I need a counter." so I
type `counter`. Then I move the cursor to the start and say "Oh, this counter needs to be
an integer." and add an `int` to the front to get `int counter`. Conceptually, I knew the
concept that I wanted to express, and understood it as a `counter`. I then decided, that
the appropriate type for this counter was an `int`. I cannot think of any way in which my
brain would say "I need an integer." and *then* say "Oh, and that integer should be called
`counter`." Of course, I have no real data to back up this assertion, and other individuals
may think differently. However, I'm willing to bet that my thought process is more likely
than the alternative. As a result, Sanity puts the variable name first, and its type second.
This is intended to make writing the language more fluent because it will align with how the
human brain expresses these concepts. If this were a language that was intended for
full-scale use, I might put more effort into this. However, because it is an experimental
language, I can simply say: "It's my language, so I'm gonna do it the way I want to."

The general syntax would look something like the following. Since this language is still so
early in its development this may not be entirely thought out or necessary accurate of the
final product.

The `var` keyword denotes a variable declaration which can be modified. The `let` keyword
denotes a constant declaration which cannot be modified.

```
var counter: int = 0;
counter = 1;

let counter2: int = 0;
counter2 = 1; // ERR: Cannot reassign a read-only value defined with `let`
```

The type can be inferred by using the `:=` operator, which allows the developer to omit
the type and let it be inferred from the value assigned to it.

```
var counter := 1; // Counter implicitly has the type int.
counter = "Hello"; // ERR: Cannot assign a string to an integer variable.
```

Note, that inferred types are not dynamic, their type cannot change over time like
JavaScript or C#'s `dynamic`. Classes can simply be typed by their name while lambdas use
a more complex syntax.

```
let myCar: Car = new Car("Mitsubishi Eclipse");

// Map integers to strings in a one-line function with an implicit return.
let mapper: (number: int) -> string = number.toString();

// Multi-line lambdas can use a block syntax.
let complexMapper: (number: int) -> string = {
    let incremented = number + 1;
    return incremented.toString();
};

// Lambda declarations without associated definitions may omit parameter names.
function map(list: List<int>, mapper: (int) -> string) {
    ...
}
```

Anonymous objects can be used for named parameters to functions and to group multiple
sets of data into a single object using JavaScript-like destructure syntax. See
[Type-safe Anonymous Objects](#type-safe-anonymous-objects) for more details.

```
let myCar: Car = new Car();
let me: Person = new Person();

// A complex anonymous object type can be declared inline.
let roadtrip: {car: Car, driver: Person} = {car = myCar, driver = me};

// That can get verbose, so the same type can be inferred from the value.
let secondRoadtrip := {car = myCar, driver = me};

// Anonymous object types can be used in function parameters.
function startTrip(trip: {car: Car, driver: Person}) { ... }
startTrip(roadtrip);

// Anonymous object types provide a named argument solution, where the values can be
// provided individually.
let friendsCar: Car = new Car();
let friend: PErson = new Person();
startTrip({car = friendsCar, driver = friend});
```

For complex lambdas or anonymous objects, it can be annoying to refer to them by listing
out the type each time. This can be alleviated through a type alias.

```
alias Mapper = (int) -> string;
function map(list: List<int>, mapper: Mapper) { ... }

alias Roadtrip = {car: Car, driver: Person};
let myCar: Car = new Car();
let me: Person = new Person();
let roadtrip: Roadtrip = {car = myCar, driver = me};
```

Anonymous objects provide a lightweight and simple solution to composing multiple pieces
of data into a single entity. Sanity will also support algebraic types (notably the OR)
to enable a single type to represent multiple possible values. See [No Nulls](#no-nulls)
for details on this.

#### Type-safe Anonymous Objects

In strongly typed object-oriented languages, classes and structs tend to be explicitly
defined. This can be quite annoying when it comes to throwaway objects which exist for
a short period of time or only try to connect two systems together. A good example of
this is to try returning two items from a single function. Consider the following Java
example.

```java
class Result {
    public final Stuff stuff;
    public final OtherStuff otherStuff;
    
    public Result(final Stuff stuff, final OtherStuff otherStuff) {
        this.stuff = stuff;
        this.otherStuff = otherStuff;
    }
}

class Elsewhere {
    public static Result getResult() {
        final Stuff stuff = getStuff();
        final OtherStuff otherStuff = getOtherStuff();
        
        return new Result(stuff, otherStuff);
    }
}
```

It is incredibly verbose and annoying to define a separate class just to hold a couple
pieces of data. It can be difficult to name this class, because it often doesn't have a
strong abstraction model which it represents. A more brief way around this is to use a
`Pair` object, though Java does not have a standard `Pair` or `Tuple` class in its
standard library (JavaFX doesn't count). If it did, it would look something like this:

```java
class Elsewhere {
    public static Pair<Stuff, OtherStuff> getResult() {
        final Stuff stuff = getStuff();
        final OtherStuff otherStuff = getOtherStuff();
        
        return new Pair<>(stuff, otherStuff);
    }
    
    public static void useResult() {
        final Pair<Stuff, OtherStuff> result = getResult();
        
        final Stuff stuff = result.first;
        final OtherStuff otherStuff = result.second;
    }
}
```

This drops the need for the explicitly defined class, which is nice, but the problem with
this is that the `Pair` class looses the relationship between the two values. Which one is
first and which one is second? If they happen to have the same type, this can be
particularly tricky and easy to get wrong. If returning many different values, then a
`Tuple` class might be used, however generics are not quite strong enough to represent an
unlimited list of distinct types (see [First Class Generics](#first-class-generics) for more
on that). C# for instance,
[only goes up to an 8-tuple](https://msdn.microsoft.com/en-us/library/dd383325(v=vs.110).aspx).
Beyond that, you must actually nest `Tuple` objects in order to get more than 8 values.

JavaScript actually solves this quite elegantly. If you want to return multiple objects, you
can simply make an object literal containing those values. ES6 destructuring embraces this
concept and provides even more brevity.

```javascript
function getResult() {
    const stuff = getStuff();
    const otherStuff = getOtherStuff();
    
    // Return an object mapping the string "stuff" to the `stuff` value
    // and the string "otherStuff" to the `otherStuff` value.
    return {stuff, otherStuff};
}

function useResult() {
    // Destructure the result by looking up the strings "stuff" and "otherStuff"
    // and storing their values into variables of the same names.
    const {stuff, otherStuff} = getResult();
}
```

This format is nice because it has practically no boilerplate, no need for a complex
intermediate representation, and no need to name or abstract that representation to attempt
to provide more meaning than is actually present. Of course, JavaScript is not type-safe and
there is no guarantee that the values destructured are actually present. Sanity aims to provide
a similar system in a type-safe manner. This might look like (tentative syntax):

```
function getResult() {
    var stuff: Stuff = getStuff();
    var otherStuff: OtherStuff = getOtherStuff();
    
    // Construct an anonymous object containing these two values.
    return {stuff, otherStuff};
}

function useResult() {
    var {stuff: Stuff, otherStuff: OtherStuff} = getResult();
}
```

The anonymous object `{stuff, otherStuff}` is effectively equivalent to the explicitly defined
Java class mentioned earlier. This defines a class which has two fields `stuff` and `otherStuff`
of the given types. Those fields are `final` (or whatever Sanity's equivalent of `final` becomes)
and are simply accessed by name. This type can then be easily converted and passed around making
a good use for named parameters. For instance:

```
function doStuff(param1: int, param2: int, {stuff: Stuff}) {
    doSomethingWithStuff(stuff);
}

function useResult() {
    // result is implicitly the anonymous class which contains both values.
    // The type is explicitly listed for clarity, Sanity will support type inference to make this
    // less verbose and annoying.
    var result: {stuff: Stuff, otherStuff: OtherStuff} = getResult();
    
    // result can be casted from a {stuff: Stuff, otherStuff: OtherStuff} -> {stuff: Stuff}.
    doStuff(0, 1, result);
    
    // Named arguments can also be passed in directly.
    var myStuff: Stuff = result.stuff;
    doStuff(0, 1, {stuff: myStuff});
    
    // Because these are compiled classes, they are type safe.
    print(result.notStuff); // ERR: notStuff does not exist on type: {stuff: Stuff, otherStuff: OtherStuff}.
    doStuff(0, 1, {notStuff: 0}); // ERR: notStuff does not exist on type: {stuff: Stuff}.
}
```

Type-safe anonymous objects provide a means of elegantly creating an intermediate data format which
is clear and obvious without any unnecessary boilerplate. It also provides a simple and easy means
of creating named arguments to functions. Data can be easily stored and extracted from these objects
without requiring explicitly defined classes or awkward `Tuple`-like objects. An unlimited number of
values and types can be easily stored with any additional complexity.

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

