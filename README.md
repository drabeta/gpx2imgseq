# gpx2imgseq

**Under developement**

Reads the input-File from command line and creates image sequences for `speed` and `course`.
Each image is build of three "layer": Background, Gauge and Arrow but in different order.
The Arrow-Image is rotated depending of `speed` or `course` from the GPX-File.

<table>
  <tr>
    <th colspan="2">Speed</th>
    <th colspan="2">Course</th>
    <th colspan="2">Example</th>
  </tr>
  <tr>
    <td>Background</td>
    <td rowspan="3"><img src="/misc/speed.png?raw=true" width="128"></td>
    <td>Background</td>
    <td rowspan="3"><img src="/misc/course.png?raw=true" width="128"></td>
    <td rowspan="3"><img src="/misc/example.gif?raw=true" width="128"></td>
  </tr>
  <tr>
    <td>Arrow</td>
    <td>Gauge</td>
  </tr>
  <tr>
    <td>Gauge</td>
    <td>Arrow</td>
  </tr>
</table>


## TODO

Unsorted
- Other arrtibuts of GPX like `time`, `lat`, `lon` and `elevation`. Maybe an Map depending on the Trackpoints.
- Make it configurable: Commandline options or Configfile ? `--speed`, `--course`, ect
- Collect dependencies (make,gcc,libmagick++-dev)
- Themes; Organize source images in folders and choose by cmd-line option `--theme`
- Crop and other image manipulation for output image `--speed_crop`
- `--all`; Generate all images we know about, use defaut layout
- `--layout`; Sets/overwrites the default layout for image generation: `b0,g0,a1`


