# timetabling-problem-solver
A C++ metaheuristics solver for the ETP (examination timetabling problem).

# Data representation
- Vector of exams: `v[i]` is the timeslot assigned to exam `i`
- Vector of timeslots: `v[t]` is a list of exams scheduled on timeslot `t`

# Metaheuristic
## 1. Multi-start: Genetic Algorithm

Solution &rarr; chromosome  
Exam (element of the solution) &rarr; gene

#### Initial population
- Population size: max(#exams * 0.1, 10)
- Population: random chromosomes (feasible solution):
  1. exams are sorted by number of conflicts
  2. exams are placed in random timeslots
  3. restart from scratch when stuck
 
#### Evolution
1. Sorting chromosomes according to the fitness score (`1/penalty`)
2. Offspring generation
  - Elite Chromosomes: 20%
  - Crossover (adapted for feasibility): 30%
  - Mutation: 30%
  - Inversion (adapted for feasibility): 20%

#### Stopping condition
(used > 80% of total time) OR ( (used > 50% of t.t.) AND (no improving solutions from more than 8% of t.t) )

## 2. Neighborhood Search: Simulated Annealing
#### Initialization
- Initial solution: the best one found by GA
- Initial temperature: `(F(x_AVG) - F(x*)) / ln(0.5)`
- Cooling rate: `0.99`
- Plateau: 10 * `n_exams` * `n_timeslots`

#### Neighbour
1. Pick a random exam
2. Pick a random timeslot (different from current one)
3. Move the exam to that timeslot if is feasible to do it
4. Move it if `rand(0, 1) < p`, where `p` is a function of the candidate and the current penalties


# Posible improvements
- Use more C++ syntax instead of C's one
- Multi-thread solution generation (and the entire problem itself)
- Incremental penaly computation when moving examx
