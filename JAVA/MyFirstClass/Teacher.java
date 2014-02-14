public class Teacher  extends Human {
	public String subject;
	
	public int  give_questions(){
		return (int) (Math.random()*5.99);
	}
	
	public int put_mark( int wr_ans){
		return (5 - wr_ans);
	}

}
