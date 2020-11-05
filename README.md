# Game of Life

[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), also known simply as Life, is a cellular automaton made by the British mathematician John Horton Conway in 1970.

## Installation

### Dependencies
 - [SDL2](https://libsdl.org/download-2.0.php)

Also you need a c++11 compatible compiler.

### Setup

```bash
git clone https://github.com/bitt4/gol
cd gol
make
./gol
```

## Usage

### Options

| Option                   | Effect                               |
|--------------------------|--------------------------------------|
|`-w`, `--width=NUM`       | Set width of the grid in cells       |
|`-h`, `--height=NUM`      | Set height of the grid in cells      |
|`-c`, `--cell-size=NUM`   | Size of cell in pixels               |
|`-s`, `--seed=[NUM]`      | Set seed for random number generator |
|`-v`, `--speed=NUM`       | Grid update frequency                |
|`-H`, `--help`            | Display help                         |

### Key bindings

| Key                            |  Description   |
|--------------------------------|----------------|
| <kbd>SPACE</kbd>               | Toggle pause   |
| <kbd>+</kbd>,<kbd>=</kbd>      | Increase speed |
| <kbd>-</kbd>                   | Decrease speed |

## Example
Example of Pulsar oscillator pattern:
![](demo.gif)