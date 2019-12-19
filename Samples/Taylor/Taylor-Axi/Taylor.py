#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

# Parameters of the model
nbreSaves = 20
stopTime = 80.0e-6
nbrePoints = 250 
width = 3.2
heigh = 32.4
nbElementsWidth = 5
nbElementsHeigh = 50
speed = 287000

# Material parameters
young = 206000
poisson = 0.3
density = 7.83e-09
heatCapacity = 4.6e+08
taylorQuinney = 0.9
A = 806.0
B = 614.0
C = 0.0089
n = 0.168
m = 1.1
depsp0 = 1.0
Tm = 1540.0
T0 = 20.0

dxWidth = width/(nbElementsWidth);
dxHeigh = heigh/(nbElementsHeigh);

# Creates the main Object
dynELA = dnl.DynELA("Taylor")

# Creates the Nodes
nbNodes = 1
allNS = dnl.NodeSet("NS_All")
for j in range (nbElementsHeigh+1): 
    for i  in range (nbElementsWidth+1):
        dynELA.createNode(nbNodes, i*dxWidth, j*dxHeigh, 0.00)
        dynELA.add(allNS, nbNodes)
        nbNodes += 1
nbNodes -= 1
print("Number of nodes created:", dynELA.getNodesNumber())    

# Creates the Elements
dynELA.setDefaultElement(dnl.Element.ElQua4NAx)
nbElements = 1
allES = dnl.ElementSet("ES_All")
for j in range (nbElementsHeigh):
    for i in range (nbElementsWidth):
        n1 = (i+(j*(nbElementsWidth+1))+1)
        n2 = (i+(j*(nbElementsWidth+1))+2)
        n3 = (i+((j+1)*(nbElementsWidth+1))+2)
        n4 = (i+((j+1)*(nbElementsWidth+1))+1)
        dynELA.createElement(nbElements, n1, n2, n3, n4)
        dynELA.add(allES, nbElements)
        nbElements += 1
nbElements -= 1
print("Number of elements created:",dynELA. getElementsNumber())    

bottomNS=dnl.NodeSet("NS_Bottom")
dynELA.add(bottomNS,1,nbElementsWidth+1)

axisNS=dnl.NodeSet("NS_Axis")
dynELA.add(axisNS,1,nbNodes,nbElementsWidth+1)

histNS = dnl.NodeSet("NS_Hist")
dynELA.add(histNS, 1)

histES = dnl.ElementSet("ES_Hist")
dynELA.add(histES, 1)

# Creates the hardening law
hardLaw = dnl.JohnsonCookLaw()
hardLaw.setParameters(A, B, C, n, m, depsp0, Tm, T0)

# Creates the material
steel = dnl.Material("Steel")
steel.setHardeningLaw(hardLaw)
steel.youngModulus = young
steel.poissonRatio = poisson
steel.density = density
steel.heatCapacity = heatCapacity
steel.taylorQuinney = taylorQuinney
steel.initialTemperature = T0

dynELA.add(steel,allES)

# Declaration of a boundary condition for bottom line
bottomBC = dnl.BoundaryRestrain('BC_bottom')
bottomBC.setValue(0,1,1)
dynELA.attachConstantBC(bottomBC,bottomNS)

# Declaration of a boundary condition for axis line
axisBC = dnl.BoundaryRestrain('BC_axis')
axisBC.setValue(1,0,1)
dynELA.attachConstantBC(axisBC,axisNS)

# Declaration of the initial speed
speedBC = dnl.BoundarySpeed('BC_speed')
speedBC.setValue(0, -speed, 0)
dynELA.attachInitialBC(speedBC,allNS)

solver = dnl.Explicit("Solver")
solver.setTimes(0,stopTime)
dynELA.add(solver)
solver.setComputeTimeStepFrequency(1)
#solver.setTimeStepSafetyFactor(1.09)
dynELA.setSaveTimes(0,stopTime,stopTime/nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile("vonMisesHistory")
vonMisesHist.setFileName(dnl.String("vonMises.plot"))
vonMisesHist.add(histES, 0, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime / nbrePoints)
dynELA.add(vonMisesHist)

plasticStrainHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainHist.setFileName(dnl.String("plasticStrain.plot"))
plasticStrainHist.add(histES, 0, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime / nbrePoints)
dynELA.add(plasticStrainHist)

temperatureHist = dnl.HistoryFile("temperatureHistory")
temperatureHist.setFileName(dnl.String("temperature.plot"))
temperatureHist.add(histES, 0, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime / nbrePoints)
dynELA.add(temperatureHist)

dtHist = dnl.HistoryFile("dtHistory")
dtHist.setFileName(dnl.String("dt.plot"))
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime / nbrePoints)
dynELA.add(dtHist)

keHist = dnl.HistoryFile("keHistory")
keHist.setFileName(dnl.String("ke.plot"))
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime / nbrePoints)
dynELA.add(keHist)

parallel = dnl.Parallel()
dynELA.add(parallel)
parallel.setCores(4)

dynELA.solve()

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
