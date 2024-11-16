clear
clc
SB = 100*10^6
VB = 230*10^3
Pg1 = 0
Qg1 = 0
Pd1 = 50*10^6
Qd1 = 40*10^6

Pg2 = 0
Qg2 = 0
Pd2 = 170*10^6
Qd2 = 105*10^6

Pg3 = 0
Qg3 = 0
Pd3 = 200*10^6
Qd3 = 124*10^6

Pg4 = 318*10^6
Qg4 = 0
Pd4 = 80*10^6
Qd4 = 50*10^6


%% 1º Passo - Matriz de Admitâncias
Y11 = 1/(0.0074+j*0.037) + 1/(0.010+j*0.05) + j*0.05 +j*0.04;
Y12 = -1/(0.010+j*0.05);
Y13 = -1/(0.0074+j*0.037);
Y14 = 0 + j*0;
Y21 = Y12;
Y22 = 1/(0.010+j*0.05) + 1/(0.0074+j*0.037)+j*0.05+j*0.04;
Y23 = 0 + j*0;
Y24 = Y13;
Y31 = Y13;
Y32 = Y23;
Y33 = 1/(0.0074+j*0.037)+1/(0.0127+j*0.064)+j*0.04+j*0.065;
Y34 = -1/(0.0127+j*0.064);
Y41 = Y14;
Y42 = Y24;
Y43 = Y34;
Y44 = 1/(0.0074+j*0.037)+1/(0.0127+j*0.064)+j*0.04+j*0.065;

Ybus = [Y11 Y12 Y13 Y14
        Y21 Y22 Y23 Y24
        Y31 Y32 Y33 Y34
        Y41 Y42 Y43 Y44];

V2 = 1;
V1 = 1;
V3 = 1;
V4 = 1;

G22 = real(Y22);
B22 = imag(Y22);

G33 = real(Y33);
B33 = imag(Y33);

G44 = real(Y44);
B44 = imag(Y44);

theta11 = angle(Y11);
theta12 = angle(Y12);
theta13 = angle(Y13);
theta14 = angle(Y14);

theta21 = angle(Y21);
theta22 = angle(Y22);
theta23 = angle(Y23);
theta24 = angle(Y24);

theta31 = angle(Y31);
theta32 = angle(Y32);
theta33 = angle(Y33);
theta34 = angle(Y34);

theta41 = angle(Y41);
theta42 = angle(Y42);
theta43 = angle(Y43);
theta44 = angle(Y44);

delta1 = 0;
delta2 = 0;
delta3 = 0;
delta4 = 0;
iteracao = 0;

for x = 1:3 % Usei for para teste, mas na verdade deve ser usado um while com a condição do projeto
    iteracao = x

    %% 2º Passo - Cálculo das injeções de potência

    P2o = (abs(V2)^2)*G22+abs(V2*V1*Y21)*cos(theta21+delta1-delta2)+abs(V2*V4*Y24)*cos(theta24+delta4-delta2);
    P3o = (abs(V3)^2)*G33+abs(V3*V1*Y31)*cos(theta32+delta1-delta3)+abs(V3*V4*Y34)*cos(theta34+delta4-delta3);
    P4o = P3o;
    Q2o = -(abs(V2)^2)*B22-abs(V2*V1*Y12)*sin(theta21+delta1-delta2)-abs(V2*V4*Y24)*sin(theta24+delta4-delta2);
    Q3o = -(abs(V3)^2)*B33-abs(V3*V1*Y13)*sin(theta31+delta1-delta3)-abs(V3*V4*Y34)*sin(theta34+delta4-delta3);
    Q4o = -(abs(V4)^2)*B44-abs(V4*V1*Y14)*sin(theta41+delta1-delta4)-abs(V4*V3*Y43)*sin(theta43+delta3-delta4);

    %% 3° Passo - Cálculo dos Resíduos

    DeltaP2o = (Pg2-Pd2)-P2o;
    DeltaP3o = (Pg3-Pd3)-P3o;
    DeltaP4o = (Pg4-Pd4)-P4o;

    DeltaQ2o = (Qg2-Qd2)-Q2o;
    DeltaQ3o = (Qg3-Qd3)-Q3o;
    DeltaQ4o = (Qg4-Qd4)-Q4o;

    %% 4° Passo - Cálculo da Matriz Jacobiana

    H22o = -Q2o+(-(abs(V2))^2)*B22;
    H23o = -abs(V2*V3*Y23)*sin(theta23+delta3-delta2);
    H24o = -abs(V2*V4*Y24)*sin(theta24+delta4-delta2);
    H32o = -abs(V3*V2*Y32)*sin(theta32+delta2-delta3);
    H33o = -Q3o-(abs(V3)^2)*B33;
    H34o = -abs(V3*V4*Y34)*sin(theta34+delta4-delta3);
    H44o = -Q4o-(abs(V4)^2)*B44;
    H42o = -abs(V4*V2*Y42)*sin(theta42+delta2-delta4);
    H43o = -abs(V4*V3*Y43)*sin(theta43+delta3-delta4);

    J22o = P2o -(abs(V2)^2)*G22;
    J23o = 0;
    J24o = -abs(V2*V4*Y24)*cos(theta24+delta4-delta2);
    J32o = 0;
    J33o = P3o-(abs(V3))^2*G33;
    J34o = -abs(V3*V4*Y34)*cos(theta34+delta4-delta3);
    J42o = -J24o;
    J43o = -J34o;

    N22o = P2o+(abs(V2))^2*G22;
    N23o = 0;
    N32o = 0;
    N33o = P3o+((abs(V3))^2)*G33;
    N42o = abs(V4*V2*Y42)*cos(theta42+delta2-delta4);
    N43o = abs(V4*V3*Y43)*cos(theta43+delta3-delta4);
    N24o = -N42o;
    N34o = -N43o;

    L22o = Q2o-(abs(V2)^2)*B22;
    L23o = 0;
    L32o = 0;
    L33o = Q3o+(abs(V3)^2)*B33;

    % 5° Passo - Montagem da Matriz Jacobiana

    jacobiana = [H22o H23o H24o J22o J23o
        H32o H33o H34o J32o J33o
        H42o H43o H44o J42o J43o
        N22o N23o N24o L22o L23o
        N32o N33o N34o L32o L33o];

    residuos = [DeltaP2o
        DeltaP3o
        DeltaP4o
        DeltaQ2o
        DeltaQ3o];

    Deltas = jacobiana\residuos

    deltadelta2 = Deltas(1);
    deltadelta3 = Deltas(2);
    deltadelta4 = Deltas(3);
    deltaV2 = Deltas(4);
    deltaV3 = Deltas(5);

    % 6° Passo - Cálculo das tensões para segunda iteração

    delta2 = delta2 + deltadelta2
    delta3 = delta3 + deltadelta3
    delta4 = delta4 + deltadelta4

    V2 = (abs(V2))*(1+(abs(deltaV2))/abs(V2))
    V3 = (abs(V3))*(1+(abs(deltaV3))/abs(V3))
end


I12 = (V1-V2)*Y12
I23 = (V2-V3)*Y23
I34 = (V3-V4)*Y34
I41 = (V4-V1)*Y41

S12 = V1*I12
S23 = V2*I23
S34 = V3*I34
S41 = V4*I41


