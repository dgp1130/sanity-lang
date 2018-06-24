# Constructors

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
that the implementation detail of constructing a class has leaked out of the
class. If `Model` or `SuperModel` wanted to implement a cache which reuses
previously created objects, they can't do it without breaking `Elsewhere`.

## Limitations of Constructors

There are also a number of things a constructor can't do:

* Constructors cannot return `null`.
* Constructors cannot be asynchronous.
* Constructors cannot return a subclass.
* Constructors cannot return an already existing object.

Factories solve a lot of these problems, but factories have their own issues. They
don't play well with subclassing and inheritance in general, because subclasses
cannot extend an object returned by another method. Factories also don't work well
with frameworks which often need hooks into constructors or need to own them entirely
and have to implement their own lifecycle methods instead.

## Syntactical Exceptions

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
more intuitive. C++ uses initializer lists to set `const` fields, which can make more
syntactic sense even if it is no more flexible than Java. Kotlin omits the `new` operator.
These all improve things slightly, but don't change the underlying functionality of
constructors and the inheritance hierarchy model, particularly in the direct coupling of a
subclass' construction to that of its superclass. I am asserting that the this core concept
of constructors is flawed and can be done better. **Constructors are a fundamentally broken
mechanism for instantiating objects.** I believe this can be done better in a far simpler
and more flexible fashion.

## Constructor Solution

