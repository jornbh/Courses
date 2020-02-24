# Linearisert invers dq-transformasjon 

Ved en feil i den inverterte transformasjonen vil signalet man gi ut være rotert med en vinkel $\tilde{\theta}$, men ellers oppføre seg likt som tre-fase-signalene ellers.

Når disse skal transformeres tilbake til den ideelle refferanse-rammen $dq^*$ vil de oppføre seg veldig likt som hvordan feilen i estimatet oppførte seg, på grunn av symetrien i produktet mellom 

$$
\cos \left({\theta} \right) \cos\left({\theta + \tilde{\theta}}\right) = \cos\left({\theta + \tilde{\theta}}\right) \cos \left({\theta} \right)
$$

Dermed vil analysen for d være nøyaktig lik.

Når det gjelder q-framen vil man i stedet få 

$$
-\sin(\theta_1)\cos(\theta_1+ \tilde{\theta}) -\sin(\theta_2)\cos(\theta_2+ \tilde{\theta}) -\sin(\theta_3)\cos(\theta_3+ \tilde{\theta})
$$

Når man setter inn $\tilde{\theta}=0$ får man 

$$
  -\sin(2\theta_1) -\sin(2\theta_2)-\sin(2\theta_3)
$$
og fordi gangingen med 2, fremdeles gir samme fordeling vil disse til sammen bli 0. 

Deretter deriverer vi med hensyn på $\tilde{\theta}$, og får 

$$
\sin(\theta_1)\sin(\theta_1+ \tilde{\theta}) +\sin(\theta_2)\sin(\theta_2+ \tilde{\theta}) +\sin(\theta_3)\sin(\theta_3+ \tilde{\theta})
$$
Når vi setter inn $\tilde{\theta}=0$ for å få likevektsunktet ender vi opp med 

$$
\sin^2(\theta_1) +\sin^2(\theta_2) +\sin^2(\theta_3)
$$
Og siden 

$$
\sin^2(\theta) = \frac{1}{2}\left( 1 + \sin(2\theta) \right)
$$

vil svaret bli 
$$
\frac{\partial V_q}{\partial \tilde{\theta}} = \frac{3}{2}\sqrt{\frac{2}{3}} \hat{V} = \sqrt{\frac{3}{2}}\hat{V}
$$

Den siste summeres til 0, uansett hvilke vinkler man har. 