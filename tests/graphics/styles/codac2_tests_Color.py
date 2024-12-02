#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou, Maël GODARD
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class ArrayMatcherWithTolerance:
    def __init__(self, expected, tolerance=1):
        self.expected = expected
        self.tolerance = tolerance

    def match(self, actual):
        if len(actual) != len(self.expected):
            return False
        for i in range(len(self.expected)):
            if abs(actual[i] - self.expected[i]).mid() > self.tolerance:
                return False
        return True

    def describe(self):
        return f'is close to {self.expected} with tolerance {self.tolerance}'


import re

class HtmlColorMatcherWithTolerance:
    def __init__(self, expected: str, tolerance: int = 1):
        assert self._is_valid_color(expected), f"Invalid color format: {expected}"
        self.expected = expected
        self.tolerance = tolerance

    def _is_valid_color(self, color: str) -> bool:
        """Validate the color format as #RRGGBBAA or #RRGGBB."""
        return bool(re.match(r"^#[0-9A-Fa-f]{6}([0-9A-Fa-f]{2})?$", color))

    def _hex_to_components(self, hex_color: str):
        """Convert a #RRGGBBAA or #RRGGBB color to a tuple of integers."""
        if len(hex_color) == 7:  # #RRGGBB
            return tuple(int(hex_color[i:i+2], 16) for i in range(1, 7, 2))
        elif len(hex_color) == 9:  # #RRGGBBAA
            return tuple(int(hex_color[i:i+2], 16) for i in range(1, 9, 2))

    def match(self, actual: str) -> bool:
        """Check if the actual color matches the expected color within the tolerance."""
        if not self._is_valid_color(actual):
            return False

        expected_components = self._hex_to_components(self.expected)
        actual_components = self._hex_to_components(actual)

        if len(expected_components) != len(actual_components):
            return False

        return all(abs(e - a).mid() <= self.tolerance for e, a in zip(expected_components, actual_components))

    def describe(self) -> str:
        """Provide a description of the matcher."""
        return f"is close to {self.expected} with tolerance {self.tolerance}"

    
def is_close_to(expected, tolerance=1):
    return ArrayMatcherWithTolerance(expected, tolerance)

def is_close_to_html_color(expected, tolerance=1):
    return HtmlColorMatcherWithTolerance(expected, tolerance)

