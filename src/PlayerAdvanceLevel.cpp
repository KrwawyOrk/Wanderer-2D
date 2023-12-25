#include "PlayerAdvanceLevel.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

PlayerAdvanceLevel::PlayerAdvanceLevel()
{
    m_experienceTable = ReadExperienceTable( "pliki/experience_points_table.json" );
}

int PlayerAdvanceLevel::CheckForLevelAdvance( Player* player )
{
    //std::vector<int> levelPoints = { 100, 250, 500, 1000, 2000 }; // Example points required for each level

    int level = 1;
    for (int points : m_experienceTable) {
        if (player->GetExperiencePoints() >= points) {
            level++;
        }
        else {
            break;
        }
    }

    return level;
}

void PlayerAdvanceLevel::PrintLevelAdvanceInformations( Player* player )
{
    std::cout << "Current level: " << CheckForLevelAdvance( player  ) << std::endl;
}

std::vector<int> PlayerAdvanceLevel::ReadExperienceTable( const std::string& file_path )
{
    std::vector<int> experience_requirements;

    std::ifstream file( file_path );
    if (file.is_open())
    {
        json data;
        file >> data;

        if (data.contains( "experience" ) && data["experience"].is_array())
        {
            for (const auto& exp : data["experience"])
            {
                if (exp.is_number_integer())
                {
                    experience_requirements.push_back( exp );
                }
            }
        }
        else
        {
            std::cerr << "Invalid JSON format: 'experience' array is missing or not an array" << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to open the file: " << file_path << std::endl;
    }

    return experience_requirements;
}