module SimulationDomain

using ..ParticleData: Particle, ParticlePool, sqrDis
using ..BasicTypes
using ..InteractionModel

struct SimulationData

    endTime_::Float64
    deltaTime_::Float64

    particleRadius_::Float64
    influenceRadius_::Float64

    gravity_::Vec3F

    particlePool_::ParticlePool
    neighborPool_::Vector{Vector{Int64}}

    SimulationData(endTime::Float64, deltaTime::Float64, particleRadius::Float64) = new(
        endTime,
        deltaTime,
        particleRadius,
        4.0 * particleRadius,
        Vec3F(0.0, -9.81, 0.0),
        ParticlePool(),
        Vector{Int64}[]
    )
end

function summary(data::SimulationData)
    println("Summary")

    println("End Time: ", length(data.endTime_))
    println("Delta Time: ", length(data.deltaTime_))
    println("Particle Radius: ", length(data.particleRadius_))
    println("Number of Particles: ", length(data.particlePool_))
end

function updateNeighborPool(data::SimulationData)

    resize!(data.neighborPool_, length(data.particlePool_))

    for i in 1:length(data.particlePool_)
        p1 = data.particlePool_[i]

        if (p1.type_ == disabled)
            continue
        end
        data.neighborPool_[i] = Int64[]

        for j in 1:length(data.particlePool_)
            p2 = data.particlePool_[j]
            if (i == j || p2.type_ == disabled)
                continue
            end

            if (sqrDis(p1, p2) < data.influenceRadius_^2)
                push!(data.neighborPool_[i], j)     
            end
        end
    end
end

    # function updateNumberDensities(data::SimulationData)::Float64

    # for id in 1:length(data.particlePool_)
    #     for id in 1:length(data.particlePool_)

    #         weighting(data.particleRadius_, data.influenceRadius_)
    #         for id in 1:length(data.particlePool_)

    #         end
    #     end
# end

function weighting(radius::Float64, influenceRadius::Float64)::Float64
    if radius <= influenceRadius
        return influenceRadius / radius - 1.0
    else
        return 0.0
    end
end

    if abspath(PROGRAM_FILE) == @__FILE__

end

end
