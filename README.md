<h1 align="center">
	FOV Overhaul - an ASI mod for Dying Light 2<br>
</h1>

## What is an ASI?
An ASI is basically just a DLL file but renamed. A DLL is just a library that programs or games use, by loading said libraries into memory and then using whatever exported functions and such from that said library. Or in this case, simply loading the library (the mod) into the game's process (basically memory) and modifying memory. 

## Features
#### - Increase Extra FOV beyond the game's options limit by using the mod's keybindings
#### - Stop reducing FOV when entering a safezone
#### - No need to replace game files or to add paks to the game, because the mod gets injected during game startup using an ASI Loader

## Requirements
### [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/x64-latest/version.zip) - extract the DLL file from the archive and paste it inside the game's exe directory (`Dying Light 2 Stay Human\ph\work\bin\x64`)

## How do I use it?
By default, to increase or decrease your FOV, you can press CTRL and + or CTRL and - respectively.

A config file `FOVOverhaul.ini` is stored in the same folder as the mod file. For now, it contains keybind options and a value for safezone FOV reduction (set to 0 by default, -10 is the game's original value).
Changes made to the config file are constantly refreshed in the mod.
If you want to regenerate the config file, delete it and it will automatically create a new file.

## I'm a newbie, how do I build from source?
1. Clone the repository
2. Open the solution
3. Make sure the `Release` configuration is selected
4. Click Build -> Build Solution
5. The final `FOVOverhaul.asi` file will be located in `DL2FOVOverhaul\x64\Release`, paste that in the game's exe directory (`Dying Light 2 Stay Human\ph\work\bin\x64`) alongside the provided ASI Loader (renamed to `version.dll` from this README.

## Credits
### - [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)

## License
This repository is licensed under the [MIT License](LICENSE).
