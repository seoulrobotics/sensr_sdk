#ifndef SENSR_H
#define SENSR_H

#include "sensr_client.h"

namespace sensr
{
enum LabelType
{
  Car = 0,
  Pedestrian = 1,
  Cyclist = 2,
  Misc = 3,
  Max = 4
};
}

#endif // SENSR_H