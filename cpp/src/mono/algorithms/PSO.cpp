#include "PSO.hpp"

#include <iostream>
#include <limits>
#include <future>

#include "../Random.hpp"
#include "../ThreadPool.hpp"

namespace mono::algorithms {
    PSO::PSO(size_t popsize, double c1, double c2, double inertia) {
        this->popsize = popsize;
        this->c1 = c1;
        this->c2 = c2;
        this->inertia = inertia;
    }

    void PSO::initialize(Problem& problem) {
        const vector<double>& lb = problem.getLowerBoundaries();
        const vector<double>& ub = problem.getUpperBoundaries();
        const size_t dimension = problem.getDimension();
        vector<double> rb(lb.size());
        Random& random = Random::getInstance();

        for (size_t j = 0; j < lb.size(); j++) rb[j] = ub[j] - lb[j];

        solutions = vector<double>(popsize * dimension);
        velocity = vector<double>(popsize * dimension);
        particleBestSolutions = vector<double>(popsize * dimension);
        globalBestSolution = vector<double>(dimension);
        particleBestCosts = vector<double>(popsize);
        costs = vector<double>(popsize);
        globalBestCost = std::numeric_limits<double>::max();

        for (size_t i = 0; i < popsize; i++) {
            particleBestCosts[i] = std::numeric_limits<double>::max();
            for (size_t j = 0; j < dimension; j++) {
                size_t index = i * dimension + j;
                solutions[index] = particleBestSolutions[index] = random.nextUniform() * rb[j] - lb[j];
                velocity[index] = 0.0;
            }
        }
    }

    void PSO::evaluate(Problem& problem) {
    #if 0
        ThreadPool& pool = ThreadPool::getInstance();
        vector<std::future<void>> results;
        const size_t dimension = problem.getDimension();

        for (size_t member = 0; member < popsize; member++) {
            results.emplace_back(pool.enqueue(
                [this, &problem] (size_t dimension, size_t member) {
                    double* data = solutions.data();
                    costs[member] = problem.evaluate(data + dimension * member);
                    if (costs[member] < particleBestCosts[member]) {
                        particleBestCosts[member] = costs[member];
                        for (size_t j = 0; j < dimension; j++) {
                            particleBestSolutions[member * dimension + j] = solutions[member * dimension + j];
                        }
                    }
                }, dimension, member)
            );
        }
        
        for (size_t member = 0; member < popsize; member++) {
            results[popsize - member - 1].get();
            if (costs[member] < globalBestCost) {
                globalBestCost = costs[member];
                for (size_t j = 0; j < dimension; j++) {
                    globalBestSolution[j] = solutions[member * dimension + j];
                }
            }
        }
    #elif 0
        ThreadPool& pool = ThreadPool::getInstance();
        vector<std::future<void>> results;
        const size_t dimension = problem.getDimension();
        double* data = solutions.data();

        for (size_t member = 0; member < popsize; member++) {
            results.push_back(pool.enqueue(
                [this, &problem, data] (size_t dimension, size_t member) {
                    costs[member] = problem.evaluate(data + dimension * member);
                    if (costs[member] < particleBestCosts[member]) {
                        particleBestCosts[member] = costs[member];
                        for (size_t j = 0; j < dimension; j++) {
                            particleBestSolutions[member * dimension + j] = solutions[member * dimension + j];
                        }
                        if (costs[member] < globalBestCost) {
                            globalBestCost = costs[member];
                            for (size_t j = 0; j < dimension; j++) {
                                globalBestSolution[j] = solutions[member * dimension + j];
                            }
                        }
                    }
                }, dimension, member)
            );
        }
    #elif 0
        ThreadPool& pool = ThreadPool::getInstance();
        vector<std::future<void>> results;
        const size_t dimension = problem.getDimension();
        double* data = solutions.data();

        for (size_t member = 0; member < popsize; member++) {
            results.push_back(pool.enqueue(
                [this, &problem, data] (size_t dimension, size_t member) {
                    costs[member] = problem.evaluate(data + dimension * member);
                    if (costs[member] < particleBestCosts[member]) {
                        particleBestCosts[member] = costs[member];
                        for (size_t j = 0; j < dimension; j++) {
                            particleBestSolutions[member * dimension + j] = solutions[member * dimension + j];
                        }
                        if (costs[member] < globalBestCost) {
                            globalBestCost = costs[member];
                            for (size_t j = 0; j < dimension; j++) {
                                globalBestSolution[j] = solutions[member * dimension + j];
                            }
                        }
                    }
                }, dimension, member)
            );
        }
    #elif 0
        std::vector<std::future<void>> futures(popsize);
        for (size_t member = 0; member < popsize; member++) {
            auto fut = std::async([] {
                // const size_t dimension = problem.getDimension();
                // double* data = solutions.data();
                // costs[member] = problem.evaluate(data + dimension * member);
                // if (costs[member] < particleBestCosts[member]) {
                //     particleBestCosts[member] = costs[member];
                //     for (size_t j = 0; j < dimension; j++) {
                //         particleBestSolutions[member * dimension + j] = solutions[member * dimension + j];
                //     }
                //     if (costs[member] < globalBestCost) {
                //         globalBestCost = costs[member];
                //         for (size_t j = 0; j < dimension; j++) {
                //             globalBestSolution[j] = solutions[member * dimension + j];
                //         }
                //     }
                // }
            });
            futures[member] = std::move(fut);
        }

        std::for_each(futures.begin(), futures.end(), [] (std::future<void>& fut) {
            fut.wait();
        });
    #elif 1
        double* data = solutions.data();
        const size_t dimension = problem.getDimension();
        for (size_t member = 0; member < popsize; member++) {
            costs[member] = problem.evaluate(data + dimension * member);
            if (costs[member] < particleBestCosts[member]) {
                particleBestCosts[member] = costs[member];
                for (size_t j = 0; j < dimension; j++) {
                    particleBestSolutions[member * dimension + j] = solutions[member * dimension + j];
                }
                if (costs[member] < globalBestCost) {
                    globalBestCost = costs[member];
                    for (size_t j = 0; j < dimension; j++) {
                        globalBestSolution[j] = solutions[member * dimension + j];
                    }
                }
            }
        }
    #endif
    }

