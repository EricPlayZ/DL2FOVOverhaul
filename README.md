<h1 align="center">
	<img src="https://github.com/EricPlayZ/DL2FOVOverhaul/blob/master/Extra Files/Images/DL2 FOV Changer - Cropped.png" width=40%><br>
	FOV Overhaul - an ASI mod for Dying Light 2<br>
</h1>

## What is an ASI?
An ASI is basically just a DLL file but renamed. A DLL is just a library that programs or games use, by loading said libraries into memory and then using whatever exported functions and such from that said library. Or in this case, simply loading the library (the mod) into the game's process (basically memory) and modifying memory. 

## Features
#### - Increase Extra FOV beyond the game's options limit by using the mod's keybindings
#### - Stop the FOV reduction when entering a safezone
#### - No need to replace game files or to add paks to the game, because the mod gets injected during game startup using an ASI Loader
#### - More to come (hopefully)!

## Requirements
### [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/x64-latest/winmm-x64.zip)

## Installation
Download the archive from the [Releases](https://github.com/EricPlayZ/DL2FOVOverhaul/releases) section and extract the files from inside the archive to the game's exe folder (`Dying Light 2 Stay Human\ph\work\bin\x64`).

## Uninstallation
Delete `winmm.dll`, `FOVOverhaul.asi` and `FOVOverhaul.ini` from the game's exe folder (`Dying Light 2 Stay Human\ph\work\bin\x64`).

## How do I use it?
When you launch the game, a console window will appear. When it does, DON'T CLOSE IT! Closing it would also close the game, as the console window is part of the game theoretically (the DLL gets injected into the game and the DLL allocates a console window, meaning the game process technically created that window, and killing the console window would kill the game process too). Information about the mod's state will be displayed here, including your Extra FOV (and actual FOV) values.

By default, to increase or decrease your FOV, you can press CTRL and + or CTRL and - respectively.

A config file `FOVOverhaul.ini` is stored in the same folder as the mod file. For now, it contains keybind options and a value for safezone FOV reduction (set to 0 by default, 10 is the game's original value).

To change your keybinds, visit [this link from Microsoft](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes) which contains all virtual key codes, and write the name of the keycode you want to use, an example would be `VK_SUBTRACT` for the `FOVDecrease`, which is the minus key on your keyboard for decreasing your FOV. The modifier key is the second key you press, such as CTRL, ALT or SHIFT.

Changes made to the config file are constantly refreshed in the mod, so you can edit the config file while the game is running.
If you want to regenerate the config file, delete it and it will automatically create a new file.

## Does this work with Multiplayer?
This hasn't been tested, so try it at your own risk. It does not directly modify game files, so the game does not detect the game being modified, but there might be an anti-cheat that I don't know about, so again, if anyone is brave enough to try, go ahead and tell me the results, I will update this section afterwards.

## This isn't working anymore, what do I do?
~~Pointers unfortunately usually change every game update. To prevent this, there is something called pattern scanning, which searches for a pattern of bytes which should hopefully not change every game update. I'll update the mod in the future with this, to prevent having to update the mod every time the game gets updated as well.~~

~~If the mod stops working, wait for me to update the mod, it should not take more than a few days if I have access to my computer.~~

Since version 1.0.1, the mod uses pattern scanning to find the necessary pointer for classes. It uses instructions from a function related to Steam lobbies, so this pattern should last a long time without needing an update. This means this mod won't need frequent updating, unless class offsets change, but they should rarely change too.

If there is another reason this mod isn't working, please open an issue and provide as much detail as possible to make it easier for me to debug the issues.

## I'm a newbie, how do I build from source?
1. Clone the repository.
2. Open the solution.
3. Make sure the `Release` configuration is selected.
4. Click Build -> Build Solution.
5. The final `FOVOverhaul.asi` file, alongside the default `FOVOverhaul.ini` config, will be located in `x64\Release`, paste that in the game's exe directory (`Dying Light 2 Stay Human\ph\work\bin\x64`) alongside the provided ASI Loader (link is up above in the Requirements section or down below in the Credits section; the DLL **needs to be renamed** to `winmm.dll` in order for the game to properly detect and use it).

## Credits
### [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)

## License
This repository is licensed under the [MIT License](LICENSE).
