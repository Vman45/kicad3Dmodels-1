/*
 *      file: vcom.cpp
 *
 *      Copyright 2012 Dr. Cirilo Bernardo (cjh.bernardo@gmail.com)
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include <iostream>
#include <fstream>
#include <iomanip>

#include "vdefs.h"
#include "vcom.h"
#include "vrmlmat.h"

using namespace std;

#define MAX_TABS (6)

namespace kc3d {

// Create the VRML header and ancillary comments
int SetupVRML(std::string filename, std::ofstream &fp)
{
    if (filename.empty())
    {
        ERRBLURB;
        cerr << "empty filename\n";
        return -1;
    }

    if (fp.is_open()) fp.close();

    fp.open(filename.c_str());
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "could not open file '" << filename << "'\n";
        return -1;
    }

    fp << "#VRML V2.0 utf8\n";
    fp << "#File: " << filename << "\n";
    fp << "#License: The contents of this file were generated by software;\n";
    fp << "#    the user is free to use, modify, and distribute this file\n";
    fp << "#    without restrictions.\n\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file '" << filename << "'\n";
        fp.close();
        return -1;
    }
    return 0;
}


// Set up a VRML Transform block
int SetupXForm(std::string name, std::ofstream &fp, int tabs)
{
    if (name.empty())
    {
        ERRBLURB;
        cerr << "empty VRML Transform name\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML Transform '" << name << "'\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    fp << fmt << "DEF " << name << " Transform {\n";
    fp << fmt << "    children [\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}

// Close a VRML Transform block
int CloseXForm(std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML Transform\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    // close Children
    fp << fmt << "    ]\n";
    // close Transform
    fp << fmt << "}\n\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}


// Set up a VRML Shape block (includes appearance and geometry)
int SetupShape(VRMLMat &color, bool reuse_color,
        std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML Shape\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');
    fp << fmt << "Shape {\n";
    fp << fmt << "    appearance Appearance {\n";
    if (!color.GetName().empty())
    {
        if (!reuse_color)
        {
            color.WriteMaterial(fp, tabs + 2);
        }
        else
        {
            fp << fmt << "        material USE " << color.GetName() << "\n";
        }
    }
    else
    {
        ERRBLURB;
        cerr << "invalid material appearance; using default bright red\n";
        fp << fmt << "        material Material {\n";
        fp << fmt << "            diffuseColor 1 0 0\n";
        fp << fmt << "            emissiveColor 1 0 0\n";
        fp << fmt << "            specularColor 1 0 0\n";
        fp << fmt << "            ambientIntensity 1\n";
        fp << fmt << "            transparency 0\n";
        fp << fmt << "            shininess 0.5\n";
        fp << fmt << "        }\n";
    }
    fp << fmt << "    }\n";
    fp << fmt << "    geometry IndexedFaceSet {\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}

// Close a VRML Shape and Geometry block
int CloseShape(std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML Shape/Geometry\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    fp << fmt << "    }\n";
    fp << fmt << "}\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}


// Write a VRML coordinate block
int WriteCoord(double *x, double *y, double *z, int np, std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML Coord\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (np < 3)
    {
        ERRBLURB;
        cerr << "invalid number of coordinate points (" << np << "); must be >= 3\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    fp << fmt << "coord Coordinate { point [\n";
    fp << fmt << "   ";
    int i;
    double tx, ty, tz;
    for (i = 0; i < (np -1); ++i)
    {
        tx = x[i];
        ty = y[i];
        tz = z[i];
        if ((tx < 1e-9) && (tx > -1e-9)) tx = 0;
        if ((ty < 1e-9) && (ty > -1e-9)) ty = 0;
        if ((tz < 1e-9) && (tz > -1e-9)) tz = 0;
        fp << setprecision(8) << " " << tx << " " << ty << " " << tz << ",";
        if (!((i + 1) % 6))
            fp << "\n" << fmt << "   ";
    }
    tx = x[i];
    ty = y[i];
    tz = z[i];
    if ((tx < 1e-9) && (tx > -1e-9)) tx = 0;
    if ((ty < 1e-9) && (ty > -1e-9)) ty = 0;
    if ((tz < 1e-9) && (tz > -1e-9)) tz = 0;
    fp << setprecision(8) << " " << tx << " " << ty << " " << tz << " ]\n";
    fp << fmt << "}\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}


// Set up a VRML coordIndex block
int SetupCoordIndex(std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML CoordIndex\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    fp << fmt << "coordIndex [\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}

// Close a VRML coordIndex block
int CloseCoordIndex(std::ofstream &fp, int tabs)
{
    if (!fp.good())
    {
        ERRBLURB;
        cerr << "bad stream for VRML CoordIndex\n";
        return -1;
    }

    if (!fp.is_open())
    {
        ERRBLURB;
        cerr << "no open file\n";
        return -1;
    }

    if (tabs < 0) tabs = 0;
    if (tabs > MAX_TABS) tabs = MAX_TABS;
    string fmt(tabs*4, ' ');

    fp << fmt << "]\n";

    if (!fp.good())
    {
        ERRBLURB;
        cerr << "error writing to file\n";
        return -1;
    }
    return 0;
}

}   // namespace kc3d
