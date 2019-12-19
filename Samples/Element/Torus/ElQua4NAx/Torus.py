#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

nbreSaves = 10      # nombre de fichiers graphiques
nbrePoints = 100    # numbre de points dans le time history
stopTime = 0.01      # temps total de simulation
displacement = 10   # déplacement total en cisaillement

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

# Creates the main Object
dynELA = dnl.DynELA("Torus")

# Creates the Nodes
dynELA.createNode(1, 10.00, 0.00, 0.00)
dynELA.createNode(2, 20.00, 0.00, 0.00)
dynELA.createNode(3, 20.00, 10.0, 0.00)
dynELA.createNode(4, 10.00, 10.0, 0.00)
print("Number of nodes created:",dynELA.getNodesNumber())

# Creates the Elements
dynELA.setDefaultElement(dnl.Element.ElQua4NAx)
dynELA.createElement(1,1,2,3,4)
print("Number of elements created:",dynELA.getElementsNumber())

# Creates the Groups
allNS=dnl.NodeSet("NS_All")
dynELA.add(allNS,1,4)

rightNS = dnl.NodeSet("NS_right")
dynELA.add(rightNS, 2, 3)

leftNS = dnl.NodeSet("NS_left")
dynELA.add(leftNS, 1)
dynELA.add(leftNS, 4)

allES=dnl.ElementSet("ES_All")
dynELA.add(allES,1)

histNS=dnl.NodeSet("NS_hist")
dynELA.add(histNS, 2)

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

# Finaly link the material to the structure
dynELA.add(steel, allES)

# Declaration of a boundary condition for left part
leftBC = dnl.BoundaryRestrain('BC_left')
leftBC.setValue(0, 1, 1)
dynELA.attachConstantBC(leftBC,leftNS)

# Declaration of a boundary condition for right part
rightBC = dnl.BoundaryRestrain('BC_right')
rightBC.setValue(0, 1, 1)
dynELA.attachConstantBC(rightBC,rightNS)

# Declaration of a ramp function to apply the load
ramp = dnl.RampFunction("constantFunction")
ramp.set(dnl.RampFunction.Constant, 0, stopTime)

# Declaration of a boundary condition for right part
speed = dnl.BoundarySpeed()
speed.setValue(displacement,0,0)
speed.setFunction(ramp)
dynELA.attachConstantBC(speed, leftNS)

# Declaration of the explicit solver
solver = dnl.Explicit("Solver")
solver.setTimes(0,stopTime)
dynELA.add(solver)
dynELA.setSaveTimes(0,stopTime,stopTime/nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile("vonMisesHistory")
vonMisesHist.setFileName(dnl.String("vonMises.plot"))
vonMisesHist.add(allES, 0, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(vonMisesHist)

temperatureHist = dnl.HistoryFile("temperatureHistory")
temperatureHist.setFileName(dnl.String("temperature.plot"))
temperatureHist.add(allES, 0, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(temperatureHist)

plasticStrainHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainHist.setFileName(dnl.String("plasticStrain.plot"))
plasticStrainHist.add(allES, 0, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(plasticStrainHist)

plasticStrainsHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainsHist.setFileName(dnl.String("plasticStrains.plot"))
plasticStrainsHist.add(allES, 1, dnl.Field.plasticStrain)
plasticStrainsHist.add(allES, 2, dnl.Field.plasticStrain)
plasticStrainsHist.add(allES, 3, dnl.Field.plasticStrain)
plasticStrainsHist.add(allES, 4, dnl.Field.plasticStrain)
plasticStrainsHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(plasticStrainsHist)

densityHist = dnl.HistoryFile("densityHistory")
densityHist.setFileName(dnl.String("density.plot"))
densityHist.add(allES, 0, dnl.Field.density)
densityHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(densityHist)

dtHist = dnl.HistoryFile("dtHistory")
dtHist.setFileName(dnl.String("dt.plot"))
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(dtHist)

keHist = dnl.HistoryFile("keHistory")
keHist.setFileName(dnl.String("ke.plot"))
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(keHist)

StressHist = dnl.HistoryFile("StressHistory")
StressHist.setFileName(dnl.String("Stress.plot"))
StressHist.add(allES, 0, dnl.Field.StressXX)
StressHist.add(allES, 0, dnl.Field.StressYY)
StressHist.add(allES, 0, dnl.Field.StressZZ)
StressHist.add(allES, 0, dnl.Field.StressXY)
StressHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(StressHist)

uxHist = dnl.HistoryFile("dispX")
uxHist.setFileName(dnl.String("dispX.plot"))
uxHist.add(histNS, dnl.Field.displacementX)
uxHist.setSaveTime(stopTime/nbrePoints)
dynELA.add(uxHist)


# Parallel solver
parallel = dnl.Parallel()
dynELA.add(parallel)
parallel.setCores(1)

# Run the main solver
dynELA.solve()

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