    void PSO::evolve(Problem& problem) {
    #if 0
        ThreadPool& pool = ThreadPool::getInstance();
        size_t nThreads = pool.size();
        size_t batchSize = popsize / nThreads;
        size_t dimension = problem.getDimension();
        Random& random = Random::getInstance();
        vector<std::future<void>> results;
        
        for (size_t member = 0; member < popsize; member += batchSize) {
            size_t offset = member * dimension;
            results.push_back(pool.enqueue(
                [this, &random, &batchSize] (double* position, double* velocity, double* pBest, double* gBest, size_t ldim) {
                    for (size_t i = 0; i < batchSize; i++) {
                        for (size_t j = 0; j < ldim; j++) {
                            size_t index = j + i * ldim;
                            velocity[index] = inertia * velocity[index];
                                 + c1 * /* random.nextUniform() */ 0.5 * (pBest[index] - position[index])
                                 + c2 * /* random.nextUniform() */ 0.5 * (gBest[j] - position[index]);
                            position[index] += velocity[index];
                        }
                    }
                }, solutions.data() + offset, velocity.data() + offset, particleBestSolutions.data() + offset, globalBestSolution.data() + offset, dimension)
            );
        }

        for (std::future<void>& result: results)
            result.get();
    #elif 1
        // ThreadPool& pool = ThreadPool::getInstance();
        Random& random = Random::getInstance();
        const size_t dimension = problem.getDimension();

        for (size_t member = 0; member < popsize; member++) {
            for (size_t j = 0; j < dimension; j++) {
                const size_t index = member * dimension + j;
                velocity[index] = inertia * velocity[index]
                    + c1 * /* random.nextUniform() */ 0.5 * (particleBestSolutions[index] - solutions[index])
                    + c2 * /* random.nextUniform() */ 0.5 * (globalBestSolution[j] - solutions[index]);
                solutions[index] += velocity[index];
            }
        }
    #endif
    }

    Result PSO::solve(Problem& problem, size_t iterations) {
        initialize(problem);
        for (std::size_t i = 0; i < iterations; i++) {
            evaluate(problem);
            evolve(problem);
            // if ((i + 1) % 10 == 0 || i == 0) {
            //     std::cout << "PSO on iteration " << i + 1
            //         << " with cost " << globalBestCost << std::endl;
            // }
        }
        evaluate(problem);
        return Result(globalBestSolution, globalBestCost);
    }
};