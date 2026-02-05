{\rtf1\ansi\ansicpg1252\cocoartf2867
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #pragma once\
#include <juce_audio_processors/juce_audio_processors.h>\
\
// This structure holds what the Listener "hears" \
// so the Performer can use it for AI generation.\
struct CatalystData \{\
    float rmsLevel = 0.0f;\
    float peakLevel = 0.0f;\
    int detectedKey = 0; // Simplified for now\
\};\
\
class CatalystBridge : public juce::DeletedAtShutdown \{\
public:\
    CatalystData sharedData;\
    juce::CriticalSection lock;\
\
    // A static method to get a single instance across the app\
    static CatalystBridge& getInstance() \{\
        static CatalystBridge instance;\
        return instance;\
    \}\
\};}