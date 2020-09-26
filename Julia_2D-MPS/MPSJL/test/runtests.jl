import MPSJL

# set domain data
simData = MPSJL.SimulationData(1.0, 0.01, 0.1)
boxGeom = MPSJL.BoxGeometry(0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.5)

# init domain data
MPSJL.generateParticle(boxGeom, simData.particlePool_, simData.particleRadius_)
MPSJL.updatePreParticlePool(simData)

# report summary
MPSJL.summary(simData)

MPSJL.updateNeighborPool(simData)

while simData.currentTime_ <= simData.endTime_

    simData.currentTime_ += simData.deltaTime_
end


MPSJL.updateNumberDensity(simData)

println(simData.neighborPool_[5])
println(simData.particlePool_[5].numberDensity_)






