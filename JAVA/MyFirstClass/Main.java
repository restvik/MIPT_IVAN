public class Main {

		public static void main(String args[]) {
			Teacher teachers[] = new Teacher[2]; // у меня в школе два учителя 
			Student students[] = new Student[3]; // и 3 ученика для простоты
			for (int i=0; i < teachers.length ; i++){
				teachers[i] = new Teacher();
			}
			for (int i=0; i < students.length ; i++){
				students[i] = new Student();
			}
			// присваиваю учетелем имена и их предмет
			teachers[0].human_name = "Maria Vasil'evna";
			teachers[0].subject = "mathematics"; 
			teachers[1].human_name = "Viktor Genad'evich";
			teachers[1].subject = "physics";
			// выведу их на экран в качестве преподовательского состава в школе
			System.out.println("Professors in class:");
			System.out.println("  " + teachers[0].subject + ":  " + teachers[0].human_name + "\n");
			System.out.println("  " + teachers[1].subject + ":  " + teachers[1].human_name + "\n");
			// успеваемость каждого ученика за год по двум предметам, вывожу на экран табеля каждого из них
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
		// создаю табель с оценками для вывода на экран
		private static void const_str(int mark_p, int mark_m, String name){
			String s=name+" \n Physics: "+mark_p+" Matematics: "+mark_m;
			System.out.println(s);
		}
	
}
