# cvar-unhide-s2-citadel

A Source 2 plugin to reveal all console variables and commands that are marked as hidden or development-only in Deadlock based on [cvar-unhide-s2](https://github.com/saul/cvar-unhide-s2).

> [!IMPORTANT]
> You must add `-insecure` to Deadlock's launch options for this plugin to load.

## Installation

1. **Download the latest release of the plugin**: \
   https://github.com/Artemon121/cvar-unhide-s2-citadel/releases/latest

1. **Extract the contents of the ZIP to the game's mod folder.**

   - 📂 `$STEAM\steamapps\common\Deadlock\game\citadel`

   After extraction there should be an `cvar_unlocker` folder in the game folder, e.g. `Deadlock\game\citadel\cvar_unlocker\...`

1. **Update the `game\citadel\gameinfo.gi` file**: \
   Around line 70, add the `Game citadel/addons` search path. This tells the engine to load the plugin before loading Deadlock.

   ```diff
   FileSystem
   {
   	SearchPaths
   	{
      // These are optional language paths. They must be mounted first, which is why there are first in the list.
			// *LANGUAGE* will be replaced with the actual language name. If not running a specific language, these paths will not be mounted
			Game_Language		citadel_*LANGUAGE*
 
			// These are optional low-violence paths. They will only get mounted if you're in a low-violence mode.
			Game_LowViolence	citadel_lv

   +		Game	citadel/cvar_unlocker
   		Game	citadel
   		Game	core
   ```

1. **Start the game from Steam.**
   > [!WARNING]
   > Deadlock must be launched with `-insecure` in the launch options. If you don't know how to do this, take a look this [Steam Community guide](https://steamcommunity.com/sharedfiles/filedetails/?id=379782151).

If you want to disable the plugin:

- Remove the `Game	citadel/cvar_unlocker` line from the gameinfo.gi file.
- Remove `-insecure` from the game's launch options.

## Available commands

If you installed the plugin correctly, you should now be able to use the following commands in the console:

- **cvar_unhide**: Reveal all hidden/development-only convars/concommands.
- **cvarlist_md**: Write all concmds/cvars to a `cvarlist.md` file in the `citadel` game directory.
