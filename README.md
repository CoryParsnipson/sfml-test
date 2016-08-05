SFML test
=========

This was originally going to be a quick isometric tile demo experiment and it has evolved horribly into some sort of homegrown 2D game engine written in C++ and SFML.

Things to do next
-----------------
* Consider adding an array to PlayerGamepad that keeps track of which keys/mouse buttons
   are being pressed

* need to overhaul graphics classes or something because it's very clunky
   * the graphics drawing is already a performance bottleneck

* Create an entity that contains a Gamepad instance? (For drawing mouse cursor and stuff)

* Change Scene::draw to use a visitor that builds global transform instead of doing it in SceneObject?

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

* Put game specific scenes and game specific commands in a different folder hierarchy

* Flesh out UI compositor classes
   * need to decide a way to specify fixed percentage positioning etc?
