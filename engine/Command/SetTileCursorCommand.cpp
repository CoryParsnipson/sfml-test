#include "SetTileCursorCommand.h"
#include "Grid.h"
#include "Entity.h"

SetTileCursorCommand::SetTileCursorCommand(Grid& grid, const std::shared_ptr<UpdateSelectionRectCommand>& usr_command, Entity* tile_cursor)
: tile_cursor_(tile_cursor)
, usr_command_(usr_command)
, utc_command_(grid, tile_cursor)
{
}

SetTileCursorCommand::~SetTileCursorCommand() {
}

void SetTileCursorCommand::execute() {
   if (!this->usr_command_) {
      return;
   }

   sf::Vector2f* click_pos = this->usr_command_->click_pos();
   sf::Vector2f* release_pos = this->usr_command_->release_pos();

   if (click_pos) {
      this->utc_command_.click_pos(this->tile_cursor_->get_screen_coordinate(*click_pos));
   }
   
   if (release_pos) {
      this->utc_command_.release_pos(this->tile_cursor_->get_screen_coordinate(*release_pos));
   }

   this->utc_command_.execute();
}

void SetTileCursorCommand::unexecute() {
   this->utc_command_.unexecute();
}
