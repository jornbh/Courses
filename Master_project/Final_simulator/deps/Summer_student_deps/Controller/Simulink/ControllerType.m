classdef ControllerType < Simulink.IntEnumType
    enumeration
        AB(1)
        Cascade(2)
        MPC(3)
        LQR(4)
    end
end