My solution to this problem is to decouple subclass construction from superclass
construction. With this one change the overall model of constructors can be completely
reworked to solve a lot of these issues. Lets start off simple without worrying about
inheritance. Consider the following Java-like snippet:

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
instead. What about `final` variables, how might they look?

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
creates a server which serves an HTML5 application cache (dated example I know, but I
actually encountered this which is why I'm using it here). If the cache is enabled, it
will check dev mode to determine if it should show a development version of the app cache
or the production version. If the cache is disabled, then it will load a non-existent cache
so the browser doesn't load anything.

```java
public class Server {
    final string cacheLocation;
    
    public static Server create(final boolean devModeEnabled, final boolean cacheEnabled) {
        if (cacheEnabled) {
            if (devModelEnabled) {
                return new Server(cacheLocation = "manifest_dev.appcache");
            } else {
                return new Server(cacheLocation = "manifest.appcache");
            }
        } else {
            return new Server(cacheLocation = "does_not_exist.appcache");
        }
    }
}
```

Notice how nested if statements work just fine with `final` variables. Running this same kind
of logic in a traditional constructor would require nested ternary operations which can be
complicated and difficult to read.

One other advantage is that the implementation detail of having created an object is missing
from the caller which provides much more flexibility as a result. Consider the following case:

```java
public class Model {
    final int foo;
    
    public static Model from(final int foo) {
        return new Model(foo = foo);
    }
}

public class Elsewhere {
    public static void doSomething() {
        final Model model = Model.from(0);
    }
}
```

What if one day we decided that constructing a new `Model` is expensive, and we should
cache all existing ones and resue them as necessary? This is painless to do, does not
require changes to `Elsewhere` and needed no foresight at the time we initially wrote
`Model`!

```java
public class Model {
    static final Map<int, Model> map = new Map<>();
    
    final int foo;
    
    public static Model from(final int foo) {
        final Model cachedModel = map.get(foo);
        if (cachedModel != null) {
            return cachedModel;
        } else {
            final Model model = new Model(foo = foo);
            map.put(foo, model);
            return model;
        }
    }
}

public class Elsewhere {
    public static void doSomething() {
        final Model model = Model.from(0);
    }
}
```


## Inheritance

Sanity is not going to use a traditional inheritance model like Java does (see
[Proxies](../README.md#proxies) and [Mixins](../README.md#mixins) for Sanity's solution
to that particular issue). However, it is possible to use this system with a Java-like
inheritance model, it simply requires decoupling between superclass and subclass construction.

The first issue with inheritence is that by calling a superclass' factory, the object has
already been instantiated. This makes abstract classes impossible, and requires concrete
superclasses to be extended after they have been constructed, which does not make a whole lot
of sense. However, we can get around this by not instantiating the type immediately, instead
we use a `ctor<T>` type which holds the data it was given, without actually instantiating the
object. `ctor` would be a keyword in the language which simply `malloc`s the data and stores
it. However, this would be a distinct type from `T`, as none of `T`'s methods are exposed and
its data is read-only. A separate `from` keyword is used to instantiate a child class off its
superclass' `ctor`.

```java
public class SuperModel {
    final int foo;
    
    public static ctor<SuperModel> create(final int foo) {
        return new ctor<SuperModel>(foo = foo);
    }
    
    public void print() {
        System.out.println(this.foo);
    }
}

public class Model extends SuperModel {
    final int bar;
    
    public static Model create(final int foo, final int bar) {
        final ctor<SuperModel> smCtor = SuperModel.create(foo);
        
        smCtor.print(); // ERR: print() does not exist on ctor<SuperModel>
        
        final Model model = new Model(bar = bar) from smCtor;
        
        model.print(); // Success
        
        return model;
    }
}
```

In the above example, the super class is not constructed directly, but rather made into a
`ctor<SuperModel>` which simply holds the `foo` variable as it was provided. Notably it is
**not** a `SuperModel`, so `print()` cannot be called on it. Once `Model.create()` has the
`ctor<SuperModel>` it constructs a `Model` on it using the `from` keyword.

This structure decouples superclass construction from subclass construction. Any number of
operations or function calls could be made between the two. The `ctor<SuperModel>` could
be passed in and out of functions, saved to a `Map`, retrieved at later time, and then
instantiated into a `Model`. `abstract` classes can *only* become a `ctor<T>` while `final`
classes can *never* be a `ctor<T>`.

I have used the term "factory" to describe functions which return a new instance of a class
or a `ctor` of a class. In Java-land, "factories" are typically static methods, but that is
not a requirement. One static factory would be required to get a single instance of the
class, but subsequent instances can be created off the first. Consider:

```java
public class SuperModel {
    final int foo;
    
    public static ctor<SuperModel> create(final int foo) {
        return new ctor<SuperModel>(foo = foo);
    }
    
    protected ctor<SuperModel> clone() {
        return new ctor<SuperModel>(foo = this.foo);
    }
}

public class Model {
    final int bar;
    
    public static Model create(final int foo, final int bar) {
        return new Model(bar = bar) from SuperModel.create(foo);
    }
    
    public Model clone() {
        return new Model(bar = this.bar) from super.clone();
    }
}
```

Notice how the instance method `clone()` is used to copy the class's data into a new
instance. This is done in a much more abstracted way than is currently possible using
traditional constructors. `SuperModel#clone()` is responsible for cloning only its own
data while `Model#clone()` only copies its own information, simply calling out to
`SuperModel` without any extra knowledge or effort of what that does. Recall how this
must be done in Java today:

```java
public class SuperModel {
    final int foo;
    
    public SuperModel(final int foo) {
        this.foo = foo;
    }
    
    public SuperModel(final SuperModel base) {
        this.foo = base.foo;
    }
}

public class Model {
    final int bar;
    
    public Model(final int foo, final int bar) {
        super(foo);
        this.bar = bar;
    }
    
    public Model(final Model base) {
        super(base);
        this.bar = base.bar;
    }
    
    public Model clone() {
        return new Model(this);
    }
}
```

The use of copy constructors is required for this kind of problem and is very confusing
to follow. Most experienced programmers are used to this, but try explaining to a
beginner and its complexity becomes immediately obvious.

As one more example, the `Serializable` interface in Java is a perfect example of what
constructors do poorly. It is impossible to define this interface in simple fashion in Java
and it is actually implemented by the language itself, with special keywords and semantics
rather than by actual Java code. Deserializing a class does not invoke its constructor,
which makes no sense in the case of Java. However, this can be done quite easily using this
replacement for constructors.

```java
public interface Serializable<T> {
    public void serialize(OutputStream out);
    public static T deserialize(InputStream in); // Assuming interfaces can support static methods.
}

public class Model implements Serializable<Model> {
    final int foo;
    
    public void serialize(final OutputStream out) {
        out.writeInt(foo);
    }
    
    public static Model deserialize(final InputStream in) {
        return new Model(foo = in.readInt());
    }
}
```

This does have a bit of an issue with inheritance because `deserializable` returns a `T`
while an abstract parent class would have to return a `ctor<T>`. This could be solved with
an algebraic OR of the types (which Sanity will support, see
[Type System](../README.md#type-system)) or another construct could be added. A `ctor<Model>`
could be returned with some kind of `create()` method if absolutely necessary.

The only major issue I see with this inheritance model is that superclasses do not have
any hook which executes at construction-time (when the final concrete subclass instantiates
the object). This means that superclasses can't perform any initialization at
construction-time, only at factory-invocation-time. This has one positive side effect in
that it is impossible for a superclass to call an abstract method implemented by a subclass.
This is possible in Java in particular, but heavily discouraged because it means the
subclass' implementation of that abstract method will be invoked before its constructor has
a chance to initialize it.

If the lack of construction-time computations really became an issue, some kind of
`onConstruct()` method could be called which would enable the class to do whatever it needed
to. I would try to avoid this if possible as I think many classes would be better designed
without this feature, but it may be impractical to avoid in many use cases.

This system would also be tricky to implement in the compiler using current practices, as
superclass and subclass data usually coexist in the same block of memory. With this model,
a particular `ctor<T>` could be instantiated into one of many different subclasses of `T`
with no way of knowing which it will become. This would be an issue as the compiler would
not know how much memory to allocate. There are certainly ways around this issue, the compiler
could separate the superclass' memory from the subclass, simply leaving pointers to get from
one to the other. Depending on optimization, the compiler could store each superclass in the
class hierarchy separately until construction time, only then copying the values into a single
coherent space.

There's also the question of whether or not it is possible to reuse a `ctor<T>` to construct
multiple subclasses. I would argue that's fine, as there is nothing semantically wrong with
that idea, but it might take extra work in the compiler to support that depending on how it
is implemented under the hood.

At the end of the day, this is a far simpler, more intuitive, and flexible way of constructing
objects which has the potential to be much better than existing construction paradigms.
