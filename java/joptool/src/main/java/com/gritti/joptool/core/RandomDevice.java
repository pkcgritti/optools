package com.gritti.joptool.core;

import java.util.Random;

public class RandomDevice {
	static private RandomDevice instance;
	private Random device;

	public RandomDevice() {
		device = new Random();
	}

	public double nextUniform() {
		return device.nextDouble();
	}

	public double nextGaussian() {
		return device.nextGaussian();
	}

	static public RandomDevice getInstance() {
		if (RandomDevice.instance == null) {
			RandomDevice.instance = new RandomDevice();
		}
		return RandomDevice.instance;
	}
}
