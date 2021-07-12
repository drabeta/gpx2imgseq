# gpx2imgseq

**Under developement**

Reads the input-File from command line and creates image sequences for `speed` and `course`.
Each image is build of three "layer": Background, Gauge and Arrow but in different order.
The Arrow-Image is rotated depending of `speed` or `course` from the GPX-File.

Speed
- Background
- Arrow
- Gauge
<img src="/images/speed.png?raw=true" width="128">

Course
- Background
- Gauge
- Arrow
<img src="/images/course.png?raw=true" width="128">


## TODO

Lots of things.
- Other arrtibuts of GPX like `time`, `lat`, `lon` and `elevation`. Maybe an Map depending on the Trackpoints.
- Make it configurable: Commandline options or Configfile ?
- Collect dependencies

