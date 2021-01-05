# Rust Is Great

(For more than just systems programming)

Written: XXXX-XX-XX

## Rust is Great

I've been [getting](https://github.com/crockeo/bplist-rs) [into](https://github.com/crockeo/lisp-rs) [Rust](https://github.com/crockeo/pj) [recently](https://github.com/crockeo/advent-of-code-2020) which, for me, means watching a lot of Rust content. It's really nice to see a community of people who are so excited by a new and promising technology like Rust. If you've watched or read anything about the language, I'm sure you've heard why folks evangelize it: *Rust provides memory safety without the runtime overhead incurred from a garbage collector by performing extensive compile-time checks.*

There's a lot of color missing from that description, so I invite you to watch any one of the plethora of conference talks explicating this:

* For a Microsoft perspective: [Ryan Levick - Rust at Microsoft](https://www.youtube.com/watch?v=NQBVUjdkLAA)
* For a Facebook perspetive: [Jeremy Fitzhardinge - Bringing Rust Home to Meet the Parents](https://www.youtube.com/watch?v=kylqq8pEgRs)
* For a C++ perspective: [Isabella Muerte - The Symbiotic Relationship of C++ and Rust](https://www.youtube.com/watch?v=YZomx3Jt4Xs)
* Many, many more.

All of these are great talks and, if you haven't already, I'd suggest you watch _at least_ one of them. If you watch multiple I think you're going to notice they all fall back on the same pitch I summarized above: Rust is fast and Rust is memory safe. This pitch works great when you're convincing folks of the very real 10x improvement over C and C++ for systems programming, but it misses the mark for other programmers.

For many engineers, systems programmers' concerns around garbage collection and memory safety are just a distant nightmare of retaking your final for &lt;insert undergraduate C course here&gt;. The aforementioned 10x improvement is very real, but totally inapplicable to the engineers who, when faced with a deliverable on the horizon, reach into their toolbox of high-level garbage collected languages like [JavaScript](https://www.javascript.com/), [Python](https://python.org), or [Go](https://golang.org). To them, the development velocity of a familiar, expressive tool outweighs the performance they don't need and the memory safety they already have.

## Rust is Reaching Out

From the outside it looks like the Rust community is trying, in earnest, to reach that deliverable-minded engineer. One needs only look at the change in Rust's slogan from 2018 to now:

2018:

> Rust is a systems programming language that runs blazingly fast, prevents segfaults, and guarantees thread safety

???? (I got this from [this post](https://blog.rust-lang.org/2018/11/29/a-new-look-for-rust-lang-org.html) but I don't actaully know when it was used):

> Rust: The programming language that empowers everyone to become a systems programmer.

Now:

> A language empowering everyone to build reliable and efficient software.

The Rust community seems to have moved from positioning it as a systems language to a general-purpose language that _happens_ to be useful for systems programming. The slogan change alone won't get more people on board, but I think it signals a shift in Rust recruiting. Rebecca Turner did a [great talk](https://youtu.be/BBvcK_nXUEg) at [RustConf 2020](https://rustconf.com/) pitching "Rust for non systems-programmers."

TODO: *Add more flavor here:*

* What other outreach is Rust doing?
* Has it been working so far (are there surveys breaking down the # of people in the Rust community who identify as "systems programmers")
* Are there any stated goals of this path?

## My Pitch Collection

TODO: *write all of these*

### For the buildsystem engineer

* can only define `impl` as the creator of the `trait` or the `struct`
* means you can have multiple versions of the same package installed
    * package `A` depends on package `B=0.1`
    * package `B==0.1` depends on package `C==0.2`
    * package `A` depends on package `C==0.`
    * totally kosher

### For the LISP engineer

* macros oh macros galore!
* show some good examples of the usage of macros, like
    * tokio main?
    * structopt?
    * serde?
    * etc.
* talk about the macro system

talk about macros in rust:

https://www.youtube.com/watch?v=dZiWkbnaQe8

### For the product engineer

* tokio and async
* modern and stable HTTP processing
* ???
