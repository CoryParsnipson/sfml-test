#ifndef UPDATE_SELECTION_RECT_COMMAND
#define UPDATE_SELECTION_RECT_COMMAND

#include "dependencies.h"
#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Entity;

// ----------------------------------------------------------------------------
// UpdateSelectionRectCommand
//
// Set the size and position of a provided selection rectangle entity.
// ----------------------------------------------------------------------------
class UpdateSelectionRectCommand : public Command {
public:
   UpdateSelectionRectCommand(Entity* selection_rect);
   virtual ~UpdateSelectionRectCommand();

   virtual void execute();
   virtual void unexecute();

   void click_pos(sf::Vector2f pos);
   void click_pos(float x, float y);

   sf::Vector2f* click_pos();

   void release_pos(sf::Vector2f pos);
   void release_pos(float x, float y);

   sf::Vector2f* release_pos();

   void visible(bool visible);
   
private:
   bool set_visible_;

   Entity* selection_rect_;
   Entity* old_selection_rect_;

   sf::Vector2f* click_pos_;
   sf::Vector2f* release_pos_;
};

#endif
