### Build

#### Server

```sh
$ cd ./server/

// First time only
$ cmake -S . -B build

// Debug build
$ cmake --build build
or
$ cmake --build build --config Debug

// Release build
$ cmake --build build --config Release
```

#### Client

```sh
$ cd ./client/

$ npm run build
```

### Run

```sh
$ cd ./server/
$ ./build/main
```
