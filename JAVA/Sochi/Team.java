	
public class Team{
	private String teamName;
	private int groupNumber;
	private int tournamentRating=1;
	private int groupPlace=4;
	private int points=0;
	private int differentPucks=0;
	private int pucks=0;
	private int ratingIIHF;
	
	//Get
	public int  getGroupNumber () {
	      return groupNumber;
	}
	public String getTeamName () {
	      return teamName;
	}
	public int getTournamentRating () {
	      return tournamentRating;
	}
	public int getGroupPlace () {
	      return groupPlace;
	}
	public int getPoints () {
	      return points;
	}
	public int getDifferentPucks () {
	      return differentPucks;
	}
	public int getPucks () {
	      return pucks;
	}
	public int getRatingIIHF () {
	      return ratingIIHF;
	}
	//Set
	public void setGroupNumber (int number) {
	      groupNumber = number;
	}
	public void setTeamName (String interTeamName) {
	      teamName = interTeamName;
	}
	public void setTournamentRating (int interTournamentRating) {
	      tournamentRating = interTournamentRating;
	}
	public void setRatingIIHF (int interRatingIIHF) {
	      ratingIIHF = interRatingIIHF;
	}
	//Methods
	public double prepare(){			
		return Math.random()*ratingIIHF;
	}
	public void sumTournamentRating(int different){
		tournamentRating+=different;
	}
	public void sumPoints (int interPoints) {
	      points+=interPoints;
	}
	public void sumDifferentPucks (int interDifferentPucks) {
	      differentPucks+=interDifferentPucks;
	}
	public void deductDifferentPucks (int interDifferentPucks) {
	      differentPucks-=interDifferentPucks;
	}
	public void sumPucks (int interPucks) {
	      pucks+=interPucks;
	}
	public void chGroupPlace () {
	      groupPlace--;
	}
}

