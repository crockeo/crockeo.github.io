+++
title = "Graph Task Management"
description = "Moving past the list, into something more confusing."
date = "2024-07-18"
draft = true
+++

# Overview

For a while now (2-3yrs) I've been on the search for the """perfect""" task management system.
Throughout this odyssey I developed some strong opinions on how such a system should work:

- It should be open source, and extensible.
- It should give me ownership of my data.
- It should be available across multiple devices.
- It should provide some mechanism to sync between devices.

I still think those are true, and I'd like for a system to have each of these,
but I think they've ultimately been supplanted by one goal:
**I want a system which can accurately represent the messiness of real work.**

# Idealized Systems for Idealized Work

Common task management systems are built for idealized work.
This is the kind of work that one knows ahead of time,
where none of the work is itself work discovery.
This work is not messy, and often not interdependent.

## The Mind

I think before considering systems to organize work,
we should think of work without a system.
I think of this as the 0th dimension of task management.
The mind is like a grab-bag which produces work at random.
I'm sure this approach has the most variability for people,
and for some may not even need an external system to manage it,
but unfortunately my brain is not so well trained
at producing the right information at the right time.

## The List

The most traditional task management system is the simple todo list.
One writes down a sequence of things that must be done,
where it is impossible to define any relation between the tasks.
Oftentimes one will use the metaphor of reading order for sequencing,
aka: top-down and left-to-right to suggest the idea that X must be done before Y.

```
- Do the dishes
- Walk the dog
- Take out the trash
- Repair the fence
```

This approach works great for a simple workload
where tasks are atomic units of work, and are not complex.
But it is not suited to real work:
often one is taking on large, complex projects
which cannot be so easily understood as a list of unrelated tasks.

## The Tree

The tree is the first and most obvious generalization of the list.
Instead of expressing tasks only as a list of independent work,
you are now allowed to link tasks together to form "sub-tasks".

```
- Repair the fence
  - Get supplies
    - Drill
    - Wood
    - Screws
    - Chickenwire
  - Put up the new fence post
  - Put in the chickenwire
- Put in a coop
- Buy chickens
- Incorporate S-corp "Eggckeo"
```

This approach solves one of the issues of the list
by allowing the user to decompose larger, composite tasks into many sub-tasks.
It is well suited to projects without interdependent work,
but it does not do well in places where work depends on each other.

# A Real System for Real Work

I posit that a *graph*-based task management system
is the logical next step towards representing real work:

- Create a [directed graph](https://en.wikipedia.org/wiki/Directed_graph).
- Define each task as a node with associated metadata like name, description, etc.
- Define each edge to represent a dependency from between to tasks.
  If there exists an edge `A -> B` then `A` depends on `B` being completed.
- Enforce that the graph must remain [acyclic](https://en.wikipedia.org/wiki/Directed_acyclic_graph)
  whenever an edge is added.

This approach is already a superset of the list and the tree:

- You can represent the list through a collection of nodes without edges.
  If you want to develop a linearized format like reading a list top-to-bottom
  you can add edges in reverse order of the list: `... -> C -> B -> A`.

- You can represent a tree because a tree is a special case of a directed acyclic graph.
  This is trivially true based on the [definition of a tree](https://en.wikipedia.org/wiki/Tree_(data_structure)#Mathematical_terminology).
  You could represent the earlier example as something like:

  ```
  Repair the fence -> Get supplies
  Repair the fence -> Put up the new fence post
  Repair the fence -> ...

  Get supplies -> Drill
  Get supplies -> Wood
  Get supplies -> ...

  Put in a coop

  etc.
  ```

More importantly: it also gives you new powers of expressivity.
You can now define work which depends on each other in
messy, compilcated, real ways.

For example:

- TODO: come up with a real-sounding example which isn't from work

# Aside: Previous Work

This is, unsurprisingly, a well-trodden space.
For example you have [James Fisher's TODO DAG](https://jameshfisher.com/2013/12/19/todo-dag/)
all the way back from 2013(!!!),
the app [intention](https://about.i.ntention.app/) which James mentions at the end,
[taskdb](https://github.com/andrey-utkin/taskdb),
and now-dead masterplan.so, which was [immortalized on Hacker News](https://news.ycombinator.com/item?id=30205699).

If you're interested in the space,
I suggest you read up on others' perspectives on this work.
As far as I know, none have succeeded in becoming popular
(if only they had--then I wouldn't have to emark on this journey).

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

