

#include "PlayerAudio.h"
PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();

}
PlayerAudio::~PlayerAudio()
{
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
     if (isRepeating && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
 {
     transportSource.setPosition(0.0);
     transportSource.start();
 }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // 🔑 Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }
    }
    return true;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();

}
void PlayerAudio::start()
{
    transportSource.start();

}
void PlayerAudio::mute()
{
    if (muted == false)
    {
        previousVolume = transportSource.getGain();
        setGain(0.0f);
        muted = true;
    }
    else
    {
        setGain(previousVolume);
        muted = false;
    }
}
bool PlayerAudio::isMuted()const
{
    return muted;
}

