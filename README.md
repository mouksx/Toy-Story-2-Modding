# Toy-Story-2-Modding
Tools And Decompiled Assets For/From The 1999 Video Game Toy Story 2

## File Types:

### NGN:
#### The .ngn file type contains:
Character Models, Character Animations, Level Geometry, Level Textures, Area Portals and Shape Links.  
This is the first of the 3 data files needed to load a level.  
#### Modding the .ngn file:
An editor can be found in "Toy Story 2ools/NGN", which has been developed by "EpicMinecartz" for the  
past few years. A link to his website can be found [here](http://emc.x10.mx/), And his github [here](https://github.com/EpicMinecartz).
  
  
### RAW:
#### The .raw file type contains:
Entity Data/Placement (eg. Coins and Hint Blocks), Textures, Collision, Character Models and Character Animations.  
The .raw file contains a compressed version of the level's .all file (Collision + Water/Concrete Data).  
The .raw file also only gets used for the Entity Data/Placement, meanwhile everything else is left unused.  
This is the second of the 3 data files needed to load a level.  
#### Modding the .raw file:
A decompiler can be found in "Toy Story 2ools/RAW", named "RAWDEC.exe" which was developed by "Juanmv94" on the 15th of the 5th 2020.  
This will part the .raw file into its different types, uncompressed. the first 15 or so are texture files and  
can be opened using EpicMinecartz Program "TPage.exe" which was made on the same day.
Juanmv94 also made a Collision and GFX viewer using PSX save states, which contain the .raw data.  
You can find the collision viewer [here](https://priceless-pike-6c8ff8.netlify.app/), or the GFX viewer, [here](https://priceless-pike-6c8ff8.netlify.app/indexgfx.html)
  
  
### .DAT:
#### The .dat file contains:
This is the third and final file that is necessary to load a level.
The .dat file is somewhat a small mystery for now, at the moment what I think it contains is:
Some Entity Data, Poles and Pushable Entity Data and Specific Entity Pointers.  
Currently we cannot mod it.
