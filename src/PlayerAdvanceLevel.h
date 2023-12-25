#ifndef PLAYERADVANCELEVEL_H
#define PLAYERADVANCELEVEL_H

#include <vector>
#include <string>

class Player;

class PlayerAdvanceLevel
{
public:
	PlayerAdvanceLevel();

	int CheckForLevelAdvance( Player* player );
	void PrintLevelAdvanceInformations( Player* player );
	std::vector<int> ReadExperienceTable( const std::string& file_path );

private:
	std::vector<int> m_experienceTable;
};

#endif


