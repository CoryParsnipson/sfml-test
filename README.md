SFML test
=========

This was originally going to be a quick isometric tile demo experiment and it has evolved horribly into some sort of homegrown 2D game engine written in C++ and SFML.

TODO
----
* Encapsulate sf::Font and sf::Color into wrapper classes (for serialization and stuff)
* Consider having Entities keep track of their own handle -> useful for ComponentAddedMessage and other things
* Add "Handle root" to System class to constrain them to a specific section of the scene graph?
* Create new components
   * both of these things below: consider making physics parts composites?
      * need to upgrade physicscomponent to take list of bounding boxes
   * Work on adding animations
      * need to upgrade graphicscomponent to take in animations capability?

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
