#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 19 16:07:48 2019

@author: pantale
"""

import dnlBench

test = dnlBench.Benchmark('Shear-Elastic')  
test.setPlotFiles(['Stress.plot'])
test.setTolerance(1e-1)
test.run()
