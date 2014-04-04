package ru.mipt.cs.gim.matrix.exeprions;

public class NotSquare extends Exception{
	private static final long serialVersionUID = 1L;
	public NotSquare() {
			super ("Matrix isn't square!");
	}

}
