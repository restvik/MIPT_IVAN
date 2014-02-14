public class Student extends Human {
	public int mark_m;
	public int mark_p;
	// функция считает количество неправильных ответов на некоторое колтчество заданных вопросов.
	public int get_answer(int num_questions){
		int wr_ans=0;
		
		for (int i=0; i < num_questions ; i++)
				if( (int) (Math.random()*2) == 0){
					wr_ans+=1;
				}
		return wr_ans;
	}

}
