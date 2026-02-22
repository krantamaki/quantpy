"""
The setup script that compiles the C++ libraries and adds the quantpy into the Python site-packages 
directory so that it can be imported as a library.
"""
from setuptools import setup


# Run the setup
setup(
  name             = "quantpy",
  version          = "0.0.1",
  description      = "Efficient Python package for common quantitative finance calculations",
  url              = "https://github.com/krantamaki/quantpy",
  author           = "Kasper Rantamäki",
  license          = "All rights are reserved",
  packages         = ["quantpy", 
                      "quantpy.instruments",
                      "quantpy.instruments.ZeroCouponBond",
                      "quantpy.instruments.ZeroCouponBond.pricers",
                      "quantpy.instruments.CouponBond",
                      "quantpy.math",
                      "quantpy.math.curve"
                      ],
  install_requires = ["pandas",
                      "numpy",
                      "matplotlib",
                      "scipy",
                      "quantlib"
                     ]
)