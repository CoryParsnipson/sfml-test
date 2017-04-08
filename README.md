SFML test
=========

This was originally going to be a quick isometric tile demo experiment and it has evolved horribly into some sort of homegrown 2D game engine written in C++ and SFML.

Things to do next
-----------------
* Refactor Entity class
   * need to overhaul graphics classes or something because it's very clunky
      * In the scene rendering, entity draw and entity update show most time spent in profiler
      * refactor entity component hierarchy
   * both of these things below: consider making physics parts composites?
      * need to upgrade physicscomponent to take list of bounding boxes
   * Work on adding animations
      * need to upgrade graphicscomponent to take in animations capability?

* Consider adding an array to PlayerGamepad that keeps track of which keys/mouse buttons
   are being pressed (this should be InputComponent?)

TODO
----
* make everything relative to in game units instead of pixels

* Think about making some sort of exception service? Make it graphical; print to screen
 
* the Bridge pattern can be used to isolate sfml-test specific code from SFML library code

* set up configuration for player input to control mapping and graphics settings and stuff

* think about organizing map entities by position in quadtree datastructure or something
   * a quadtree or some sort of other frustrum culling structure?

* consider swapping out the lower level openGL library (SFML) for something else for more speed? (OOGL?)

* Flesh out UI compositor classes
   * need to decide a way to specify fixed percentage positioning etc?
