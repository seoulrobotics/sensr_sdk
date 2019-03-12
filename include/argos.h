#ifndef ARGOS_H
#define ARGOS_H

#include "argos_client.h"

namespace argos
{
enum Label
{
  Label_Car = 0,
  Label_Pedestrian = 1,
  Label_Cyclist = 2,
  Label_Misc = 3,
  Label_Max = 4
};
}

#endif // ARGOS_H