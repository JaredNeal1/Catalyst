cmake_minimum_required(VERSION 3.22)

project(Catalyst VERSION 0.1.0)

# 1. Include JUCE
add_subdirectory(JUCE)

# 2. Catalyst Listener (Audio Effect)
juce_add_plugin(CatalystListener
    COMPANY_NAME "YourName"
    PLUGIN_CODE "CatL"          # 4-character unique code
    FORMATS AU VST3
    PRODUCT_NAME "Catalyst Listener")

target_sources(CatalystListener PRIVATE 
    CatalystListener/Source/PluginProcessor.cpp 
    CatalystListener/Source/PluginEditor.cpp)

# 3. Catalyst Performer (MIDI FX)
juce_add_plugin(CatalystPerformer
    COMPANY_NAME "YourName"
    PLUGIN_CODE "CatP"          # 4-character unique code
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE         # Necessary for Logic's MIDI FX slot
    FORMATS AU VST3
    PRODUCT_NAME "Catalyst Performer")

target_sources(CatalystPerformer PRIVATE 
    CatalystPerformer/Source/PluginProcessor.cpp 
    CatalystPerformer/Source/PluginEditor.cpp)

# 4. Link everything
target_link_libraries(CatalystListener PRIVATE juce::juce_audio_utils)
target_link_libraries(CatalystPerformer PRIVATE juce::juce_audio_utils)