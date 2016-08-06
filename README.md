# SmartSQL

SmartSQL is a useful and reliable tool designed for web developers using the popular MySQL server. It enables you to browse and edit data.

## Install SmartSQL on Ubuntu

```
$ sudo add-apt-repository ppa:stephane-martarello/smartsql
$ sudo apt-get update
$ sudo apt-get install smartsql
```

## Compiling SmartSQL on Ubuntu 16.04 (Xenial)
### Prerequisites

* [Qt 5.5](http://www.qt.io/)
  * `sudo apt-get install qt5-default qt5-qmake libqt5sql5-mysql`
  * `sudo apt-get install qtcreator` (recommandations)
* g++ 4.8 or later
  * `sudo apt-get install g++`
* Debian/Ubuntu (apt-get)
  * `sudo apt-get install build-essential libgl1-mesa-dev libmysqlclient-dev libssl-dev libfontconfig1-dev libfreetype6-dev libxrender-dev`

### Build SmartSQL

You can build SmartSQL with
```
$ cd $SOURCE_DIRECTORY
$ qmake
$ make
```

## License

SmartSQL is under the [GNU General Public License, version 3.](https://opensource.org/licenses/GPL-3.0)
