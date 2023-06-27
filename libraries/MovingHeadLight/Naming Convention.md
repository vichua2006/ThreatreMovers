# Naming Convention

## Classes:

- **Class names** should have first letter of each word capped (e.g. `StepperMotor`, `DualStepper`).
- Members such as **pins** and **constant properties** (such as gear ratio) should be in all caps, with words separated by an underscore (e.g. `GEAR_RATIO`).
- More complex **object members** and **static members** (such as`isPanHallClosed`)  should follow the same convention as class name, *except* the first letter is lowercase (e.g. `panHall` in `DualHallSensors`).
- Method names should be all lower case with each word separated by an underscore. **Arguments** are the same (e.g. `min_angle_difference(double new_position)`).

## General

- **Constants** (such as `PanHallPin`) have the first letter of each word capitalized.
- **Class instances** should have all lowercase with each word separated by an underscore (e.g. `pan_tilt_sensors`).
- **Functions** should have the first letter of each word capitalized, *except* the first letter lowered (e.g. `isZero(double x)`). **Arguments** are whatever.