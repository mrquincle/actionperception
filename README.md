<!-- Uses markdown syntax for neat display at github -->

# Action-Perception
Perception is intertwined with action for robots. This changes a lot for machine learning techniques:
* sensorimotor invariances can be learned: turning your head will result in a rotating visual view, if a sensorimotor invariance is known this can be compensated and the view is stable nevertheless
* sequences of action-perception events can be learned, as in reinforcement learning in case an external reward is applied (modeling the robot and environment as a POMDP)
* predictions of action-perception chains are possible
* information-theoretic properties of perception (e.g. the entropy) can be used as "intrinsic reward" to guide robot actions, to implement for example a robot that is curious, or that avoids danger at all times

## What does this code do?
This code will contain standard and non-standard POMDP benchmarks (which include reward) and non-POMDP benchmark (which do not include reward) and methods to solve the corresponding problems. One of the POMDP problems if the "Recycling Robots" benchmark problem in which two robots need to cooperate to retrieve large cans from the environment. This namely gives them a higher reward than collecting small cans. Each time the robot runs out of energy it is punished. Note, that this a multi-robot scenario. 

The methods that are developed most often use a form of dynamic programming. Dynamic programming is closely related to the Bellman equation. It narrows down to the fact that if you can write something as an optimisation process, intermediate results can also be described as the same optimisation process. If you maximize over a series of values you can use the results of maximizing over a subset of these values. Hence, it is possible to write such process in a recursive form, the so-called Bellman equation. 

## Where can I read more?
* [Wikipedia](http://en.wikipedia.org/wiki/Bellman_equation)

## Copyrights
The copyrights (2012) belong to:

- Author: Anne van Rossum
- Almende B.V., http://www.almende.com and DO bots B.V., http://www.dobots.nl
- Rotterdam, The Netherlands
