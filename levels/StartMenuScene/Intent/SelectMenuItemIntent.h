#ifndef SELECT_MENU_ITEM_INTENT_H
#define SELECT_MENU_ITEM_INTENT_H

#include "Intent.h"

// ----------------------------------------------------------------------------
// SelectMenuItemIntent
// 
// This intent signals that the player is choosing a menu item.
// ----------------------------------------------------------------------------
class SelectMenuItemIntent : public Intent {
public:
   SelectMenuItemIntent(InputDevice::DeviceId device_id, InputElementPtr input_element)
   : Intent("SelectMenuItemIntent", device_id, input_element)
   {
   }

   virtual ~SelectMenuItemIntent() {}
};

#endif
