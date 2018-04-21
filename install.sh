#!/usr/bin/env bash

set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run this script as root"
  exit
fi

if [ -f /usr/local/bin/keyblock ]; then
  echo "* Unloading existing service..."
  launchctl unload /Library/LaunchDaemons/com.johanbaath.keyblock.plist
fi

echo "* Compiling..."
echo ""
make
echo ""
mkdir -p /usr/local/etc/keyblock
cp settings.sample.json /usr/local/etc/keyblock/
mv keyblock /usr/local/bin/
cp com.johanbaath.keyblock.plist /Library/LaunchDaemons/
echo "* Loading service..."
launchctl load -w /Library/LaunchDaemons/com.johanbaath.keyblock.plist

echo ""
echo "Done!"
echo ""

if [ ! -f /usr/local/etc/keyblock/settings.json ]; then
  echo "Please create/edit the file: /usr/local/etc/keyblock/settings.json"
  echo "An example configuration has been created here: /usr/local/etc/keyblock/settings.sample.json"
  echo ""
  echo "When you are ready, run: sudo launchctl start com.johanbaath.keyblock"
else
 echo "The service has been updated to the latest version and restarted."
fi

echo ""
