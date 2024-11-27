#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)


import unittest
from codac import *

class TestInversion(unittest.TestCase):

  def tests_Inversion(self):

    x = Matrix([
      [ 1, 2, 0 ],
      [ 3, 4, 1 ],
      [ 0, 1, 0 ],
    ])

    y = inverse_enclosure(x)


    self.assertTrue((x*y).contains(Matrix.eye(3,3)));


if __name__ ==  '__main__':
  unittest.main()
