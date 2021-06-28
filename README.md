<p align="center">
  <img src=".github/logo.png", style="width:75%">
</p>
<p align="center">
  <img src=https://img.shields.io/github/issues-closed-raw/QFSW/JiTBoy.svg?color=51c414>
  <img src=https://img.shields.io/github/issues-raw/QFSW/JiTBoy.svg?color=c41414&style=popout>
</p>



JiTBoy is a project and paper that explores JIT Binary Translation as an Accelerated Means of Emulation. It contains an interpreter, a JIT emulator, and a hybrid emulator for emulating MIPS-1 binary on x86 CPUs running Windows.

### Paper

The paper written about JiTBoy can be found <a href=".github/paper.pdf" target="_blank">here</a>

### Build Instructions

The JiTBoy project can be built by building the solution `JiTBoy.sln` either through Visual Studio 2019 or with MSBuild. JiTBoy has two different build configurations that can be used:

| Configuration | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| `Debug`       | `Debug` builds enable debugging in addition to producing more detailed logs of the emulator execution, but the performance is significantly worse. Use `Debug` builds for running single tests or investigating test failures and errors. |
| `Release`     | `Release` builds give the highest performance but take longer to build; furthermore, no logs are produced from release builds. Use `Release` for running mass tests or collecting performance data. |

### Usage
JiTBoy can be run by executing `JiTBoy.exe` in a command line interface (CLI) such as cmd. The executable will be in either `Release` or `Debug` depending on which configuration the project was built with. By default, JiTBoy will run all tests on all emulator configurations with no performance data collected. To collect performance data, provide one of the following `--timing` arguments

 

| Argument            | `batch_size` | `threshold` | `precision` |
| ------------------- | ------------ | ----------- | ----------- |
| `--timing=none`     | 1            | 0           | 1           |
| `--timing=fast`     | 1            | 2           | 0.1         |
| `--timing=accurate` | 10           | 10          | 0.01        |
| `--timing=final`    | 100          | 10          | 0.0001      |

JiTBoy can also be run on a single test with an emulator config of choice by using `--single`; it is recommended to build with the `Debug` configuration for improved logging. To use `--single`, invoke JiTBoy with the following format:

```
JiTBoy.exe --single test_path emulator_config
```

`test_path` must be a relative or absolute path to the test file desired. If a normal assembly file is provided, then no test validation will be performed. `emulator_config` must be one of the following emulators:

| Emulator      | Argument Options                                             |
| ------------- | ------------------------------------------------------------ |
| `JIT`         | `-L`    Enables direct linking                               |
| `Hybrid`      | `-L`    Enables direct linking<br />`-S`    Enables speculative compilation<br />`-Tx`  Sets the compilation threshold to `x` |
| `Interpreter` | None                                                         |

The emulator config is case insensitive. For example, the following would invoke the test `long_mul.s` under the JIT emulator with direct linking enabled: 

```
JiTBoy.exe --single tests/mips/func/long_mul.s JIT -L
```