+++
title = "The Shape of Tools"
description = "Why some tools feel good and others feel bad."
date = "2024-07-14"
draft = true
+++

# Convex vs. Concave Tools

I like to think of tools as belonging on a gradient between
[convex](https://en.wikipedia.org/wiki/Convex_polygon)
and [concave](https://en.wikipedia.org/wiki/Concave_polygon).
To me these are useful terms,
because they represent physical manifestations
of how one feels using these tools.

A *convex tool* is one that's meant to be held,
and it knows its limits as "just a tool."
I call it convex because it it feels like an object.
Examples include:

- A hammer
- A pencil
- [Emacs](https://www.gnu.org/software/emacs/)
- [Tree-sitter](https://tree-sitter.github.io/tree-sitter/)
- [Linux](https://en.wikipedia.org/wiki/Linux).

(TODO: NOTE TO SELF, maybe turn this just from "switching" cost
to also making you engage with them at every level of product ownership)

A *concave tool* is one that means to hold you, or deceive you.
It often provides something genuinely very good,
and then like the [Angerfish](https://en.wikipedia.org/wiki/Anglerfish),
it uses this as lure to do something shady behind the scenes.
I call it concave because it feels like a shape that would wrap around you
and trick you from behind while you're not looking.
Examples include:

- Modern [John Deere tractors](https://www.deere.com/en/tractors/).
- The [Apple](https://www.apple.com/) ecosystem.
- Certain developer tooling startups, like [Graphite](https://graphite.dev/).

Also keep in mind that each of these tools are genuinely very good.
Modern tractor technology helps farmers improve yields,
helping the planet stay fed.
I'm writing this blog post on a Mac laptop.
I've used Graphite extensively at work and found it very useful
when you need to break up changes into multiple pieces.

However each of these tools *also* build up high switching costs
around their consumers over time.

- John Deere famously prevented farmers from repairing their own tractors,
  instead forcing them to engage with John Deere-licensed technicians.
  This drove more profit to John Deere through licensing programs
  and sales of certified parts,
  but forced farmers to be reliant on an external company for their farming.

- Apple... is Apple. They are famous for building a high-quality
  tightly integrated walled garden around their products.
  Do your friends use iMessage? You'd better too if you want blue bubbles!
  Do you have an Apple Watch? It's a shame that half of its features only work with iOS.

- Graphite is a strange one to include on this list.
  It's relatively unknown compared to the other companies,
  but it presents an interesting case study in concave tooling
  for business-to-business software as a service.
  Its core product is the [Graphite CLI](https://graphite.dev/docs/graphite-cli),
  which developers can use to [stack diffs](https://www.stacking.dev/)
  a la [Phabricator](https://en.wikipedia.org/wiki/Phabricator).
  Around this tool they have built:
  - A [code review tool](https://graphite.dev/features/inbox),
    to handle large numbers of related changes being pushed up together.
  - A [merge queue](https://graphite.dev/features/merge-queue),
    to integrate better with the concept of stacked PRs.
  - A [CI optimizer](https://graphite.dev/blog/ci-optimizer), 
    to help you save money on CI costs
    created by the Graphite CLI holding Git poorly.
  - The core tool solves a problem in a very obvious way,
    but creates many smaller problems,
    for which Graphite can sell you more solutions.

# Why do They Feel Different

- They feel the same until you've decided that you don't want to use them anymore.
- For a concave tool: that's fine! You drop it and you pull up another one.
- Personal anecdote:
  - I've swapped between Emacs, Vim, Neovim, Helix, and several other editors more times in the last several years than I can remember.
  - While there is a switching cost (I had to redo my config, relearn some keybindings, etc.) it is *naturally* imposed
  - AKA: It is the essential consequence of the action, not one designed to keep the switching cost high.
- For a convex tool: you run into problems!
- Again the switching cost *should* be the essential consequence, not something built specifically to make switching worse.
- For John Deere tractors: ???
- For Apple:
  - You lose the ability to chat with blue messages and use certain features.
    - TBH this was excusable back when everyone had their own protocols, or you just had to use SMS.
    - But once RCS was released this was kind of crazy!
  - Same thing with proprietary handshakes making you lose 
- For Graphite:
  - The core product is actually quite simple! I've made a clone in just like 4hrs of work or so.
  - The rest of the product suite is just tooling they've built to make the core product less shit.
- I think that's where the feeling comes from, it's a tacit understanding
  that someone else has decided to make your life worse for a product you've paid for.
  The actual cost of the switching matters less, I think, than the knowledge that someone chose for you to have a harder time switching.

# Why Make Concave Tools

- So if it feels worse for users...why do it?
- Money money money money money.
- Repurchases and subscriptions.
  - When you sell a product which someone has to buy once,
    you need to make sure they will want to buy it again
    when they need to repurchase.
    This is the Apple model,
    you need to want to buy an iPhone the next time you purchase a phone.
  - This is the same logic as a subscription,
    but the cadence of repurchase is much faster.
    Essentially every time that you would pay for a subscription
    you have the option to switch to something else.
  - At each of these repurchasing decisions,
    you need to make sure the customer wants to buy your product.
    You can do that in two ways:
    - Make your product better than the competition
    - Make the cost of switching products sufficiently hard
  - Ultimately you have to sit at the quality + difficulty > competition.
- Extracting more money
  - The other side of this is not just "preventing people from moving,"
    but also "how do you get more money from your existing customers."
  - This one is...complicated and will betray some of my underlying biases :)
  - Every publicly traded company has a duty to its shareholders to make "line go up,"
    and eventually "line go up" means making more money off of your users.
  - This is especially true in industries fueled by venture capital (e.g. tech!)
    because user bases can be developed independently of a product.
  - Eventually concerns for profitability come knocking and a company almost always
    has to do one or more of the following to get profitable:
    - Lower quality of service
    - Increase pricing
  - But also note that this doesn't sit alone!
    Making things worse or increasing the price makes people want to leave.
    Making it hard to leave keeps people around during these transitions.

# How to Fix

- There are multiple ways to think about "fixing"
  - One is "how do you fix it for everybody"
  - Another is "how do you not be complicit"
- For everybody is a bit hard! The one thing I can think of here is regulation.
  - E.g. "right to repair" regulation
  - Or having guidelines to avoid specific "dark patterns" (I wonder what'll happen from [ICPEN](https://icpen.org/news/1360)).
  - I don't really know how to solve this one, it's a bit beyond me and I'm not an expert here.
  - But from a first principles perspective, it feels like the right place for government.
    - It's a problem which affects almost all users of modern technology.
    - It is caused by tragedy of the commons-style behavior,
      where every company is incentivized to do this to stay ahead of the competition,
      but it ends up creating a race for the bottom.
- For individuals, I think there are more options:
  - Like, just don't! If you are working on a product which is going this direction,
    try to use your position inside of the company to push againt this.
    This is usually hard, and especially so if you don't have political capital at the company.
  - Try to make products which do not engage with venture capital.
    I know this mostly as [bootstrapping](https://www.investopedia.com/terms/b/bootstrapping.asp),
    but I'm sure there are other funding models!
