## Worksheet 3: Parallelizing Fluidchen using MPI

### Validation

#### Lid-Driven Cavity

The prpblem of Lid-driven cavity from Worksheet 1 was used for validating the parallel implementation with imax = 300, jmax = 300. A very low timestep size was obtained for xlength, ylength = 1. To reduce the total computation time, xlength, ylength was changed to 10 units. The table below shows the runtime for different combination of (iproc, jproc) with other parameters kept same as in the Worksheet 1.

(iproc, jproc) | Time(s) |
--- | --- |
(1, 1) | 3767.00  |
(2, 2) | 1444.43  |
(1, 4) | 1414.71  |
(3, 2) | 1429.90  | 

<p> 
<img src="docs/Worksheet3_Plots/LidDrivenCavity_VelContour_2x2.png" width="500">
</p>
<em>Velocity contour plot with Glyphs for Lid Driven Cavity simulation in parallel with (iproc, jproc) = (2, 2)</em>

#### Fluid Trap
Fluid Trap problem from Worksheet 2 was used as validation for parallel implementation with energy equation. The simulation was run on different combination of (iproc, jproc) with other parameters kept same as Worksheet 2. It was observed that for the (iproc, jproc) = (3,2), the result diverged with relaxation factor, w = 1.7. However, convergence was obtained for w < 1.66. The computation is tabulated for w = 1.5 and w = 1.7 below.

(iproc, jproc) | Time(s), w = 1.5  | Time(s), w = 1.7 |
--- | --- | --- |
(1, 1) | 72.29  | 81.01 |
(2, 3) | 26.84  | 31.76 |
(3, 2) | 27.84  | 28.17 (Not converged) |

<p> 
<img src="docs/Worksheet3_Plots/FluidTrap_TempContour_3x2.png" width="500">
</p>
<em>Temperature contour plot with Velocity Glyphs for Fluid Trap simulation in parallel with (iproc, jproc) = (3, 2)</em>

#### Comparison of Convergence Behavior for single-core case across versions
To compare the convergence behavior across the different versions for a single-core case, we removed the condition on maximum iteration for inner while loop (implicit part for solving pressure) and plotted no. of iterations for residual to drop below the tolerance level for each timestep. For Lid-driven cavity case, we compared the result from Worksheet 1, 2 & 3, while for Fluid Trap case, we compared the result for Worksheet 2 & 3. Similar trend is observed across the versions. This confirms that parallelizing the code has no effect on the numerics on a single-core simulation.
<p>
<img src= "validation/LDC_conv_WS1.png" width="500"> | <img src= "validation/LDC_conv_WS2.png" width="500"> | <img src= "validation/LDC_conv_WS3.png" width="500">
</p>
<em>Covergence comparison for Lid-driven Cavity.</em>

<p>
<img src= "validation/FT_conv_WS2.png" width="500"> | <img src= "validation/FT_conv_WS3.png" width="500">
</p>
<em>Covergence comparison for Fluid Trap.</em>

### Performance Analysis

#### Strong Scaling Analysis for Rayleigh Benard
Strong scaling analysis refers to analysing speedup and parallel efficiency with increasing number of parallel processes. In this section, we carry out the strong scaling analysis considering the Rayleigh Benard convection case from Worksheet 2. We consider the longer domain and keep all the parameters same as in the previous worksheet.

Speedup refers to the reduction factor for computation time compared to the single-core case, and parallel efficiency refers to speedup per process.

Results for strong scaling analysis are tabulated below. We obtain a significant speedup with increasing the number of cores. However, the speedup does not increase linearly as expected because of the linear part of the execution (for eg. domain decomposition) and communication overhead. Parallel efficiency also decreases with increasing no. of parallel processes. 

Case | (iproc, jproc) |  Time(s) | Speedup | Parallel Efficiency |
--- | --- | --- | --- | --- |
1 |(1, 1) |  2564.68  | 1 | - |
2 |(2, 1) |  1477.03  | 1.74 | 0.87 |
3 |(3, 1) |  1052.06  | 2.44 | 0.81 |
4 |(4, 1) |  865.12   | 2.96 | 0.74 |
5 |(1, 2) |  1505.52  | 1.70 | 0.85 |
6 |(2, 2) |  957.81   | 2.68 | 0.67 |

