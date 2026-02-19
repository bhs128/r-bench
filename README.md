# Reflector Workbench (r-bench)

R-Bench models how well various shapes of reflectors concentrate light from different angles.

In particular this software is designed to analyze the performance of different solar trough designs for use in renewable energy production.

**[Try it in your browser](https://bhs128.github.io/r-bench/)** — no install required.

## Intended scope:

- Single reflector
- Single light source (directional or point-source)
- One optional 'receiver' or 'light sink' (the pipe or panel in a solar application- r-bench will provide performance characteristics of the receiver for a given angle or averaged over a range of angles)

## Current Features:

- handles angles between 5 and 175 degrees
- supports catenary, parabola and semicircle reflector shapes
- adjustable reflector attributes (function min/max, alpha or 'steepness')
- recursively bounces light rays until they no longer intersect reflector
- both circle and line shaped 'light sinks'

## Building

### Prerequisites (Debian/Ubuntu)

```bash
sudo apt install qt6-base-dev qmake6 libgl-dev g++ make
```

### Build

```bash
qmake6 r-bench.pro
make
```

### Run

```bash
./r-bench
```

**WSL2 note:** GUI works out of the box on WSL2 with WSLg (Windows 10 build 19044+ / Windows 11).

## Web Version

A standalone single-page web app is available in the `web/` directory — one HTML file, zero dependencies.
It uses Canvas 2D for GPU-accelerated rendering and runs entirely in the browser.

Hosted at **https://bhs128.github.io/r-bench/**

To run locally:
```bash
cd web && python3 -m http.server 8000
```

## Technology

Desktop: Qt6 C++ (originally Qt4, ported 2026).  
Web: Vanilla JavaScript + Canvas 2D.

## Downloads

Pre-built binaries for Windows, macOS, and Linux are available on the [Releases](https://github.com/bhs128/r-bench/releases) page.

The Windows release includes all required Qt DLLs — no installation needed, just unzip and run.
