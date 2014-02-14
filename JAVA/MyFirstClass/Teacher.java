public class Teacher  extends Human {
	// предмет который преподователь препадает
	public String subject;
	// задает вопросы ученику
	public int  give_questions(){
		return (int) (Math.random()*5.99);
	}
	// ставит ему оценку на основе полученных ответов
	public int put_mark( int wr_ans){
		return (5 - wr_ans);
	}

}
