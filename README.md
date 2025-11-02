# Embedded

## Getting started
Repository contains `pico-sdk` as a submodule. After cloning the
repository, run the following:

```
git submodule update --init
git submodule foreach "git submodule update --init"
```


Simple Hello World for Pico — initializes the board, prints a greeting, and toggles an LED.  
Basic arithmetic demo — performs and prints simple arithmetic operations.  
Arithmetic with history — calculator-style example that records and prints a history of operations.  
Input hysteresis/button demo — demonstrates input hysteresis (debouncing) and button handling.  

## Usage
```
mkdir build
cd build
cmake ..
make
```
