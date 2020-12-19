classdef MPCType < Simulink.IntEnumType
    enumeration
        MPCDiscrete(1)
        MPCPseudoDiscrete(2)
        LQR(3)
        LQRDiffIntegral(4)
        % Agnostic parameters
        MPCDiscreteAgnostic(5)
        MPCPseudoDiscreteAgnostic(6)
        LQRAgnostic(7)
        LQRDiffIntegralAgnostic(8)
        MPCDiscreteDiffIntegralAgnostic(9)
    end
end