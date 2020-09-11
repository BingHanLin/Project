module InteractionModel

using ..ParticleData
using ..BasicTypes

function kernel(radius::Float64, influenceRadius::Float64)::Float64
    if radius <= influenceRadius
        return influenceRadius / radius - 1.0
    else
        return 0.0
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
