/** 
 *  \file codac2_ColorMap.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <cmath>
#include "codac2_Color.h"
#include "codac2_Interval.h"
#include"codac2_assert.h"

namespace codac2
{
  /**
   * \struct ColorMap
   * \brief Represents a set of RGB values
   */
  struct ColorMap : public std::map<float,Color>
  {

    Color color (float r) const;

    static ColorMap haxby()
    {
      return ColorMap({{0,Color({39.,90.,211.})},
                        {1,Color({40.,123.,245.})},
                        {2,Color({45.,155.,253.})},
                        {3,Color({73.,209.,255.})},
                        {4,Color({100.,230.,254.})},
                        {5,Color({118.,235.,226.})},
                        {6,Color({135.,236.,187.})},
                        {7,Color({194.,252.,165.})},
                        {8,Color({217.,251.,151.})},
                        {9,Color({233.,241.,131.})},
                        {10,Color({252.,201.,96.})},
                        {11,Color({255.,184.,84.})},
                        {12,Color({255.,170.,75.})},
                        {13,Color({255.,167.,83.})},
                        {14,Color({255.,200.,158.})},
                        {15,Color({255.,233.,217.})}
                        });

      
    }

    static ColorMap basic() // Can't use default as name
    {
      return ColorMap({{0,Color({10.,0.,121.})},
                        {1,Color({40.,0.,150.})},
                        {2,Color({20.,5.,175.})},
                        {3,Color({0.,10.,200.})},
                        {4,Color({0.,25.,212.})},
                        {5,Color({0.,40.,224.})},
                        {6,Color({26.,102.,240.})},
                        {7,Color({13.,129.,248.})},
                        {8,Color({25.,175.,255.})},
                        {9,Color({50.,190.,255.})},
                        {10,Color({68.,202.,255.})},
                        {11,Color({97.,225.,240.})},
                        {12,Color({106.,235.,225.})},
                        {13,Color({124.,235.,200.})},
                        {14,Color({138.,236.,174.})},
                        {15,Color({172.,245.,168.})},
                        {16,Color({205.,255.,162.})},
                        {17,Color({223.,245.,141.})},
                        {18,Color({240.,236.,121.})},
                        {19,Color({247.,215.,104.})},
                        {20,Color({255.,189.,87.})},
                        {21,Color({255.,160.,69.})},
                        {22,Color({244.,117.,75.})},
                        {23,Color({238.,80.,78.})},
                        {24,Color({255.,90.,90.})},
                        {25,Color({255.,124.,124.})},
                        {26,Color({255.,158.,158.})},
                        {27,Color({245.,179.,174.})},
                        {28,Color({255.,196.,196.})},
                        {29,Color({255.,215.,215.})},
                        {31,Color({255.,235.,235.})},
                        {32,Color({255.,254.,253.})}
                        });
    }

    static ColorMap blue_tube()
    {
      return ColorMap({{0,Color({76.,110.,127.})},
                        {1,Color({136.,197.,228.})}
                        });
    }

    static ColorMap red_tube()
    {
      return ColorMap({{0,Color({169.,55.,0.})},
                        {1,Color({241.,140.,54.})}
                        });
    }

    static ColorMap rainbow()
    {
      ColorMap cmap;
      int i = 0;
      for(int h = 300 ; h > 0 ; h-=10)
      {
        cmap[i]=Color({(float)h,100.,100.},Color::HSV);
        i++;
      }
      return cmap;
    }

  };
}