# Notater for parametric sensitivity

Om det finnes en matrise som har høyre eigen-vektorer som dekker hele vektor-rommet må det også finnes venstre eigen-vektorer. Disse vil også ha det samme settet av eigen-verider.

$$ A\phi_i = \lambda_i\phi_i\

\Rarr \psi_i A = \lambda_i \psi $$

Vanligvis vil vi også skallere vektorene, slik at 

$$
\psi_i \phi_i = 1
$$

Ved å lage matriser $\phi$ og $\psi$ som inneholder alle disse vektorene kan man lage matriser som oppfyller kravene.

$$
A\phi = \phi\Lambda
$$

$$
\Rarr \phi^{-1}A\phi = \Lambda
$$

$$
\psi A \phi = \Lambda
$$

Participation-factor er målet på den rellative deltagelsen for en tilstand k i en mode i om motsatt. 

$$
\bold{P}=
\left(\begin{array}{cc}
    p_{11} & p_{12} & \dots & p_{1n} \\
    p_{21} & p_{22} & \dots & p_{2n} \\
    \dots  & \dots  &\dots  &\dots \\
    p_{n1} & p_{n2} & \dots & p_{nn}
\end{array}\right)
= \left( 
\begin{array}{cc}
    \phi_{11}\psi_{11} & \phi_{12}\psi_{21} & \dots &\phi_{1n}\psi_{n1} \\
    \phi_{21}\psi_{12} & \phi_{22}\psi_{22} & \dots &\phi_{2n}\psi_{n2} \\
    \dots              & \dots              &\dots  &\dots    \\
    \phi_{n1}\psi_{1n} & \phi_{n2}\psi_{2n} & \dots & \phi_{nn}\psi_{nn}\\
\end{array}
\right)

$$

Dette er et rellativt mål av biddraget til en state k i mode i, og omvendt. 

## Parametric sensitivity

Uttrykker den deriverte av en eigen-verdi, gitt en spesifikk parameter. 

$$
\alpha_{i,k} = \frac{d\lambda_i}{d\rho_i} = \frac{  \phi_i^T \frac{\delta A}{\delta \rho_k }\psi_i }{\phi_i^T \psi_i }
$$

Nyttig for å finne ut av hvilke parametre som best kan brukes for å forbedre responsen til et system.
