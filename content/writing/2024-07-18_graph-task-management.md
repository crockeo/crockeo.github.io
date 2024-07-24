+++
title = "Graph Task Management"
description = "Moving past the list, into something more confusing."
date = "2024-07-21"
draft = true
+++

# Overview

For the past 4 years I've been on the search for the """perfect""" task management system. For me that means the system should:

- Give me ownership of my data.
- Be available across multiple devices.
- Be usable offline.
- Sync between devices, when internet is present.
- Have a "nice" user experience.
- Be free and open source.

I settled on [Things](https://culturedcode.com/things/), as it checks all of those boxes--except that it's not open source. It is, however, one of the few remaining pieces of software you can buy once and use forever. I thought it was a good deal to trade $60 for freedom from thinking about task management.

Except that I am still shackled to this work by one last idea: **Existing systems don't let you represent how messy real work is.**

All task management systems I've seen force to contort their work to suit the system, rather than the system supporting the structure of the work.

This blog post is me thinking about this problem, and speculating on the kind of system which could support the messiness of real work

# Common Task Management Systems

I will start by enumerating the possible structures of task management systems.

Here "structure" means ignoring the metadata of tasks, like support for scheduling work, assigning deadlines, adding tags, etc. While these are useful for managing work, they do not change how you represent or relate work.

This enumeration is useful because it will  let us incrementally discover and build solutions for the problems with various task management systems. It naturally leads us towards the point of this blog post: the graph.

## 0 Dimensions // The Mental Grab Bag

The first option is simply: don't organize your work! This ends up being a grab bag which sits in your mind. I call it a "grab bag" because, without recording one's work externally, the brain will present information to you in an unordered, unstructured fashion.

I'm sure there is variability in each person's mind. If you don't feel that this section accurately represents how you manage work in your mind: congratulations! Some of us are cursed to live with gale winds blowing ideas haphazardly through our heads.

```
   Do   dishes
     the             the
Take           Repair   fence
  out
   the trash     dog
               the
            Walk
```

This system works effectively when there are environmental signals to remind one of the work they need to do. You can avoid the extra job of recording the things you need to do, and just rely on these signals. A dirty dish reminds you to clean the dishes, a whining dog reminds you to take it on a walk.

It does not work effectively when there is not an environmental signal, like if you have to schedule an appointment with your doctor for an annual physical. Your doctor is not there to remind you, and so this task can fall through the cracks.

## 1 Dimension // The List

The simplest *external* task management system is the list: you just write down a collection of things which must be done. The list then serves as your environmental signal.

The list does not provide any explicit way to articulate relationships between the work, but folks will often use the metaphor of reading order for sequencing. For English: top-down and left-to-right reading order means that tasks which appear closer to the top-left must be done before that which appears closer to the bottom-right.

For example:

```
- Do the dishes
- Walk the dog
- Take out the trash
- Repair the fence
```

This system is effective for work where tasks are independent and simple.

It stops being effective when your work is complex enough that each task cannot fit into your head. In this case you can either:

- Keep the list simple, but keep much of the _what_ undefined, because you are missing work that you will need to do.
- Or enumerate all of your work, but keep much of the _how_ undefined, because you are missing how each of your tasks relate to each other.

## 2 Dimensions // The Tree

The tree is a first step towards moving more of the _what_ and _how_ of work out of your mind and onto a page. I call it a "tree" because it's representable by a [tree data structure](https://en.wikipedia.org/wiki/Tree_(data_structure)).

A tree is a list of tasks where each task may have a sub-list of tasks. This extension allows you to decompose complicated work into many simple pieces, _and_ it allows you to define a parent/child relationship between the complicated and simple tasks.

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

Where before the tasks "take out the trash" and "repair the fence" contained some amount of hidden, unsaid work in them (undefined _what_), now each of their component sub-tasks are specified and attributed to the super-task.

Note that there is still no mechanism to relate independent tasks to each other. We still leave the ordering of  "take out the trash" and "repair the fence" up to metaphor. You cannot articulate an explicit ordering between them without also implying a hierarchy. This is the curse of a tree-based system: to define a relationship between two tasks you also must make one the "parent" and one the "child." There is no way to link two tasks together such that one depends on the other without also implying this parent/child relationship.

This system is effective at representing more complex problems, by allowing the user to decompose them into smaller tasks. It solves the undefined _what_ vs. undefined _how_ dilemma for the parent/child relationship.

However this system fails to articulate relationships between work when the relationship is more complicated than parent/child.

## 3 Dimensions // The Graph

The graph is the next step towards generalizing the concept of relationships between tasks. Again, I'm using computer science terminology and call this a "graph" because it's representable by a [directed graph data structure](https://en.wikipedia.org/wiki/Directed_graph).

This formulation is probably less immediately familiar to folks who haven't developed software before, so I'll take a bit longer explaining it than the other systems to task management:

- You have an unordered collection of tasks, which corresponds to the set of vertices `V` of a directed graph.

- You have a relationship named something like "blocking", which can be thought of as two tasks `From` and `To`, where if `From` "blocks" `To`, `From` must be done before `To`. This corresponds to the set of "arrows" `A` or "edges" `E` of a directed graph.

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
                                 Go Shopping <---------------/
```

This diagram should be read as `A -> B` = `A` "blocks" `B`.

This graph represents the kind of common-sense plan that one may create when optimizing household work:

- It's trash day, so you need to bring out the trash. You don't want to have anything left over, so you want to make sure everything you'll need to take out for the evening is in the trash when you bring it out.
- You need to walk your dog, and don't want to have to store its...bags after the walk.
- You also need to throw out any trash from cleaning up the dishes.
- But to do the dishes you need to buy new dish soap first.
- Etc.

The graph inherits all of the properties of representing complex work from the tree, but it also provides the means to represent interconnected work. Where trees imposed that each child may only have one parent, we allow any task to "block" and "be blocked by" any number of tasks. This freedom allows the graph to represent kinds of connections which are impossible to represent on the tree.

This system effectively represents all kinds of real world work I have been able to think up. If a reader can come up with an example of real world tasks which is not effectively representable by this system: please let me know!

However this system is _not_ effective at presenting its representation of work to a user. The extra freedom in the many-to-many "blocking" relationship makes conceptualizing the graph difficult. Take the earlier example: "collect the bathroom trash," "buy dish soap," and "buy new leash" are the three tasks which are not "blocked." But which should you do? Which should you do first? And why should you do them? To answer these questions you need to understand the shape of the graph around these tasks, which is not as easy to do as just glancing at a todo list.

That is the drawback of the graph: it is not useful without a computer program. A list or list-of-lists are useful, even when just expressed as pen on paper. Looking at a graph, especially as it grows in side, is not useful. Our brains simply are not well equipped to glance at a graph and understand its meaning.

# Aside: Previous Work

Taking a break from the navel gazing... This is actually a well-trodden space! You should take a minute to go through and read some related work. There's some good thought out there on this exact subject, and it's been very useful in forming my opinion:

- [James Fisher's TODO DAG](https://jameshfisher.com/2013/12/19/todo-dag/) discussing much the same topic, from all the way back in 2013!

- The app [intention](https://about.i.ntention.app/) which James mentions at the end of his blog post.

- [taskdb](https://github.com/andrey-utkin/taskdb) which I stumbled upon while looking into this space.


# The Computer Program

How should a useful graph-based task management system be designed? I think the answer lies in a secret to the design of _any_ task management system: there are secretly _two_ users.

- *The planner*, who is processing their upcoming work, and understanding what they will have to do as *the executor*. They need to keep the context of the entire graph in their head, but they do not need to know very much about each individual piece of work. They need to easily reshape the graph throughout the iterative planning process.

- *The executor*, who is looking to leverage the work of *the planner* to find out what the next, most important thing is for them to work on. They need to spend the majority of their mental budget understanding the specifics of one piece of work at a time, and so they should not have to think about the structure of the graph.

Traditional task management systems support each of these users 50/50, but in its most naive implementation a graph-based system would skew much further towards _the planner_. It's crucial to design an expereience which satisfies _both_ users, so you can break the math and make an 100/100 solution.

## Designing for the Planner

The planner needs to work primarily at the graph level, while zooming in temporarily to the task level to communicate information to the executor. As such: their user interface should be based around an interactive graph visualization.

Planning is often a fractal problem, so there will be levels of detail which are more or less useful to the planner at different times. The planner should be able to zoom in /  Often planning is a fractal problem, so they should be able to limit the graph to different views. They should be able to zoom out to only see high-level objects, and they should be able to zoom in see much more detail belonging only to tasks which transitively block some specific task.

Planning is highly creative, so the planner should stay in flow as much as possible. They need to have a toolkit to create and reshape the graph without having to move out of the flow state. The interactive graph interface should have the means to interact with the graph as an object.

And finally: the planner needs to communicate to the future executor. It should be seamless to edit metadata associated with a task which will be helpful for an executor to look at in the future. This should include information like name, description, expected durection, deadline, scheduled date, etc.

## Designing for the Executor

The executor needs to work primarily at the task level, allowing them to dig deeply into individual chunks of work. Their user interface should be centered around tasks.

However the executor should be able to temporarily zoom out to see the context of the graph around a task. This is important to understand the details of a task, and to make decisions about how a task should be completed.

The executor should be able to see a linearized "path" of work towards completing a given goal. They should be able to reorder tasks to use the metaphor of reading direction to articulate task ordering. But they should not be able to reorder it in such a way which violates the "blocking" relationships of the graph.

For contexts where multiple people are working on the same goal, they should be able to have independent paths generated towards the same work.

I'm less confident in the experience of the executor (because it's harder to put together!) but I think this is a space that can be iterated on.

# Extracting Useful Information

The graph provides another useful artifact: the shape of your work in the computer reflects the shape of your work in real life. You can leverage this property to extract interesting information from your task management system, particularly when its used by multiple people.

You can use graphs to answer interesting queries, like:

- How long will some task take? How parallelizable is it? What number of people is best to jointly optimize cost and speed? All assuming you have annotated time expectations.

- What work is most important to optimize finishing a workstream as long as possible? How do you address [long poles](https://devblogs.microsoft.com/oldnewthing/20080805-00/) as soon as possible?

- Etc.

Some of this can only reasonably be done using a graph (e.g. the parallelization question), and the rest of it will surely have higher quality data when your data source accurately represents the true state of the world.

I think more queries will grow organically through use, so I'm excited to build a system which can handle this work.

# My Own Hubris

To cap all of this off: I've decided to join the ranks before me and try my own hand at building some kind of graph-based task management software: [ekad](https://github.com/crockeo/ekad).

I'm starting off very simply: just make a CLI to interact with a task graph. Assuming I stick with it (which is a big assumption, considering my [long history of abandoning software](https://github.com/crockeo?tab=repositories)) the rough (very, *very* rough) roadmap is something like:

- **Explore the set of features I need using a CLI**,
  which is low effort to build and maintain.
  Use this to find out the right UX for myself
  as both *the planner* and *the executor*.

- **Build an interactive user interface** around this experience
  to reduce recurring key-presses and to keep important information in front of me.

- **Implement local-first synchronization**
  using something like [Automerge](https://automerge.org/).
  Preferably I would do this with peer-to-peer connections at first,
  and then provide the option of remote sync later.

# Questions, Comments, Concerns?

Please let me know if you have any thoughts about this work!
Especially:

- Let me know if you know other people working in this space.
- If I've missed something super obvious.
- Or if you have an interest in participating in this work.
