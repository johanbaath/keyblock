# keyblock

A tiny application to block unintended duplicate keypresses caused by defect first- and second-generation butterfly keyboards from Apple.

It works by intercepting and blocking certain keyboard events depending on how long ago a key-down event was triggered.

The application is designed to run in the background at all times. It uses a minimal amount of CPU and RAM.

## Why?

I don't want to pay ~€300-400 for a replacement keyboard. The issue can easily develop again until Apple releases a new generation of keyboards for the MacBook series, or adjusts the design of the current model.

The issue is widespread and is described in more detail [here](https://forums.macrumors.com/threads/macbook-pro-13-2016-double-key-press.2025843/), [here](https://forums.macrumors.com/threads/2016-macbook-pro-keyboard-issue.2015249/), [here](https://forums.macrumors.com/threads/help-2017-macbook-pro-15-double-gg.2111805/), [here](https://www.ifixit.com/Answers/View/402016/Keyboard+%27B%27+key+double+hit+problem), [here](https://discussions.apple.com/thread/7754550) and [here](https://forums.macrumors.com/threads/2016mbp-sticky-key-double-strokes.2099054/).

## Installing

`$ sudo ./install.sh`

This will build and install `keyblock` as a launch daemon.

## Uninstalling

`$ sudo ./uninstall.sh`

This will remove all installed files, including configuration files, and unload the launch daemon.

## Configuration

Copy `settings.sample.json` to `settings.json` and place it in the `/usr/local/etc/keyblock` directory after you run the install script described above.

The configuration file accepts an array of objects under the property `keys`. Each object has the following properties:

* `keyCode`
    * The key code to watch in decimal format.

* `delay`
    * The time in milliseconds that has to elapse between key events. If the key event is triggered within the delay, the event will be blocked.

In the sample configuration, the application will block the `n` key (on my keyboard, your key code might differ) if the key is pressed twice or more within `100 ms`.

### Finding key codes

I recommend using [this handy application](https://manytricks.com/keycodes/) to find the correct key codes.

# License

[MIT](http://opensource.org/licenses/MIT)

Copyright (c) 2018-present, Johan Bååth AB.
