module SimulationDomain

# Box domain datatype
struct BoxDomain
    xMin_::Float64
    yMin_::Float64
    zMin_::Float64

    xMax_::Float64
    yMax_::Float64
    zMax_::Float64

    filledHeight_::Float64
end

if abspath(PROGRAM_FILE) == @__FILE__
    println(methods(Particles))
    println(methods(BoxDomain))
end

end
