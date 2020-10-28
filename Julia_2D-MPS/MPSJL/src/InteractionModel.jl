module InteractionModel

using ..ParticleData: Particle, ParticlePool
using ..BasicTypes

function sqrDis(p1::Particle, p2::Particle)::Float64
    return sum((p1.pos_ - p2.pos_) .^ 2)
end

function weighting(radius::Float64, influenceRadius::Float64)::Float64
    if radius <= influenceRadius
        return influenceRadius / radius - 1.0
    else
        return 0.0
    end
end

function grad(
    id::Int64,
    particles::ParticlePool,
    numberDensityRef::Float64,
    influenceRadius::Float64,
)::Vec3F

    grad = Vec3F(0.0, 0.0, 0.0)

    for neighborID = 1:length(particles[id].neighbors_)
        w = weighting(
            sqrDis(particles[neighborID].pos_, particles[id].pos_)^0.5,
            influenceRadius,
        )
    end
end

# function numberDensity(p::Particle, pool::ParticlePool)::Float64
# if r <= model.influenceRadius_
#     return model.influenceRadius_ / r - 1
# else
#     return 0.0
# end
# end



if abspath(PROGRAM_FILE) == @__FILE__

end

end
