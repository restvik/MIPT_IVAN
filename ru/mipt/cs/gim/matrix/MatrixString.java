package ru.mipt.cs.gim.matrix;

public class MatrixString {
	private int widght;
	protected double[] values;
	
	public MatrixString(int widght) {
		this.widght = widght;
		values = new double [widght];
		for ( int i = 0; i < widght; ++i)
			values[i] = Math.random()*10;
	}
	
	public int getWidght()
	{
		return  widght;
	}
}
