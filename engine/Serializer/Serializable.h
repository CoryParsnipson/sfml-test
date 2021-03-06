#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>

#include "Serializer.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Scene;

// ----------------------------------------------------------------------------
// Serializable
//
// Any game object that needs to be able to written to string form and/or
// re-constituted from a string should inherit this.
// ----------------------------------------------------------------------------
class Serializable {
public:
   virtual std::string serialize(Serializer& s) = 0;
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d) = 0;
};
#endif
