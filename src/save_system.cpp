#include "save_system.hpp"
#include <filesystem>
#include <fstream>

std::optional<SaveSystem::Save> SaveSystem::load(const char *path)
{
    Save save {};
    std::ifstream input(path);
    if (input.is_open()) {
        size_t weapons_deque_size = 0;
        size_t engines_deque_size = 0;
        std::deque<bool> weapons;
        std::deque<bool> engines;

        input.read(reinterpret_cast<char*>(&save.score), sizeof(unsigned int)); // read score
        input.read(reinterpret_cast<char*>(&save.equiped_weapon), sizeof(WeaponEntity::PlayerType)); // read current engine 
        input.read(reinterpret_cast<char*>(&save.equiped_engine), sizeof(EngineEntity::PlayerType)); // read current weapon 
        input.read(reinterpret_cast<char*>(&weapons_deque_size), sizeof(size_t)); // read weapon deque size
        input.read(reinterpret_cast<char*>(&engines_deque_size), sizeof(size_t)); // read engine deque size
        
        weapons.resize(weapons_deque_size);
        engines.resize(engines_deque_size);
        
        input.read(reinterpret_cast<char*>(&weapons[0]), weapons_deque_size * sizeof(bool));    
        input.read(reinterpret_cast<char*>(&engines[0]), engines_deque_size * sizeof(bool));

        save.weapons = weapons;
        save.engines = engines;
        return save;
    } else {
        return {};
    }
}

std::optional<SaveSystem::Settings> SaveSystem::loadSettings(const char *path)
{
    Settings settings {};
    std::ifstream input(path);
    if (input.is_open()) {
        input.read(reinterpret_cast<char*>(&settings.brightness), sizeof(float)); // read saved brightness value 
        input.read(reinterpret_cast<char*>(&settings.sound), sizeof(float)); // read sound value 
        input.read(reinterpret_cast<char*>(&settings.is_fullscreen), sizeof(bool)); // read fullscreen bool 
        return settings;
    } else {
        return {};
    }
}

void SaveSystem::save(Save &data, const char *path)
{
    size_t weapons_deque_size = data.weapons.size();
    size_t engines_deque_size = data.engines.size();

    std::filesystem::remove(path);
    std::ofstream output(path, std::ios_base::binary);
    
    // Save player data
    output.write(reinterpret_cast<char*>(&data.score), sizeof(unsigned int)); // Write score 
    output.write(reinterpret_cast<char*>(&data.equiped_weapon), sizeof(WeaponEntity::PlayerType)); // Write player weapon 
    output.write(reinterpret_cast<char*>(&data.equiped_engine), sizeof(EngineEntity::PlayerType)); // Write player engine
                                                                                       //
    output.write(reinterpret_cast<char*>(&weapons_deque_size), sizeof(size_t)); 
    output.write(reinterpret_cast<char*>(&engines_deque_size), sizeof(size_t)); 

    output.write(reinterpret_cast<char*>(&data.weapons[0]), data.weapons.size() * sizeof(bool)); // Write unlockable weapons
    output.write(reinterpret_cast<char*>(&data.engines[0]), data.engines.size() * sizeof(bool)); // Write unlockable engines
}

void SaveSystem::save(Settings &data, const char *path)
{
    std::filesystem::remove(path);
    std::ofstream output(path, std::ios_base::binary);

    output.write(reinterpret_cast<char*>(&data.brightness), sizeof(float)); // Write score 
    output.write(reinterpret_cast<char*>(&data.sound), sizeof(float)); // Write player weapon 
    output.write(reinterpret_cast<char*>(&data.is_fullscreen), sizeof(bool)); // Write player engine
}
