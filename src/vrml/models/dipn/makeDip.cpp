/*
  file: makeDip.cpp

  Copyright 2012, Cirilo Bernardo (cjh.bernardo@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>

  Notes:
    This is a quick and dirty test of the dip class for
    creating DIPn VRML models for use with KiCad.

    TODO:
        + implement batch processing by providing the an input file
            on the command line or optionally streaming an input file
            via stdin.
        + the input file will contain immutable data controlling the
            dimensions of the DIPn package and pins.
        + the input file will contain a comma separated list of the
            number of pins on each package to be generated.
        + The output file name shall be dipN + SUFFIX; the suffix shall
            be a member of the struct 'dipparams'. In the test code
            the suffix is coerced based on the setting of the `metric'
            parameter and the value of the pin row spacing.
 */

#include <stdio.h>

#include "dip.h"
#include "transform.h"

#include "k3dconfig.h"

using namespace kc3ddip;

int main()
{
  Dip dil;
  char name[256];
  int i;
  DipParams dp;
  dil.SetCaseColor(K3D_TOP_DIR "/mcad/colors/ceram_gry.mat");
  dil.SetPinColor(K3D_TOP_DIR "/mcad/colors/tin.mat");
  dil.SetParams(dp);

  for(i = 4; i <= 28; i+=2)
  {
    sprintf(name, "dil%d_i300.wrl", i);
    dil.SetPins(i);
    dil.Build(name);
  }

  /*
  // demo of the pin skipping function
  int j;
  for(i = 4; i <= 28; i+=2)
  {
    sprintf(name, "dil%d_i300.wrl", i);
    dil.SetPins(i);
    for (j = 3; j <= (i/2 -2); ++j)
    {
        dil.SetPin(j, false);
        dil.SetPin(i - j +1, false);
    }
    dil.Build(name);
  }
  */

  return 0;
}