<p> 
<img src="docs/Worksheet3_Plots/StrongScalingAnalysis.png" width="500">
</p>
<em>Speedup with increasing parallel processes (considering only Cases 1-4)</em>

Interesting result is the comparison between Case 4 and Case 6, each with 4 parallel processes. Case 4 (4,1) gives a much higher parallel efficiency than Case 6 (2,2). This can be explained by considering the total communication overhead. In this case we have (imax, jmax) = (85, 18). Therefore total number of cells across which results must be shared is, 54 cells (3*jmax) in Case 4 and 103 cells (imax + jmax) in Case 6.

#### Weak Scaling Analysis for Lid-Driven Cavity

Weak scaling analysis refers to comparison of total computation time with increasing the number of parallel processes while keeping the workload per process same. In this section, we carry out the weak scaling analysis on the Lid-driven cavity case. The parameters are kept same as in the Worksheet 1. We increase the workload by increasing the number of points in the domain such that each process works with same number of points as in the original domain.

(iproc, jproc) | (imax, jmax) | Time(s) |
--- | --- | --- |
(1, 1) | (50, 50) | 129.21  |
(1, 2) | (50, 100) | 333.90  |
(2, 2) | (100, 100) | 712.29 |

As you can clearly observe, weak scaling does not hold as per the above observation. However, there is another factor in play here, the adaptive time stepping. In the simulations above, we kept the domain size fixed, (xlength, ylength) = (1, 1). Increasing the number of points leads to smaller cell size, which in turn leads to, smaller time step. This means, we take much larger number of steps for same end time of simulation leading to a much larger communication overhead. To compensate for the adaptive time stepping, we repeated the weak scaling analysis with changing domain size to ensure that cell size remains same as in the original problem.
  
(iproc, jproc) | (imax, jmax) | (xlen, ylen) | Time(s) |
--- | --- | --- | --- |
(1, 1) | (50, 50) | (1, 1) | 129.21  |
(1, 2) | (50, 100) | (1, 2) | 137.28  |
(1, 2) | (100, 50) | (2, 1) | 175.10 |
(2, 2) | (100, 100) | (2, 2) | 217.11 |

<p> 
<img src="docs/Worksheet3_Plots/WeakScalingAnalysis.png" width="500">
</p>
<em>Computation time with increasing parallel processes with workload per process kept constant</em>

We can already observe that weak scaling results are much better. However, we still see a significant difference. Keeping in mind the effect of relaxation factor (w) in the validation cases above, we attribute the difference to the numerical algorithm solving the implicit pressure equation. In the parallel case, we use the old values for the cells on boundary connecting the domains in one step. This essentially means that we are no longer solving the equations with a "pure" SOR method.  

We observed that the run time was very sensitive to the other applications running in the computer. We tried our best to run the simulations with minimum applications open and kept it consistent across all simulations in one test, however the effect cannot be completely neglected. A better but time consuming way would be to run each simulation multiple times and take average. 

---
## Worksheet 2

### Plain Shear Flow
We begin with modeling the simple case of Plain Shear Flow in a pipe. No slip boundary conditions are used in this case. It can be observed that the pressure is continuosly decreasing along the length of the pipe. The reason for the decrease in pressure is frictional loss due to the presence of viscosity. From velocity contour, it can be observed that the highest velocity is seen along the centerline after the boundary layer is fully developed and a steady flow is reached. Velocity at wall is zero as expected. The velocity of 1.5 in the center matches with the value obtained from the analytical expression of velocity profile in pipe flow. 

<p>
<img src= "docs/Worksheet2_Plots/PlainShear_Pressure_Contour.png" width="500"> | <img src="docs/Worksheet2_Plots/PlainShear_VelocityX_Contour.png" width="500">
</p>
<em>Pressure Contour plot for pipe flow [left], and Velocity contour plot [right] </em>

<p>
<img src= "docs/Worksheet2_Plots/PlainShear_VelocityX_Profile.png" width="500"> | <img src="docs/Worksheet2_Plots/PlainShear_Pressure_Profile.png" width="500">
</p>
<em>Velocity profile along the section [left], and Pressure and Velocity magnitude profile along the centerline [right] </em>