class TestColor(unittest.TestCase):
    
    def test_Color(self):
        # Red

        d_rgb = [255, 0, 0]
        d_rgba = [255, 0, 0, 255]
        d_hsv = [0, 100, 100]
        d_hsva = [0, 100, 100, 100]
        d_rgb_f= [1.0, 0.0, 0.0]
        d_rgba_f= [1.0, 0.0, 0.0, 1.0]
        d_hsv_f= [0.0, 1.0, 1.0]
        d_hsva_f= [0.0, 1.0, 1.0, 1.0]


        colors = [
            Color(d_rgb, InterpolMode.RGB),
            Color(d_rgba, InterpolMode.RGB),
            Color(255, 0, 0, interpol_mode=InterpolMode.RGB),
            Color(255, 0, 0, 255, InterpolMode.RGB),
            Color(d_hsv, InterpolMode.HSV),
            Color(d_hsva, InterpolMode.HSV),
            Color(0, 100, 100,  interpol_mode=InterpolMode.HSV),
            Color(0, 100, 100, 100, InterpolMode.HSV),
            Color(d_rgb_f, InterpolMode.RGB),
            Color(d_rgba_f, InterpolMode.RGB),
            Color(1.0, 0.0, 0.0, interpol_mode=InterpolMode.RGB),
            Color(1.0, 0.0, 0.0, 1.0, InterpolMode.RGB),
            Color(d_hsv_f, InterpolMode.HSV),
            Color(d_hsva_f, InterpolMode.HSV),
            Color(0.0, 1.0, 1.0,  interpol_mode=InterpolMode.HSV),
            Color(0.0, 1.0, 1.0, 1.0, InterpolMode.HSV),
            Color("#FF0000")
        ]

        for c in colors:
            self.assertTrue(is_close_to_html_color("#FF0000").match(c.hex_str()))
            self.assertTrue(is_close_to([255, 0, 0]).match(c.rgb()))
            self.assertTrue(is_close_to([255, 0, 0, 255]).match(c.rgba()))
            self.assertTrue(is_close_to([0, 100, 100]).match(c.hsv()))
            self.assertTrue(is_close_to([0, 100, 100, 100]).match(c.hsva()))

        # Pink full opacity

        d_rgb = [229,128,255]
        d_rgba = [229,128,255,255]
        d_hsv = [288,50,100]
        d_hsva = [288,50,100,100]
        d_rgb_f = [229.0/255.0, 128.0/255.0, 255.0/255.0]
        d_rgba_f = [229.0/255.0, 128.0/255.0, 255.0/255.0, 1.0]
        d_hsv_f = [288.0/360.0, 50.0/100.0, 100.0/100.0]
        d_hsva_f = [288.0/360.0, 50.0/100.0, 100.0/100.0, 1.0]

        colors = [
            Color(d_rgb, InterpolMode.RGB),
            Color(d_rgba, InterpolMode.RGB),
            Color(229,128,255,  interpol_mode=InterpolMode.RGB),
            Color(229,128,255,255, InterpolMode.RGB),
            Color(d_hsv, InterpolMode.HSV),
            Color(d_hsva, InterpolMode.HSV),
            Color(288,50,100,  interpol_mode=InterpolMode.HSV),
            Color(288,50,100,100, InterpolMode.HSV),
            Color(d_rgb_f, InterpolMode.RGB),
            Color(d_rgba_f, InterpolMode.RGB),
            Color(229.0/255.0, 128.0/255.0, 255.0/255.0, interpol_mode=InterpolMode.RGB),
            Color(229.0/255.0, 128.0/255.0, 255.0/255.0, 1.0, InterpolMode.RGB),
            Color(d_hsv_f, InterpolMode.HSV),
            Color(d_hsva_f, InterpolMode.HSV),
            Color(288.0/360.0, 50.0/100.0, 100.0/100.0,  interpol_mode=InterpolMode.HSV),
            Color(288.0/360.0, 50.0/100.0, 100.0/100.0, 1.0, InterpolMode.HSV),
            Color("#E580FF")
        ]

        for c in colors:
            self.assertTrue(is_close_to_html_color("#E580FF").match(c.hex_str()))
            self.assertTrue(is_close_to([229,128,255]).match(c.rgb()))
            self.assertTrue(is_close_to([229,128,255,255]).match(c.rgba()))
            self.assertTrue(is_close_to([288,50,100]).match(c.hsv()))
            self.assertTrue(is_close_to([288,50,100,100]).match(c.hsva()))

        # Pink 40% opacity

        a_rgb=102
        a_hsv=40
        d_rgba = [229,128,255,a_rgb]
        d_hsva = [288,50,100,a_hsv]
        d_rgba_f = [229.0/255.0, 128.0/255.0, 255.0/255.0, 102.0/255.0]
        d_hsva_f = [288.0/360.0, 50.0/100.0, 100.0/100.0, 40.0/100.0]

        colors = [
            Color(d_rgba, InterpolMode.RGB),
            Color(229,128,255,a_rgb, InterpolMode.RGB),
            Color(d_hsva, InterpolMode.HSV),
            Color(288,50,100,a_hsv, InterpolMode.HSV),
            Color(d_rgba_f, InterpolMode.RGB),
            Color(229.0/255.0, 128.0/255.0, 255.0/255.0, 102.0/255.0, InterpolMode.RGB),
            Color(d_hsva_f, InterpolMode.HSV),
            Color(288.0/360.0, 50.0/100.0, 100.0/100.0, 40.0/100.0, InterpolMode.HSV),
            Color("#E580FF66")
        ]
        for c in colors:
            self.assertTrue(is_close_to_html_color("#E580FF66").match(c.hex_str()))
            self.assertTrue(is_close_to([229,128,255]).match(c.rgb()))
            self.assertTrue(is_close_to([229,128,255,a_rgb]).match(c.rgba()))
            self.assertTrue(is_close_to([288,50,100]).match(c.hsv()))
            self.assertTrue(is_close_to([288,50,100,a_hsv]).match(c.hsva()))




if __name__ ==  '__main__':
  unittest.main()