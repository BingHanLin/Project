import MPSJL

simData = MPSJL.SimulationData(1.0, 0.01, 0.1)
boxGeom = MPSJL.BoxGeometry(0., 0., 0., 1., 1., 0., 0.5)

MPSJL.generateParticle(boxGeom, simData.particlePool_, simData.particleRadius_)

MPSJL.updateNeighborPool(simData)

MPSJL.summary(simData)

println(simData.neighborPool_[5])


