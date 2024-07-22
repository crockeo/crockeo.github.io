+++
title = "Graph Task Management"
description = "Moving past the list, into something more confusing."
date = "2024-07-21"
draft = true
+++

# Overview

For the past 4 years I've been on the search for the """perfect""" task management system.
For me that means the system should:

- Be free and open source.
- Give me ownership of my data.
- Be available across multiple devices.
- Provide some mechanism to sync between those devices.
- Have a streamlined, primarily keyboard-based user experience.

I settled on [Things](https://culturedcode.com/things/),
as it checks all of those boxes--except that it's not open source.
It is, however, one of the few remaining pieces of software you can buy once and use forever.
I thought it was a good deal to trade $60 for freedom from thinking about task management.

Except that I am still shackled to this work by one last idea:
**Existing systmes don't let you accurately represent the messiness of real work.**

Or put another way:
each task management system I've seen forces the user
to contort their work into a format suitable to the system,
but the system should instead support the essential structure of the work.
I'm captured by that idea, and want to think long enough
on how one could develop a system which supports the structure of messy work,
while still being pleasant to use.

The rest of this blog post will be:

- A brief overview of ways you can structure task management systems.
- A pitch for graphs being well-suited to task management systems.
- Musings on how to build an good user experience for such a system.
- A request for involvement, if anyone in the community has ideas or feedback.

# Common Task Management Systems

Before talking about graph-based task management,
I want to enumerate the "essential" structure of other task management systems.
Here "essential" just means ignoring the metadata of tasks,
like support for scheduling work, assigning deadlines, adding tags, etc.
These are not structural differentiators, and so they're not relevant here.

This enumeration is useful as it allows us to collect some limitations of these systems
which leads us naturally towards the structure of a graph.

## 0 Dimensions // The Mind Grab-Bag

Before considering a system which organizes work, let's consider the alternative.
I consider the "grab-bag" the 0th dimension of a task management system.
Usually this is stored in one's brain, and it's presented to them semi-randomly.
You have no means to record, order, or relate work to each other in a durable fashion.
Instead you get to grab work from the bag semi-randomly--usually by association.

I'm sure there is variability in each person's mind.
If you don't feel that this section accurately represents
how you manage work in your mind: congratulations!
Some of us are cursed to live with gale winds blowing
ideas haphazardly through our heads.

```
   Do   dishes
     the             the
Take           Repair   fence
  out
   the trash     dog
               the
            Walk
```

This system works effectively when there are environmental signals
to remind one of the work they need to do,
like when there are dishes in the sink,
or a dog that is whining.

It does not work effectively when there is not an environmental signal,
like if you have to schedule an appointment with your doctor for an annual physical.
Nor does it work well when you want to make absolutely sure that you get something done;
ultimately you're at the whim of your associative memory!

## 1 Dimension // The List

The simplest *external* task management system is the list.
One writes down a sequence of things that must be done
without any explicit relation between the tasks.

Often one will use the metaphor of reading order for sequencing.
Top-down and left-to-right suggest that which appears closer to the top-left
must be done before that which appears closer to the bottom-right.

For example:

```
- Do the dishes
- Walk the dog
- Take out the trash
- Repair the fence
```

This system is effective for simple workloads, where tasks are independent, atomic units of work.
Crucially each element of the list needs to be simple enough that it can fit into your head.

It is not effective when your work is complex enough that it cannot fit into your head.
In that case, you end up having a system which is half structured and half unstructured.
The structured half of the list acts as environmental signal to trigger the grab-bag of your mind.
You can attempt to sidestep this by enumerating all of your work,
even the parts of a task which belong to another task,
but in doing so you trade the unstructured _what_ (of what work must be done)
with the unstructured _how_ (of how the work relates to each other).

## 2 Dimensions // The Tree

The tree is a first step in generalizing the list to address the
issue of decomposing large, complicated work into small, understandable work.

I call it a "tree" because it's representable by a
[tree data structure](https://en.wikipedia.org/wiki/Tree_(data_structure)),
but it just means a list where each task can have sub-tasks.

For example:

```
- Do the dishes
- Walk the dog
- Take out the trash
  - Collect trash from bathrooms
  - Actually take out the trash
- Repair the fence
  - Get supplies
    - Drill
    - Wood
    - Screws
    - Chickenwire
  - Put up the new fence post
  - Put in the chickenwire
```

Where before the tasks "take out the trash" and "repair the fence"
contained some amount of hidden, unsaid work in them (unstructured _what_),
now each of their component sub-tasks are specified and attributed to the super-task.

Note that there is still no mechanism to relate independent tasks to each other.
There is nothing determining the order of "take out the trash" and "repair the fence,"
and you couldn't articulate an ordering between them without also implying a hierarchy.
This is the curse of a tree-based system:
to define a relationship between two tasks
you also have to make one the "parent" and one the "child."
There is no way to link two tasks together such that one depends on the other
without also implying this parent/child relationship.

This system is effective at representing more complex problems,
by allowing the user to decompose them into smaller tasks.
It solves the unstructured _what_ vs. unstructured _how_ dilemma
for the parent/child relationship.
Most every todo app I've encountered uses this system,
but with some extra coat of paint on top.

However this system is not effective at representing non-parent/child relationships.
It is not hard to imagine a real world scenario where such a relationship would crop up.
Imagine:

- Person 1 is shipping a project to develop a new piece of infrastructure for their company.
- Person 2 depends on this infrastructure to ship a feature.

Shipping the infrastructure is not a child of shipping the feature,
but that is the only way one could represent this dependency
using the tree system.

## 3 Dimensions // The Graph

The graph is the next step towards generalizing the concept of relationships between tasks.
Again, I'm using computer science terminology
and call this a "graph" because it's representable by a
[directed graph data structure](https://en.wikipedia.org/wiki/Directed_graph).

This formulation is probably less immediately familiar to folks
who haven't developed software before,
so I'll take a bit longer explaining it than the other systems to task management:

- You can have a collection of tasks,
  where there is no implication of ordering of preference.
  This corresponds to the set of vertices `V` of a directed graph.

- You can have a relationship named something like "blocking",
  which can be thought of as two tasks `FromTask` and `ToTask`,
  where if `FromTask` "blocks" `ToTask`,
  `FromTask` must be done before `ToTask`.
  This corresponds to the set of "arrows" `A` or "edges" `E` of a directed graph.

- Together these form a directed graph
  which you can use to articulate complicated relationships between work.

As a simple example, imagine some of the tasks from earlier:

```
             _  Take out the trash  _
             /|                    |\
            /                        \
Collect the bathroom trash  Collect the kitchen trash <- Walk the dog
                                     /|\                    /|\
                                      |                      |
                                Do the dishes                |
                                     /|\                     |
                                      |                      |
                                Buy dish soap          Buy new leash
                                      |                      |
                                     \|/                     |
                               Shopping List <---------------/
```

While this graph of tasks isn't obvious at first,
you can think through how it represents a common sense plan
that one might come up with in their head:

- I need to take out the trash, and I don't want any leftover trash in the house.
- I'm about to walk the dog, so I might as well bring out any... bags I have to collect.
- The kitchen trash is the largest, so why not put it in there!
- But I may also have some trash from cleaning the dishes
  (e.g. food too big for the sink, but too small to be saved).
- But to do that I need to buy new dish soap, because I're out.
- So I need to go to the store to get the dish soap.
- _Oh_! And I forgot that I needed to buy a new leash, because mine broke...

This is a bit of a contrived situation,
but you can see how even a simple set of tasks
can be well represented in a graph.

This system is effective at representing complex _and_ interrelated work.
Complex work is well-handled by the same property available in trees:
you can break down nebulous tasks into smaller, more understandable tasks.
Interrelated work is well-handled by the fact that task `A` blocking task `B`
does not need to imply that `A` is the child of `B`,
so you can freely annotate which work depends on which other work.

I have yet to find a kind of work this system cannot effectively represent.
If a reader can come up with an example of real world tasks
which is not effectively representable by this system: please let me know!

_But_ I have noticed that this system struggles
with certain tasks common to a task management system.
Take the earlier example: you could do an analysis of the graph and find out that
"collect the bathroom trash," "buy dish soap," and "buy new leash"
are the three tasks which are "actionable" (aka: there is nothing blocking them).
But which should you do? Which should you do first? And why should you do them?
To answer these questions you have to understand the graph around these tasks,
which is not as easy to do as just glancing at a todo list.

That is the drawback of the graph: it is not useful by itself.
A list or list-of-lists are useful, even when just expressed as pen on paper.
Looking at a graph, especially as it grows in side, is not useful.
Our brains simply are not well equipped to glance at a graph and understand its meaning.
A graph is powerful tool for articulating the structure of work,
but it _needs_ a well-designed user experience powered by a computer
to let a user effectively interpret its contents.

# Aside: Previous Work

Taking a break from the navel gazing...
This is a well-trodden space.
You should take a minute to go through and read some related work.
There's some good thought out there on this exact subject,
and it's been very useful in forming my opinion:

- [James Fisher's TODO DAG](https://jameshfisher.com/2013/12/19/todo-dag/)
  discussing much the same topic,
  from all the way back in 2013!

- The app [intention](https://about.i.ntention.app/)
  which James mentions at the end of his blog post.

- [taskdb](https://github.com/andrey-utkin/taskdb)
  which I stumbled upon while looking into this space.

---

TODO: edit here and below


# How to make it work?

I think it comes down to two design problems:

- Building an effective user experience both for planning and for executing.
- Extracting useful information out of the graph.

## Planning and Excecuting

Phrasing task management as a graph is ultimately a double-edged sword.
On one side: it gives you a powerful tool to represent complex
relationships between messy chunks of work.
On the other: it forces you to see the messiness of your work
every time you want to reference you task management system.

Graphs are not easy to parse, especially for those without an
education in math or computer science.
Looking at a graph *when you want to get something done*
is not an effective use of your time.

Graph-based task management systems have to support two different users:

- *The planner*, who is processing their upcoming work, and understanding
  what they will have to do as *the executor*.

- *The executor*, who is looking to leverage the work of *the planner*
  to find out what the next, most important thing is for them to work on.

While these are often the same person, they are not *the same user*.
They have very different goals and so need different user experiences.

### The Planner

*The planner* needs to be able to translate the complex system of work
into a system which can accurately represent the complexity accurately.
They need to be able to see the structure of how each chunk of work connects to each other,
but they do not need to know about details of any piece of work.

This user should look at the graph as a graph,
be able to see the structure of the complex web of work they're designing,
and to be able to easily reshape the structure as they see fit.

### The Executor

*The executor* should be able to trust the work of *the planner*
without having to consider the graph.
The system should be able to present *the executor* with
the next, most important thing for them to work on,
without having to consider the overall structure of the work.

In the case of multiple executors (e.g. a team),
the system should be able to decompose the work into multiple
non-overlapping paths so that two or more executors
can collaborate in finishing the work.

Crucially in either case:
*the executor* should not have to think of the graph,
they should only have to think of lists of possible tasks.
In this way, despite the underlying complexity,
the resulting user experience for *the executor*
is as simple as the simplest task management system.

## Extracting Information

When using a task management system shared by multiple users,
you may build a repository of work
which is larger than can fit into one person's head.
At that point, you have the opportunity to extract  ~ insights ~  about your work.

I've typically found task management systems
(like [JIRA](https://www.atlassian.com/software/jira) and [Linear](https://linear.app))
to have *very* low signal from their insights.
I think this comes from two places:

- Engineering teams who do not want to use the tools (fair enough, me neither!)
- Systems which cannot accurately capture work.

The former is hard to solve, and beyond me. I am but a humble software engineer.
The latter, though, I think is in the scope of this blog post.
Extending a task management system to be natively graph based
gives you much richer information about the shape and structure of your work.
You can ask much more interesting queries about it as well, like:

- How long will it take (assuming some decent number of tasks in the graph are annotated with time estimates).

- More interestingly: how long would it take with N people working on it.

- Or perhaps even better: what is the most optimal number of people to allocate
  given some provided weight between speed vs. cost.

- What work should you start on earliest so as to make the workstream finish as quickly as possible?
  Or said another way: what is the [long pole](https://devblogs.microsoft.com/oldnewthing/20080805-00/)
  of a workstream?

There's definitely much more that I haven't thought of yet.
To me the exciting first step is building a graph data structure
which can accurately represent the work.
And then the questions (and how you answer them using the graph)
will grow organically from use.

# My Own Hubris

To cap all of this off: I've decided to join the ranks before me
and try my own hand at building some kind of graph-based task management software:
[ekad](https://github.com/crockeo/ekad).

I'm starting off very simply: just make a CLI to interact with a task graph.
I'm hoping to continue improving this incrementally
as I use this for more and more of my real work.

Assuming I stick with it
(which is a big assumption,
considering my
[long history of abandoning software](https://github.com/crockeo?tab=repositories))
the rough (very, *very* rough) roadmap is something like:

- **Explore the set of features I need using a CLI**,
  which is low effort to build and maintain.
  Use this to find out the right UX for myself
  as both *the planner* and *the executor*.

- **Build an interactive user interface** around this experience
  to reduce recurring key-presses and to keep important information in front of me.
  This will probably be a text user interface at first,
  since that's easier for me to build in a cross-platform way.

- **Implement local-first synchronization**
  using something like [Automerge](https://automerge.org/).
  Preferably I would do this with peer-to-peer connections at first,
  and then provide the option of remote sync later.

- **Make a GUI**
  either by exposing the core code as a reusable library
  that can be pulled into native applications,
  or by finding a sufficiently good cross-platform GUI toolkit.

# Questions, Comments, Concerns?

Please let me know if you have any thoughts about this work!
Especially:

- Let me know if you know other people working in this space.
- If I've missed something super obvious.
- Or if you have an interest in participating in this work.

