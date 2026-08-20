# CXT-12E4 Vial firmware

Vial-QMK firmware for the CXT Studio CXT-12E4 macropad: twelve MX-style keys, four clickable rotary encoders, and twelve WS2812 RGB LEDs driven by an ATmega32U4.

The factory firmware identifies itself as CXT Labs `5754:c401` and uses a Chinese VIA-derived configurator. This port replaces it with genuine Vial-QMK firmware that works with [Vial Web](https://vial.rocks/) in a WebHID-capable browser.

## Status

The firmware has been built, flashed, read back, and validated on physical CXT-12E4 hardware.

- USB application identity: `5754:c401`
- Atmel DFU identity: `03eb:2ff4`
- MCU: ATmega32U4
- Bootloader: Atmel DFU
- Matrix: 4 rows × 4 columns
- Inputs: 12 keys, 4 encoder presses, and 4 rotary encoders
- Lighting: 12 WS2812 LEDs through VialRGB
- Dynamic layers: 6
- Macro slots: 6
- Approximate macro storage: 695 bytes shared across the six slots
- Firmware size: 18,416 of 28,672 application bytes (64%)
- Tested Vial-QMK base: `dd43959ae5c08d8a28d38a1acf7b04e86b14a344`

The tested firmware is attached to the GitHub release as `cxt_studio_12e4_vial.hex`.

SHA-256:

```text
1e46a4cad5f5a0c947f969c0cbaf8f8ed15dc54ed8cbfcae42c161fb4d0aa6c0
```

## Physical control layout

Vial displays each rotary direction separately because clockwise and counterclockwise actions are independently assignable. Each physical knob therefore appears as a three-control group: `[CCW] [press] [CW]`.

The groups follow the macropad's physical Y formation:

```text
+-----+-----+-----+-----+     [ brightness ]     [ RGB mode ]
| Esc | F11 |     |Stop |          [ hue ]
+-----+-----+-----+-----+
|     |     |Prev | Fwd |
+-----+-----+-----+-----+          [ volume ]
|     |Play |Play |Next |
+-----+-----+-----+-----+
```

Default encoder mappings on layer 0:

| Physical position | CCW | Press | CW |
| --- | --- | --- | --- |
| Top left | RGB brightness down | Unassigned | RGB brightness up |
| Top right | Previous RGB mode | Toggle RGB | Next RGB mode |
| Center | RGB hue down | Unassigned | RGB hue up |
| Bottom | Volume down | Mute | Volume up |

Layers 1–5 start transparent and can be configured in Vial.

## Vial security unlock

Sensitive Vial actions require a physical unlock chord. Hold the first and fourth encoder buttons simultaneously: the bottom volume knob and the top-right RGB-mode knob.

## Repository contents

The directory below is an overlay for a Vial-QMK checkout:

```text
keyboards/cxt_studio/12e4/keymaps/vial/
├── config.h   # UID, 6/6 EEPROM allocation, unlock chord, RGB trimming
├── keymap.c   # Default keymap and six-layer encoder map
├── rules.mk   # Vial/VialRGB features and size-saving build options
└── vial.json  # Embedded Vial definition and physical Y layout
```

`udev/70-cxt12e4-vial.rules` grants local desktop users access to the CXT raw-HID interfaces and its ATmega32U4 DFU bootloader on Linux.

## Build from source

Docker is the simplest way to reproduce the tested toolchain:

```bash
git clone https://github.com/vial-kb/vial-qmk.git
cd vial-qmk
git checkout dd43959ae5c08d8a28d38a1acf7b04e86b14a344
git submodule update --init --recursive
cp -R /path/to/cxt-12e4-vial/keyboards/cxt_studio/12e4/keymaps/vial \
  keyboards/cxt_studio/12e4/keymaps/
SKIP_FLASHING_SUPPORT=1 ./util/docker_build.sh cxt_studio/12e4:vial
```

The resulting file is `cxt_studio_12e4_vial.hex` in the Vial-QMK root.

## Linux permissions

Install the included rule once:

```bash
sudo install -o root -g root -m 0644 \
  udev/70-cxt12e4-vial.rules \
  /etc/udev/rules.d/70-cxt12e4-vial.rules
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=hidraw --action=change
```

Reconnect the macropad afterward if the browser was already open. The rule is deliberately restricted to CXT `5754:c401` and Atmel DFU `03eb:2ff4`.

## Flash

Warning: only flash hardware positively identified as a CXT-12E4. Keep a copy of known-good firmware and do not short unidentified PCB pads.

1. Connect the macropad.
2. Briefly press the physical reset button on the back or underside. Do not hold it.
3. Confirm that `lsusb` shows `03eb:2ff4 Atmel Corp. atmega32u4 DFU bootloader`.
4. From the prepared Vial-QMK checkout, run:

```bash
./util/docker_build.sh cxt_studio/12e4:vial:flash
```

A successful flash performs erase, programming, read-back, and validation. The board then re-enumerates as `5754:c401 CXT cxt_studio 12E4`.

If a container cannot access a newly created USB device, stop it, leave the board in DFU mode, and start the flash command again. If Linux reports permission errors, install the included udev rule before retrying.

## Open in Vial Web

Use Chrome, Chromium, or Edge:

1. Open [vial.rocks](https://vial.rocks/).
2. Select **Start Vial**.
3. Choose **CXT cxt_studio 12E4** in the WebHID picker.

After installing a new definition, close and reopen the existing Vial tab so it reloads the layout embedded in the firmware.

## Porting notes and progress

The work proceeded as follows:

1. The connected device was identified as USB `5754:c401`, matching QMK's `cxt_studio/12e4` hardware definition.
2. Upstream QMK supplied the ATmega32U4 matrix, encoder pins, WS2812 layout, and Atmel DFU bootloader definition. Vial-QMK contained the keyboard definition but no Vial keymap.
3. A Vial keymap was added with a generated keyboard UID, a two-button security unlock, VialRGB, dynamic encoder maps, and four initial layers.
4. The first full-feature build exceeded the safe 28,672-byte application region by 3,898 bytes. Fourteen canned RGB animations and unused optional modules were removed. Vial mapping, macros, media keys, layers, encoder maps, and direct RGB control were retained.
5. The compact build succeeded at approximately 18 KB and was flashed through the `03eb:2ff4` Atmel DFU bootloader. Erase, program, read-back, and validation all passed.
6. Linux `hidraw` access initially prevented Vial Web from opening the device. The included narrowly scoped udev rule fixed current and future WebHID access and also permits non-root DFU updates.
7. The first encoder layout incorrectly used matrix coordinates as encoder direction coordinates. Vial reported `KeyError: (0, 3, 2)`. The definition was corrected to expose direction `0` and `1` for each of encoder indices `0`–`3`, while keeping encoder presses as ordinary matrix inputs.
8. Vial represents each direction with a separate round widget. The controls were grouped as `[CCW] [press] [CW]` and arranged to match the physical Y formation.
9. Physical encoder positions were verified and reordered: brightness at top left, RGB mode at top right, hue in the center, and volume at the bottom.
10. The EEPROM allocation was changed from four layers and sixteen macro slots to six layers and six macro slots. The Vial build ID changed, causing firmware to initialize the new EEPROM layout safely.

## Deliberately omitted features

To remain comfortably inside the ATmega32U4 application region, this build disables canned reactive RGB animations, tap dance, combos, key overrides, mouse keys, NKRO, QMK settings, console, command mode, Caps Word, Layer Lock, Repeat Key, Auto Shift, Space Cadet, Grave Escape, and Magic key handling.

Vial key mapping, six layers, six macros, all four configurable encoders, media keys, the encoder presses, and direct VialRGB control remain enabled.

## License

GPL-2.0-or-later. See [LICENSE](LICENSE).
