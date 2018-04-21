#!/usr/bin/env bash

set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run this script as root"
  exit
fi

echo "* Unloading existing service..."
launchctl unload /Library/LaunchDaemons/com.johanbaath.keyblock.plist
echo "* Removing all related files and directories..."
rm -rf /usr/local/etc/keyblock
rm /usr/local/bin/keyblock
rm /Library/LaunchDaemons/com.johanbaath.keyblock.plist

echo ""
echo "Done!"
echo ""
echo "The service and its configuration has been removed."
echo ""
