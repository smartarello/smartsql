# SmartSQL

SmartSQL is a useful and reliable tool designed for web developers using the popular MySQL server. It enables you to browse and edit data.

## Compiling SmartSQL on Unix
### Prerequisites

* [Qt 5.6](http://www.qt.io/download/)
* g++ 4.8 or later
* Debian/Ubuntu (apt-get)
  * `sudo apt-get install build-essential libgl1-mesa-dev`
* Fedora/RHEL/CentOS (yum)
  * `sudo yum groupinstall "C Development Tools and Libraries"`
  * `sudo yum install mesa-libGL-devel`
* openSUSE (zypper)
  * `sudo zypper install -t pattern devel_basis`

You can build SmartSQL with
```
$ cd $SOURCE_DIRECTORY
$ qmake
$ make
```

## License

SmartSQL is under the [GNU General Public License, version 3.](https://opensource.org/licenses/GPL-3.0)
