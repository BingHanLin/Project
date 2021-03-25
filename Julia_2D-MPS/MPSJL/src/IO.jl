module IO

using ..SimulationDomain: SimulationData

function summary(data::SimulationData)
    println("=================================================")
    println("Summary")
    println("End Time: ", length(data.endTime_))
    println("Delta Time: ", length(data.deltaTime_))
    println("Particle Radius: ", length(data.particleRadius_))
    println("Number of Particles: ", length(data.particlePool_))
    println("Number of Particles: ", length(data.particlePool_))
    println("=================================================")
end

end