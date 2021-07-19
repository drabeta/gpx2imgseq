# gpx2imgseq

**Under developement**
That helps to write MD-Files [Online MD-Editor](https://pandao.github.io/editor.md/en.html).

## Thoughts
*Just thoughts i made to make decisions, start working, become ideas... For me it's helpful to re-read my thoughts, order them and invent better/new ways to implement*
Let's try... 

## How to use...
```sh
g2i \
    --speed=b0,a1,g0 \    # layerorder: backg,arrow,gauge; arrow rotates
	--course=b0,g0,a1,fancy # layerorder: backg,gauge,arror; arrow rotates; theme: fancy
g2i --theme=fancy --speed=b,g
```
....thinking... make an image max 3 layer. Controled by cmd-option like above. Pass b,a,g as needed. Image will be generaded in specified order i.e. `--speed=b1` just use rotated Backgound-Image or `--course=a0,g1` first draw arrow on new image and lay rotated gauge over it (arrow points always up/north, but gauge with scale 0-360 rotates).

Currently only square images makes sense for rotating (rotates aroung the center point). In my speed image only the upper half is relevant, but i use this sequence as video overlay so i can place it on the bottom line, lower half outside of clip-area. A way to `crop` would be nice so it can be placed anywhere.
Thinkering... If we now add more options to an parameter it becomes a bit messy: Options for options in options... i.e `--speed=b0,a1,g0,c256x128+128+0`. **NO!** Handle this with additional parameters like `--speed_crop=256x128+128+0`

So we want
```sh
g2i \
    --speed=b0,a1 \
	--speed_crop=256x128+128+0
```

**damn it**
`--all`would also be nice... but in which order... all as `b0,g0,a1` - the default, any additional parameter to overwrite for specific image.
And with this any body expects that with no paramter all images with default order will be generated.

**STOP  right HERE !**
What ever the future will bring, i can't prepare right now for that.
Implement what we have.

What did you think about it ?
