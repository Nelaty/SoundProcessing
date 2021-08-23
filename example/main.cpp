#include <sp/config/configuration.hpp>
#include <sp/import/soundFileImporter.hpp>
#include <sp/player/player.hpp>

#include <sp/filter/soundPipeline.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);

    std::cout << "Working directory: " << sp::Configuration::workingDir() << "\n";

    // Read data
    const auto soundFileDir = sp::Configuration::getSoundDir();
    const auto wavFilePaths = sp::Configuration::listFiles(soundFileDir, "wav", true);
    const auto oggFilePaths = sp::Configuration::listFiles(soundFileDir, "ogg", true);

    if(wavFilePaths.empty() || oggFilePaths.empty()) {
        spdlog::error("no wav and ogg files present. aborting");
        return -1;
    }


    //auto pipeline = sp::filter::SoundPipeline{};
    //pipeline.add(std::make_shared<sp::filter::AmplitudeFilter>(1.0f));

    auto wav = sp::SoundFileImporter::import(wavFilePaths[rand() % wavFilePaths.size()]);
    auto ogg = sp::SoundFileImporter::import(oggFilePaths[0]);

    //pipeline.apply(wav);

    sp::Player player(soundFileDir);
    player.initPlayAndCleanup(wav);

    //player.initPlayAndCleanup(wavFilePaths[0]);
    //player.initPlayAndCleanup(wavFilePaths[1]);
    //player.initPlayAndCleanup(wavFilePaths[2]);

    return -1;
}