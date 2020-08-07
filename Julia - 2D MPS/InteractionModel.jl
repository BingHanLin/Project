push!(LOAD_PATH, ".")

module InteractionModel

using ParticleData

struct Model

    endTime_::Float64
    deltaTime_::Float64

    particleRadius_::Float64
    influenceRadius_::Float64

    gravity_::Tuple{Float64,Float64,Float64}

    numWallLayer_::Int64
    numDummyLayer_::Int64

    fluidPool_::ParticlePool
    wallPool_::ParticlePool
    dummpyPool_::ParticlePool

    Model(endTime::Float64, deltaTime::Float64, particleRadius::Float64) = new(
        endTime,
        deltaTime,
        particleRadius,
        6.1 * particleRadius,
        (0.0, 0.0, -9.81,),
        1,
        3,
        ParticlePool(),
        ParticlePool(),
        ParticlePool()
    )
end



# function numberDensity(model::Model,p::Particle)::Float64
#     if r <= model.influenceRadius_
#         return model.influenceRadius_ / r - 1
#     else
#         return 0.0
#     end
# end

function kernel(r::Float64, influenceRadius::Float64)::Float64
    if r <= influenceRadius
        return influenceRadius / r - 1.0
    else
        return 0.0
    end
end

if abspath(PROGRAM_FILE) == @__FILE__
    model = Model(0.5, 0.01, 0.02)
    println(model)
end

end
