package com.gritti.joptool.core;

import com.github.fommil.netlib.BLAS;

public class Matrix {
	private int rows = 0;
	private int cols = 0;
	private double[] data = null;

	public Matrix(int rows, int cols) {
		reshape(rows, cols);
	}

	public Matrix display() {
		for (int i = 0; i < rows; i++) {
			displayRow(i);
		}
		System.out.write('\n');
		return this;
	}

	public static Matrix subtract(Matrix A, Matrix B) {
		Matrix C = A.clone();
		C.subtract(B);
		return C;
	}

	public Matrix subtract(Matrix other) {
		if (rows != other.rows || cols != other.cols)
			throw new IllegalArgumentException("Matrices have incompatible dimensions");
		return map((double value, int index) -> (value - other.data[index]));
	}

	public static Matrix multiply(Matrix A, Matrix B) {
		if (A.cols != B.rows)
			throw new IllegalArgumentException("Matrices have incompatible dimensions");
		Matrix C = new Matrix(A.rows, B.cols);
		BLAS.getInstance().dgemm("t", "t", A.rows, B.cols, A.cols, 1.0, A.data, A.cols, B.data, B.cols, 0.0, C.data,
				C.cols);
		return C;
	}

	public Matrix multiply(Matrix other) {
		Matrix C = Matrix.multiply(this, other);
		rows = C.rows;
		cols = C.cols;
		data = C.data.clone();
		return this;
	}

	public Matrix multiply(final double scalar) {
		map((double value) -> scalar * value);
		return this;
	}

	public Matrix displayRow(int row) {
		String repr = "[";
		final int offset = row * cols;
		for (int j = 0; j < cols; j++) {
			repr += String.format("%12.4f", data[offset + j]);
			repr += (j == cols - 1) ? "    ]" : " ";
		}
		System.out.println(repr);
		return this;
	}

	static public Matrix eye(int dim) {
		Matrix result = Matrix.zeros(dim);
		for (int i = 0; i < result.rows; i++) {
			result.data[i * result.rows + i] = 1.0;
		}
		return result;
	}

	static public Matrix from(double[] data, int rows) {
		int cols = data.length / rows;
		Matrix result = new Matrix(rows, cols);
		result.map((double value, int index) -> {
			return (index >= data.length) ? 0.0 : data[index];
		});
		return result;
	}

	static public Matrix ones(int rows, int cols) {
		return new Matrix(rows, cols).fill(1.0);
	}

	static public Matrix ones(int dim) {
		return Matrix.ones(dim, dim);
	}

	static public Matrix zeros(int rows, int cols) {
		return new Matrix(rows, cols);
	}

	static public Matrix zeros(int dim) {
		return Matrix.zeros(dim, dim);
	}

	static public Matrix randn(int rows, int cols, double mean, double std) {
		RandomDevice generator = RandomDevice.getInstance();
		Matrix result = new Matrix(rows, cols);
		result.map(() -> generator.nextGaussian() * std + mean);
		return result;
	}

	static public Matrix randn(int rows, int cols) {
		return Matrix.randn(rows, cols, 0.0, 1.0);
	}

	static public Matrix randn(int dim, double mean, double std) {
		return Matrix.randn(dim, dim, mean, std);
	}

	static public Matrix randn(int dim) {
		return Matrix.randn(dim, 0.0, 1.0);
	}

	static public Matrix random(int rows, int cols, double gain, double offset) {
		RandomDevice generator = RandomDevice.getInstance();
		Matrix result = new Matrix(rows, cols);
		result.map(() -> generator.nextUniform() * gain + offset);
		return result;
	}

	static public Matrix random(int rows, int cols) {
		return Matrix.random(rows, cols, 1.0, 0.0);
	}

	static public Matrix random(int dim, double gain, double offset) {
		return Matrix.random(dim, dim, gain, offset);
	}

	static public Matrix random(int dim) {
		return Matrix.random(dim, 1.0, 0.0);
	}

	public Matrix fill(final double value) {
		BasicMapper filler = (double current) -> value;
		map(filler);
		return this;
	}

	public Matrix map(DefaultMapper mapFcn) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				data[i * cols + j] = mapFcn.execute(data[i * cols + j], i, j);
			}
		}
		return this;
	}

	public Matrix map(DirectMapper mapFcn) {
		for (int i = 0; i < rows * cols; i++) {
			data[i] = mapFcn.execute(data[i], i);
		}
		return this;
	}

	public Matrix map(BasicMapper mapFcn) {
		for (int i = 0; i < rows * cols; i++) {
			data[i] = mapFcn.execute(data[i]);
		}
		return this;
	}

	public Matrix map(EmptyMapper mapFcn) {
		for (int i = 0; i < rows * cols; i++) {
			data[i] = mapFcn.execute();
		}
		return this;
	}

	public Matrix forEach(DefaultIterator itFcn) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				itFcn.execute(data[i * cols + j], i, j);
			}
		}
		return this;
	}

	public Matrix forEach(DirectIterator itFcn) {
		for (int i = 0; i < rows * cols; i++) {
			itFcn.execute(data[i], i);
		}
		return this;
	}

	public Matrix forEach(BasicIterator itFcn) {
		for (int i = 0; i < rows * cols; i++) {
			itFcn.execute(data[i]);
		}
		return this;
	}

	public Matrix reshape(int rows, int cols) {
		if (data == null) {
			data = new double[rows * cols];
		} else {
			double[] oldData = data;
			data = new double[rows * cols];
			int dim = Math.min(oldData.length, data.length);
			for (int i = 0; i < dim; i++) {
				data[i] = oldData[i];
			}
		}
		this.rows = rows;
		this.cols = cols;
		return this;
	}

	public double[] getData() {
		return data.clone();
	}

	public int getRows() {
		return rows;
	}

	public int getCols() {
		return cols;
	}

	public interface DefaultMapper {
		double execute(double value, int row, int col);
	}

	public interface DirectMapper {
		double execute(double value, int index);
	}

	public interface BasicMapper {
		double execute(double value);
	}

	public interface EmptyMapper {
		double execute();
	}

	public interface DefaultIterator {
		void execute(double value, int row, int col);
	}

	public interface DirectIterator {
		void execute(double value, int index);
	}

	public interface BasicIterator {
		void execute(double value);
	}

	public Matrix clone() {
		Matrix mClone = new Matrix(rows, cols);
		mClone.data = data.clone();
		return mClone;
	}
}
