package com.gritti.joptool.mono;

public class Result {
	private final double[] solution;
	private final double[] history;

	public Result(double[] solution, double[] history) {
		this.solution = solution.clone();
		this.history = history.clone();
	}

	public double[] getSolution() {
		return solution.clone();
	}

	public double[] getHistory() {
		return history.clone();
	}

	public double getCost() {
		return history[history.length - 1];
	}
}
