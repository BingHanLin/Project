module SimulationDomain

using ..ParticleData: Particle, ParticlePool
using ..BasicTypes
using ..InteractionModel: sqrDis, weighting

using LinearAlgebra
mutable struct SimulationData
    currentTime_::Float64
    endTime_::Float64
    deltaTime_::Float64

    particleRadius_::Float64

    gravity_::Vec3F

    particlePool_::ParticlePool

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


function updatePreParticlePool(data::SimulationData)
    data.preParticlePool_ = data.particlePool_
end

function updateNeighborPool(data::SimulationData)

    for i = 1:length(data.particlePool_)
        p1 = data.particlePool_[i]

        if (p1.type_ == disabled)
            continue
        end
        data.particlePool_[i].neighbors_ = Int64[]

        for j = 1:length(data.particlePool_)
            p2 = data.particlePool_[j]
            if (i == j || p2.type_ == disabled)
                continue
            end

            if (sqrDis(p1, p2) < data.influenceRadius_^2)
                push!(data.particlePool_[i].neighbors_, j)
            end
        end

    end
end

function updateNumberDensity(data::SimulationData)
    for i = 1:length(data.particlePool_)
        p1 = data.particlePool_[i]
        if (p1.type_ == disabled)
            continue
        end
        p1.numberDensity_ = 0.0
        for j in data.particlePool_[i].neighbors_
            p2 = data.particlePool_[j]
            distance = sqrDis(p1, p2)^0.5
            p1.numberDensity_ += weighting(distance, data.influenceRadius_)
        end
    end
end


# ⟨∇2ϕ⟩i=2dλn0∑i≠j[(ϕj−ϕi)w(|rj–ri|)]

# function laplacian(data::SimulationData, id1::Int64)::Vec3F
#     p1 = data.particlePool_[id1]
#     for j in data.neighborPool_[id1]
#         p2 = data.particlePool_[j]
#         distance = sqrDis(p1, p2)^0.5
#         p1.numberDensity_ += weighting(distance, data.influenceRadius_)
#     end
# end

# if abspath(PROGRAM_FILE) == @__FILE__

# end

end
