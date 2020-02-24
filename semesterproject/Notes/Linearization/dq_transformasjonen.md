# Linearisering av dq-transformasjonen

Spenningene er (Om amplituden og justeringsfaktoren nuller ut hverandre):

$$ v_d= cos(\theta_1+ \tilde{\theta})\cdot cos(\theta_1) + cos(\theta_2+ \tilde{\theta})\cdot cos(\theta_2) + cos(\theta_3+ \tilde{\theta})\cdot cos(\theta_3); $$

$$ v_q = -sin(\theta_1+ \tilde{\theta})\cdot cos(\theta_1) + -sin(\theta_2+ \tilde{\theta})\cdot cos(\theta_2) + -sin(\theta_3+ \tilde{\theta})\cdot cos(\theta_3); $$

Hvor

$$ \theta_1 = \theta +0 $$

$$ \theta_2 = \theta -\frac{2\pi}{3} $$

$$ \theta_1 = \theta +\frac{2\pi}{3} $$

## d-retningen

$$ \cos^2{\theta} = \frac{1}{2}(\cos(2\theta) +1) $$

Og vi vet at et balansert system ($120\degree$ forskyving) vil ha at reell om immaginær del summeres til 0\.

I det ideelle tilfellet har man:

$$ \frac{V_d}{\hat{V}}\sqrt{\frac{3}{2}} = \left(\frac{1}{2}+ \cos(\theta_1)\right) + \left(\frac {1}{2}+ \cos(\theta_2)\right) + \left(\frac{1}{2}+ \cos(\theta_3)\right) $$

$$ \left( \cos(\theta_1)\right) + \left( \cos(\theta_2)\right) + \left( \cos(\theta_3)\right) =0 $$

$$ {V_d}={\hat{V}} \sqrt{\frac{3}{2}} $$

Men det er en $\tilde{\theta}$ i den ene sinusen i tillegg. Så vi må derivere med hensyn på den.

Etter å ha derivert med hensyn på $\tilde{\theta}$, og satt inn $\tilde{\theta}$ for å få operrasjons-punktet

$$

- \cos(\theta_1)\cdot \sin(\theta_1) - \cos(\theta_2)\cdot\sin(\theta_2) - \cos(\theta_3)\cdot \sin(\theta_3) $$

Men om man bruker Euler-likningen får man.

$$ sin(\theta)cos(\theta) = \frac{sin(2\theta)}{2} $$

Og fra tidligere bevis får vi at disse tre vil summeres til 0, slik at

$$ \frac{\partial V_d}{\partial \tilde{\theta}} = 0 $$

Som betyr at det ikke vil være noen påvirkning i lineariseringen

## q-retningen

Uttrykket er i stedet bassert på $-\sin(\theta+\tilde{\theta})$. Men ellers helt likt.

Dermed er $v_q=0$ ved likevektspunktet, men til gjengjeld er

$$ \frac{\partial V_q}{\partial \tilde{\theta}} = \frac{3}{2} \cdot \sqrt{\frac{2}{3}}V_{eq,out} $$

$$ cos^2(x) = \frac{1}{2} \left( 1+ cos(2 \cdot \theta ) \right)

$$


## Matlab-koden som jeg brukte for å regne ut dette

```matlab
%% Linearizing the Park tranform (Applies to everything that is park-transformed)
syms T1 T2 T3 TT phi

%% Disclaimer
% each of the voltages is actually suposed to be multiplied by a factor
% $\sqrt{\frac{2}{3}}$ from the transformation, and a peak-voltage
% $\hat{V}$, giving the amplitude of the voltage



%% Code

v_d(T1,T2,T3) = cos(T1+ TT)*cos(T1) + cos(T2+ TT)*cos(T2) + cos(T3+ TT)*cos(T3);
v_q(T1,T2,T3) = -sin(T1+ TT)*cos(T1) + -sin(T2+ TT)*cos(T2) + -sin(T3+ TT)*cos(T3);

balanced_d(TT) = v_d(phi,phi -(2*pi/3),phi + 2*pi/3);
balanced_q(TT) = v_q(phi,phi -(2*pi/3),phi + 2*pi/3);


dV_d(TT)= diff( balanced_d);
dV_q(TT)= diff( balanced_q);

disp("V_d at the equilybrium is:");
disp(balanced_d(0))
disp("V_q at the equilybrium is:");
disp(balanced_q(0))
disp("The differentiated of V_d at the operating point is:")
disp(dV_d(0))

disp("The differentiated of V_q at the operating point is:")
disp(dV_q(0))


%% Remember some trigonometric identities
%%
% $cos^2(x) = \frac{1}{2}  \left( 1+ cos(2 \cdot \theta ) \right)$
%%
% $sin(\theta)cos(\theta) = \frac{sin(2\theta)}{2}$
%% The results are therfore
%%
% $$
% v_d = \frac{3}{2}
% $$
%%
% $$
% v_q = 0
% $$
%%
% $$
% \frac{dv_d}{dt} = 0
% $$
%%
% $$
% \frac{dv_q( \tilde{\theta}=0)}{d \tilde{\theta}} = -\frac{3}{2}
% $$
```
