# Window Calculator (`win_calc`)

A GTK4 desktop application written in C for calculating window sash dimensions, profile geometries, and generating automated cut lists.

![Window Calculator UI running on GNOME](docs/demo.png)

> **Alt-text**: The main window showing Cairo drawing canvas on the right and sash input parameters on the left sidebar.

## Why

Calculations for timber window frames are traditionally done manually or in clunky spreadsheets, leading to measurement errors during cutting. I built this tool as an MVP to learn pure C11 memory management, GTK4 UI paradigms, and Cairo 2D canvas rendering while solving real workshop dimensional math.

## Features

- **Dynamic CAD Preview**: Renders frame, sash, and opening directions (tilt & turn) on a Cairo drawing canvas.
- **Parametric Math Engine**: Derives exact sash and profile cut lengths from the external frame dimensions.
- **Cut List Generation**: Tabular output breaking down logs with overdimention of :
  - Frame head, sill, left & right jamb.
  - Sash top & bottom rail, left & right stile.
  - Glass panel.
- **Config Persistence**: Save and reload custom profile profiles (`.ini`) and project states (`.win`) via `GKeyFile`.

## Stack

- **Language**: C (C11)
- **GUI Toolkit**: GTK 4.x & Cairo 2D
- **Build System**: GNU Make / Meson
- **Environment**: Flatpak SDK (GNOME 49) or Native host
- **Platform**: Linux / GNOME Desktop or Mac OS

## Run it

You can build and run this project natively or inside a sandboxed Flatpak container.

### Option 1: Native Build (Local Dependencies)

Requires GTK4 installed on your host system (`libgtk-4-dev`).

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt update && sudo apt install -y build-essential libgtk-4-dev

# Compile and run
make
./win_calc
```

### Option 2: Flatpak Container Build (GNOME SDK 49)
If you don't have GTK4 installed locally and you dont have sudo privileges, the Makefile can automatically fetch org.gnome.Sdk//49 and compile using Meson inside Flatpak:

```bash
# Build inside GNOME SDK 49 container
make meson

# Run the app inside the container
make run

### Prerequisites (Ubuntu / Debian)
```
sudo apt update
sudo apt install -y build-essential libgtk-4-dev meson ninja-build
