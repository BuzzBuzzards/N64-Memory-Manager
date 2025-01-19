# Nintendo 64 ControllerPak Memory Manager
## What is it
I can call this project a low effort to derust my C/C++ knowledge by bypassing an annoying problem with a built-in LibDragon ControllerPak management system. 

By default, LibDragon recognises an off-brand (chineese, aftermarket, AliExpress) memory cards a RumblePak. This problem inherited by ALT64, which I tried to use to recover my memory card.

As a result, I decided to write my own tool to manage and recover my cheap cards.

## Project basis
By the most part, this ControllerPak manager is an expantion of built-in example (cpaktest) from LibDragon developers.

## What it can do?
- Show the conents of your memory card
- Format your memory card
- Save your memory card contents to your flashcard SD
- Load memory card dump from SD card (doesn't work RN)

## How to use
This utility is a CLI-based application. I see on-screen instructions as a self-explainotory ones.

## How to build
```
make
```