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

// For developing
$ npm run dev

// Release build
$ npm run build
```

### Run

```sh
$ cd ./server
$ ./build/main
```
