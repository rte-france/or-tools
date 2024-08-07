# Autogenerated using ProtoBuf.jl v1.0.14 on 2024-01-02T18:44:47.231
# original file: /usr/local/google/home/tcuvelier/.julia/artifacts/cc3d5aa28fb2158ce4ff5aed9899545a37503a6b/include/ortools/math_opt/infeasible_subsystem.proto (proto3 syntax)

import ProtoBuf as PB
using ProtoBuf: OneOf
using ProtoBuf.EnumX: @enumx

export var"ModelSubsetProto.Bounds", ModelSubsetProto
export ComputeInfeasibleSubsystemResultProto

struct var"ModelSubsetProto.Bounds"
    lower::Bool
    upper::Bool
end
PB.default_values(::Type{var"ModelSubsetProto.Bounds"}) = (;lower = false, upper = false)
PB.field_numbers(::Type{var"ModelSubsetProto.Bounds"}) = (;lower = 1, upper = 2)

function PB.decode(d::PB.AbstractProtoDecoder, ::Type{<:var"ModelSubsetProto.Bounds"})
    lower = false
    upper = false
    while !PB.message_done(d)
        field_number, wire_type = PB.decode_tag(d)
        if field_number == 1
            lower = PB.decode(d, Bool)
        elseif field_number == 2
            upper = PB.decode(d, Bool)
        else
            PB.skip(d, wire_type)
        end
    end
    return var"ModelSubsetProto.Bounds"(lower, upper)
end

function PB.encode(e::PB.AbstractProtoEncoder, x::var"ModelSubsetProto.Bounds")
    initpos = position(e.io)
    x.lower != false && PB.encode(e, 1, x.lower)
    x.upper != false && PB.encode(e, 2, x.upper)
    return position(e.io) - initpos
end
function PB._encoded_size(x::var"ModelSubsetProto.Bounds")
    encoded_size = 0
    x.lower != false && (encoded_size += PB._encoded_size(x.lower, 1))
    x.upper != false && (encoded_size += PB._encoded_size(x.upper, 2))
    return encoded_size
end

struct ModelSubsetProto
    variable_bounds::Dict{Int64,var"ModelSubsetProto.Bounds"}
    variable_integrality::Vector{Int64}
    linear_constraints::Dict{Int64,var"ModelSubsetProto.Bounds"}
    quadratic_constraints::Dict{Int64,var"ModelSubsetProto.Bounds"}
    second_order_cone_constraints::Vector{Int64}
    sos1_constraints::Vector{Int64}
    sos2_constraints::Vector{Int64}
    indicator_constraints::Vector{Int64}
end
PB.default_values(::Type{ModelSubsetProto}) = (;variable_bounds = Dict{Int64,var"ModelSubsetProto.Bounds"}(), variable_integrality = Vector{Int64}(), linear_constraints = Dict{Int64,var"ModelSubsetProto.Bounds"}(), quadratic_constraints = Dict{Int64,var"ModelSubsetProto.Bounds"}(), second_order_cone_constraints = Vector{Int64}(), sos1_constraints = Vector{Int64}(), sos2_constraints = Vector{Int64}(), indicator_constraints = Vector{Int64}())
PB.field_numbers(::Type{ModelSubsetProto}) = (;variable_bounds = 1, variable_integrality = 2, linear_constraints = 3, quadratic_constraints = 4, second_order_cone_constraints = 5, sos1_constraints = 6, sos2_constraints = 7, indicator_constraints = 8)

function PB.decode(d::PB.AbstractProtoDecoder, ::Type{<:ModelSubsetProto})
    variable_bounds = Dict{Int64,var"ModelSubsetProto.Bounds"}()
    variable_integrality = PB.BufferedVector{Int64}()
    linear_constraints = Dict{Int64,var"ModelSubsetProto.Bounds"}()
    quadratic_constraints = Dict{Int64,var"ModelSubsetProto.Bounds"}()
    second_order_cone_constraints = PB.BufferedVector{Int64}()
    sos1_constraints = PB.BufferedVector{Int64}()
    sos2_constraints = PB.BufferedVector{Int64}()
    indicator_constraints = PB.BufferedVector{Int64}()
    while !PB.message_done(d)
        field_number, wire_type = PB.decode_tag(d)
        if field_number == 1
            PB.decode!(d, variable_bounds)
        elseif field_number == 2
            PB.decode!(d, wire_type, variable_integrality)
        elseif field_number == 3
            PB.decode!(d, linear_constraints)
        elseif field_number == 4
            PB.decode!(d, quadratic_constraints)
        elseif field_number == 5
            PB.decode!(d, wire_type, second_order_cone_constraints)
        elseif field_number == 6
            PB.decode!(d, wire_type, sos1_constraints)
        elseif field_number == 7
            PB.decode!(d, wire_type, sos2_constraints)
        elseif field_number == 8
            PB.decode!(d, wire_type, indicator_constraints)
        else
            PB.skip(d, wire_type)
        end
    end
    return ModelSubsetProto(variable_bounds, variable_integrality[], linear_constraints, quadratic_constraints, second_order_cone_constraints[], sos1_constraints[], sos2_constraints[], indicator_constraints[])
