SFML test
=========

This was originally going to be a quick isometric tile demo experiment and it has evolved horribly into some sort of homegrown 2D game engine written in C++ and SFML.

Things to do next
-----------------
* Some sort of "GameObject" class to unify everything that's drawable and updateable? Replacement for Entity class that can be shoved into a SceneGraphNode
   * this class should probably also be a scene graph node
   * Figure out a nice way to create a scene graph node interface. Is there a way to get rid of the extra layer of objects?

* make everything relative to in game units instead of pixels

* both of these things below: consider making physics parts composites?
   * need to upgrade physicscomponent to take list of bounding boxes
   * Work on adding animations
      * need to upgrade graphicscomponent to take in animations capability?

TODO
----

* Think about making some sort of exception service? Make it graphical; print to screen
 
* the Bridge pattern can be used to isolate sfml-test specific code from SFML library code

* optimize serialize and serializer interface

* set up configuration for player input to control mapping and graphics settings and stuff

* think about organizing map entities by position in quadtree datastructure or something
   * a quadtree or some sort of other frustrum culling structure?

* consider swapping out the lower level openGL library (SFML) for something else for more speed? (OOGL?)
   * need to redo facade on sfml drawable objects, because it is super bad

* Serialize map into an xml file instead of reading from handmade text file.

* Service locator is pretty ugly. Refactor?

* need to overhaul graphics classes or something because it's very clunky

* Flesh out UI compositor classes
   * need to decide a way to specify fixed percentage positioning etc?
