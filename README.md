# SmartSQL

SmartSQL is a useful and reliable tool designed for web developers using the popular MySQL server. It enables you to browse and edit data.

## Install SmartSQL on Ubuntu

```
$ sudo add-apt-repository ppa:stephane-martarello/smartsql
$ sudo apt-get update
$ sudo apt-get install smartsql
```

## Compiling SmartSQL on Unix
### Prerequisites

* [Qt 5.6](http://www.qt.io/download/)
  * Install Qt sources
* g++ 4.8 or later
* Debian/Ubuntu (apt-get)
  * `sudo apt-get install build-essential libgl1-mesa-dev libmysqlclient-dev libssl-dev libfontconfig1-dev libfreetype6-dev`

### Build the MySQL plugin
```
$ cd $QTSOURCEDIR/qtbase/src/plugins/sqldrivers/mysql
$ qmake "INCLUDEPATH+=/usr/local/include" "LIBS+=-L/usr/local/lib -lmysqlclient_r" mysql.pro
$ make
$ make install
```

### Build SmartSQL

You can build SmartSQL with
```
$ cd $SOURCE_DIRECTORY
$ qmake
$ make
```

## License

SmartSQL is under the [GNU General Public License, version 3.](https://opensource.org/licenses/GPL-3.0)
