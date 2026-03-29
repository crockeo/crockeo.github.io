+++
title = "What Build if Free?"
description = "What would I build if I were free from the constraints of capital?"
date = "2026-03-29"
+++

# Preface

Before starting, I want to lay out my intent with this blog post.

- I'm frustrated with much software I use every day. I want to outline a kind of software I would like to use.

- I hope that in putting this out into the world, someone will tell me this already exists, and I can stop thinking about building it.

# Intro

Unfortunately I have to eat 😔, which means that I cannot build whatever I want. Unfortunately "wouldn't this be cool" is not a good monetization strategy. In fact for me "wouldn't it be cool" often sits in direct _opposition_ to monetization. The software I find most exciting removes centralized control, provides data ownership, and gives interoperability.

So, say money were not an issue. Maybe I become unimaginably rich. Maybe a nice VC who wants to throw away some money. What would I build?

# Themes

First, a grab bag:

<div style="display: grid; grid-template-columns: auto auto auto">
<ul>
  <li>Homelab</li>
  <li>Offline first</li>
  <li>Peer-to-peer</li>
</ul>
<ul>
  <li>Self-hostable</li>
  <li>Live multiplayer</li>
  <li>CRDTs</li>
</ul>
<ul>
  <li>Task management</li>
  <li>Planning</li>
  <li>Budgeting</li>
</ul>
</div>

These are split into two groups:

1. Technical requirements which support digital liberty.
2. Problems I happen to want to solve.

First I would make solving these problems easy (make a library), and then easily solve these problems (make some apps).

# Plan

All of the problems I want to solve can be structured as block storage with transactions and peer-to-peer sync.

## Building blocks

Define a `Block` to be composed of:

- A primary key `id`.
- A type identifier `type`.
- Timekeeping metadata like `created_at`, `modified_at`, `deleted_at`.
- A `blob` which is just a bunch of opaque bytes used by an app.

Define a `Machine` as a unique identifier which is stable for a given user-machine-app triplet.

## Layers

The library would be built out of layers:

- A storage medium which supports lookups and transactional writes.
- Peer discovery and auth.
- Pluggable transport mechanisms.
- A sync algorithm to trade updates back and forth.
- An approach to merging data.
- Good developer tooling.

### Storage

At rest a `Block` is stored in the storage layer. The storage layer needs to be able to do two things:

- Uniquely address a `Block` by its ID.
- Allow one to atomically update any number of `Blocks` at once.

You can imagine implementing this in any number of ways: A SQL database, a filesystem, cloud object storage.

This structure must give `O(1)` access to a given block based on its ID. Using this and transactional writes, you can support other more interesting kinds of lookups. You could create a `Block` which is an index. Or you could store a number of other blocks' keys.

### Peer discovery and auth

To start this would be very simple. Each `Machine` would catalog a set of peers it wanted to connect to, and a mechanism to trust that peer. Imagine a manifest which described IP addresses to connect to and that peer's base64 encoded public key. You can imagine implementing auth on this based on a [ed25519](https://en.wikipedia.org/wiki/EdDSA#Ed25519) challenge-response.

Later I would like to enable end-to-end encryption, so that one could share their data over an untrusted relay knowing that only _their_ nodes can access their data. I know that this is possible, but I don't know how to do it, so I'm ignoring it for now.

### Connecting to peers

I would want to make this system support multiple transport mechanisms. It should work on any transport mechanism that can send a logical stream of bytes. To I would target good old fashioned TCP (for most nodes) and websockets (for the web). It should be easy to support a new transport protocol.

### Sync algorithm

The sync algorithm would allow two peers to symmetrically exchange data, with either end initiating a connection. The sync algorithm would choose the minimum number of `Blocks` to send over.

At a high level the sync algorithm would look like:

- Two peers connect to each other.
- They select a leader.
  - Send and receive a randomly generated number.
  - Whoever gets a higher number is the leader.
  - Upon tie, try again.
- They negotiate a ser/de format
  - They send over ser/de format preferences as a set of format IDs and a float value in `[0, 1]`.
  - This is done in a single ser/de format that every node must support, like `json`.
  - They choose the option which jointly optimizes the preference.
  - If there is no single winner, the leader chooses randomly.
- They exchange metadata to enable selective synchronization.
  - What type of blocks does each side care about?
  - When was the last time they have synced with each other `Machine`?
- Using this information they each calculate which `Blocks` to stream based on type and recency of modification.
- They take turns (with the leader going first) sending all of their `Blocks` and the other reading.

I am a bit afraid of sync algorithms, because I know the cost of under-selecting `Blocks` to share. I'm sure the final version of this would be better thought through. Perhaps this would be a good place to use deterministic testing [like Antithesis](https://antithesis.com/).

### Merging data

This is the part where you expect me to say "everything is a CRDT," but I actually don't think that's the right approach!

Merging data should be an application-level concern. The core library only understands `Blocks` as containing an opaque series of bytes. The only conflict free algorithm we could implement is "last-write-wins," which is not suitable for many applications.

Instead, I want to let each application define a set of merging algorithms. Each one would apply to one or more types of `Block`. If the application contains such a merging algorith, it is applied. If it does not contain a merging algorithm, we use last-write-wins.

## Admonition

I know this isn't perfect! There are a couple of things I'm thinking about, even right after this stream of consciousness blog post.

- **How does live multiplayer work?** I'm thinking you just have a keepalive connection to a peer where you continuously stream updates to a set of `Blocks`. Who knows! 🤷

- **How do you evolve schemas (`Block` and `blob`)?** `Block` schema is hard, but at least for the blob perhaps we include a `version` tag. If someone sends a `version` that you do not support, you could just ignore the `Block` update entirely.

- **How does it _scale_?** I don't think it does! I don't know if it needs to. I'm concerned about unique identifiers for `Machines`, and each `Machine` having to track each other `Machine`. I plan to deploy these apps with at most 5 or 6 nodes (all of my and my wife's devices).

- **How do you do permissions?** I think this can be solved at the application layer. If you have listed a peer as trusted, presumably you trust the code it's running. You can mark certain `Blocks` as being owned by certain users, and preventing other users from updating them. A particular node could reject modifications to `Blocks` from `Machines` belonging to users who do not have write permissions, but this leaves the machine in an inconsistent state.

If you have more questions or critiques, please share them! I want this idea to be as good as it can be :)

# Apps

What could I build with this?

- **A relay** would blindly collect and relay `Blocks` from every `Machine` it can peer with. It would need to act a little differently, as it would have no "application layer" to perform merges. Instead it would need to store each most recent version of each `Block` as per each `Machine` who has synced to it.

- **A budgeting app** would be structured as 3 kinds of `Blocks`, with supporting indices. An account, a transfer, and a recurring transfer (which would generate a transfer on a recurring basis). One can import data from their financial providers using SimpleFIN.

- **A task manager and planner** would be structured as two kinds of `Blocks`. A task and a graph. A task would contain all of the metadata typically associated with a task in task management (title, description, due date, etc.). A graph would just contain connections between task IDs.
