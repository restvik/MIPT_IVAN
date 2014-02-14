public class Main {

		public static void main(String args[]) {
			Teacher teachers[] = new Teacher[2];
			Student students[] = new Student[3];
			for (int i=0; i < teachers.length ; i++){
				teachers[i] = new Teacher();
			}
			
			for (int i=0; i < students.length ; i++){
				students[i] = new Student();
			}
			teachers[0].human_name = "Maria Vasil'evna";
			teachers[0].subject = "mathematics"; 
			teachers[1].human_name = "Viktor Genad'evich";
			teachers[1].subject = "physics";
			
			System.out.println("Professors in class:");
			System.out.println("  " + teachers[0].subject + ":  " + teachers[0].human_name + "\n");
			System.out.println("  " + teachers[1].subject + ":  " + teachers[1].human_name + "\n");
			
			students[0].human_name = "Ivan";
			students[0].mark_m = teachers[0].put_mark(students[0].get_answer(teachers[0].give_questions()));
			students[0].mark_p = teachers[1].put_mark(students[0].get_answer(teachers[1].give_questions()));
			const_str(students[0].mark_p , students[0].mark_m , students[0].human_name);
			
			students[1].human_name = "Ann";
			students[1].mark_m = teachers[0].put_mark(students[1].get_answer(teachers[0].give_questions()));
			students[1].mark_p = teachers[1].put_mark(students[1].get_answer(teachers[1].give_questions()));
			const_str(students[1].mark_p , students[1].mark_m , students[1].human_name);
			
			students[2].human_name = "Alex";
			students[2].mark_m = teachers[0].put_mark(students[2].get_answer(teachers[0].give_questions()));
			students[2].mark_p = teachers[1].put_mark(students[2].get_answer(teachers[1].give_questions()));
			const_str(students[2].mark_p , students[2].mark_m , students[2].human_name);
		}
		
		private static void const_str(int mark_p, int mark_m, String name){
			String s=name+" \n Physics: "+mark_p+" Matematics: "+mark_m;
			System.out.println(s);
		}
	
}
