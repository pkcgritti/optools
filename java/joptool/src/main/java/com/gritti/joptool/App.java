package com.gritti.joptool;

import com.github.fommil.netlib.BLAS;
import com.gritti.joptool.core.Matrix;
import com.gritti.joptool.utils.Chrono;

public class App {
	public static void main(String[] args) {
		double[] data = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
		Matrix A = Matrix.from(new double[] { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, 2);
		Matrix B = Matrix.from(new double[] { 1.0, 4.0, 2.0, 5.0, 3.0, 6.0 }, 3);
		A.display();
		B.display();
		Matrix.multiply(A, B).display();

		Chrono.getInstance().execute(1, () -> {
			int M = 100;
			int N = 100;
			int K = 50;
			Matrix Atest = Matrix.ones(M, K);
			Matrix Btest = Matrix.ones(K, N);
			Matrix Ctest = Matrix.multiply(Atest, Btest);
			// Ctest.display();
		}).digest();

		System.out.println(BLAS.getInstance().getClass().getName());

		// C.display();
	}
}
