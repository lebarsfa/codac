#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou, Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *


class TestColor(unittest.TestCase):
    
    def test_Color(self):
        # Red

        d_rgb = [255, 0, 0]
        d_rgba = [255, 0, 0, 255]
        d_hsv = [0, 100, 100]
        d_hsva = [0, 100, 100, 100]


        colors = [
            Color(d_rgb, Model.RGB),
            Color(d_rgba, Model.RGB),
            Color(d_hsv, Model.HSV),
            Color(d_hsva, Model.HSV),
            Color("#FF0000")
        ]

        for c in colors:
            self.assertTrue(Color([255, 0, 0]).rgb().vec().match(Approx(c.rgb().vec(), 1.0)))

        # # Pink full opacity

        # d_rgb = [229,128,255]
        # d_rgba = [229,128,255,255]
        # d_hsv = [288,50,100]
        # d_hsva = [288,50,100,100]
        # d_rgb_f = [229.0/255.0, 128.0/255.0, 255.0/255.0]
        # d_rgba_f = [229.0/255.0, 128.0/255.0, 255.0/255.0, 1.0]
        # d_hsv_f = [288.0/360.0, 50.0/100.0, 100.0/100.0]
        # d_hsva_f = [288.0/360.0, 50.0/100.0, 100.0/100.0, 1.0]

        # colors = [
        #     Color(d_rgb, Model.RGB),
        #     Color(d_rgba, Model.RGB),
        #     Color(229,128,255,  interpol_mode=Model.RGB),
        #     Color(229,128,255,255, Model.RGB),
        #     Color(d_hsv, Model.HSV),
        #     Color(d_hsva, Model.HSV),
        #     Color(288,50,100,  interpol_mode=Model.HSV),
        #     Color(288,50,100,100, Model.HSV),
        #     Color(d_rgb_f, Model.RGB),
        #     Color(d_rgba_f, Model.RGB),
        #     Color(229.0/255.0, 128.0/255.0, 255.0/255.0, interpol_mode=Model.RGB),
        #     Color(229.0/255.0, 128.0/255.0, 255.0/255.0, 1.0, Model.RGB),
        #     Color(d_hsv_f, Model.HSV),
        #     Color(d_hsva_f, Model.HSV),
        #     Color(288.0/360.0, 50.0/100.0, 100.0/100.0,  interpol_mode=Model.HSV),
        #     Color(288.0/360.0, 50.0/100.0, 100.0/100.0, 1.0, Model.HSV),
        #     Color("#E580FF")
        # ]

        # for c in colors:
        #     self.assertTrue(is_close_to_html_color("#E580FF").match(c.hex_str()))
        #     self.assertTrue(is_close_to([229,128,255]).match(c.rgb()))
        #     self.assertTrue(is_close_to([229,128,255,255]).match(c.rgba()))
        #     self.assertTrue(is_close_to([288,50,100]).match(c.hsv()))
        #     self.assertTrue(is_close_to([288,50,100,100]).match(c.hsva()))

        # # Pink 40% opacity

        # a_rgb=102
        # a_hsv=40
        # d_rgba = [229,128,255,a_rgb]
        # d_hsva = [288,50,100,a_hsv]
        # d_rgba_f = [229.0/255.0, 128.0/255.0, 255.0/255.0, 102.0/255.0]
        # d_hsva_f = [288.0/360.0, 50.0/100.0, 100.0/100.0, 40.0/100.0]

        # colors = [
        #     Color(d_rgba, Model.RGB),
        #     Color(229,128,255,a_rgb, Model.RGB),
        #     Color(d_hsva, Model.HSV),
        #     Color(288,50,100,a_hsv, Model.HSV),
        #     Color(d_rgba_f, Model.RGB),
        #     Color(229.0/255.0, 128.0/255.0, 255.0/255.0, 102.0/255.0, Model.RGB),
        #     Color(d_hsva_f, Model.HSV),
        #     Color(288.0/360.0, 50.0/100.0, 100.0/100.0, 40.0/100.0, Model.HSV),
        #     Color("#E580FF66")
        # ]
        # for c in colors:
        #     self.assertTrue(is_close_to_html_color("#E580FF66").match(c.hex_str()))
        #     self.assertTrue(is_close_to([229,128,255]).match(c.rgb()))
        #     self.assertTrue(is_close_to([229,128,255,a_rgb]).match(c.rgba()))
        #     self.assertTrue(is_close_to([288,50,100]).match(c.hsv()))
        #     self.assertTrue(is_close_to([288,50,100,a_hsv]).match(c.hsva()))



if __name__ ==  '__main__':
  unittest.main()