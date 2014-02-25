public class Main {
	private static Team  teams[] = new Team[12];

	public static void main(String args[]){
		int pos = 1;
		putInfo();
	// games games
		groupGames (teams[0] , teams[1] , teams[2] , teams[3]);// group A
		groupGames (teams[4] , teams[5] , teams[6] , teams[7]);// group B
		groupGames (teams[8] , teams[9] , teams[10] ,teams[11]);// group C
	//summarize total points
		sumPoints();
	//total place in group
		groupPlace(0, 1, 2, 3);
		groupPlace(4, 5, 6, 7);
		groupPlace(8, 9, 10, 11);
		for (int i=0 ; i < 12 ; i++)
			if (teams[i].getGroupPlace() == 1)
				teams[i].sumTournamentRating(1000000000);
	//table place
		for (int i=0 ; i < 12 ; i++){
			int  max=0 , index = 0;
			for(int j=0 ; j <12 ; j++)
				if( max < teams[j].getTournamentRating()){
					max = teams[j].getTournamentRating();
					index = j;
				}
			teams[index].setTournamentRating(pos);
			pos++;
		}
	//pla off
		playOff();
	//print results
		for (int i=1 ; i <= 12 ; i++)
			for(int j=0; j < 12 ; j++)
				if ( teams[j].getTournamentRating() == i)
					System.out.println(i + "place : " + teams[j].getTeamName() );
	}
	private static void playOff(){
		Game games = new Game();
		for (int i=0; i < 12 ; i++){
			for(int j=0; j < 12 ; j++)
				switch (teams[i].getTournamentRating()*100 + teams[j].getTournamentRating()){
					case 512: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(12); teams[j].setTournamentRating(5);}   break;
					case 611: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(11); teams[j].setTournamentRating(6);} break;
					case 710: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(10); teams[j].setTournamentRating(7);} break;
					case 809: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(9); teams[j].setTournamentRating(8);} break;
				}
		}
		for (int i=0; i < 12 ; i++){
			for(int j=0; j < 12 ; j++)
				switch (teams[i].getTournamentRating()*100 + teams[j].getTournamentRating()){
					case 108: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(8); teams[j].setTournamentRating(1);}   break;
					case 207: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(7); teams[j].setTournamentRating(2);} break;
					case 306: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(6); teams[j].setTournamentRating(3);} break;
					case 405: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(5); teams[j].setTournamentRating(4);} break;
				}
		}
		for (int i=0; i < 12 ; i++){
			for(int j=0; j < 12 ; j++)
				switch (teams[i].getTournamentRating()*10 + teams[j].getTournamentRating()){
					case 14: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(4); teams[j].setTournamentRating(1);}   break;
					case 23: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(3); teams[j].setTournamentRating(2);} break;
				}
		}
		for (int i=0; i < 12 ; i++){
			for(int j=0; j < 12 ; j++)
				switch (teams[i].getTournamentRating()*10 + teams[j].getTournamentRating()){
					case 12: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(2); teams[j].setTournamentRating(1);}   break;
					case 34: if (games.gamePlayOff(teams[i], teams[j])!=0){teams[i].setTournamentRating(4); teams[j].setTournamentRating(3);} break;
				}
		}
		
		
	}
	private static void putInfo(){
		for (int i=0; i < teams.length ; i++)
			teams[i] = new Team();
		//1 -USA
			teams[0].setTeamName("USA");
			teams[0].setRatingIIHF(289);
		//2 -Russia
			teams[1].setTeamName("Russia");
			teams[1].setRatingIIHF(304); 
		//3 -Slovenia
			teams[2].setTeamName("Slovenia");
			teams[2].setRatingIIHF(221);
		//4 -Slovakia
			teams[3].setTeamName("Slovakia");
			teams[3].setRatingIIHF(284);
		//5 -Finland
			teams[4].setTeamName("Finland");
			teams[4].setRatingIIHF(306);
		//6 -Canada
			teams[5].setTeamName("Canada");
			teams[5].setRatingIIHF(294);
		//7 -Austria
			teams[6].setTeamName("Austria");
			teams[6].setRatingIIHF(226);
		//8 -Norway
			teams[7].setTeamName("Norway");
			teams[7].setRatingIIHF(268); 
		//9 -Sweden
			teams[8].setTeamName("Sweden");
			teams[8].setRatingIIHF(310);
		//10 -Switzerland
			teams[9].setTeamName("Switzerland");
			teams[9].setRatingIIHF(284);
		//11 -Czech Republic
			teams[10].setTeamName("Czech Republic");
			teams[10].setRatingIIHF(297);
		//12 -Latvia
			teams[11].setTeamName("Latvia");
			teams[11].setRatingIIHF(252);		
		
	}
	private static void groupGames(Team team_1, Team team_2, Team team_3, Team team_4){
		Game games = new Game();
		games.gameGr (team_1 , team_2);
		games.gameGr (team_1 , team_3);
		games.gameGr (team_1 , team_4);
		games.gameGr (team_2 , team_3);
		games.gameGr (team_2 , team_4);
		games.gameGr (team_3 , team_4);
	}

	private static void sumPoints(){
			for(int i=0 ; i<12 ; i++)
				totalPoints(i);						
	}
	private static void totalPoints(int teamNomber){
		int points=1;
		points *= teams[teamNomber].getPoints();
		points = points*1000 + teams[teamNomber].getDifferentPucks() + 100;
		points = points*100 + teams[teamNomber].getPucks();
		points = points*1000 + teams[teamNomber].getRatingIIHF();
		teams[teamNomber].setTournamentRating(points);		
	}
	private static void groupPlace (int team1 , int team2 , int team3 , int team4){
		changPlace(team1 , team2);
		changPlace(team1 , team3);
		changPlace(team1 , team4);
		changPlace(team2 , team3);
		changPlace(team2 , team4);
		changPlace(team3 , team4);
	}
	private static void changPlace (int team1 , int team2){
		if ( teams[team1].getTournamentRating() > teams[team2].getTournamentRating())
			teams[team1].chGroupPlace();
		else 
			teams[team2].chGroupPlace();
	}
}
