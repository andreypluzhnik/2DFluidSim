# OpenGL Fluid Simulation
This is an interactive fluid simulation that I built on OpenGL.
<div align="center">
<img src="./media/demo.gif">
</div>

## Governing Equations
It solves the Navier-Stokes equation for a 2D incompressible fluid:
 
$$\frac{\partial u}{\partial t} = - (u \cdot \nabla)u + \gamma \nabla^2 u + \frac{\nabla p}{\rho} + F_{external}$$

Since the program so far only treats incompressible fluids, it is stipulated that the divergence of the velocity field vanishes, i.e. 

$$\nabla \cdot u = 0$$

## Solving the N-S Equations
The program follows an algorithm similar to the one laid out in [Stam 1999](https://pages.cs.wisc.edu/~chaol/data/cs777/stam-stable_fluids.pdf). Essentially the [Helmholtz Decomposition Theorem](https://en.wikipedia.org/wiki/Helmholtz_decomposition) tells us that we can solve for a compressible field $w$, which decomposes into $u$ and $\nabla p$, and subsequently subtract out $\nabla p$ to obtain $u = w - \nabla p$. 

The intermediate field $w$ is calculated by 'splitting' the N-S equation into seperate terms and solving them w.r.t. $\frac{\partial u}{\partial t}$ sequentially in the order:

- advection
- diffusion
- external forces


Finally the incompressible flow is determined with the help of the aforementioned Helmholtz decomposition step. 

Advection is performed with a semi-implicit scheme, which is explained in Stam 1999. 
Both diffusion and Helmholtz decomposition steps furnish second order linear PDEs which can be realized by sparse matrices. The program approximates them numerically using Jacobi iteration. There are other sparse matrix solvers out there: Conjugate Gradient Descent, LU/QR/Cholesky decomposition, to name a few, but Jacobi iteration seems to do a good enough job to simulate a fluid like appearance.

### Vorticity Confinement
To capture fine scale fluctuations, the program considers the vorticity confinement term from [Fedkiw et. al 2001](https://web.stanford.edu/class/cs237d/smoke.pdf) as an extra exernal force:

$$F_{conf} = \frac{\epsilon}{\lvert \eta \rvert} (\eta \times \omega)$$

Where $\omega = \nabla \times u$ and $\eta = \nabla \lvert \omega \rvert$.

## Other Useful Resources

- [GPU Gems 1](https://developer.nvidia.com/gpugems/gpugems/part-vi-beyond-triangles/chapter-38-fast-fluid-dynamics-simulation-gpu)
- [Fluid Dynamics Book](https://link.springer.com/book/10.1007/978-1-4612-0883-9)(Provides a discussion of Helmholtz Decomposition)
- [Reference and Tutorial for OpenGL](https://learnopengl.com/)
