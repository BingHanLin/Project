module InteractionModel

using ..ParticleData: Particle, ParticlePool
using ..BasicTypes

struct InteractionModel
    refNumberDensity_::Float64
    influenceRadius_::Float64
end

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
    getter::Function,
)::Vec3F

    grad = Vec3F(0.0, 0.0, 0.0)

    for neighborID = 1:length(particles[id].neighbors_)
        vec = particles[neighborID].pos_ - particles[id].pos_
        sqr = sqrDis(particles[neighborID].pos_, particles[id].pos_)
        w = weighting(sqr^0.5, influenceRadius)
        varDiff = getter(particles[neighborID]) - getter(particles[id])
        grad += varDiff * vec * w / varDiff
    end

    return grad

end

function getU(p::Particle)::Float64
    return p.vel_[0]
end

function getV(p::Particle)::Float64
    return p.vel_[1]
end

function getW(p::Particle)::Float64
    return p.vel_[2]
end

function getP(p::Particle)::Float64
    return p.pressure_
end




if abspath(PROGRAM_FILE) == @__FILE__

end

end
