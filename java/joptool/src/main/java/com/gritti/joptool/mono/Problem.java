package com.gritti.joptool.mono;

import com.gritti.joptool.core.Matrix;

public abstract class Problem {
	protected Matrix lowerBounds;
	protected Matrix upperBounds;
	protected Matrix rangeBounds;

	protected int dimension;

	public Problem(int dimension, double[] lowerBounds, double[] upperBounds) {
		this.dimension = dimension;
		this.lowerBounds = Matrix.from(lowerBounds, 1);
		this.upperBounds = Matrix.from(upperBounds, 1);
		this.rangeBounds = Matrix.subtract(this.upperBounds, this.lowerBounds);
	}
}
