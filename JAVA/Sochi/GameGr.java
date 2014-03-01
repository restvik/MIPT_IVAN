
public class GameGr extends Game{
	private  int pucks1 = 0, pucks2 = 0;
	
	public   void gameGr( Team team1 ,  Team team2){
		int  matchDifferent = makeDifferent();
		int  matchMinPucks = makeMinPucks();
		
		changeMutchNumber();		
		if (team1.prepare() > team2.prepare()){
			team1.sumPoints(3);
			pucks1 = matchMinPucks + matchDifferent;
			pucks2 = matchMinPucks;
			team1.sumPucks(pucks1);
			team1.sumDifferentPucks(matchDifferent);
			team2.sumPucks(pucks2);
			team2.deductDifferentPucks(matchDifferent);
		}
		else 
			if (team1.prepare() < team2.prepare()){
					team2.sumPoints(3);
					pucks1 = matchMinPucks;
					pucks2 = matchMinPucks + matchDifferent;
					team2.sumPucks(pucks2);
					team2.sumDifferentPucks(matchDifferent);
					team1.sumPucks(pucks1);
					team1.deductDifferentPucks(matchDifferent);
			}
			else{
				boolids(team1,team2);
				pucks1 += matchMinPucks;
				pucks2 += matchMinPucks;
				team1.sumPucks(pucks1);
				team2.sumPucks(pucks2);
			}
		System.out.println("Group:" + team1.getGroupNumber()+ "   " + "Mutch #" + getMutchNumber()+ " " + team1.getTeamName() + " " + pucks1 + "-" + pucks2 + " " + team2.getTeamName());
	}
	private  void boolids(Team team1, Team team2){
		if (team1.getRatingIIHF() > team2.getRatingIIHF()){
			pucks1++;
			team1.sumPucks(1);
			team1.sumPoints(2);
			team2.sumPoints(1);
			team1.sumDifferentPucks(1);
			team2.deductDifferentPucks(1);
		}
		else{
			pucks2++;
			team2.sumPucks(1);
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
		return (int)(Math.random()*4)+1;
	}

}
