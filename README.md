SFML test
=========

This was originally going to be a quick isometric tile demo experiment and it has evolved horribly into some sort of homegrown 2D game engine written in C++ and SFML.

Setup
=====

1. Install SFML.

   On linux, with a distro that has a package manager, this is simple. sudo apt-get install sfml or something. For fedora specifically, this is "sudo dnf install SFML SFML-devel".

2. Install boost. (sudo dnf install boost boost-devel)

3. Run the makefile ("make" while in the same folder as Makefile)

TODO
----
* Encapsulate sf::Font and sf::Color into wrapper classes (for serialization and stuff)
* Work on adding animations
   * need to upgrade graphicscomponent to take in animations capability?

* Jesus fucking christ this scene graph bullshit with the "global transform" and having to calculate the global transform of every entity when swapping back and forth between screen/world space is killing me. FUCK

* Optimize Messaging system - need to add ability to send point to point messages instead of broadcast all the time. Optimization opportunity to have all messageables share a single mailbox?

* Refactor EntitySubscription - seriously this whole concept is terrible. Find a better way to iterate through entities while updating systems.

* Rewrite ObjectPool class to dynamically allocate a block of memory and in place new everything to get rid of allocator requirement

* make everything relative to in game units instead of pixels

* Think about making some sort of exception service? Make it graphical; print to screen
 
* the Bridge pattern can be used to isolate sfml-test specific code from SFML library code

* set up configuration for player input (Settings) to control mapping and graphics settings and stuff

* think about organizing map entities by position in quadtree datastructure or something
   * a quadtree or some sort of other frustrum culling structure?

* consider swapping out the lower level openGL library (SFML) for something else for more speed? (OOGL?)

* Flesh out UI compositor classes
   * need to decide a way to specify fixed percentage positioning etc?
