package com.gritti.joptool.utils;

import java.util.ArrayList;
import java.util.List;

public class Chrono {
	static private Chrono instance;
	private List<Long> measurements;

	private Chrono() {
		measurements = new ArrayList<Long>();
	}

	public Chrono execute(Executable executable) {
		Long start = System.currentTimeMillis();
		executable.execute();
		Long end = System.currentTimeMillis();
		measurements.add(end - start);
		return this;
	}

	public Chrono execute(int times, Executable executable) {
		for (int i = 0; i < times; i++)
			execute(executable);
		return this;
	}

	public Chrono digest() {
		System.out.println(measurements);
		return this;
	}

	static public Chrono getInstance() {
		if (Chrono.instance == null)
			Chrono.instance = new Chrono();
		return Chrono.instance;
	}

	public interface Executable {
		void execute();
	}
}
