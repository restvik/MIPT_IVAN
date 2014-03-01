
public class GamePlayOff extends Game{
	
	public   int gamePlayOff (Team team1 , Team team2){
		changeMutchNumber();
		if (team1.prepare() > team2.prepare()){
			result(team1.getTeamName() , team1.getTeamName() , team2.getTeamName());
			return 1;
		}
		else 
			if (team1.prepare() < team2.prepare())
			{
				result(team2.getTeamName() , team1.getTeamName() , team2.getTeamName());
				return 0;
			}
			else
				if (team1.getRatingIIHF() > team2.getRatingIIHF())
				{
					result(team1.getTeamName(), team1.getTeamName() , team2.getTeamName());
					return 1;
				}
				else
				{
					result(team2.getTeamName(), team1.getTeamName() , team2.getTeamName());
					return 0;
				}
	}
	
	private void result(String name, String name1, String name2 ){
		System.out.println("Play Off mutch #" + getMutchNumber() + "     " + name1 + " VS " + name2 +  "      Winner: " + name);
	}	
}
