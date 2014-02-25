
public class Game {
	public   int gamePlayOff (Team team1 , Team team2){
		if (team1.prepare() > team2.prepare())
			return 1;
		else 
			if (team1.prepare() < team2.prepare())
				return 0;
			else
				if (team1.getRatingIIHF() > team2.getRatingIIHF())
					return 1;
				else
					return 0;
	}
	public   void gameGr( Team team1 ,  Team team2){
		int  matchDifferent = makeDifferent();
		int  matchMinPucks = makeMinPucks();
		
		if (team1.prepare() > team2.prepare()){
			team1.sumPoints(3);
			team1.sumPucks(matchMinPucks + matchDifferent);
			team1.sumDifferentPucks(matchDifferent);
			team2.sumPucks(matchMinPucks);
			team2.deductDifferentPucks(matchDifferent);
		}
		else 
			if (team1.prepare() < team2.prepare()){
					team2.sumPoints(3);
					team2.sumPucks(matchMinPucks + matchDifferent);
					team2.sumDifferentPucks(matchDifferent);
					team1.sumPucks(matchMinPucks);
					team1.deductDifferentPucks(matchDifferent);
			}
			else{
				boolids(team1,team2);
				team1.sumPucks(matchMinPucks);
				team2.sumPucks(matchMinPucks);
			}
	}
	private  void boolids(Team team1, Team team2){
		if (team1.getRatingIIHF() > team2.getRatingIIHF()){
			team1.sumPoints(2);
			team2.sumPoints(1);
			team1.sumDifferentPucks(1);
			team2.deductDifferentPucks(1);
		}
		else{
			team2.sumPoints(2);
			team1.sumPoints(1);
			team2.sumDifferentPucks(1);
			team1.deductDifferentPucks(1);
		}
	}
	private  int makeMinPucks(){
		return (int)(Math.random()*5);
	}
	private int makeDifferent(){
		return (int)(Math.random()*5);
	}

}
