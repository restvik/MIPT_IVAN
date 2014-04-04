package ru.mipt.cs.gim.matrix;

import ru.mipt.cs.gim.matrix.exeprions.*;

public class MatrixMath {

	public static Matrix transposition (Matrix matrix){
		Matrix resultMatrix = new Matrix ( matrix.height, matrix.widght);
		for (int i=0; i < matrix.height; ++i)
			for (int j=0; j< matrix.widght; ++j)
				try {resultMatrix.strings[j].values[i] =  matrix.strings[i].values[j];}
				catch (ArrayIndexOutOfBoundsException e) {}
		return resultMatrix;
		
	}
	
	public static Matrix nullMatrix(Matrix matrix){
		for (int i=0; i < matrix.height; ++i)
			for (int j=0; j< matrix.widght; ++j)
				matrix.strings[i].values[j]=0;
		return matrix;
	}
	
	public static Matrix multiple (Matrix matrix1, Matrix matrix2) throws WrongSize {
		if (matrix1.widght != matrix2.height)
			throw new WrongSize("Wrong size of matrices for multiple!!");
		Matrix resultMatrix = new Matrix ( matrix1.height, matrix2.widght);
		resultMatrix = MatrixMath.nullMatrix( resultMatrix);
		for (int i=0; i < matrix1.height; ++i)
			for (int j=0; j< matrix2.widght; ++j)
				for (int k=0; k<matrix1.widght; ++k)
					try {	resultMatrix.strings[i].values[j] +=  matrix1.strings[i].values[k] * matrix2.strings[k].values[j];	}
					catch (ArrayIndexOutOfBoundsException e) {}
		return resultMatrix;
	}
	
	public static  Matrix multipleVector (Vector vector1 , Vector vector2) throws WrongSize{
		if (vector1.height != vector2.height)
			throw new WrongSize("Wrong size of vector for multiple!!");
		Matrix resultVector = new Matrix (1 , 1);
		Matrix transpVector = new Matrix (vector1.height,1);
		transpVector = MatrixMath.transposition(vector1);
		resultVector = MatrixMath.multiple(transpVector, vector2);
		return resultVector;
	}
	public static Matrix addition (Matrix matrix1, Matrix matrix2) throws WrongSize{
		if ( (matrix1.widght != matrix2.widght) || (matrix1.height != matrix2.height))
			throw new WrongSize("Wrong size of matrices for fold");
		Matrix resultMatrix = new Matrix ( matrix1.widght, matrix1.height);
		for (int i=0; i < matrix1.height; ++i)
			for (int j=0; j< matrix1.widght; ++j)
				try { resultMatrix.strings[i].values[j] =  matrix1.strings[i].values[j] + matrix2.strings[i].values[j];}
				catch (ArrayIndexOutOfBoundsException e) {}
		return resultMatrix;
		
	}
	
	public static Matrix substruct (Matrix matrix1, Matrix matrix2) throws WrongSize{
		if ( (matrix1.widght != matrix2.widght) || (matrix1.height != matrix2.height))
			throw new WrongSize("Wrong size of matrices for fold");
		Matrix resultMatrix = new Matrix ( matrix1.widght, matrix1.height);
		for (int i=0; i < matrix1.height; ++i)
			for (int j=0; j< matrix1.widght; ++j)
				try { resultMatrix.strings[i].values[j] =  matrix1.strings[i].values[j] - matrix2.strings[i].values[j];}
				catch (ArrayIndexOutOfBoundsException e) {}
		return resultMatrix;
		
	}

	
	public static void printMatrix (Matrix matrix)	{
		for (int i=0; i< matrix.height; ++i){
			for (int j=0; j < matrix.widght; ++j)
				System.out.print (String.format("%3.3f ",matrix.strings[i].values[j]));
			System.out.println("");
		}
	}	
	
	
	
	 public static double determinant(Matrix matrix)throws NotSquare{
		 if (matrix.isSquare== false)
			 throw new NotSquare();
	        double det=0;
	        if (matrix.height == 2){
	            det=matrix.strings[0].values[0]*matrix.strings[1].values[1]-matrix.strings[1].values[0]*matrix.strings[0].values[1];
	        }
	        else{
	            int koeff=1;
	            for(int i=0; i < matrix.height; i++){
	                if(i%2==1){  
	                    koeff=-1;
	                }
	                else{
	                    koeff=1;
	                };                
	                det += koeff*matrix.strings[0].values[i]*MatrixMath.determinant(MatrixMath.getMinor(matrix,0,i)); 
	            }
	        }
	        return det;
	    }
	    
	   
	    private static Matrix getMinor(Matrix matrix, int row, int column){
	        int minorLength = matrix.height - 1;
	        Matrix minor = new Matrix(minorLength , minorLength);
	        int dI=0;
	        int dJ=0;
	        for(int i=0; i<=minorLength; i++){
	            dJ=0;
	            for(int j=0; j<=minorLength; j++){
	                if(i==row){
	                    dI=1;
	                }
	                else{
	                    if(j==column){
	                        dJ=1;
	                    }
	                    else{
	                        minor.strings[i-dI].values[j-dJ] = matrix.strings[i].values[j];
	                    }
	                }
	            }
	        }
	        
	        return minor;
	   }
}
