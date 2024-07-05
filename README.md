#DISCLAIMER!!!!
##a LOT of the info written here is old information, as i created this almost 5 years ago. i did not fully understand what each file contained with 100% accuracy, and i aim to rewrite
a lot of this in the near future. the only trustworthy snippet right now is the info about the NGN files.


# Toy Story 2 Modding
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
can be opened using EpicMinecartz Program "TPage.exe" which was made on the same day which can be found in  
the same location as RAWDEC.exe, Reminder to ALT+F4 when you're done rather than closing it normally.  
Juanmv94 also made a Collision and GFX viewer using PSX save states, which contain the .raw data.  
You can find the collision viewer [here](https://priceless-pike-6c8ff8.netlify.app/), or the GFX viewer, [here](https://priceless-pike-6c8ff8.netlify.app/indexgfx.html)
  
  
### DAT:
#### The .dat file contains:
The .dat file is somewhat a mystery for now, What I think it contains is:  
Some Entity Data, Poles and Pushable Entity Data and Specific Entity Pointers.  
This is the third and final file that is necessary to load a level.
#### Modding the .dat file:
Currently we cannot mod it.
