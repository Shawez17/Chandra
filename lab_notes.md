Fri Aug 14 11:49:11 IST 2026
- disabled the startup way to the Pwm ic but not the aux winding path

based on reading of transformer and other stuff
- duty cycle drops at drain [?]
    how mosfte behave? and how come its pulling the thing low?

- current consumption of transformer is high[magnetic energy formula?]

Check Using Two Cable, checked

- Who is reducing my duty cycle if not Comp pin/feedback circuitm
    only use to check the current spark buddy. 600mV Vds has nothin effect on Vgs as switch.
- I_sense resistor consumes all my current but why half thhe voltage?
    high duty cycle (98%), make inductor to not loose sight of current. when turned off , thats why
Goal: reduce and control duty cycle.

Tue Aug 18 10:47:16 IST 2026
- Connect R_comp1-C_comp1 pin to R_fb-fb pin. [X]
- c_comp2 nonGnd to r_comp-comp pin [X]
- R_t1-Vref to Pin3-Q1-S with C_ and R_ramp in series
    their slope detrmines the value of duty cycle

Vlaue of R and C fro a particular duty cycle. What duty cycle


