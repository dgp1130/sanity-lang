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

## Getting Started

Sanity is still in early development and not at a releasable state. Mainly as notes
for myself, there is some documentation on building Sanity from source and running
it [here](doc/dev_setup.md).

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
    let incremented := number + 1;
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
let friend: Person = new Person();
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
to enable a single type to represent multiple possible values. See
[Null and Exceptions](#null-and-exceptions) for details on this.

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

The one asterisk I can think of is that an anonymous object probably cannot be exactly the same as
the equivalent Java class. This is because an anonymous type can be casted down to a more limited
type. Casting means that there may be additional values not specified by the type. If additional
values are present, then the object's size any layout could vary in a way that traditional class
would not. Anonymous objects as specified here, would likely need to be implemented with some kind
of map under-the-hood, much like a JavaScript object.

Type-safe anonymous objects provide a means of elegantly creating an intermediate data format which
is clear and obvious without any unnecessary boilerplate. It also provides a simple and easy means
of creating named arguments to functions. Data can be easily stored and extracted from these objects
without requiring explicitly defined classes or awkward `Tuple`-like objects. An unlimited number of
values and types can be easily stored with any additional complexity.

#### First-Class Generics (Type Parameters)

#### Type Operations

Types in Sanity can be conceptualized as sets. For instance, the `int` type can be thought of as a set
of all 32-bit integers. The algebraic OR of two types is similar to performing the union operation of
two types. However, there are other set operations that can be applied as well, and this can enable
(among other things) one type to be "subtracted" from another (not final syntax).

```
int|string|float - float == int|string;
```

How can this be useful? Consider a `Pipe` object which takes a set of data of one type as input and
performs operations on it, return the new value as an output within a `Pipe` to streamline composition.
Imagine mapping a type `A = A1|A2|A3` to a type `B = B1|B2|B3` with a direct correlation between
`A1 -> B1`, `A2 -> B2`, and `A3 -> B3`. Rather than using a single transformation function which then
disambiguates the `A` subtype and converts it to the appropriate `B` subtype, it can be beneficial to
use three different transformations, each responsible for a single subtype. This could look something
like this:

```
let pipeOfA: Pipe<A, void> = ...;
let pipeOfB: Pipe<B, void> = pipeOfA
    .on((a1: A1) -> B1.from(a1))
    .on((a2: A2) -> B2.from(a2))
    .on((a3: A3) -> B3.from(a3))
    .pipe();
```

Each `on()` call is responsible for mapping one subtype of `A` to one subtype of `B`. Now, is it
possible to do this in a type-safe manner? Consider the following definition:

```
class Pipe<A, B> {
    let value: A|B;
    
    func on<A_SUBTYPE : A, B_SUBTYPE : B>(cb: (a: A_SUBTYPE) -> B_SUBTYPE) : Pipe<A - A_SUBTYPE, B> {
        if (value instanceof A_SUBTYPE) {
            let b : B_SUBTYPE = cb()
            return new Pipe<A - A_SUBTYPE, B>(b);
        } else {
            return new Pipe<A - A_SUBTYPE, B>(a);
        }
    }
    
    func pipe() {
        #if (A != void) {
            throw CompilerError("Need to handle all A subtypes in #on() before calling #pipe().");
        }
        
        return new Pipe<B, void>(value as B);
    }
}
```

This `Pipe` has a value which starts as an `A`. Each time `.on()` is called, the `A` subtype it consumes
is removed from the `A` type. The value is passed through if it is given a callback expecting a different
subtype. When it is called with the correct subtype, it invokes the callback to convert the `A` into a `B`,
which then passes through the remaining `.on()` calls as the types continue to narrow.

After all the `.on()` calls, the type has been narrowed to `Pipe<void, B1|B2|B3>` because all `A` subtypes
have been removed, and `void` represents the empty set or "empty type". Calling `pipe()` performs a compile
time check to verify that all `A` types have been handled. If any are left, it is a compile-time error.
Otherwise, it can confidently create a new `Pipe` loading the `B` in the first position ready to the repeat
the process with another set of `.on()` calls mapping `B` values to some other subsequent type.

The `#if` syntax for a compile-time statement comes from
[Jai](https://github.com/BSVino/JaiPrimer/blob/master/JaiPrimer.md), Sanity will have a very similar concept.

Being able to perform operations on the underlying types in generics can allow for far more powerful
functions and classes with more accurate type systems. Subtracting one type from another is just one example
of what this could do.

TODO: Continue with negative types and so on...

#### Null and Exceptions

`null` has been called the "billion-dollar mistake", and while I don't entirely agree with that, the
current concept of `null` can be drastically improved. `null` has quite a few problems in its current
incarnation.

* Attempting to deference a `null` is a runtime exception.
* `null` exceptions are difficult or impossible to detect at compile-time.
* `null` is a single value which represents the lack of a value. Logically however, there may be many
different forms of "no value". For instance, not connecting to the server might yield a `null` value,
but successfully connecting and then receiving a server error might also yield a `null` value despite
the fact that they represent different outcomes.
* `null` often overlaps with exceptions. When should one return a `null` and when should one throw an
exception?

Exceptions also have a few interesting challenges:

* Checked exceptions in languages like Java allow the compiler to verify that all exceptions are
handled. Most languages do not have this guarantee and most developers don't use checked exceptions.
This means it is hard to know what exceptions a given function call can make and whether or not you
have handled all of them.
* try-catch syntax is not perfect. It often covers more statements than it needs to, and if one of
them throws an exception, it may be caught in a manner that was not expected. For instance:

```java
try {
    final Car car = requestCarInfo(carId);
    saveToDatabase(car);
} catch (final NetworkException ex) {
    System.out.println("Failed to get car info.");
}
```

Here, the try-catch was intended to catch an error from `requestCarInfo()` but `saveToDatabase()`
actually makes a network request and can throw a `NetworkException`. If it does, it will be caught
too and display the wrong error message. The `saveToDatabase()` call cannot be easily moved out of
the try-catch because it requires access to `car` which must be inside the try-catch. The declaration
of `car` can be moved out of the try-catch, leaving the initialization inside. However, this means it
cannot be `final` for no good reason and will be in scope for much longer than is necessary.

Sanity solves these problems by removing the concept of `null` as a singular value and replacing the
`throw` semantic, instead *returning* the errors directly. It uses algebraic types to pull this off.
In Sanity, any type can be the algebraic OR of multiple other types. These types may or may not
contain data and can declared inline. As an example:

```
// Car is an existing class, but TransportError and ServerError are declared inline, so they use the
// `type` keyword. TransportError is simply a type with no data, while ServerError contains a message.
function requestCarInfo(carId: int) -> Car | type TransportError | type ServerError {message: string} {
    ...
    
    if (...) {
        return new Car();
    } else if (...) {
        return new TransportError();
    } else {
        return new ServerError({message: response.message});
    }
}

function lookupCar(carId: int) {
    // This type is explicitly listed for clarity, the := operator could be used to infer the type.
    let result: Car | TransportError | ServerError = requestCarInfo(carId);
    
    // The `when` operator invokes the appropriate lambda function provided for the type of the result.
    // The `result` variable is casted to the relevant type in the body of each function.
    when (result) {
        Car = print("Make: " + result.make + ", Model: " + result.model);
        TransportError = print("There was a network error, please try again.");
        ServerError = { // Multi-line lambdas are acceptable.
            print("The server returned an error: " + result.message);
        };
    }
}
```

Instead of using `null` or exceptions to handle the error cases of this function, it simply returns an
algebraic OR of the various outcomes it can have with the appropriate data. The caller uses the `when`
operator to disambiguate the possibilities and perform the appropriate action. The `when` operator works
by utilizing reflection to check the type of the result and then invoking the lambda associated with that
type. It auto casts the value to the more specific type to save programmer effort. The `when` operator also
requires that all possible types are handled. This ensures that no cases are missed without requiring the
overhead of checked exceptions. The caller can directly handle the error, or it can easily return it back
up to its caller by adding it to its own possible responses. This allows errors to propagate up the call
stack until they end up at the appropriate level of abstraction for handling them.

Existing types can be combined into an algebraic OR by utilizing the `|` operator, and throwaway types can
be declared inline using the `type` keyword. A `type` followed by only a name is simply a symbol which
represents a particular outcome with no associated data. A `type` can be followed by an anonymous object
which contains all the data for that type.

Unfortunately, this won't fully enforce that all outcomes are handled at compile-time. The `result` type
can be hard casted to any of its subtypes, which will be a runtime error if not possible. Hopefully, such
an action should be relatively rare and unnecessary.

Beyond replacing the concept of `null`, this also replaces many uses of exceptions, certainly checked
exceptions. However, Sanity will still have unchecked exceptions because there are a couple uses for them
which are not covered by this idea. The main use of unchecked exceptions is for runtime errors which should
never happen in practice. This would include assertion errors, illegal argument errors, illegal state errors,
and other issues which indicate a programming issue which is unrecoverable. As an example:

```
function colorShape(shape: Shape, color: string) {
    if (color == "red") {
        ...
    } else if (color == "blue") {
        ...
    } else {
        throw new IllegalArgumentError("Unknown color: " + color);
    }
}
```

There is no practical instance where a caller would catch the `IllegalArgumentException` and be able to do
anything useful to handle it. As a result, the idea of returning an `IllegalArgument` type, is not useful to
the caller and simply gets in the way without providing any benefit. There are a few reasons to `catch` an
exception, though not particularly many:

* Runners which catch a fatal error, and then restart the program.
* Test frameworks which use errors to propagate assertion failures. Manually declaring and returning these
assertions from each test would be infuriating.
* A logger which catches exceptions simply to log them and possibly rethrow.

As a result, Sanity will have exceptions to support these use cases, but 99% of error cases, should return
algebraic types with all possible outcomes. This is a better system for these common use cases, while
throwing exceptions should only be used for extreme instances where returning exceptions up the call stack
is impractical.

### Pattern Matching

Most functional languages support pattern matching, looking something like the following (example is Haskell):

```haskell
factorial :: (Integral a) => a -> a
factorial 0 = 1
factorial n = n * factorial (n - 1)
```

This is a very powerful tool which has one critical limitation, patterns can only be constants or bindings
to variable names. For more complex pattern matching, Haskell uses guards (example shamelessly stolen from
http://learnyouahaskell.com/syntax-in-functions):

```haskell
bmiTell :: (RealFloat a) => a -> String
bmiTell bmi
    | bmi <= 18.5 = "You're underweight, you emo, you!"
    | bmi <= 25.0 = "You're supposedly normal. Pffft, I bet you're ugly!"
    | bmi <= 30.0 = "You're fat! Lose some weight, fatty!"
    | otherwise   = "You're a whale, congratulations!"
```

This is really a hack around pattern matching, because it isn't quite as flexible as it need to be to support many use
cases. Sanity will also have pattern matching, but utilize it slightly differently. The parameters given won't just be
constants, they will be functions. Consider the following Haskell-like example:

```haskell
bmiTell :: (RealFloat a) => a -> String
bmiTell (<=18.5) = "You're underweight, you emo, you!"
bmiTell (<=25.0) = "You're supposedly normal. Pffft, I bet you're ugly!"
bmiTell (<=30.0) = "You're fat! Lose some weight, fatty!"
bmiTell _ = "You're a whale, congratulations!"
```

Recall that functional languages like Haskell strictly follow the lambda calculus concept that all functions accept
exactly one parameter and return exactly one result. This means that a function which accepts two parameters, is
actually a function that accepts one parameter and returns a function which accepts the second parameter. While
conceptually a multi-parameter function, it is really a curry of many single-parameter functions. This can be hard to
see in Haskell because it does this automatically, but clearer to explain in something like JavaScript:

```javascript
const traditionalAdd = (a, b) => a + b;
traditionalAdd(1, 2); // 3
const curriedAdd = (a) => (b) => a + b;
curriedAdd(1)(2); // 3
```

The curried format has many benefits, one of which is that functions can be partially applied:

```javascript
const curriedAdd = (a) => (b) => a + b;
const addOne = curriedAdd(1);
addOne(2); // 3
addOne(3); // 4
addOne(4); // 5
```

Binary operators like `+` are considered functions of two parameters, and can also be partially applied. The result is a
function which accepts a single parameter to complete the operation (switching back to Haskell).

```haskell
addOne = (+) 1 -- Partially apply one to the + operator
addOne 2 -- 3
addOne 3 -- 4
addOne 4 -- 5
```

Sanity will utilize this in a pattern match by the treating the provided pattern as a function which accepts the value
to match and returns whether or not it matched as a boolean.

```haskell
relatesToFive (<5) = "The value is less than 5."
relatesToFive (>5) = "The value is greater than 5."
relatesToFive _ = "The value is equal to 5."
```

This can be used to compute arbitrarily complex logic in the pattern match. Consider the infix operator `fand`, a custom
functional implementation of the `&&` operator, which accepts two functions and invokes each of them with the same value
and performs a boolean AND on the result.

```haskell
fand :: (Integral a) => (a -> Bool) -> (a -> Bool) -> Bool
fand a b n = a(n) && b(n)

fnot :: (Integral a) => (a -> Bool) -> Bool
fnot a n = not a(n)

isOdd :: Integral -> Integral
isOdd n => n % 2 == 1

relatesToZeroAndFive (>0 `fand` <5 `fand` isOdd) = "The value is between 0 and 5 and is odd."
relatesToZeroAndFive (>0 `fand` <5 `fand` fnot isOdd) = "The value is between 0 and 5 and is even."
relatesToZeroAndFive (<=0) = "The value is less than or equal to 0."
relatesToZeroAndFive (>=5) = "The value is greater than or equal to 5."
```

This removes the need for a separate guard syntax, because the pattern matching itself is just as powerful. There are
still a couple open questions. For example it is often necessary to bind to the real value even with these complex
matches, maybe that could look something like:

```haskell
relatesToZeroAndFive value <- (>0 `fand` <5) = "The value " ++ show value ++ " is between 0 and 5."
```

The compiler is also a little dumber for providing more flexible pattern matching. It can no longer guarantee that all
possibilities are handled. This could lead to additional errors and missed edge cases. Of course, with guards the same
thing is possible, even in Haskell. Haskell's syntax somewhat discourages use of pattern matching in this way while
Sanity would embrace it. Like all the other ideas here, I'm not entirely convinced it's a good one, but the point of
Sanity is to try these kinds of things and find out. The exact syntax Sanity would use to accomplish this is still up in
the air.

### Context

### Compile-Time Execution

### Elm-style Streams

### Import/Export and Module System

### Testing Support

### Properties

### Events

#### Deep Notify

### Miscellaneous

#### Variables Read-Only by Default

#### this Always Lexically Bound

