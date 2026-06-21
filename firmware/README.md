# VDO Breakout Firmware

ATtiny402 firmware for gauge backlight dimming on the [VDO Breakout](../README.md) board.

Reads a 0–5 V (scaled from 0-12 V) vehicle dimmer signal on **PA6**, outputs PWM on **PA1** to drive the shared Light− MOSFET. Includes soft fade on startup/shutdown and brownout protection when board power is cut. The dimmer input expects a high-side rheostat signal; low-side vehicles need a local pot — see [Dimming](../README.md#dimming) in the project README.

## Build profiles

| Environment | Use case | Fade-in | Fade-out |
|-------------|----------|---------|----------|
| `ATtiny402` | Incandescent gauge lighting | 1500 ms | 500 ms |
| `ATtiny402_LED` | LED gauge lighting | 300 ms | 200 ms |

## Build

```bash
cd firmware
pio run -e ATtiny402          # incandescent
pio run -e ATtiny402_LED      # LED
```

## Flash

Program via UPDI on J7 using an [Adafruit UPDI Friend](https://learn.adafruit.com/adafruit-updi-friend) (or compatible SerialUPDI adapter).

1. Set `upload_port` in `platformio.ini` to your serial device (`ls /dev/cu.usb*` on macOS).
2. Connect J7: UPDI → UPDI, GND → GND, +5V → PWR (or power the board separately).
3. Flash:

```bash
pio run -e ATtiny402 -t upload        # incandescent gauges
pio run -e ATtiny402_LED -t upload    # LED gauges
```

4. Burn BOD fuse once per chip (enables 4.3 V brown-out detector):

```bash
pio run -t fuses -e ATtiny402
```

## Bench tuning

Adjust `ADC_MIN` / `ADC_MAX` in `include/config.h` if the dimmer range doesn't feel right. If PWM still glitches on power cut, try raising `VDD_SHUTDOWN_MV` slightly (e.g. 4550–4600).

Licensed under the MIT License — see [LICENSE](LICENSE).
