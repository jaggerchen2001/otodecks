/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(SpeedUPButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    //making label text visible
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(volSliderLabel);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    muteButton.addListener(this);
    loopButton.addListener(this);
    SpeedUPButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    // Truncate decimal places from values as they can easily be displayed
    speedSlider.setNumDecimalPlacesToDisplay(1);
    volSlider.setNumDecimalPlacesToDisplay(1);

    //changing volume and speed sliders into rotary knobs
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);

    //adding text to vol and speed slider
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);

    //attaching text to the volume slider component
    volSliderLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volSliderLabel.attachToComponent(&volSlider, false);

    //attaching text to the volume speed slider component
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);

    startTimer(100);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::mediumaquamarine);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


}

void DeckGUI::resized()
{
    double rowH = getHeight() / 10; 
    double columnW = getWidth() / 9;

    playButton.setBounds(0, rowH*9, columnW * 3-5, rowH);
    stopButton.setBounds(columnW*3, rowH*9, columnW * 3-5, rowH);  
    loadButton.setBounds(columnW*6, rowH*9, columnW*3-5, rowH);
    muteButton.setBounds(0, rowH*8-5, columnW * 3-5, rowH);
    loopButton.setBounds(columnW*3, rowH * 8 - 5, columnW * 3 - 5, rowH);
    SpeedUPButton.setBounds(columnW * 6, rowH * 8 - 5, columnW * 3 - 5, rowH);

    volSlider.setBounds(0, rowH*4.5, getWidth()/3*2, rowH * 3);
    speedSlider.setBounds(getWidth()/2.8, rowH*2.8, getWidth() / 3*2, rowH * 3);
    
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    
    volSliderLabel.setCentreRelative(0.58f, 0.58f);
    speedSliderLabel.setCentreRelative(0.95f, 0.4f);

    posSlider.setBounds(0, rowH * 1.8, getWidth(), rowH);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, columnW * 4, rowH * 0.75);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &SpeedUPButton)
    {
        // Toggle the spped up state
        isSpeedUp = !isSpeedUp;
        if (isSpeedUp)
        {
            player->setSpeed(2);
            SpeedUPButton.setColour(TextButton::buttonColourId, Colour(255, 110, 0));
        }
        else
        {
            player->setSpeed(1);
            // Set to the original or another color when not speed up
            SpeedUPButton.setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        }

    }
    if (button == &loopButton)
    {
        // Toggle the loop state
        isLooped = !isLooped;

        if (isLooped)
        {
            loopButton.setColour(TextButton::buttonColourId, Colour(255, 110, 0));
        }
        else
        {
            // Set to the original or another color when not looped
            loopButton.setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        }
    }

    if (button == &muteButton)
    {
        // Toggle the mute state
        isMuted = !isMuted;

        // If muted, set gain to 0, else set to a predefined volume level
        // Change the button's color based on the mute state
        if (isMuted)
        {
            player->setGain(0);
            muteButton.setColour(TextButton::buttonColourId, Colour(255, 110, 0));
        }
        else
        {
            player->setGain(1.0);
            // Set to the original or another color when not muted
            muteButton.setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
        }
    }
    if (button == &playButton)
    {
        player->start();
    }
     if (button == &stopButton)
    {
        player->stop();

    }
       if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
    }

}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    // Check if the end of the audio file has been reached.
    if (player->getPositionRelative() >= 1) {
        // Reset the position to the start of the audio file.
        player->setPositionRelative(0);

        // If loop is enabled, start playback from the beginning.
        // Otherwise, stop the playback.
        if (isLooped) {
            player->start();
        }
        else {
            player->stop();
        }
    }
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


    

