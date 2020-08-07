
module ParticleData

export Particle
export ParticlePool


# Particle datatype
mutable struct Particle
    x_::Float64
    y_::Float64
    z_::Float64
end

# ParticlePool datatype
const ParticlePool = Vector{Particle}

function findNeighbors(targetP::Particle, pool::ParticlePool, rangeR::Float32)::ParticlePool

    neighborsPool = ParticlePool()
    sqrRangeR = rangeR^2

    for p in pool
        sqrR = (targetP.x_ - p.x_)^2 + (targetP.y_ - p.y_)^2 + (targetP.z_ - p.z_)^2

        if sqrR < sqrRangeR
            push!(neighborsPool, p)
        end
    end

    return neighborsPool
end

if abspath(PROGRAM_FILE) == @__FILE__
    pool = ParticlePool()
    push!(pool, Particle(1.0, 2.0, 1.0))
    push!(pool, Particle(1.0, 1.0, 1.0))
    pool
    println(pool)
end

end
