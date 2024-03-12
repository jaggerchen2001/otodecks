/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

private:

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
    
    TextButton muteButton{"MUTE"};//I wrote this
    TextButton loopButton{"LOOP"};//I wrote this
    TextButton SpeedUPButton{ "x2" };//I wrote this
  
    juce::TextEditor trackName;
    juce::String mtrackName;

    Label volSliderLabel;
    Label speedSliderLabel;
    Label posSliderLabel;

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    //I wrote this
    //to toggle mute and button
    bool isMuted = false;
    bool isLooped = false;
    bool isSpeedUp = false;

    FileChooser fChooser{"Select a file..."};


    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