### Karman Vortex Simulation
Karman vortex refers to vortex shedding caused by flow around a blunt obstacle. In present simulation, the fluid enters from left with a constant velocity and encounters a tilted plate. This leads to vortex shedding which can be seen in the velocity or pressure contour. A small recirculation zone behind the obstacle is observed as expected. You can find link to the animation file [here](https://gitlab.lrz.de/00000000014ADC3D/cfd-lab-group-mib/-/blob/Worksheet2/docs/Worksheet2_Plots/KarmanVortex_Velocity.avi)

<p>
<img src= "docs/Worksheet2_Plots/Obstcle_VelocityMag_Contour.png" width="500"> 
</p>
<em>Velocity Contour Plot for flow around the obstacle</em>

### Flow Over a Step
In this case, we simulate the effect of a sudden step in the flow path. The fluid enters from left with a constant velocity. No slip boundary conditions are used in the upper and lower plates. The flow expands on encountering the step. A recirculation zone is formed just downstream of the step. The velocity is higher near the entrance owing to the smaller area to conserve the mass flow rate. However, the velocity is more of less unifrom across the section. In the region towards the outlet, velocity is lower in magnitude and non-uniform across the section (parabolic as seen in the pipe flow case above).

<p>
<img src= "docs/Worksheet2_Plots/Step_Pressure_Contour.png" width="500"> | <img src="docs/Worksheet2_Plots/Step_VelocityMag_Contour.png" width="500">
</p>
<em>Pressure Contour Plot [left], Velocity Contour Plot [right]</em>


### Natural Convection
This is the first case where we simulated a flow sololey due to gravity without prescribing any fixed velocity at the boundary. The simulation domain is a square where top and bottom walls are insulated, left wall is heated and right wall is cooled. Gravity acts on the system.
We conducted a first simulation with viscosity nu = 0.001 and thermal diffusivity alpha = 0.000142857. In the following picture we can notice how the hot fluid tends to expand more in the higher region of the domain, while the cold fluid expands more in the lower region. Hot fluid is less dense and hence it tends to rise, while on the other hand cold fluid is denser and tends to descend. This is exactly what we can observe through the velocity contour. Glyphs on the left-hot side point upwards, while those on the right-cold side point downwards.

<p> 
<img src="docs/Worksheet2_Plots/NaturalConvection_Temp_contour_velGlyph_case1.png" width="500" >
</p>
<em>Temperature Contour plot superimposed with Velocity Glyphs, nu = 0.001 and alpha = 0.000142857.</em>

To understand why the flow moves with a clockwise circular motion we also need to have a look at the pressure profile. Pressure is overall higher in the top region of the domain, and lower on the bottom part. This is due to buoyancy effect. Since hot gas moves upward relative to cold, the region with more hot gas (top) will see pressure building up at top. For both top and bottom region we can still see a horizontal difference in pressure. On the top region pressure is higher on the left corner, while in the bottom region pressure is higher on the right corner. Fluid moves from regions of higher pressure to those of lower pressure, hence in the top region of the domain, gas moves from left to right and in the bottom region it moves from right to left. 

<p> 
<img src="docs/Worksheet2_Plots/NaturalConvection_Press_contour_velGlyph_case1.png" width="500">
</p>
<em>Pressure Contour plot superimposed with Velocity Glyphs, nu = 0.001 and alpha = 0.000142857. </em>

The overall effect is a circular motion. Motion along the y-axis is temperature driven, while motion along the x-axis is pressure driven. Pressure profile is given by the temperature we prescribed at the boundaries.

The same observations can be also made for the second simulation, which we conducted at nu = 0.0002 and alpha = 0.000028571. However, we can point out some substantial differences.
Due to the lower thermal diffusivity in the second simulation, heat is conducted less quickly and hence heated gas moves less rapidly. This is well showed in the temperature profile, here the region of heated fluid is much more schrinked if compared to simulation one. This also reflects in the pressure profile, where we can observe an over all lower value of pressure. Pressure has a 0 value on most of the domain and presents positive values only in the top left corner.The lower value in viscosity results in fluid layers moving more easily on top of each other. 
As an overall result, the vortex center has now moved to the left side of the domain, while in simulation one it was at the exact center of the domain.

<p> 
<img src="docs/Worksheet2_Plots/NaturalConvection_Temp_contour_velGlyph_case2.png" width="500"> | <img src="docs/Worksheet2_Plots/NaturalConvection_Press_contour_velGlyph_case2.png" width="500">
</p>
<em>Temperature Contour plot superimposed with Velocity Glyphs [left] and Pressure Contour plot superimposed with Velocity Glyphs [right], nu = 0.0002 and alpha = 0.000028571. </em>


### Fluid Trap Simulation
In this case we simulate a heat driven flow with obstacles. You can observe from the Temperature contour plot that the left wall is the hot wall while the right wall is the cold wall. The flow reaches an equilibrium where the hot fluid from left section is not able to propagate into the cold section on right and cold gas from right section is not able to reach the left section. Further, in the pressure contour, one may not the high pressure region in top-left and bottom-right of the domain. This is because the hot gas in left section moves upwards creating a high pressure region top-left of domain, while high amount of cold gas sinks to bottom in the right section creating high pressure zone. If we observe the velocity glyphs, we can see that some amount of hot gas from left section reaches right and moves upward, causing slight higher temperature in top-right compared to bottom-right. Similar observation can be made for left section. 

<p> 
<img src="docs/Worksheet2_Plots/FluidTrap_TempContour_VelGlyph.png" width="500">
</p>
<em>Temperature Contour plot superimposed with Velocity Glyphs</em>

<p> 
<img src="docs/Worksheet2_Plots/FluidTrap_PressContour_VelGlyph.png" width="500">
</p>
<em>Pressure Contour plot superimposed with Velocity Glyphs</em>

### Rayleigh Benard Convection

Rayleigh Benard is a natural convection occuring in a horizontal fluid heated from bottom. The fluid in contact with the hot wall at bottom rises up till it reaches the top (typically a fluid-fluid interface). The top boundary is modelled as a cold wall in present simulation. The fluid at top turns to side but cannot move far as it encounters another coloum of fluid around it. It instead forms a loop with itself creating isolated cells called Benard Cells. This is illustrated in the figure below.

<p> 
<img src="docs/Worksheet2_Plots/ConvectionCells.png" width="500">
</p>
<em>Convection Cells or Benard Cells (Source: Wikipedia)</em>

In both the temperature and velocity contour plot, we can observe the formation of convection cells. Glyphs highlight the closed velocity loops as expected for the problem. However, the pattern is not very regular in the present case. This may be due to coarse grids or smaller domain size as Rayleigh Benard convection requires the height of the fluid layer to be much smaller than the horizontal dimensions. The simulation was repeated with a longer domanin but results obtained were qualitatively similar.

<p> 
<img src="docs/Worksheet2_Plots/RB_TempContour_VelGlyph.png" width="500">
</p>
<em>Temperature Contour plot superimposed with Velocity Glyphs</em>

<p> 
<img src="docs/Worksheet2_Plots/RB_VelContourGlyph.png" width="500">
</p>
<em>Velocity Magnitude Contour and Glyphs</em>

<p> 
<img src="docs/Worksheet2_Plots/RB_VelContourGlyph_longerdomain.png" width="500">
</p>
<em>Velocity Magnitude Contour and Glyphs for case with longer domain</em>

---

## Worksheet 1
### Pressure Visualization

The image below shows the pressure contour for the lid driven cavity simulation, after some thousands time steps. As the picture shows, the highest pressure is in the upper right corner of the lid driven cavity while the lowest pressure is observed at the left upper corner. If one also takes a look at the glyphs in the velocity profile (next section), then it's obvious that the fluid moves from high pressure to low pressure. The only exception is the top of the cavity. Here the cells used for the discretization are "attached" to the lid and hence move with it (from low pressure to high pressure). 

<p> 
<img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray.png" width="500"> | <img src="example_cases/LidDrivenCavity/Plots/contour_pressure_gray_scaled.png" width="500">
</p>
<em>Pressure Contour default [left] and scaled [right]</em>

### Velocity Visualization

A expected, velocity is zero at left, right and bottom boundaries, while it's equal to the lid's velocity (one) at the top boundary, where cells are attached to the lid (as explained in the previous section).

<p> 
<img src="example_cases/LidDrivenCavity/Plots/contour_u_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/contour_v_gray.png" width="500">
<img src="example_cases/LidDrivenCavity/Plots/Glyph_velocity_gray.png" width="500">
</p>
<em>Velocity Contour in x-direction [left], Velocity Contour in y-direction [middle] and Velocity Glyph [right]</em>


### Examination of SOR solvers behavior depending on ??

To examine the SOR solver's behaviour depending on the relaxation factor ??, we conducted the simulation with different values of ?? and for each one of these and for each time step, we searched for the maximum number of iterations necessary for the solver to converge (res < eps). Among the ?? values we used, the one that required less iterations to converge was ??=1.9. For very low values of ?? a very large number of iterations is necessary to obtain convergence. That's consistent with the SOR solver formulation since p(n+1)->p(n) for ??->0. In the table below we report also the time step at which the max iteration is reached, i.e. the time step that required the maximum number of iterations to converge.

?? | max-iter | timestep |
--- | --- | --- |
0.5 | 3499 | 592 |
1.0 | 1132 | 1992 |
1.3 | 655 | 1992 |
1.6 | 346 | 1992 |
1.7 | 260 | 1992 |
1.8 | 176 | 1992 |
1.9 | 154 | 1992 |
1.95 | 295 | - |
1.99 | 1350 | - |


### The algorithm???s behavior depending on Timestep (??t)

In our code we provided an implementation for adapting the time step size ??t in accordance to the stability condition in Equation (13). However, in order to analyse the algorithm's behaviour depending on ??t, we kept it constant and equal to the values reported in the table below throughout the whole simulation. We observed that the solution diverged for the timesteps 0.01, 0.03 and 0.05 and converged for timesteps 0.005, 0.007 and 0.009.

dt | timestep | stabilitiy |
--- | --- | --- |
0.05 | 5 | Diverged |
0.03 | 7 | Diverged |
0.01 | 71 | Diverged |
0.009 | - | Converged |
0.007 | - | Converged |
0.005 | - | Converged |

### The algorithm???s behavior depending on i_max = j_max

We also studied the algorithm's behaviour when using different values of i_max an j_max. The bigger i_max=j_max gets, the finer the grid gets (smaller dx and dy). To ensure the stability condition (Equation 12), ??t also has to smaller. We conducted our simulations for varying values of i_max=j_max, but always keeping ??t constant to 0.05. We then looked at u(i_max/2, 7*j_max/8) at the end of the simulation for each one of these time steps. The only converged result we obtained was for i_max=j_max=16 for kinematic viscosity, nu=0.01, in all the other cases, solution diverged. However, for nu = 0.001, we obtained convergence upto i_max=j_max=32. The solution diverged for cases with further refinement. In order to obtain physical results also for bigger values of i_max=j_max we would need to decrease the time step size ??t. 

imax/jmax | Cell(i,j) | u(i,j) for nu=0.01 | u(i,j) for nu=0.001 |
--- | --- | --- | --- |
16 | (8,14) | 0.183833 | 0.139114 |
32 | (16,28) | -nan | 0.208146 |
64 | (32,56) | -nan | -nan |
128 | (64,112) | +nan | -nan |
256 | (128,224) | +nan | +nan |



### Effect of kinematic viscosity

We investigated the effect of kinematic viscosity on our simulation. We looked at the velocity profile for different values of kinematic viscosity (nu). For lower values of viscosity the effect of the moving lid can propagate deeper into the fluid, however the velocity magnitude is very high only for regions of fluid that are very close to the lid. This can be understood by considering the Reynolds number (Re), which is inversely proportional to the kinematic viscosity. At large Re, the boundary layer attached to the lid is similar to a turbulent boundary layer (zero relative velocity close the wall which decreases sharply as you move away from it). Also, large Re corresponds to greater mixing in the flow causing the effect of moving lid to be felt at distant locations in the cavity.

For large viscosity (low Re), the region of "high" (close to 1) velocity magnitude extends more (as in a laminar flow boundary layer) but fluid away from the lid remains largely unaffected (the rest of the cavity is at values very close to zero).

nu = 0.01 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu01.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu01.png" width="500"> 

nu = 0.002 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu002.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu002.png" width="500"> 

nu = 0.0005 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0005.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0005.png" width="500"> 

nu = 0.0001 :
<img src="/example_cases/LidDrivenCavity/Plots/NuComparison/ustream_100_nu0001.png" width="500"> | <img src="/example_cases/LidDrivenCavity/Plots/NuComparison/u_100_nu0001.png" width="500"> 
