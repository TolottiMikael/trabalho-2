clc
close all
clear all
%% Valores Base
Sbase = 100;%MVA
Vbase = 230;%kV

%% definindo as vari�veis
lt12 = [0.010, i*0.05  ,10];
lt13 = [0.0074 , i*0.037 ,8];
lt24 = [0.0074 , i*0.037 ,8];
lt34 = [0.0127 , i*0.064 ,13];

%% descrevendo a matriz de admit�ncias
y(1,1) = (1/(lt12(1)+lt12(2))) + (1/(lt13(1)+lt13(2))) + lt12(2)/(Sbase*2) + lt13(2)/(Sbase*2);
y(1,2) = (1/(lt12(1)+lt12(2)));
y(1,3) = (1/(lt13(1)+lt13(2)));
y(1,4) = 0;

y(2,1) = y(1,2);
y(2,2) = ((1/(lt12(1)+lt12(2))) + (1/(lt24(1)+lt24(2))) + lt12(2)/(Sbase*2) + lt24(2)/(Sbase*2));
y(2,3) = 0;
y(2,4) = (1/(lt24(1)+lt24(2)));


y(3,1) = y(1,3);
y(3,2) = 0;
y(3,3) = ((1/(lt13(1)+lt13(2))) + (1/(lt34(1)+lt34(2))) + lt13(2)/(Sbase*2) + lt34(2)/(Sbase*2));
y(3,4) = (1/(lt34(1)+lt34(2)));

y(4,1) = 0;
y(4,2) = y(2,4);
y(4,3) = y(3,4);
y(4,4) = ((1/(lt24(1)+lt24(2))) + (1/(lt34(1)+lt34(2))) + lt24(2)/(Sbase*2) + lt34(2)/(Sbase*2));


Y = [y(1,1) -y(1,2) -y(1,3) -y(1,4);
    -y(2,1) y(2,2) -y(2,3) -y(2,4);
    -y(3,1) -y(3,2) y(3,3) -y(3,4);
    -y(4,1) -y(4,2) -y(4,3) y(4,4)
    ];
    

% 
% y = [9.11-i*45.1 -3.85+i*19.23 -5.26+i*25.96 +0;
%     -3.85+i*19.23 9.11-i*45.1 0 -2.26+i*26.96;
%     -2.26+i*26.96 0 8.31-i*40.86 -3.04+i*15.11;
%     0 -5.26+i*25.96 -3.04+i*15.11 8.31-i*40.96]

%% resolvendo as correntes nos ramos
disp(y);

%% Calculando as pot�ncias
V = [ 1 1 1 1];
delta = [ 0 0 0 0];


for m = (1:1:3)
    disp('rodando');

    P(1) = 1;
    P(2) = abs(V(2))^2 * real(Y(2,2)) + abs(V(2)) * abs(V(1)) * abs(Y(2,1)) * cos( angle(Y(2,1)) + delta(1) - delta(2)) + abs(V(2)) * abs(V(4)) * abs(Y(2,4)) * cos( angle(Y(2,4)) + delta(4) - delta(2) );
    P(3) = abs(V(3))^2 * real(Y(3,3)) + abs(V(3)) * abs(V(1)) * abs(Y(3,1)) * cos( angle(Y(3,1)) + delta(1) - delta(3)) + abs(V(3)) * abs(V(4)) * abs(Y(3,4)) * cos( angle(Y(3,4)) + delta(4) - delta(3) );
    P(4) = abs(V(4))^2 * real(Y(4,4)) + abs(V(4)) * abs(V(2)) * abs(Y(4,2)) * cos( angle(Y(4,2)) + delta(2) - delta(4)) + abs(V(4)) * abs(V(3)) * abs(Y(4,3)) * cos( angle(Y(4,3)) + delta(3) - delta(4) );

    Q(1) = 1;
    Q(2) = -abs(V(2))^2 * imag(Y(2,2)) - abs(V(2)) * abs(V(1)) * abs(Y(2,1)) * sin(angle(Y(2,1)) + delta(1) - delta(2)) - abs(V(2)) * abs(V(4)) * abs(Y(2,4)) * sin(angle(Y(2,4)) + delta(4) - delta(2));
    Q(3) = -abs(V(3))^2 * imag(Y(3,3)) - abs(V(3)) * abs(V(1)) * abs(Y(3,1)) * sin(angle(Y(3,1)) + delta(1) - delta(3)) - abs(V(3)) * abs(V(4)) * abs(Y(3,4)) * sin(angle(Y(3,4)) + delta(4) - delta(3));
    Q(4) = 1;

     %calculando os res�duos
    deltaP(2) = 0 - (170/Sbase) - P(2);
    deltaQ(2) = 0 - (105/Sbase) - Q(2);
    deltaP(3) = 0 - (200/Sbase) - P(3);
    deltaQ(3) = 0 - (124/Sbase) - Q(3);

    deltaP(4) = (318/Sbase) - (80/Sbase) - P(4);

for i = (2:1:4)
    %calculando a Jacobiana
    for j = (2:1:4)
       if(i == j)
          %diagonal principal 
          H(i-1,i-1) = Q(i) -  (abs( V(i) )^2 )*imag(Y(i,i));
          
          N(i-1,i-1) = P(i) + (abs(V(i))^2)*real(Y(i,i));
          
          J(i-1,i-1) = P(i) - (abs(V(i))^2)*real(Y(i,i));
          
          L(i-1,i-1) = Q(i) -  (abs( V(i) )^2 )*imag(Y(i,i));
          
       else
           L(i-1,j-1) = -(abs(V(i)*V(j)*Y(i,j))*sin( angle(Y(i,j))+delta(j)-delta(i) ));
           H(i-1,j-1) = - (  abs( V(i)*V(j)*Y(i,j) )*sin( angle(Y(i,j))+delta(j)-delta(i) ) );
           J(i-1,j-1) = -( abs(V(i)*V(j)*Y(i,j))*cos( angle(Y(i,j))+delta(j)-delta(i) ));
           N(i-1,j-1) = ( abs(V(i)*V(j)*Y(i,j))*cos( angle(Y(i,j))+delta(j)-delta(i) ));
       end 
    end
    
end

%% convergir 

Jacob =  [[H N(1:3,1:2)];[J(1:2,1:3) L(1:2,1:2)]]
Residuos = [[deltaP(2:4)'];[deltaQ(2:3)']]

final = inv(Jacob)*Residuos

%% atualiza
d2 = final(1);
d3 = final(2);
d4 = final(3);

deltaV2 = final(4);
deltaV3 = final(5);

V(2) = abs(V(2))* ( 1 + (deltaV2/abs(V(2))) );
V(3) = abs(V(3))* ( 1 + (deltaV3/abs(V(3))) );

delta(2) = delta(2) + d2;
delta(3) = delta(3) + d3;
delta(4) = delta(4) + d4;

end

disp('final')
P
Q