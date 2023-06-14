//номер миссии
int getCurrentMission(int x) 
{
	int mission = 0;
	if ((x > 0) && (x < 600)) { mission = 0; }
	if (x > 400) { mission = 1; }
	if (x > 700) { mission = 2; }
	if (x > 2200) { mission = 3; }

	return mission;
}

std::string getTextMission(int currentMission)
{
	std::string missionText = "";

	switch (currentMission)
	{
	case 0: missionText = "1.mission нач этап и инструкции";
	case 1: missionText = "2.mission это твоя первая миссия, Энакин";
	case 2: missionText = "3.mission шобы дальше, ты там это";
	case 3: missionText = "4.mission а теперь еще что-то и так далее";
	}

	return missionText;
};