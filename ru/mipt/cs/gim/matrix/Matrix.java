package ru.mipt.cs.gim.matrix;

public class Matrix
{
	protected int height;
	protected int widght;
	protected MatrixString[] strings;
	protected boolean isSquare;

	public int getHeight() {
		return height;
	}
	public int getWidght() {
		return widght;
	}

	public Matrix (int widght, int height)
	{
		this.height = height;
		this.widght = widght;
		if (height == widght)
			isSquare = true;
		else
			isSquare = false;
		strings = new MatrixString[height];
		for (int i = 0; i < height; ++i)
			strings[i] = new MatrixString(widght);
	}

}
