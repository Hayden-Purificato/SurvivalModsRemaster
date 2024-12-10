#include "pch.h"
#include "Initialization.hpp"

void INIT::LoadTriggerPeds()
{
    nlohmann::json js;
    std::ifstream i("SurvivalsData\\TriggerPeds.json");
    i >> js;
    i.close();

    std::vector<std::string> names = js["SurvivalNames"];
    std::vector<int> playerRelshps = js["RelationWithPlayer"];
    std::vector<std::string> models = js["Models"];
    std::vector<float> positionX = js["Positions"]["x"];
    std::vector<float> positionY = js["Positions"]["y"];
    std::vector<float> positionZ = js["Positions"]["z"];
    std::vector<float> headings = js["Headings"];
    std::vector<std::string> tasks = js["Tasks"];
    TriggerPedsData::ClearTriggerPeds();

    for (size_t n = 0; n < names.size(); n++)
    {
        EntityPosition pos = EntityPosition();
        pos.coords.x = positionX.at(n);
        pos.coords.y = positionY.at(n);
        pos.coords.z = positionZ.at(n);
        pos.heading = headings.at(n);
        std::string name = names.at(n);
        std::vector<std::string> allies = js["Allies"][name];

        for (const std::string& ally : allies)
        {
            SurvivalAllies sa = SurvivalAllies();
            sa.MissionID = name;
            sa.RelGroupName = ally;
            TriggerPedsData::allies.push_back(sa);
        }

        TriggerPedsData::names.push_back(name);
        TriggerPedsData::models.push_back(models.at(n));
        TriggerPedsData::blips.push_back(BLIPS::CreateForMissionTriggerPed(pos.coords, name.c_str()));
        TriggerPedsData::positions.push_back(pos);
        TriggerPedsData::peds.push_back(0);
        TriggerPedsData::starTime.push_back(0);
        TriggerPedsData::tasks.push_back(tasks.at(n));
        TriggerPedsData::timerActive.push_back(false);
        TriggerPedsData::killedFlags.push_back(false);
        TriggerPedsData::playerRel.push_back(playerRelshps.at(n));
    }
}

void INIT::UnloadModel(Hash model)
{
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
}

void INIT::LoadModel(Hash model)
{
    STREAMING::REQUEST_MODEL(model);

    while (!STREAMING::HAS_MODEL_LOADED(model))
    {
        WAIT(50);
    }
}

Hash INIT::LoadModel(const char* modelName)
{
    Hash modelHash = MISC::GET_HASH_KEY(modelName);
    STREAMING::REQUEST_MODEL(modelHash);

    while (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        WAIT(50);
    }

    return modelHash;
}

void INIT::SpawnTriggerMarker(size_t index) {
    EntityPosition pos = TriggerPedsData::positions.at(index);
    float zCoord;
    MISC::GET_GROUND_Z_FOR_3D_COORD(pos.coords.x, pos.coords.y, pos.coords.z, &zCoord, 0, 0);
    GRAPHICS::DRAW_MARKER(1,pos.coords.x, pos.coords.y, zCoord, 0,0,0,0,0,0,4,4,1,240,0,0,192,false,false,0,false,0,0,false);
}