end

function PB.encode(e::PB.AbstractProtoEncoder, x::ModelSubsetProto)
    initpos = position(e.io)
    !isempty(x.variable_bounds) && PB.encode(e, 1, x.variable_bounds)
    !isempty(x.variable_integrality) && PB.encode(e, 2, x.variable_integrality)
    !isempty(x.linear_constraints) && PB.encode(e, 3, x.linear_constraints)
    !isempty(x.quadratic_constraints) && PB.encode(e, 4, x.quadratic_constraints)
    !isempty(x.second_order_cone_constraints) && PB.encode(e, 5, x.second_order_cone_constraints)
    !isempty(x.sos1_constraints) && PB.encode(e, 6, x.sos1_constraints)
    !isempty(x.sos2_constraints) && PB.encode(e, 7, x.sos2_constraints)
    !isempty(x.indicator_constraints) && PB.encode(e, 8, x.indicator_constraints)
    return position(e.io) - initpos
end
function PB._encoded_size(x::ModelSubsetProto)
    encoded_size = 0
    !isempty(x.variable_bounds) && (encoded_size += PB._encoded_size(x.variable_bounds, 1))
    !isempty(x.variable_integrality) && (encoded_size += PB._encoded_size(x.variable_integrality, 2))
    !isempty(x.linear_constraints) && (encoded_size += PB._encoded_size(x.linear_constraints, 3))
    !isempty(x.quadratic_constraints) && (encoded_size += PB._encoded_size(x.quadratic_constraints, 4))
    !isempty(x.second_order_cone_constraints) && (encoded_size += PB._encoded_size(x.second_order_cone_constraints, 5))
    !isempty(x.sos1_constraints) && (encoded_size += PB._encoded_size(x.sos1_constraints, 6))
    !isempty(x.sos2_constraints) && (encoded_size += PB._encoded_size(x.sos2_constraints, 7))
    !isempty(x.indicator_constraints) && (encoded_size += PB._encoded_size(x.indicator_constraints, 8))
    return encoded_size
end

struct ComputeInfeasibleSubsystemResultProto
    feasibility::FeasibilityStatusProto.T
    infeasible_subsystem::Union{Nothing,ModelSubsetProto}
    is_minimal::Bool
end
PB.default_values(::Type{ComputeInfeasibleSubsystemResultProto}) = (;feasibility = FeasibilityStatusProto.FEASIBILITY_STATUS_UNSPECIFIED, infeasible_subsystem = nothing, is_minimal = false)
PB.field_numbers(::Type{ComputeInfeasibleSubsystemResultProto}) = (;feasibility = 1, infeasible_subsystem = 2, is_minimal = 3)

function PB.decode(d::PB.AbstractProtoDecoder, ::Type{<:ComputeInfeasibleSubsystemResultProto})
    feasibility = FeasibilityStatusProto.FEASIBILITY_STATUS_UNSPECIFIED
    infeasible_subsystem = Ref{Union{Nothing,ModelSubsetProto}}(nothing)
    is_minimal = false
    while !PB.message_done(d)
        field_number, wire_type = PB.decode_tag(d)
        if field_number == 1
            feasibility = PB.decode(d, FeasibilityStatusProto.T)
        elseif field_number == 2
            PB.decode!(d, infeasible_subsystem)
        elseif field_number == 3
            is_minimal = PB.decode(d, Bool)
        else
            PB.skip(d, wire_type)
        end
    end
    return ComputeInfeasibleSubsystemResultProto(feasibility, infeasible_subsystem[], is_minimal)
end

function PB.encode(e::PB.AbstractProtoEncoder, x::ComputeInfeasibleSubsystemResultProto)
    initpos = position(e.io)
    x.feasibility != FeasibilityStatusProto.FEASIBILITY_STATUS_UNSPECIFIED && PB.encode(e, 1, x.feasibility)
    !isnothing(x.infeasible_subsystem) && PB.encode(e, 2, x.infeasible_subsystem)
    x.is_minimal != false && PB.encode(e, 3, x.is_minimal)
    return position(e.io) - initpos
end
function PB._encoded_size(x::ComputeInfeasibleSubsystemResultProto)
    encoded_size = 0
    x.feasibility != FeasibilityStatusProto.FEASIBILITY_STATUS_UNSPECIFIED && (encoded_size += PB._encoded_size(x.feasibility, 1))
    !isnothing(x.infeasible_subsystem) && (encoded_size += PB._encoded_size(x.infeasible_subsystem, 2))
    x.is_minimal != false && (encoded_size += PB._encoded_size(x.is_minimal, 3))
    return encoded_size
end
