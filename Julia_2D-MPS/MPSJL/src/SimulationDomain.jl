module SimulationDomain

using ..ParticleData: Particle, ParticlePool, sqrDis
using ..BasicTypes
using ..InteractionModel

using LinearAlgebra
struct SimulationData
    currentTime_::Float64
    endTime_::Float64
    deltaTime_::Float64

    particleRadius_::Float64
    influenceRadius_::Float64

    gravity_::Vec3F

    particlePool_::ParticlePool
    neighborPool_::Vector{Vector{Int64}}

    preParticlePool_::ParticlePool


    SimulationData(endTime::Float64, deltaTime::Float64, particleRadius::Float64) = new(
        0.0,
        endTime,
        deltaTime,
        particleRadius,
        4.0 * particleRadius,
        Vec3F(0.0, -9.81, 0.0),
        ParticlePool(),
        Vector{Int64}[],
    )
end

function summary(data::SimulationData)
    println("Summary")
    println("End Time: ", length(data.endTime_))
    println("Delta Time: ", length(data.deltaTime_))
    println("Particle Radius: ", length(data.particleRadius_))
    println("Number of Particles: ", length(data.particlePool_))
end


function updatePreParticlePool(data::SimulationData)
    data.preParticlePool_ = data.particlePool_
end

function weighting(radius::Float64, influenceRadius::Float64)::Float64
    if radius <= influenceRadius
        return influenceRadius / radius - 1.0
    else
        return 0.0
    end
end

function updateNeighborPool(data::SimulationData)

    resize!(data.neighborPool_, length(data.particlePool_))

    for i = 1:length(data.particlePool_)
        p1 = data.particlePool_[i]

        if (p1.type_ == disabled)
            continue
        end
        data.neighborPool_[i] = Int64[]

        for j = 1:length(data.particlePool_)
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

function updateNumberDensity(data::SimulationData)
    for id1 = 1:length(data.particlePool_)
        p1 = data.particlePool_[id1]
        if (p1.type_ == disabled)
            continue
        end
        p1.numberDensity_ = 0.0
        for id2 in data.neighborPool_[id1]
            p2 = data.particlePool_[id2]
            distance = sqrDis(p1, p2)^0.5
            p1.numberDensity_ += weighting(distance, data.influenceRadius_)
        end
    end
end


# ⟨∇2ϕ⟩i=2dλn0∑i≠j[(ϕj−ϕi)w(|rj–ri|)]

# function laplacian(data::SimulationData, id1::Int64)::Vec3F

#     p1 = data.particlePool_[id1]

#     for id2 in data.neighborPool_[id1]
#         p2 = data.particlePool_[id2]
#         distance = sqrDis(p1, p2)^0.5
#         p1.numberDensity_ += weighting(distance, data.influenceRadius_)
#     end

# end

if abspath(PROGRAM_FILE) == @__FILE__

end

end
